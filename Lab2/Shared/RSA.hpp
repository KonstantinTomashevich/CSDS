#pragma once
#include <boost/multiprecision/cpp_int.hpp>

namespace RSA
{
struct PublicKey
{
    boost::multiprecision::int256_t n;
    boost::multiprecision::int256_t e;
};

struct PrivateKey
{
    boost::multiprecision::int256_t n;
    boost::multiprecision::int256_t d;
};

void GenerateKeys (PublicKey &publicKey, PrivateKey &privateKey);
void Encode (const PublicKey &publicKey, boost::multiprecision::int256_t &value);
void Decode (const PrivateKey &privateKey, boost::multiprecision::int256_t &value);
}
