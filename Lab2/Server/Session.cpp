#include "Session.hpp"
#include <boost/random.hpp>
#include <boost/log/trivial.hpp>
#include <Shared/MessageType.hpp>

namespace States
{
const char *INITIAL = "Initial";

const char *SESSION_KEY_GENERATION = "SessionKeyGeneration";

const char *WAITING_FOR_AUTH = "WaitingForAuth";

const char *VALIDATING_AUTH = "ValidatingAuth";

const char *WAITING_FOR_QUERIES = "WaitingForQueries";

const char *SENDING_FILE = "SENDING_FILE";
}

Session::Session (boost::asio::io_context &ioContext)
    : socket_ (ioContext)
{
}

Session::~Session ()
{
    BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Ended!";
}

boost::asio::ip::tcp::socket &Session::GetSocket ()
{
    return socket_;
}

void Session::Start ()
{
    BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Started!";

    stateMachine_ = std::make_unique <StateMachine> (
        std::unordered_map <std::string, StateMachine::State> {
            {
                States::INITIAL,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Entered state " <<
                                                 States::INITIAL << "!";
                        AsyncWaitForInput (1 + RSA::PublicKey::N_SIZE + RSA::PublicKey::E_SIZE);
                        return {};
                    },
                    {
                        {
                            (char) MessageType::CTS_RSA_KEY,
                            [this] () -> std::string
                            {
                                ReadRSAKey ();
                                return States::SESSION_KEY_GENERATION;
                            }
                        }
                    }
                }
            },

            {
                States::SESSION_KEY_GENERATION,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Entered state " <<
                                                 States::SESSION_KEY_GENERATION << "!";

                        GenerateSessionKey ();
                        return (char) MessageType::STC_SESSION_KEY;
                    },
                    {
                        {
                            (char) MessageType::STC_SESSION_KEY,
                            [this] () -> std::string
                            {
                                WriteSessionKey ();
                                return States::WAITING_FOR_AUTH;
                            }
                        }
                    }
                }
            },

            {
                States::WAITING_FOR_AUTH,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Entered state " <<
                                                 States::WAITING_FOR_AUTH << "!";
                        // TODO: Implement.
                        return {};
                    },
                    {
                        {
                            (char) MessageType::CTS_AUTH_REQUEST,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::VALIDATING_AUTH;
                            }
                        }
                    }
                }
            },

            {
                States::VALIDATING_AUTH,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Entered state " <<
                                                 States::VALIDATING_AUTH << "!";
                        // TODO: Implement.
                        return (char) (true ? MessageType::STC_AUTH_SUCCESSFUL : MessageType::STC_AUTH_FAILED);
                    },
                    {
                        {
                            (char) MessageType::STC_AUTH_SUCCESSFUL,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::WAITING_FOR_QUERIES;
                            }
                        },

                        {
                            (char) MessageType::STC_AUTH_FAILED,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::WAITING_FOR_AUTH;
                            }
                        }
                    }
                }
            },

            {
                States::WAITING_FOR_QUERIES,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Entered state " <<
                                                 States::WAITING_FOR_QUERIES << "!";
                        // TODO: Implement.
                        return {};
                    },
                    {
                        {
                            (char) MessageType::STC_SESSION_KEY,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::WAITING_FOR_QUERIES;
                            }
                        },

                        {
                            (char) MessageType::CTS_FILE_REQUEST,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::WAITING_FOR_QUERIES;
                            }
                        }
                    }
                }
            },

            {
                States::SENDING_FILE,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Entered state " <<
                                                 States::SENDING_FILE << "!";
                        // TODO: Implement.
                        return (char) (true ? MessageType::STC_FILE : MessageType::STC_UNABLE_TO_SEND_FILE);
                    },
                    {
                        {
                            (char) MessageType::STC_FILE,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::WAITING_FOR_QUERIES;
                            }
                        },

                        {
                            (char) MessageType::STC_UNABLE_TO_SEND_FILE,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
                                return States::WAITING_FOR_QUERIES;
                            }
                        }
                    }
                }
            }
        },
        States::INITIAL
    );
}

void Session::AsyncWaitForInput (std::size_t expectedCount)
{

    boost::asio::async_read (socket_, boost::asio::buffer (buffer_), boost::asio::transfer_exactly (expectedCount),
                             [this] (const boost::system::error_code &error, std::size_t bytesTransferred) -> void
                             {
                                 if (error)
                                 {
                                     BOOST_LOG_TRIVIAL (info) << "Session [" << this << "]: Caught error " << error
                                                              << ", aborting...";
                                     delete this;
                                 }
                                 else
                                 {
                                     stateMachine_->Consume (buffer_[0]);
                                 }
                             });
}

void Session::GenerateSessionKey ()
{
    boost::random::mt11213b base_gen (clock ());
    boost::random::independent_bits_engine <boost::random::mt11213b,
                                            128, boost::multiprecision::uint128_t> gen (base_gen);

    boost::multiprecision::uint128_t sessionKey = gen ();
    boost::multiprecision::export_bits (sessionKey, currentSessionKey_.begin (), 8);

    BOOST_LOG_TRIVIAL(debug) << "Session [" << this << "]: Generated session key " << sessionKey << ".";
}

void Session::ReadRSAKey ()
{
    auto nStart = buffer_.begin () + 1;
    auto nEnd = nStart + RSA::PublicKey::N_SIZE;
    auto eStart = nEnd;
    auto eEnd = eStart + RSA::PublicKey::E_SIZE;

    boost::multiprecision::import_bits (rsaPublicKey_.n, nStart, nEnd);
    boost::multiprecision::import_bits (rsaPublicKey_.e, eStart, eEnd);

    BOOST_LOG_TRIVIAL(debug) << "Session [" << this << "]: Received RSA public key: n = " << rsaPublicKey_.n <<
                             ", e = " << rsaPublicKey_.e << ".";
}

void Session::WriteSessionKey ()
{
    buffer_[0] = (uint8_t) MessageType::STC_SESSION_KEY;
    boost::multiprecision::int256_t sessionKey;
    boost::multiprecision::import_bits (sessionKey, currentSessionKey_.begin (), currentSessionKey_.end ());

    RSA::Encode (rsaPublicKey_, sessionKey);
    boost::multiprecision::export_bits (sessionKey, buffer_.begin () + 1, 8);

    boost::asio::write (socket_, boost::asio::buffer (buffer_, 1 + RSA::MESSAGE_SIZE), boost::asio::transfer_all ());
    BOOST_LOG_TRIVIAL(debug) << "Session [" << this << "]: Encoded session key sent: " << sessionKey << "!";
}
