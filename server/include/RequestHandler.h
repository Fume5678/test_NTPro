#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H


#include <httpparser/request.h>
#include <httpparser/response.h>

class RequestHandler {
public:
    std::string get_response(httpparser::Request request);
    httpparser::Response route(httpparser::Request request);

    // REST API
    httpparser::Response add_user(httpparser::Request req);
    httpparser::Response add_order(httpparser::Request req);
    httpparser::Response get_orders(httpparser::Request req);
};

#endif // REQUESTHANDLER_H
