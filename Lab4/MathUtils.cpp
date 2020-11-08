#include <cstdlib>
#include "MathUtils.hpp"

namespace MathUtils
{
int Jacobi (int64_t a, int64_t n)
{

    int s;
    int64_t b = a;
    int64_t e = 0;
    int64_t m;
    int64_t n1;
    int64_t a1;


    if (a == 0)
    {
        return 0;
    }

    if (a == 1)
    {
        return 1;
    }

    while ((b & 1) == 0)
    {
        b >>= 1, e++;
    }

    a1 = b;
    m = n % 8;

    if (!(e & 1))
    {
        s = 1;
    }
    else if (m == 1 || m == 7)
    {
        s = +1;
    }
    else if (m == 3 || m == 5)
    {
        s = -1;
    }

    if (n % 4 == 3 && a1 % 4 == 3)
    {
        s = -s;
    }

    if (a1 != 1)
    {
        n1 = n % a1;
    }
    else
    {
        n1 = 1;
    }

    return s * Jacobi (n1, a1);
}


void ExtendedEuclid (int64_t a, int64_t b, int64_t *x, int64_t *y, int64_t *d)
{
    int64_t q;
    int64_t r;
    int64_t x1;
    int64_t x2;
    int64_t y1;
    int64_t y2;

    if (b == 0)
    {
        *d = a;
        *x = 1;
        *y = 0;
        return;

    }

    x2 = 1;
    x1 = 0;
    y2 = 0;
    y1 = 1;

    while (b > 0)
    {
        q = a / b;
        r = a - q * b;

        *x = x2 - q * x1;
        *y = y2 - q * y1;

        a = b;
        b = r;

        x2 = x1;
        x1 = *x;
        y2 = y1;
        y1 = *y;
    }

    *d = a;
    *x = x2;
    *y = y2;
}


int64_t Inverse (int64_t a, int64_t b)
{
    int64_t d, x, y;
    ExtendedEuclid (a, b, &x, &y, &d);

    if (d == 1)
    {
        return x;
    }

    return 0;
}


int64_t ExpMod (int64_t x, int64_t b, int64_t n)
{
    int64_t a = 1, s = x;
    while (b != 0)
    {
        if (b & 1)
        {
            a = (a * s) % n;
        }

        b >>= 1;
        if (b != 0)
        {
            s = (s * s) % n;
        }
    }

    return a;
}


std::optional <int64_t> SquareRootMod (int64_t a, int64_t p)
{
    int64_t ai;
    int64_t b;
    int64_t c;
    int64_t d;
    int64_t e;
    int64_t i;
    int64_t r;
    int64_t s = 0;
    int64_t t = p - 1;

    if (Jacobi (a, p) == -1)
    {
        return {};
    }

    do
    {
        do
        {
            b = rand () % p;
        }
        while (b == 0);
    }
    while (Jacobi (b, p) != -1);

    while (!(t & 1))
    {
        s++;
        t >>= 1;
    }

    ai = Inverse (a, p);
    c = ExpMod (b, t, p);
    r = ExpMod (a, (t + 1) / 2, p);

    for (i = 1; i < s; i++)
    {
        e = ExpMod (2, s - i - 1, p);
        d = ExpMod ((r * r % p) * ai % p, e, p);

        if (d == p - 1)
        {
            r = r * c % p;
        }

        c = c * c % p;
    }

    return {r};
}

std::optional <uint64_t> SquareRootModulus (uint64_t argument, uint64_t modulus)
{
    return SquareRootMod (argument, modulus);
}
}