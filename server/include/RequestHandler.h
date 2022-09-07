#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <map>
#include <functional>
#include <httpparser/request.h>
#include <httpparser/response.h>

using namespace httpparser;
using std::vector;

class RequestHandler{
public:
    std::string get_response(Request request);

    Response route(Request req){
        Response resp; 
        resp.status = 200;
        resp.content = vector<char>();
        resp.versionMajor = 1;
        resp.versionMinor = 1;

        std::string uri = req.uri;
        if (uri == "/api/add_order"){
            resp.statusCode = 200;
        }
        if (uri == "/api/remove_order"){
            resp.statusCode = 200;
        }
        if (uri == "/api/get_orders"){
            resp.statusCode = 200;

        }

        return resp;
    }
};

Response add_order(Request){

}

#endif // REQUESTHANDLER_H
