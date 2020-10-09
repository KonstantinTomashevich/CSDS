#pragma once
#include <iostream>
#include <boost/asio.hpp>

#include <Shared/StateMachine.hpp>
#include <Shared/RSA.hpp>
#include <Shared/Idea.hpp>

class Session final
{
public:
    explicit Session (boost::asio::io_context &ioContext);
    ~Session ();

    boost::asio::ip::tcp::socket &GetSocket ();

    void Start ();

private:
    void Abort ();
    void AbortOnFatalError (const boost::system::error_code &error);

    void AsyncWaitForInput (std::size_t expectedCount = 1);
    void GenerateSessionKey ();
    void ReadRSAKey ();

    void WriteSessionKey ();
    bool ReadAndValidateAuth ();
    bool TrySendFile ();

    boost::asio::ip::tcp::socket socket_;
    std::unique_ptr <StateMachine> stateMachine_;

    RSA::PublicKey rsaPublicKey_;
    Idea::Key currentSessionKey_;
    std::array <uint8_t, 1024> buffer_;

    std::size_t userToken_ = 0;
};
