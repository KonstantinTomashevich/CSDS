#pragma once

#include <cstdint>
#include <optional>

namespace EM
{
void RandomGenerate (int64_t M, int64_t &a, int64_t &b);

class EllipticGroup final
{
private:
    struct Config final
    {
        int64_t m_;
        int64_t a_;
        int64_t b_;
    };

public:
    struct Value
    {
        int64_t x_;
        int64_t y_;

        bool operator == (const Value &rhs) const;

        bool operator != (const Value &rhs) const;
    };

    struct PrivateKey
    {
        uint64_t multiplier_;
    };

    struct PublicKey
    {
        Value value_;
    };

    struct CommonKey
    {
        Value value_;
    };

    struct Signature
    {
        uint64_t r_;
        uint64_t s_;

        bool operator == (const Signature &rhs) const;
        bool operator != (const Signature &rhs) const;
    };

    enum class SignatureCheckResult
    {
        SUCCESSFUL,
        MATH_ERROR,
        FAILED
    };

    class ValuesIterator final
    {
    public:
        ValuesIterator (const EllipticGroup &group, int64_t currentX);

        ValuesIterator &operator ++ ();

        Value operator * () const;

        bool operator == (const ValuesIterator &rhs) const;

        bool operator != (const ValuesIterator &rhs) const;

    private:
        void UpdateCurrentY ();

        int64_t currentX_;
        std::optional <int64_t> currentY_;
        const EllipticGroup &group_;
    };

    explicit EllipticGroup (const Config &config);

    const Config &GetConfig () const;

    std::optional <Value> PickRandomG (uint64_t &maxOrder) const;

    std::optional <EllipticGroup::Value> AddPoints (const Value &first, const Value &second) const;

    std::optional <EllipticGroup::Value> MultiplyPoint (uint64_t times, const Value &point) const;

    int64_t Negate (int64_t arg) const;

    Value Negate (const Value &arg) const;

    PrivateKey GeneratePrivateKey (uint64_t order) const;

    PublicKey AssemblePublicKey (const PrivateKey &privateKey, const Value &g) const;

    CommonKey AssembleCommonKey (const PrivateKey &myPrivateKey, const PublicKey &otherPublicKey) const;

    uint64_t StripMessageHash (uint64_t messageHash, uint64_t gOrder) const;

    std::optional <EllipticGroup::Signature> Sign (uint64_t messageHash, const PrivateKey &privateKey,
                                                   const Value &g, uint64_t gOrder) const;

    SignatureCheckResult Check (const Signature &signature, uint64_t messageHash,
                                const PublicKey &publicKey, const Value &g, uint64_t gOrder) const;

    ValuesIterator begin () const;

    ValuesIterator end () const;

private:
    Config config_;
};
}
