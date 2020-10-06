#include <iostream>
#include <boost/asio.hpp>
#include "Server.hpp"

int main (int argc, char **argv)
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage:" << std::endl << "    <server_executable> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context ioContext;
        Server server (ioContext, std::atoi (argv[1]));
        ioContext.run ();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what () << "\n";
    }

    return 0;
}
