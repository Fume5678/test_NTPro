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
        {"type", order_pair.type},
        {"value", value},
        {"price", price},
    };

    return res.dump();
}

OrderHandler::OrderHandler() {
    comp_order_sell = [](const shared_ptr<Order>& v1,
                         const shared_ptr<Order>& v2) {
        bool res = (int)(v1->price * 10000) > (int)(v2->price * 10000);
        return res;
    };

    comp_order_buy = [](const shared_ptr<Order>& v1,
                        const shared_ptr<Order>& v2) {
        bool res = (int)(v1->price * 10000) < (int)(v2->price * 10000);
        return res;
    };
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
        order_map.at(order.order_pair).push(new_order);
    } catch (std::out_of_range&) {
        if (order.order_pair.type == "BUY") {
            order_map[order.order_pair] = OrderList(comp_order_buy);
        } else {
            order_map[order.order_pair] = OrderList(comp_order_sell);
        }
        order_map[order.order_pair].push(new_order);
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

[[deprecated]] std::optional<std::reference_wrapper<OrderHandler::OrderList>>
    OrderHandler::get_order_list(OrderPairType pair) {
    // TODO переписать или удалить. После каждого вызова очищает лист ордеров
    try {
        return order_map.at(pair);
    } catch (std::out_of_range&) {
        std::cout << "[INFO] pair not found: " << (std::string)(pair)
                  << std::endl;
    } catch (std::exception& e) {
        std::cout << "[ERROR] " << e.what() << std::endl;
    }

    return std::nullopt;
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

void OrderHandler::match(OrderPairType order_pair) {
    mtx.lock();

    try {
        order_pair.type        = "BUY";
        OrderList& orders_buy  = order_map.at(order_pair);
        order_pair.type        = "SELL";
        OrderList& orders_sell = order_map.at(order_pair);

        for (;!orders_sell.empty() && !orders_buy.empty();) {

            auto order_buy  = orders_buy.top();
            auto order_sell = orders_sell.top();

            if (order_buy->price < order_sell->price) {
                break;
            }

            float swaping_val =
                std::min(order_buy.get()->value, order_sell.get()->value);

            User& user_buy = UserHandler::get_instance()
                                 ->get_user(order_buy.get()->user_id)
                                 .value();

            User& user_sell = UserHandler::get_instance()
                                  ->get_user(order_sell.get()->user_id)
                                  .value();

            user_buy.change_balance(order_buy.get()->order_pair.target,
                                    -swaping_val * order_buy.get()->price);

            user_buy.change_balance(order_buy.get()->order_pair.source,
                                    swaping_val);

            user_sell.change_balance(order_sell.get()->order_pair.target,
                                     swaping_val * order_buy.get()->price);

            user_sell.change_balance(order_sell.get()->order_pair.source,
                                     -swaping_val);

            order_buy.get()->value -= swaping_val;
            order_sell.get()->value -= swaping_val;

            if (int(order_buy->value * 10000) == 0) {
                 orders_buy.pop();
            }
            if (int(order_sell->value * 10000) == 0) {
                 orders_sell.pop();
            }
        }
    } catch (std::out_of_range&) {
        std::cout << "[INFO] pair not found" << std::endl;
    } catch (std::exception& e) {
        std::cout << "[WARN]: " << e.what() << std::endl;
    }

    mtx.unlock();
    return;
}
