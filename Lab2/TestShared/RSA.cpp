#include <boost/test/unit_test.hpp>
#include <ctime>
#include <Shared/RSA.hpp>

BOOST_AUTO_TEST_SUITE (TestRSA)

BOOST_AUTO_TEST_CASE (EncodeDecode)
{
    RSA::PublicKey publicKey;
    RSA::PrivateKey privateKey;
    RSA::GenerateKeys (publicKey, privateKey);

    std::vector <uint8_t> messageBytes = {14, 66, 99, 42, 48, 99, 164, 244, 122, 90, 49, 11, 23, 41, 54, 67};
    boost::multiprecision::int256_t message;
    boost::multiprecision::import_bits (message, messageBytes.begin (), messageBytes.end ());

    BOOST_REQUIRE (message < publicKey.n);
    boost::multiprecision::int256_t messageCopy = message;

    RSA::Encode (publicKey, message);
    BOOST_REQUIRE (message != messageCopy);

    RSA::Decode (privateKey, message);
    BOOST_REQUIRE_EQUAL (message, messageCopy);

    std::vector <uint8_t> decodedMessageBytes;
    boost::multiprecision::export_bits (message, std::back_inserter (decodedMessageBytes), 8);
    BOOST_REQUIRE (messageBytes == decodedMessageBytes);
}

BOOST_AUTO_TEST_SUITE_END ()
