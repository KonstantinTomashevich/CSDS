#include "RSA.hpp"
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/integer/mod_inverse.hpp>

namespace RSA
{
#define BIT_COUNT 64

static boost::multiprecision::cpp_int random_prime ()
{
    using namespace boost::random;
    using namespace boost::multiprecision;

    typedef cpp_int int_type;
    mt11213b base_gen (clock ());
    independent_bits_engine <mt11213b, BIT_COUNT, int_type> gen (base_gen);
    //
    // We must use a different generator for the tests and number generation, otherwise
    // we get false positives.
    //
    mt19937 gen2 (clock ());

    while (true)
    {
        int_type n = gen ();
        if (miller_rabin_test (n, 25, gen2))
        {
            // Value n is probably prime, see if (n-1)/2 is also prime:
            if (miller_rabin_test ((n - 1) / 2, 25, gen2))
            {
                return n;
            }
        }
    }

    BOOST_ASSERT(false);
    return 0;
}

void GenerateKeys (PublicKey &publicKey, PrivateKey &privateKey)
{
    boost::multiprecision::cpp_int q = random_prime ();
    boost::multiprecision::cpp_int p = random_prime ();

    publicKey.n = p * q;
    privateKey.n = publicKey.n;
    boost::multiprecision::cpp_int secret = (p - 1) * (q - 1);

    do
    {
        publicKey.e = random_prime ();
    }
    while (publicKey.e >= secret || boost::multiprecision::gcd (publicKey.e, secret) > 1);

    privateKey.d = boost::integer::mod_inverse (publicKey.e, secret);
}

void Encode (const PublicKey &publicKey, boost::multiprecision::cpp_int &value)
{
    value = boost::multiprecision::powm (value, publicKey.e, publicKey.n);
}

void Decode (const PrivateKey &privateKey, boost::multiprecision::cpp_int &value)
{
    value = boost::multiprecision::powm (value, privateKey.d, privateKey.n);
}
}
