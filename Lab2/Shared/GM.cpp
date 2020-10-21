//
// Created by vladislav on 20/10/2020.
//

#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <future>

#include "GM.hpp"

namespace {

/**
 * Generate random value.
 * @tparam T Data type.
 * @tparam bits Generated value bit size.
 * @return Generated value.
 */
template<class T, int bits>
static T GenerateRandomValue() {
  using namespace boost::multiprecision;
  using namespace boost::random;

  mt11213b base_gen(clock());
  independent_bits_engine<decltype(base_gen), bits, T> gen(base_gen);
  return gen();
}

/**
 * Generate prime value.
 * @tparam T Result data type.
 * @tparam bits Value bit size.
 * @param trials Number of trials in probability test.
 * @return Generated value.
 */
template<class T, int bits>
static T GeneratePrime(int trials = 25) {
  T value;
  do {
    value = GenerateRandomValue<T, bits>();
  } while (!miller_rabin_test(value, trials));
  return value;
}

template<class T>
bool IsOdd(const T &value) {
  return boost::multiprecision::bit_test(value, 0);
}

/**
 * Check if given value is quadratic residue using Euler's criterion.
 * (i.e if there is such x that x^2 = a mod p)
 * @tparam T Data type.
 * @param value Value to test.
 * @param modulus Odd prime modulus.
 * @return Test result
 */
template<class T>
bool IsQuadraticResidue(const T &value, const T &modulus) {
  // works only for odd modulus
  assert(IsOdd(modulus));

  T power = (modulus - 1) / 2;
  return (1 == boost::multiprecision::powm(value, power, modulus));
}

/**
 * Check if given value is not quadratic residue using Euler's criterion.
 * @tparam T Data type.
 * @param value Value to test.
 * @param modulus Odd prime modulus.
 * @return Test result
 */
template<class T>
bool IsNotQuadraticResidue(const T &value, const T &modulus) {
  // works only for odd modulus
  assert(IsOdd(modulus));

  T power = (modulus - 1) / 2;
  return (modulus - 1) == boost::multiprecision::powm(value, power, modulus);
}

template<class T>
T EncryptBit(const GM::PublicKey &key, bool bit) {
  namespace mp = boost::multiprecision;

  // generate non-quadratic residue. Legendre symbol for 'p' and 'q' is -1
  mp::uint512_t y;
  do {
    y = GenerateRandomValue<mp::uint512_t, 256>();
    y %= key.n;
  } while (1 != mp::gcd(y, key.n.convert_to<mp::uint512_t>()));
  
  // make value quadratic residue
  //y *= y;
  y = mp::powm(y, 2, key.n.convert_to<mp::uint512_t>());
  //y %= key.n;

  // if 1 then non-quadratic, else quadratic
  if (bit)
    y *= key.y;

  y %= key.n;
  return y.convert_to<T>();
}

template<class T>
bool DecryptBit(const GM::PrivateKey &key, T value) {
  // 'value' is IsQuadraticResidue for 'N' if encrypted bit is '0'
  return !(IsQuadraticResidue(value, key.p) && IsQuadraticResidue(value, key.q));
}

std::vector<boost::multiprecision::uint256_t> Encode(const GM::PublicKey &publicKey,
                                                     const boost::multiprecision::uint256_t &value) {
  namespace mp = boost::multiprecision;

  std::vector<mp::uint256_t> result;
  // encrypt each bit
  for (int i = 0; i < 256; ++i) {
    bool bit = mp::bit_test(value, i);
    result.push_back(EncryptBit<mp::uint256_t>(publicKey, bit));
  }

  return result;
}

}

namespace GM {

void GenerateKeys(PublicKey &publicKey, PrivateKey &privateKey) {
  namespace mp = boost::multiprecision;

  privateKey.p = GeneratePrime<mp::uint256_t, 128>(25);
  privateKey.q = GeneratePrime<mp::uint256_t, 128>(25);
  publicKey.n = privateKey.p * privateKey.q;

  // generate non-quadratic value
  bool isNotQuadratic;
  do {
    publicKey.y = GenerateRandomValue<mp::uint256_t, 256>();
    publicKey.y %= publicKey.n;

    isNotQuadratic = IsNotQuadraticResidue(publicKey.y, privateKey.p)
        && IsNotQuadraticResidue(publicKey.y, privateKey.q);
  } while (!isNotQuadratic);
}

boost::multiprecision::uint256_t EncryptBit(const PublicKey &key, bool bit) {
  return ::EncryptBit < boost::multiprecision::uint256_t > (key, bit);
}

bool DecryptBit(const PrivateKey &key,
                const boost::multiprecision::uint256_t &value) {
  return ::DecryptBit(key, value);
}

std::vector<boost::multiprecision::uint256_t> Encode(const PublicKey &publicKey,
                                                     const boost::multiprecision::uint256_t &value) {
  std::vector<boost::multiprecision::uint256_t> result;
  for (int i = 0; i < 256; ++i) {
    bool bit = boost::multiprecision::bit_test(value, i);
    result.push_back(EncryptBit(publicKey, bit));
  }
  return result;
}

boost::multiprecision::uint256_t Decode(const PrivateKey &privateKey,
                                        const std::vector<boost::multiprecision::uint256_t> &value) {
  namespace mp = boost::multiprecision;
  assert(value.size() == 256);

  mp::uint256_t result = 0;
  for (int i = 0; i < 256; ++i) {
    if (DecryptBit(privateKey, value[i]))
      mp::bit_set(result, i);
  }
  return result;
}

}
