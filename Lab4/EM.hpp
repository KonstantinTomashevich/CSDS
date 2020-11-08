#pragma once

#include <cstdint>
#include <optional>

namespace EM
{
bool BruteForceGenerate (uint64_t M, uint64_t &a, uint64_t &b);

class EllipticGroup final
{
private:
    struct Config final
    {
        uint64_t M_;
        uint64_t a_;
        uint64_t b_;
    };

public:
    struct Value
    {
        uint64_t x_;
        uint64_t y_;
    };

    class ValuesIterator final
    {
    public:
        ValuesIterator (const Config &config, uint64_t currentX);

        ValuesIterator &operator ++ ();

        Value operator * () const;

        bool operator == (const ValuesIterator &rhs) const;

        bool operator != (const ValuesIterator &rhs) const;

    private:
        void UpdateCurrentY ();

        uint64_t currentX_;
        std::optional<uint64_t> currentY_;
        const Config &config_;
    };

    explicit EllipticGroup (const Config &config);

    ValuesIterator begin() const;
    ValuesIterator end() const;

private:
    Config config_;
};
}
