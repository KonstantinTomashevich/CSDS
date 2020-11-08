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
        a = 1 + MathUtils::Modulo (rand (), M - 1);
        b = 1 + MathUtils::Modulo (rand (), M - 1);
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
    // TODO: Include negated values.
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

std::optional <EllipticGroup::Value> EllipticGroup::PickRandomG (uint64_t minOrder) const
{
    std::vector <Value> suitable;
    for (EM::EllipticGroup::Value value : *this)
    {
        EM::EllipticGroup::Value temp = MultiplyPoint (minOrder, value);
        if (temp != Negate (temp))
        {
            suitable.emplace_back (value);
        }
    }

    if (suitable.empty ())
    {
        return {};
    }
    else
    {
        return suitable[MathUtils::Modulo (rand (), suitable.size ())];
    }
}

EllipticGroup::Value EllipticGroup::AddPoints (
    const EllipticGroup::Value &first, const EllipticGroup::Value &second) const
{
    if (first.x_ < second.x_)
    {
        return AddPoints (second, first);
    }

    int64_t m = MathUtils::Modulo (
        first != second ?
        (first.y_ - second.y_) * boost::integer::mod_inverse (
            first.x_ - second.x_, config_.m_) :
        (3 * MathUtils::IntPower <2> (first.x_) + config_.a_) * boost::integer::mod_inverse (
            2 * first.y_, config_.m_), config_.m_);

    int64_t x = MathUtils::Modulo (MathUtils::IntPower <2> (m) - first.x_ - second.x_, config_.m_);
    int64_t y = MathUtils::Modulo (first.y_ + m * (x - first.x_), config_.m_);
    return Negate ({x, y});
}

EllipticGroup::Value EllipticGroup::MultiplyPoint (uint64_t times, const EllipticGroup::Value &point) const
{
    Value result;
    bool resultInitialized = false;
    Value temp = point;

    while (times > 0)
    {
        if (times & 1U)
        {
            if (resultInitialized)
            {
                result = AddPoints (result, temp);
            }
            else
            {
                result = temp;
                resultInitialized = true;
            }
        }

        temp = AddPoints (temp, temp);
        times >>= 1U;
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

EllipticGroup::PrivateKey EllipticGroup::GeneratePrivateKey ()
{

    return {(uint64_t) MathUtils::Modulo (rand (), config_.m_) + 1};
}

EllipticGroup::PublicKey EllipticGroup::AssemblePublicKey (const EllipticGroup::PrivateKey &privateKey, const Value &g)
{
    return {MultiplyPoint (privateKey.multiplier_, g)};
}

EllipticGroup::CommonKey EllipticGroup::AssembleCommonKey (const EllipticGroup::PrivateKey &myPrivateKey,
                                                           const EllipticGroup::PublicKey &otherPublicKey)
{
    return {MultiplyPoint (myPrivateKey.multiplier_, otherPublicKey.value_)};
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