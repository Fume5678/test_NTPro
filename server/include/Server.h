#ifndef SERVER_H
#define SERVER_H

#include <ctime>
#include <iostream>
#include <string>
#include <chrono>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>


#include <ConHandler.h>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class Server {
private:
    tcp::acceptor            acceptor_;
    boost::asio::io_context& io_context_;

    void start_accept();

public:
    // constructor for accepting connection from client
    Server(boost::asio::io_context& io_context);
    void handle_accept(ConHandler::pointer             connection,
                       const boost::system::error_code& err);
};

#endif // SERVER_H
