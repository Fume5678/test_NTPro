#ifndef CONHANDLER_H
#define CONHANDLER_H

#include <ctime>
#include <iostream>
#include <string>
#include <chrono>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <RequestHandler.h>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class ConHandler;

class ConHandler : public boost::enable_shared_from_this<ConHandler> {
private:
    tcp::socket sock;
    std::string response_msg = "HTTP/1.1 200 OK\r\n\r\n";
    std::string request_msg;
    enum { max_length = 1024 };
    char buffer[max_length];
    RequestHandler req_handler;

public:
    using pointer =  boost::shared_ptr<ConHandler>;

    ConHandler(boost::asio::io_context& io_context);
    // creating the pointer
    static pointer create(boost::asio::io_context& io_context);

    // socket creation
    tcp::socket& socket();

    void start();

    void handle_read(const boost::system::error_code& err,
                     size_t                           bytes_transferred);

    void handle_write(const boost::system::error_code& err,
                      size_t                           bytes_transferred);
};


#endif // CONHANDLER_H
