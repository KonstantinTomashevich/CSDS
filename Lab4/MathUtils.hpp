#pragma once

#include <cstdint>

namespace MathUtils
{
template <uint64_t power>
inline uint64_t IntPower (uint64_t arg)
{
    return arg * IntPower <power - 1> (arg);
}

template <>
inline uint64_t IntPower <0> (uint64_t arg)
{
    return 1;
}

uint64_t SquareRootModulus (uint64_t argument, uint64_t modulus);
}
