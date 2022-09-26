#include <gtest/gtest.h>

#include <OrderHandler.h>
#include <UserHandler.h>

TEST(user_handler, add_user_and_get_user){
    auto user_handler = UserHandler::get_instance();
    user_handler->add_user(User{"fume", "1234"});

    auto user = user_handler->get_user("fume");

    ASSERT_EQ(true, user.has_value());
    ASSERT_EQ("fume", user.value().get().get_user_id());
    ASSERT_EQ("1234", user.value().get().get_password());

    auto user2 = user_handler->get_user("abc");

    ASSERT_EQ(false, user2.has_value());
}

TEST(user_handler, verify_user){
    auto user_handler = UserHandler::get_instance();

    ASSERT_EQ(true, user_handler->verify_user(User{"fume", "1234"}));
    ASSERT_EQ(false, user_handler->verify_user(User{"fume", "5321"}));
    ASSERT_EQ(false, user_handler->verify_user(User{"fume1", "1234"}));
}

TEST(user, add_balance_on_change){
    User user = {"fume", "1235"};

    ASSERT_FLOAT_EQ(0.0, user.get_balance_by_curr("RUB"));

    user.change_balance("RUB", 100.0);
    ASSERT_FLOAT_EQ(100.0, user.get_balance_by_curr("RUB"));
}

TEST(user, change_balance){
    User user = {"fume", "1235"};

    user.change_balance("RUB", 100.0);
    ASSERT_FLOAT_EQ(100.0, user.get_balance_by_curr("RUB"));

    user.change_balance("RUB", 100.0);
    ASSERT_FLOAT_EQ(200.0, user.get_balance_by_curr("RUB"));

    user.change_balance("USD", -100.0);
    ASSERT_FLOAT_EQ(-100.0, user.get_balance_by_curr("USD"));
}


// OrderHandler
//---------------------------------------

TEST(order_handler, add_and_get_order){
    auto order_handler = OrderHandler::get_instance();
    Order new_order;
    new_order.user_id = "fume_1";
    new_order.order_pair_type = OrderPairType{"RUB", "USD", "SELL"};
    new_order.price = 20.0;
    new_order.value = 60.0;
    order_handler->add_order(new_order);

    EXPECT_EQ(false, order_handler->get_orders_by_pair(OrderPairType{"RUB", "USD", "BUY"}).has_value());

    auto getted_order_list = order_handler->get_orders_by_pair(OrderPairType{"RUB", "USD", "SELL"});
    ASSERT_EQ(true, getted_order_list.has_value());

    auto getted_order = getted_order_list->get().top();
    ASSERT_EQ("fume_1", getted_order->user_id);
    ASSERT_EQ("RUB", getted_order->order_pair_type.source);
    ASSERT_EQ("USD", getted_order->order_pair_type.target);
    ASSERT_EQ("SELL", getted_order->order_pair_type.type);
    ASSERT_FLOAT_EQ(20.0, getted_order->price);
    ASSERT_FLOAT_EQ(60.0, getted_order->value);
}

TEST(order_handler, make_match){
    auto user_handler = UserHandler::get_instance();
    user_handler->add_user(User{"fume_1", "123"});
    user_handler->add_user(User{"fume_2", "123"});
    user_handler->add_user(User{"fume_3", "123"});

    auto order_handler = OrderHandler::get_instance();
    Order order_1, order_2, order_3;
    order_1.user_id = "fume_1";
    order_1.order_pair_type = OrderPairType{"USD", "RUB", "BUY"};
    order_1.price = 62.0;
    order_1.value = 10.0;

    order_2.user_id = "fume_2";
    order_2.order_pair_type = OrderPairType{"USD", "RUB", "BUY"};
    order_2.price = 63.0;
    order_2.value = 20.0;

    order_3.user_id = "fume_3";
    order_3.order_pair_type = OrderPairType{"USD", "RUB", "SELL"};
    order_3.price = 61.0;
    order_3.value = 50.0;


    order_handler->add_order(order_1);
    order_handler->add_order(order_2);
    order_handler->add_order(order_3);

    ASSERT_EQ(0, order_handler->get_orders_by_pair({"USD", "RUB", "BUY"}).value().get().size());

    auto getted_order_list = order_handler->get_orders_by_pair({"USD", "RUB", "SELL"});
    ASSERT_EQ(true, getted_order_list.has_value());
    ASSERT_EQ(1, getted_order_list->get().size());

    auto res_order = getted_order_list->get().top();
    ASSERT_EQ("fume_3", res_order->user_id);
    ASSERT_EQ("USD", res_order->order_pair_type.source);
    ASSERT_EQ("RUB", res_order->order_pair_type.target);
    ASSERT_EQ("SELL", res_order->order_pair_type.type);
    ASSERT_FLOAT_EQ(61.0, res_order->price);
    ASSERT_FLOAT_EQ(20.0, res_order->value);
}