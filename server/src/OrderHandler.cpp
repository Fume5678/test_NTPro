#include <OrderHandler.h>

Order::Order() {
    user_id = "";
    source  = "";
    target  = "";
    value   = 0.0;
    price   = 0.0;
}

Order::Order(const Order& other) {
    this->user_id = other.user_id;
    this->source  = other.source;
    this->target  = other.target;
    this->value   = other.value;
    this->price   = other.price;
}

Order& Order::operator=(const Order& other) {
    if (this == &other) {
        return *this;
    }

    this->user_id = other.user_id;
    this->source  = other.source;
    this->target  = other.target;
    this->value   = other.value;
    this->price   = other.price;
    return *this;
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
        order_map.at({order.source, order.target}).push_back(new_order);
    } catch (std::out_of_range&) {
        OrderList pair_orders;
        pair_orders.push_back(new_order);
        order_map.insert({{order.target, order.source}, pair_orders});
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