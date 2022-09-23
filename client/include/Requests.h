#ifndef REQUESTS_H
#define REQUESTS_H

#include <boost/asio/io_context.hpp>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <httpparser/request.h>
#include <httpparser/response.h>
#include <httpparser/httpresponseparser.h>

#include <Models.h>
class Requests {
    httpparser::Response do_request(httpparser::Request req);

public:
    Requests(std::string server_addr);

    // REST API
    bool POST_add_order(Order order, std::string password);
    bool POST_verify_user(User user);
    std::string POST_add_user(User user);
    std::vector<Order> POST_get_orders(User user);

private:
    std::string             server_addr;
    boost::asio::io_context context;
};

#endif // REQUESTS_H
