#pragma once
#include <boost/multiprecision/cpp_int.hpp>

namespace RSA
{
struct PublicKey
{
    boost::multiprecision::cpp_int n;
    boost::multiprecision::cpp_int e;
};

struct PrivateKey
{
    boost::multiprecision::cpp_int n;
    boost::multiprecision::cpp_int d;
};

void GenerateKeys (PublicKey &publicKey, PrivateKey &privateKey);
void Encode (const PublicKey &publicKey, boost::multiprecision::cpp_int &value);
void Decode (const PrivateKey &privateKey, boost::multiprecision::cpp_int &value);
}
