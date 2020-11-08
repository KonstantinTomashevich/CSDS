#include "MathUtils.hpp"

#include <cmath>

namespace MathUtils
{
bool BruteForceIsPrime (uint64_t arg)
{
    for (uint64_t divCheck = 2; divCheck <= sqrt (arg); ++divCheck)
    {
        if (arg % divCheck == 0)
        {
            return false;
        }
    }

    return true;
}

// TODO: Find non-bruteforce implementation, which works good (previous found were buggy).
std::optional <uint64_t> SquareRootModulus (uint64_t argument, uint64_t modulus)
{
    argument = argument % modulus;
    for (uint64_t result = 1; result < modulus; ++result)
    {
        if ((result * result) % modulus == argument)
        {
            return result;
        }
    }

    return {};
}
}