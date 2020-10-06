#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <Shared/StateMachine.hpp>

class Session final
{
public:
    Session (boost::asio::io_context &ioContext);

    boost::asio::ip::tcp::socket &GetSocket ();

    void Start ();

private:
    boost::asio::ip::tcp::socket socket_;
    std::unique_ptr <StateMachine> stateMachine_;
};
