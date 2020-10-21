//
// Created by vladislav on 20/10/2020.
//

#ifndef SHARED_GM_H_
#define SHARED_GM_H_

#include <boost/multiprecision/cpp_int.hpp>

namespace GM {
static constexpr uint16_t MODULUS_SIZE_BITS = 256;
static constexpr uint16_t KEY_PART_SIZE_BITS = MODULUS_SIZE_BITS / 2;
static constexpr uint16_t KEY_CHUNK_SIZE_IN_BYTES = MODULUS_SIZE_BITS / 8;
static constexpr uint16_t ENCODED_CHUNK_SIZE = MODULUS_SIZE_BITS / 8;

struct PrivateKey {
  boost::multiprecision::uint256_t p;
  boost::multiprecision::uint256_t q;
};

struct PublicKey {
  boost::multiprecision::uint256_t n;
  boost::multiprecision::uint256_t y;
};

/**
 * Generate keys for Goldwasser Micali cryptosystem.
 * @param[out] publicKey Public key.
 * @param[out] privateKey Private key.
 */
void GenerateKeys(PublicKey &publicKey, PrivateKey &privateKey);
/**
 * Encrypt bit of data.
 * @param key Encryption(private) key.
 * @param bit Bit to encrypt.
 * @return Encrypted bit value.
 */
boost::multiprecision::uint256_t EncryptBit(const PublicKey &key, bool bit);
/**
 * Decrypt bit of data.
 * @param key Decryption(public) key.
 * @param value Encrypted bit value.
 * @return Decrypted bit.
 */
bool DecryptBit(const PrivateKey &key, const boost::multiprecision::uint256_t& value);





/**
 * Encrypt message.
 * @param publicKey Encryption key.
 * @param value Message to encode.
 * @return Array of numbers, where i-th number encodes i-th message bit.
 */
std::vector<boost::multiprecision::uint256_t> Encode(const PublicKey &publicKey,
                                                     const boost::multiprecision::uint256_t &value);

/**
 * Decrypt message.
 * @param privateKey Decryption key.
 * @param value Ciphertext - array of numbers. i-th number encrypts i-th bit.
 * @return Decrypted value.
 */
boost::multiprecision::uint256_t Decode(const PrivateKey &privateKey,
                                        const std::vector<boost::multiprecision::uint256_t> &value);
}

#endif //SHARED_GM_H_
