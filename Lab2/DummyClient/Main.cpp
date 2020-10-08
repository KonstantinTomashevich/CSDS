#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include <Shared/RSA.hpp>
#include <Shared/Idea.hpp>
#include <Shared/MessageType.hpp>
#include <Shared/Idea.hpp>

int main (int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cout << "Usage:" << std::endl << "    <executable> <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::resolver resolver (ioContext);
        boost::asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve (boost::asio::ip::tcp::v4 (), argv[1], argv[2]);

        boost::asio::ip::tcp::socket socket (ioContext);
        boost::asio::connect (socket, endpoints);

        BOOST_LOG_TRIVIAL (info) << "Connected to server!";

        RSA::PublicKey rsaPublicKey;
        RSA::PrivateKey rsaPrivateKey;
        Idea::Key currentSessionKey;
        std::array <uint8_t, 1024> buffer;

        BOOST_LOG_TRIVIAL (info) << "Initializing and sending public RSA key...";
        {
            RSA::GenerateKeys (rsaPublicKey, rsaPrivateKey);
            BOOST_LOG_TRIVIAL (debug) << "Generated RSA public key: n = " << rsaPublicKey.n <<
                                      ", e = " << rsaPublicKey.e << ".";

            buffer[0] = (uint8_t) MessageType::CTS_RSA_KEY;

            boost::multiprecision::export_bits (rsaPublicKey.n, buffer.begin () + 1, 8);
            boost::multiprecision::export_bits (rsaPublicKey.e, buffer.begin () + 1 + RSA::PublicKey::N_SIZE, 8);
            boost::asio::write (socket,
                                boost::asio::buffer (buffer, 1 + RSA::PublicKey::N_SIZE + RSA::PublicKey::E_SIZE),
                                boost::asio::transfer_all ());
        }

        BOOST_LOG_TRIVIAL (info) << "Waiting for session key...";
        {
            boost::asio::read (socket, boost::asio::buffer (buffer, 1 + RSA::MESSAGE_SIZE),
                               boost::asio::transfer_all ());

            if (buffer[0] != (uint8_t) MessageType::STC_SESSION_KEY)
            {
                BOOST_LOG_TRIVIAL(error) << "Expected message with code " << (uint8_t) MessageType::STC_SESSION_KEY <<
                                         ", but received " << buffer[0] << ".";
                return 1;
            }

            boost::multiprecision::int256_t sessionKey;
            boost::multiprecision::import_bits (sessionKey, buffer.begin () + 1,
                                                buffer.begin () + 1 + RSA::MESSAGE_SIZE);
            BOOST_LOG_TRIVIAL (debug) << "Received encoded session key: " << sessionKey << ".";

            RSA::Decode (rsaPrivateKey, sessionKey);
            BOOST_LOG_TRIVIAL (debug) << "Decoded session key: " << sessionKey << ".";

            // Clean buffer, so we'll be able to check if decoded session key size is really 128 bits.
            buffer.fill (0);
            boost::multiprecision::export_bits (sessionKey, buffer.begin () + 1, 8);

            for (int index = 0; index < RSA::MESSAGE_SIZE; ++index)
            {
                if (index < RSA::MESSAGE_SIZE / 2)
                {
                    currentSessionKey[index] = buffer[index + 1];
                }
                else if (buffer[index + 1] > 0)
                {
                    BOOST_LOG_TRIVIAL(error) << "Unable to downcast decoded session key to 128 bits!";
                    return 1;
                }
            }
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what () << "\n";
    }

    return 0;
}
