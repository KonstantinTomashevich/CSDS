#include "EM.hpp"
#include "MathUtils.hpp"

namespace EM
{
bool BruteForceGenerate (uint64_t M, uint64_t &a, uint64_t &b)
{
    a = M - 1;
    b = M - 1;

    while (a--)
    {
        while (b--)
        {
            if ((4 * MathUtils::IntPower <3> (a) + 27 * MathUtils::IntPower <2> (b)) % M)
            {
                return true;
            }
        }
    }

    return false;
}

EllipticGroup::ValuesIterator::ValuesIterator (const EllipticGroup::Config &config, uint64_t currentX)
    : config_ (config),
      currentX_ (currentX),
      currentY_ (0)
{
    while (currentX_ < config_.M_)
    {
        UpdateCurrentY ();
        if (currentY_ > 0)
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
    do
    {
        ++currentX_;
        UpdateCurrentY ();
    }
    while (currentX_ < config_.M_ && currentY_ <= 0);
    return *this;
}

EllipticGroup::Value EllipticGroup::ValuesIterator::operator * () const
{
    return {currentX_, currentY_};
}

void EllipticGroup::ValuesIterator::UpdateCurrentY ()
{
    if (currentX_ > 0 && currentX_ < config_.M_)
    {
        currentY_ = MathUtils::SquareRootModulus (
            (MathUtils::IntPower <3> (currentX_) + config_.a_ * currentX_ + config_.b_) % config_.M_,
            config_.M_);
    }
    else
    {
        currentY_ = 0;
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

EllipticGroup::ValuesIterator EllipticGroup::begin () const
{
    return {config_, 1};
}

EllipticGroup::ValuesIterator EllipticGroup::end () const
{
    return {config_, config_.M_};
}
}