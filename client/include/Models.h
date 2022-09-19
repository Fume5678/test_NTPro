#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <optional>
#include <vector>

using std::optional;
using std::string;
using std::vector;

class User;
class Order;

class Order {
public:
    string user_id;
    float       price;
    float       value;
    std::string source;
    std::string target;
    std::string type; // SELL or BUY
};

class User {
public:
    string           user_id;
    optional<string> password;
    vector<Order>   orders;
};

#endif // MODELS_H
