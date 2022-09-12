#include <string>
#include <utility>
#include <iostream>
#include <RequestHandler.h>
#include <nlohmann/json.hpp>
#include <OrderHandler.h>

using namespace httpparser;
using namespace nlohmann;
using std::string;
using std::vector;

std::string RequestHandler::get_response(Request request) {
    return route(request).serialize();
}

Response RequestHandler::route(Request request) {
    std::tuple url(request.method, request.uri);

    if (url == std::tuple("POST", "/api/add_order")) {
        return add_order(request);
    } else {
        Response resp;
        resp.status     = "Not Found";
        resp.statusCode = 404;

        return resp;
    }
}

Response RequestHandler::add_order(Request req) {

    Order order;
    json  data = json::parse(req.content_as_str());
    try {
        order.user_id = data.at("user_id");
        order.source  = data.at("source");
        order.target  = data.at("target");
        order.value   = data.at("value");
        order.price   = data.at("price");
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }
    OrderHandler::get_instance().get()->add_order(order);

    Response             resp;
    Response::HeaderItem cont_type{"Content-Type", "application/json"};
    resp.headers.push_back(cont_type);
    resp.content    = vector<char>();
    resp.statusCode = 200;
    resp.status     = "Ok";

    return resp;
}