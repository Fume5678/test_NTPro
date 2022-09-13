#include <OrderHandler.h>
#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <UserHandler.h>
#include <type_traits>

using namespace nlohmann;

Order::Order() {
    user_id    = "";
    order_pair = {"", ""};
    value      = 0.0;
    price      = 0.0;
}

Order::Order(const Order& other) {
    this->user_id    = other.user_id;
    this->order_pair = other.order_pair;
    this->value      = other.value;
    this->price      = other.price;
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
    mtx.lock();
    // TODO добавить сортировку по цене при добавлении
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

    match(order.order_pair);
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

void OrderHandler::match(OrderPair order_pair) {
    mtx.lock();
    OrderList orders = order_map.at(order_pair);
    std::swap(order_pair.source, order_pair.target);
    OrderList orders_reverse = order_map.at(order_pair);

    auto comp = [](const shared_ptr<Order> a, const shared_ptr<Order> b) {
        return a.get()->price < b.get()->price;
    };

    while (!orders.empty() && !orders_reverse.empty()) {

        auto order = std::max_element(orders.begin(), orders.end(), comp);

        auto order_reverse =
            std::max_element(orders.begin(), orders.end(), comp);

        double swap_val =
            std::abs(order->get()->value - order_reverse->get()->value);

        User user = UserHandler::get_instance()
                        ->get_user(order->get()->user_id)
                        .value();

        User user_reverse = UserHandler::get_instance()
                                ->get_user(order_reverse->get()->user_id)
                                .value();

        user.change_balance(
            order->get()->order_pair.source,
            swap_val * order->get()->price);

        user.change_balance(
            order->get()->order_pair.target,
            swap_val / order->get()->price);

        user_reverse.change_balance(
            order->get()->order_pair.target,
            swap_val * order_reverse->get()->price);

        user_reverse.change_balance(
            order->get()->order_pair.source,
            swap_val / order_reverse->get()->price);
        
    }

    mtx.unlock();
}