#ifndef REQUESTS_H
#define REQUESTS_H

#include <boost/asio/io_context.hpp>
#include <string>
#include <iostream>
#include <boost/asio.hpp>

class Requests {
    void do_request(std::string msg);
public:       
    Requests();
    
private:
    std::string server_addr;
    boost::asio::io_context context;
};

#endif // REQUESTS_H
