#include "EM.hpp"
#include "MathUtils.hpp"

#include <boost/integer/mod_inverse.hpp>
#include <unordered_map>
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
    if (currentY_.has_value())
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
    std::vector <Value> suitable;
    maxOrder = 1;

    for (EM::EllipticGroup::Value value : *this)
    {
        std::optional <EM::EllipticGroup::Value> temp = MultiplyPoint (maxOrder, value);
        if (temp.has_value () && temp != Negate (temp.value ()))
        {
            temp = value;
            uint64_t order;

            for (order = 1; order <= config_.m_; ++order)
            {
                temp = AddPoints (temp.value (), value);
                if (!temp.has_value () || temp == Negate (temp.value ()))
                {
                    break;
                }
            }

            if (order > maxOrder)
            {
                maxOrder = order;
                suitable.clear ();
            }
            else if (order == maxOrder)
            {
                suitable.emplace_back (value);
            }
        }
    }

    if (suitable.empty ())
    {
        return {};
    }
    else
    {
        return suitable[MathUtils::Modulo (MathUtils::Random::Get ().Next (), suitable.size ())];
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

EllipticGroup::PrivateKey EllipticGroup::GeneratePrivateKey (uint64_t order)
{

    return {(uint64_t) MathUtils::Modulo (MathUtils::Random::Get ().Next (), order - 1) + 1};
}

EllipticGroup::PublicKey
EllipticGroup::AssemblePublicKey (const EllipticGroup::PrivateKey &privateKey, const Value &g)
{
    return {MultiplyPoint (privateKey.multiplier_, g).value ()};
}

EllipticGroup::CommonKey EllipticGroup::AssembleCommonKey (const EllipticGroup::PrivateKey &myPrivateKey,
                                                           const EllipticGroup::PublicKey &otherPublicKey)
{
    return {MultiplyPoint (myPrivateKey.multiplier_, otherPublicKey.value_).value ()};
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