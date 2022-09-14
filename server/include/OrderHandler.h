
#ifndef ORDERHANDLER_H
#define ORDERHANDLER_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <queue>
#include <functional>
#include <exception>
#include <optional>

using std::map;
using std::mutex;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;
using std::weak_ptr;
// using std::tuple;

class OrderPair {
public:
    std::string source;
    std::string target;

    friend bool operator==(const OrderPair& a, const OrderPair& b) {
        return (a.source == b.source) && (a.target == b.target);
    }

    operator string() const {
        return source + "/" + target;
    }
};

class Order {
public:
    std::string user_id;
    float       price;
    float       value;
    OrderPair   order_pair;

    Order();

    Order(const Order& other);

    Order& operator=(const Order& other);

    std::string serialize() const;
};

// using OrderPair = pair<string, string>;

class OrderHandler {
    void match(OrderPair order_pair);

    OrderHandler();

public:
    using CompOrder = std::function<bool(const shared_ptr<Order>& v1,
                                         const shared_ptr<Order>& val2)>;
    using OrderList = std::priority_queue<shared_ptr<Order>,
                                          std::vector<shared_ptr<Order>>,
                                          CompOrder>;
    using UserMap   = map<string, vector<weak_ptr<Order>>>;
    using OrderMap  = map<std::string, OrderList>;

    static std::shared_ptr<OrderHandler> get_instance();

    void add_order(const Order& order);

    std::optional<std::reference_wrapper<OrderList>> get_order_list(OrderPair pair);

    std::optional<vector<Order>> get_orders_by_user(std::string user_id);

    // std::optional<const OrderList&>
    //     get_orders_by_pairs(OrderPair order_pair) const{
    //     try {
    //         return std::optional{order_map.at(order_pair)};
    //     } catch(std::out_of_range& e){
    //         return std::nullopt;
    //     }
    // }

private:
    CompOrder comp_order;
    mutex     mtx;
    OrderMap  order_map;
    UserMap   users;
};

#endif // ORDERHANDLER_H
