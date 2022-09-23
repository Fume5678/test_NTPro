#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <map>

using std::optional;
using std::string;
using std::vector;
using std::map;

class User;
class Order;

class Order {
public:
    string      user_id;
    float       price;
    float       value;
    string source;
    string target;
    string type; // SELL or BUY

    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        os << "{";
        os << "\"source\": " << order.source;
        os << " \"target\": " << order.target;
        os << " \"type\": " << order.type;
        os << " \"value\": " << order.value;
        os << " \"price\": " << order.price;
        os << "}";
        return os;
    }
};

class User {
public:
    string           user_id;
    optional<string> password;
    map<string, float>    balance;
};

#endif // MODELS_H
