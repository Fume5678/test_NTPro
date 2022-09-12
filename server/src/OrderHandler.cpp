#include <OrderHandler.h>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

Order::Order() {
    user_id = "";
    order_pair = {"", ""};
    value   = 0.0;
    price   = 0.0;
}

Order::Order(const Order& other) {
    this->user_id = other.user_id;
    this->order_pair  = other.order_pair;
    this->value   = other.value;
    this->price   = other.price;
}

Order& Order::operator=(const Order& other) {
    if (this == &other) {
        return *this;
    }

    this->user_id    = other.user_id;
    this->order_pair = other.order_pair;
    this->value      = other.value;
    this->price      = other.price;
    return *this;
}

std::string Order::serialize() const {
    auto res = json{
        {"user_id", user_id},
        {"source", order_pair.source},
        {"target", order_pair.target},
        {"value", value},
        {"price", price},
    };

    return res.dump();
}

std::shared_ptr<OrderHandler> OrderHandler::get_instance() {
    static std::shared_ptr<OrderHandler> s{new OrderHandler};
    return s;
}

void OrderHandler::add_order(const Order& order) {
    // TODO не добавляет ключи в спискам ордеров
    mtx.lock();
    shared_ptr<Order> new_order = std::make_shared<Order>(order);
    try {
        order_map.at(order.order_pair).push_back(new_order);
    } catch (std::out_of_range&) {
        OrderList pair_orders;
        pair_orders.push_back(new_order);
        order_map.insert({order.order_pair, pair_orders});
    }

    weak_ptr<Order> new_order_weak = new_order;
    try {
        users.at(order.user_id).push_back(new_order_weak);
    } catch (std::out_of_range&) {
        vector<weak_ptr<Order>> orders_weak;
        orders_weak.push_back(new_order_weak);
        users.insert({order.user_id, orders_weak});
    }
    mtx.unlock();
}

std::optional<vector<Order>>
    OrderHandler::get_orders_by_user(std::string user_id) {

    try {
        vector<Order> orders;

        for (auto it = users.at(user_id).begin(); it != users.at(user_id).end();
             it++) {
            if ((*it).expired()) {
                users.at(user_id).erase(it);
            } else {
                orders.push_back(*(*it).lock().get());
            }
        }

        return std::move(orders);
    } catch (std::out_of_range&) {
        return std::nullopt;
    } catch (std::exception& e) {
        std::cout << "[WARN]: " << e.what() << std::endl;
        return std::nullopt;
    }
}

void OrderHandler::match(Order& order) {
    mtx.lock();

    mtx.unlock();
}