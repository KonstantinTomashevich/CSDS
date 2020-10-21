//
// Created by vladislav on 21/10/2020.
//

#include <boost/test/unit_test.hpp>
#include <boost/multiprecision/integer.hpp>
#include <ctime>
#include <Shared/GM.hpp>

BOOST_AUTO_TEST_SUITE (TestGM)

BOOST_AUTO_TEST_CASE (EncryptDecryptBitTest) {
  using namespace GM;
  using namespace boost::multiprecision;

  PrivateKey privateKey;
  privateKey.p = 5;
  privateKey.q = 11;
  PublicKey publicKey;
  publicKey.n = privateKey.p * privateKey.q;
  publicKey.y = 17;

  uint256_t encrypted;
  bool decrypted;

  encrypted = EncryptBit(publicKey, true);
  decrypted = DecryptBit(privateKey, encrypted);
  BOOST_REQUIRE(decrypted == true);

  encrypted = EncryptBit(publicKey, false);
  decrypted = DecryptBit(privateKey, encrypted);
  BOOST_REQUIRE(decrypted == false);
}

BOOST_AUTO_TEST_CASE (SimpleEncodeDecodeTest) {
  using namespace GM;
  using namespace boost::multiprecision;

  PrivateKey privateKey;
  privateKey.p = 5;
  privateKey.q = 11;
  PublicKey publicKey;
  publicKey.n = privateKey.p * privateKey.q;
  publicKey.y = 17;

  uint256_t values[] = {
      0, 1, 2, 3, 4, 5, 6, 8, 32, 11, 54, 28, 17, 44
  };

  for (const uint256_t &value : values) {
    auto enc = Encode(publicKey, value);
    BOOST_REQUIRE_EQUAL(value, Decode(privateKey, enc));
  }
}

BOOST_AUTO_TEST_CASE (ComplexEncodeDecodeTest) {
  namespace mp = boost::multiprecision;

  GM::PublicKey publicKey;
  GM::PrivateKey privateKey;
  GM::GenerateKeys(publicKey, privateKey);

  mp::uint256_t values[] =
      {8, 234, 23465, 45652343, 546, 3242345, 4565, 1324, 3524354654, 322,
       24534};

  for (const mp::uint256_t &value : values) {
    auto enc = GM::Encode(publicKey, value);
    BOOST_REQUIRE_EQUAL(value, GM::Decode(privateKey, enc));
  }
}

BOOST_AUTO_TEST_SUITE_END ()
