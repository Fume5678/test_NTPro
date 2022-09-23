#include <Requests.h>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using namespace boost::asio::ip;

Requests::Requests(std::string server_addr) : server_addr{server_addr} {}

httpparser::Response Requests::do_request(httpparser::Request req) {
    httpparser::Response resp;
    tcp::socket socket{context};

    std::string host = server_addr.substr(0, server_addr.find(":"));
    std::string port = server_addr.substr(server_addr.find(":") + 1, server_addr.length());
    // connection
    socket.connect(tcp::endpoint(
        address::from_string(host),
        std::stoi(port)));

    boost::system::error_code error;
    std::string buf = req.serialize();
    std::cerr << "[INFO] request: " << buf << std::endl;
    auto buffer_tmp = boost::asio::buffer(buf);
    boost::asio::write(socket, buffer_tmp, error);
    if (error) {
        cout << "[ERROR] Send failed: " << error.message() << endl;
    }

    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if( error && error != boost::asio::error::eof ) {
        cout << "[ERRO] Recieve failed: " << error.message() << endl;
    }
    else {
        const char* c_data = boost::asio::buffer_cast<const char*>(receive_buffer.data());

        string data{c_data};

        std::cerr << "[INFO] responce: " << data << std::endl;

        httpparser::HttpResponseParser parser; 
        int res = parser.parse(
            resp, data.c_str(), data.c_str() + data.length());
        
        if(res != httpparser::HttpResponseParser::ParsingCompleted) {
            cout << "Fail parse responce" << endl;
        }
        else {
            std::cout << "Ok" << endl;
        }
    }

    return resp;
}

std::string Requests::POST_add_user(User user) {
    httpparser::Request req; 
    req.uri = "/api/add_user";
    req.method = "POST";

    nlohmann::json body = {
        {"user_id", user.user_id},
        {"password", user.password.value()},
    };

    httpparser::Request::HeaderItem cont_type{"Content-Type", "application/json"};
    req.headers.push_back(cont_type);
    req.str_to_content(body.dump());

    auto res = do_request(req).content_as_str();
    return res;
}

bool Requests::POST_add_order(Order order, std::string password) {
    httpparser::Request req; 
    req.uri = "/api/add_order";
    req.method = "POST";
    
    nlohmann::json body = {
        {"password", password},
        {"user_id", order.user_id},
        {"source", order.source},
        {"target", order.target},
        {"type", order.type},
        {"value", order.value},
        {"price", order.price},
    };
    

    httpparser::Request::HeaderItem cont_type{"Content-Type", "application/json"};
    req.headers.push_back(cont_type);
    req.str_to_content(body.dump());

    httpparser::Response resp = do_request(req);
    if(resp.statusCode == 200){
        cerr << "[INFO] Order added" << endl;
        return true;
    } else {
        cerr << "[ERROR] Order not added: " << resp.statusCode << " "
             << resp.status << endl;
        return false;
    }
}

bool Requests::POST_verify_user(User user) {
    httpparser::Request req; 
    req.uri = "/api/verify_user";
    req.method = "POST";

    nlohmann::json body = {
        {"user_id", user.user_id},
        {"password", user.password.value()},
    };

    httpparser::Request::HeaderItem cont_type{"Content-Type", "application/json"};
    req.headers.push_back(cont_type);
    req.str_to_content(body.dump());

    if(do_request(req).statusCode == 200) {
        return true;
    } else {
        return false;
    }
}

std::vector<Order> Requests::POST_get_orders(User user) {
    std::vector<Order> orders;

    httpparser::Request req; 
    req.uri = "/api/get_user_orders";
    req.method = "POST";

    nlohmann::json body = {
        {"user_id", user.user_id},
        {"password", user.password.value()},
    };

    httpparser::Request::HeaderItem cont_type{"Content-Type", "application/json"};
    req.headers.push_back(cont_type);
    req.str_to_content(body.dump());

    string resp_body = do_request(req).content_as_str();
    try{
        nlohmann::json resp_json = nlohmann::json::parse(resp_body);

        for(auto& order : resp_json["orders"]) {
            Order new_order;
            new_order.user_id = order["user_id"].get<string>();
            new_order.source = order["source"].get<string>();
            new_order.target = order["target"].get<string>();
            new_order.type = order["type"].get<string>();
            new_order.value = order["value"].get<float>();
            new_order.price = order["price"].get<float>();

            orders.push_back(new_order);
        }

    }catch(std::exception& e){
        cerr << "[ERROR] " << e.what() << endl;
    }

    return orders;
}
