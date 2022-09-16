#include <string>
#include <utility>
#include <iostream>
#include <RequestHandler.h>
#include <nlohmann/json.hpp>
#include <OrderHandler.h>
#include <UserHandler.h>

using namespace httpparser;
using namespace nlohmann;
using std::string;

std::string RequestHandler::get_response(Request request) {
    return route(request).serialize();
}

Response RequestHandler::route(Request request) {
    std::tuple url(request.method, request.uri);

    if (url == std::tuple("POST", "/api/add_user")) {
        return add_user(request);
    }
    if (url == std::tuple("POST", "/api/add_order")) {
        return add_order(request);
    }
    if (url == std::tuple("POST", "/api/get_orders")) {
        return get_orders(request);
    }
    if (url == std::tuple("POST", "/api/get_user_orders")) {
        return get_user_orders(request);
    }
    if (url == std::tuple("POST", "/api/get_userdetail")) {
        return get_userdetail(request);
    } else {
        Response resp;
        resp.status     = "Not Found";
        resp.statusCode = 404;
        return resp;
    }
}

/// @example
/// {
///     "user_id": "123123123"
///     "password": "123456"
/// }
httpparser::Response RequestHandler::add_user(httpparser::Request req) {
    std::string user_id;
    std::string password;
    try {
        json   data = json::parse(req.content_as_str());
        user_id = data.at("user_id");
        password = data.at("password");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }

    std::string body = "User added";
    if(UserHandler::get_instance()->get_user(user_id).has_value()) {
        body = "User already exists";
    } else {
        UserHandler::get_instance()->add_user(User{user_id, password});
    }

    Response resp;
    resp.statusCode = 200;
    resp.status     = "Ok";
    resp.str_to_content(body);
    return resp;
}

/// @example
/// {
///     "user_id": "123123123",
///     "password": "123456",
///     "source": "RUB",
///     "target": "USD",
///     "type": "BUY",
///     "value": 20,
///     "price": 61
/// }
Response RequestHandler::add_order(Request req) {

    Order order;
    std::string password;
    json  data = json::parse(req.content_as_str());
    try {
        order.user_id    = data.at("user_id");
        order.order_pair = {data.at("source"), data.at("target"), data.at("type")};
        order.value      = data.at("value");
        order.price      = data.at("price");
        password         = data.at("password");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }

    if (!UserHandler::get_instance()->verify_user(User{order.user_id, password})) {
        Response resp;
        resp.statusCode = 403;
        resp.status     = "Unauthorized";
        return resp;
    }

    OrderHandler::get_instance().get()->add_order(order);

    Response resp;
    resp.statusCode = 200;
    resp.status     = "Ok";

    return resp;
}

/// ! Этот запрос очищает список ордеров, надо переписать. Использовался для отладки
/// @example
/// {
///     "source": "RUB",
///     "target": "USD",
///     "type": "SELL"
/// }
Response RequestHandler::get_orders(Request req) {
    OrderPairType pair;
    json   data = json::parse(req.content_as_str());
    try {
        pair.source = data.at("source");
        pair.target = data.at("target");
        pair.type = data.at("type");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }

    auto opt_orders =
        OrderHandler::get_instance().get()->get_order_list(pair);

    int count = 0;
    std::stringstream sstr_body;
    sstr_body << "{\"orders\":[";

    if (opt_orders.has_value()) {
        auto orders = opt_orders.value();
        while(!orders.get().empty()){
            if(0 < count){
                sstr_body << ", ";
            } else {
                count++;
            }
            sstr_body << orders.get().top().get()->serialize() ;
            orders.get().pop();

        }
    }

    sstr_body << "]}";

    Response resp;
    resp.statusCode = 200;
    resp.status     = "Ok";
    Response::HeaderItem cont_type{"Content-Type", "application/json"};
    resp.headers.push_back(cont_type);
    resp.str_to_content(sstr_body.str());

    return resp;
}


/// @example
/// {
///     "user_id": "123123123"
///     "password": "123456"
/// }
Response RequestHandler::get_user_orders(Request req) {
    string user_id;
    string password;
    json   data = json::parse(req.content_as_str());
    try {
        user_id = data.at("user_id");
        password = data.at("password");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }
    if (!UserHandler::get_instance()->verify_user(User{user_id, password})) {
        Response resp;
        resp.statusCode = 403;
        resp.status     = "Unauthorized";
        return resp;
    }

    auto opt_orders =
        OrderHandler::get_instance().get()->get_orders_by_user(user_id);

    int count = 0;
    std::stringstream sstr_body;
    sstr_body << "{\"orders\":[";

    if (opt_orders.has_value()) {
        auto orders = opt_orders.value();
        for (const auto& order : orders) {
            if(0 < count){
                sstr_body << ", ";
            } else {
                count++;
            }
            sstr_body << order.serialize();
        }
    }

    sstr_body << "]}";

    Response resp;
    resp.statusCode = 200;
    resp.status     = "Ok";
    Response::HeaderItem cont_type{"Content-Type", "application/json"};
    resp.headers.push_back(cont_type);
    resp.str_to_content(sstr_body.str());

    return resp;
}

/// @example
/// {
///     "user_id":  "123123123"
///     "password": "123456"
/// }
httpparser::Response RequestHandler::get_userdetail(httpparser::Request req) {
    string user_id;
    string password;
    json   data = json::parse(req.content_as_str());
    try {
        user_id = data.at("user_id");
        password = data.at("password");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }
    if (!UserHandler::get_instance()->verify_user(User{user_id, password})) {
        Response resp;
        resp.statusCode = 403;
        resp.status     = "Unauthorized";
        return resp;
    }

    auto user = UserHandler::get_instance()->get_user(user_id);

    json body = {
        {"user_id", user_id},
    };
    json balance_arr;

    for (const auto& bal: user->get().get_balance()) {
        // json one_note = json{
        //     {bal.first, bal.second}
        // };
        body["balance"][bal.first] = bal.second;
    }

    Response resp;
    resp.statusCode = 200;
    resp.status     = "Ok";
    Response::HeaderItem cont_type{"Content-Type", "application/json"};
    Response::HeaderItem cont_len{"Content-Length",
                                  std::to_string(body.dump().length())};
    resp.headers.push_back(cont_type);
    resp.headers.push_back(cont_len);
    resp.str_to_content(body.dump());
    return resp;
}

/// @example
/// {
///     "user_id": "123123123"
///     "password": "123456"
/// }
httpparser::Response RequestHandler::verify(httpparser::Request req) {
    string user_id;
    string password;
    json   data = json::parse(req.content_as_str());
    try {
        user_id = data.at("user_id");
        password = data.at("password");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        Response resp;
        resp.statusCode = 400;
        resp.status     = "Bad Request";
        return resp;
    }
    if (!UserHandler::get_instance()->verify_user(User{user_id, password})) {
        Response resp;
        resp.statusCode = 403;
        resp.status     = "Unauthorized";
        return resp;
    }

    Response resp;
    resp.statusCode = 200;
    resp.status     = "Ok";
    return resp;
}
