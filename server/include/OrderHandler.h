
#ifndef ORDERHANDLER_H
#define ORDERHANDLER_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <utility>
#include <exception>
#include <optional>

using std::map;
using std::mutex;
using std::pair;
using std::string;
using std::vector;
using std::shared_ptr;
using std::weak_ptr;


class Order {
public:
    std::string user_id;
    std::string source;
    std::string target;
    double      value;
    double      price;

    Order();

    Order(const Order& other);

    Order& operator= (const Order& other);
};

using OrderPair = pair<string, string>;
using OrderList = vector<shared_ptr<Order>>;
using UserMap = map<string, vector<weak_ptr<Order>>>;
using OrderMap = map<pair<string, string>, OrderList>;

class OrderHandler {
    // void match(Order& order) {
    //     mtx.lock();

    //     vector<Order>* sell_orders = &order_map.at({order.target, order.source});


    //     mtx.unlock();
    // }

    OrderHandler() {}
    
public:
    static std::shared_ptr<OrderHandler> get_instance();

    void add_order(const Order& order);

    // std::optional<const OrderList&>
    //     get_orders_by_pairs(OrderPair order_pair) const{
    //     try {
    //         return std::optional{order_map.at(order_pair)};
    //     } catch(std::out_of_range& e){
    //         return std::nullopt;
    //     }
    // }

    // std::optional<const vector<Order>&>
    //     get_orders_by_user(std::string user_id) {}

private:
    mutex     mtx;
    OrderMap order_map;
    UserMap users;
};

#endif // ORDERHANDLER_H
