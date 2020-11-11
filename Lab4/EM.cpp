#include "EM.hpp"
#include "MathUtils.hpp"

#include <boost/integer/mod_inverse.hpp>
#include <vector>

namespace EM
{
void RandomGenerate (int64_t M, int64_t &a, int64_t &b)
{
    do
    {
        a = 1 + MathUtils::Modulo (MathUtils::Random::Get ().Next (), M - 1);
        b = 1 + MathUtils::Modulo (MathUtils::Random::Get ().Next (), M - 1);
    }
    while (MathUtils::Modulo (4 * MathUtils::IntPower <3> (a) + 27 * MathUtils::IntPower <2> (b), M) == 0);
}

bool EllipticGroup::Value::operator == (const EllipticGroup::Value &rhs) const
{
    return x_ == rhs.x_ &&
           y_ == rhs.y_;
}

bool EllipticGroup::Value::operator != (const EllipticGroup::Value &rhs) const
{
    return !(rhs == *this);
}

bool EllipticGroup::Signature::operator == (const EllipticGroup::Signature &rhs) const
{
    return r_ == rhs.r_ &&
           s_ == rhs.s_;
}

bool EllipticGroup::Signature::operator != (const EllipticGroup::Signature &rhs) const
{
    return !(rhs == *this);
}

EllipticGroup::ValuesIterator::ValuesIterator (const EllipticGroup &group, int64_t currentX)
    : group_ (group),
      currentX_ (currentX),
      currentY_ (0)
{
    while (currentX_ < group_.GetConfig ().m_)
    {
        UpdateCurrentY ();
        if (currentY_.has_value ())
        {
            break;
        }
        else
        {
            ++currentX_;
        }
    }
}

EllipticGroup::ValuesIterator &EllipticGroup::ValuesIterator::operator ++ ()
{
    if (currentY_.has_value ())
    {
        uint64_t negated = group_.Negate (currentY_.value ());
        if (negated > currentY_)
        {
            currentY_ = negated;
            return *this;
        }
    }

    do
    {
        ++currentX_;
        UpdateCurrentY ();
    }
    while (currentX_ < group_.GetConfig ().m_ && !currentY_.has_value ());
    return *this;
}

EllipticGroup::Value EllipticGroup::ValuesIterator::operator * () const
{
    return {currentX_, currentY_.value ()};
}

void EllipticGroup::ValuesIterator::UpdateCurrentY ()
{
    if (currentX_ > 0 && currentX_ < group_.GetConfig ().m_)
    {
        currentY_ = MathUtils::SquareRootModulus (
            MathUtils::IntPower <3> (currentX_) + group_.GetConfig ().a_ * currentX_ + group_.GetConfig ().b_,
            group_.GetConfig ().m_);

        if (currentY_.has_value ())
        {
            uint64_t negated = group_.Negate (currentY_.value ());
            if (negated < currentY_)
            {
                currentY_ = negated;
            }
        }
    }
    else
    {
        currentY_ = {};
    }
}

bool EllipticGroup::ValuesIterator::operator == (const EllipticGroup::ValuesIterator &rhs) const
{
    return currentX_ == rhs.currentX_;
}

bool EllipticGroup::ValuesIterator::operator != (const EllipticGroup::ValuesIterator &rhs) const
{
    return !(rhs == *this);
}

EllipticGroup::EllipticGroup (const EllipticGroup::Config &config)
    : config_ (config)
{

}

const EllipticGroup::Config &EllipticGroup::GetConfig () const
{
    return config_;
}

std::optional <EllipticGroup::Value> EllipticGroup::PickRandomG (uint64_t &maxOrder) const
{
    std::vector <Value> all;
    for (Value value : *this)
    {
        all.emplace_back (value);
    }

    maxOrder = 1;
    std::vector <Value> suitable;

    for (Value value : all)
    {
        uint64_t order = 1;
        std::optional<Value> temp = value;

        while (temp.has_value() && temp != Negate(temp.value()))
        {
            temp = AddPoints(temp.value(), value);
            ++order;
        }

        if (order > maxOrder && MathUtils::BruteForceIsPrime(order))
        {
            suitable.clear();
            maxOrder = order;
        }

        if (order == maxOrder)
        {
            suitable.emplace_back(value);
        }
    }

    if (suitable.empty())
    {
        return {};
    }
    else
    {
        return suitable[MathUtils::Random::Get().Next() % suitable.size()];
    }
}

std::optional <EllipticGroup::Value> EllipticGroup::AddPoints (
    const EllipticGroup::Value &first, const EllipticGroup::Value &second) const
{
    int64_t m;
    if (first != second)
    {
        uint64_t inverse = boost::integer::mod_inverse (MathUtils::Modulo (
            first.x_ - second.x_, config_.m_), config_.m_);

        if (inverse == 0)
        {
            return {};
        }

        m = MathUtils::Modulo (MathUtils::Modulo (first.y_ - second.y_, config_.m_) *
                               inverse, config_.m_);
    }
    else
    {
        uint64_t inverse = boost::integer::mod_inverse (2 * first.y_, config_.m_);
        if (inverse == 0)
        {
            return {};
        }

        m = MathUtils::Modulo (MathUtils::Modulo (
            3 * MathUtils::IntPower <2> (first.x_) + config_.a_, config_.m_) * inverse, config_.m_);
    }

    int64_t x = MathUtils::Modulo (MathUtils::IntPower <2> (m) - first.x_ - second.x_, config_.m_);
    int64_t y = MathUtils::Modulo (first.y_ + m * MathUtils::Modulo (x - first.x_, config_.m_), config_.m_);
    return Negate ({x, y});
}

std::optional <EllipticGroup::Value>
EllipticGroup::MultiplyPoint (uint64_t times, const EllipticGroup::Value &point) const
{
    std::optional <Value> result {};
    std::optional <Value> temp = point;

    while (times > 0)
    {
        if (times & 1U)
        {
            if (result.has_value ())
            {
                result = AddPoints (result.value (), temp.value ());
                if (!result.has_value ())
                {
                    return {};
                }
            }
            else
            {
                result = temp;
            }
        }

        temp = AddPoints (temp.value (), temp.value ());
        times >>= 1U;

        if (!temp.has_value ())
        {
            return {};
        }
    }

    return result;
}

int64_t EllipticGroup::Negate (int64_t arg) const
{
    return MathUtils::Modulo (config_.m_ - MathUtils::Modulo (arg, config_.m_), config_.m_);
}

EllipticGroup::Value EllipticGroup::Negate (const EllipticGroup::Value &arg) const
{
    return {arg.x_, Negate (arg.y_)};
}

EllipticGroup::PrivateKey EllipticGroup::GeneratePrivateKey (uint64_t order) const
{

    return {(uint64_t) MathUtils::Modulo (MathUtils::Random::Get ().Next (), order - 1) + 1};
}

EllipticGroup::PublicKey
EllipticGroup::AssemblePublicKey (const EllipticGroup::PrivateKey &privateKey, const Value &g) const
{
    return {MultiplyPoint (privateKey.multiplier_, g).value ()};
}

EllipticGroup::CommonKey EllipticGroup::AssembleCommonKey (const EllipticGroup::PrivateKey &myPrivateKey,
                                                           const EllipticGroup::PublicKey &otherPublicKey) const
{
    return {MultiplyPoint (myPrivateKey.multiplier_, otherPublicKey.value_).value ()};
}

uint64_t EllipticGroup::StripMessageHash (uint64_t messageHash, uint64_t gOrder) const
{
    uint64_t mask = 0;
    while (gOrder)
    {
        mask = (mask << 1U) | 1U;
        gOrder >>= 1U;
    }

    return messageHash & mask;
}

std::optional <EllipticGroup::Signature> EllipticGroup::Sign (uint64_t messageHash, const PrivateKey &privateKey,
                                                              const Value &g, uint64_t gOrder) const
{
    messageHash = StripMessageHash (messageHash, gOrder);
    if (messageHash % gOrder == 0)
    {
        return {};
    }

    std::vector <uint64_t> possibleKeys;
    possibleKeys.reserve (gOrder - 1);

    for (uint64_t index = 1; index < gOrder; ++index)
    {
        possibleKeys.emplace_back (index);
    }

    while (!possibleKeys.empty ())
    {
        uint64_t index = MathUtils::Random::Get ().Next () % possibleKeys.size ();
        uint64_t key = possibleKeys[index];
        possibleKeys.erase (possibleKeys.begin () + index);

        Signature signature {};
        std::optional <Value> temp = MultiplyPoint (key, g);

        if (!temp.has_value ())
        {
            continue;
        }

        signature.r_ = MathUtils::Modulo (temp.value ().x_, gOrder);
        if (signature.r_ == 0)
        {
            continue;
        }

        uint64_t kInverse = boost::integer::mod_inverse ((int64_t) key, (int64_t) gOrder);
        if (kInverse == 0)
        {
            continue;
        }

        signature.s_ = MathUtils::Modulo (
            kInverse * MathUtils::Modulo (
                messageHash + MathUtils::Modulo (signature.r_ * privateKey.multiplier_, gOrder), gOrder),
            gOrder);

        if (signature.s_ == 0)
        {
            continue;
        }

        return signature;
    }

    return {};
}

EllipticGroup::SignatureCheckResult
EllipticGroup::Check (const Signature &signature, uint64_t messageHash, const EllipticGroup::PublicKey &publicKey,
                      const EllipticGroup::Value &g, uint64_t gOrder) const
{
    messageHash = StripMessageHash (messageHash, gOrder);
    uint64_t sInverse = boost::integer::mod_inverse ((int64_t) signature.s_, (int64_t) gOrder);

    if (sInverse == 0)
    {
        return SignatureCheckResult::MATH_ERROR;
    }

    uint64_t u1 = MathUtils::Modulo (sInverse * messageHash, gOrder);
    if (u1 == 0)
    {
        return SignatureCheckResult::MATH_ERROR;
    }

    uint64_t u2 = MathUtils::Modulo (sInverse * signature.r_, gOrder);
    if (u2 == 0)
    {
        return SignatureCheckResult::MATH_ERROR;
    }

    std::optional <Value> u1G = MultiplyPoint (u1, g);
    if (!u1G.has_value ())
    {
        return SignatureCheckResult::MATH_ERROR;
    }

    std::optional <Value> u2H = MultiplyPoint (u2, publicKey.value_);
    if (!u2H.has_value ())
    {
        return SignatureCheckResult::MATH_ERROR;
    }

    std::optional <Value> result = AddPoints (u1G.value (), u2H.value ());
    if (!result.has_value ())
    {
        return SignatureCheckResult::MATH_ERROR;
    }

    auto res= signature.r_ == MathUtils::Modulo (result.value ().x_, gOrder) ? SignatureCheckResult::SUCCESSFUL
                                                                          : SignatureCheckResult::FAILED;
    if (res == SignatureCheckResult::FAILED)
    {
        result = {};
    }

    return res;
}

EllipticGroup::ValuesIterator EllipticGroup::begin () const
{
    return {*this, 1};
}

EllipticGroup::ValuesIterator EllipticGroup::end () const
{
    return {*this, config_.m_};
}
}