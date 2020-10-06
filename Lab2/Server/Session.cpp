#include "Session.hpp"
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

boost::asio::ip::tcp::socket &Session::GetSocket ()
{
    return socket_;
}

void Session::Start ()
{
    std::cout << "Session [" << this << "]: Started!" << std::endl;

    stateMachine_ = std::make_unique <StateMachine> (
        std::unordered_map <std::string, StateMachine::State> {
            {
                States::INITIAL,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        std::cout << "Session [" << this << "]: Entered state " << States::INITIAL << "!" << std::endl;
                        // TODO: Implement.
                        return {};
                    },
                    {
                        {
                            (char) MessageType::CTS_RSA_KEY,
                            [this] () -> std::string
                            {
                                // TODO: Implement.
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
                        std::cout << "Session [" << this << "]: Entered state " <<
                                  States::SESSION_KEY_GENERATION << "!" << std::endl;
                        // TODO: Implement.
                        return (char) MessageType::STC_SESSION_KEY;
                    },
                    {
                        {
                            (char) MessageType::STC_SESSION_KEY,
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
                States::WAITING_FOR_AUTH,
                {
                    [this] (StateMachine &owner) -> std::optional <char>
                    {
                        std::cout << "Session [" << this << "]: Entered state " <<
                                  States::WAITING_FOR_AUTH << "!" << std::endl;
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
                        std::cout << "Session [" << this << "]: Entered state " <<
                                  States::VALIDATING_AUTH << "!" << std::endl;
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
                        std::cout << "Session [" << this << "]: Entered state " <<
                                  States::WAITING_FOR_QUERIES << "!" << std::endl;
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
                        std::cout << "Session [" << this << "]: Entered state " <<
                                  States::SENDING_FILE << "!" << std::endl;
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

    delete this;
}
