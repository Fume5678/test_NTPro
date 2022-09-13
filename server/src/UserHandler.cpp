#include <UserHandler.h>
#include <exception>
#include <iostream>

using namespace std;

User::User(std::string user_id) : user_id{user_id} {
    balance = {{"RUB", 500.0}, {"USD", 500.0}};
}

double User::get_balance_by_curr(std::string currency) const{
    try{
        return balance.at(currency);
    } catch(std::exception& e){
        return 0.0;
    }
}

User::BalanceMap& User::get_balance() {
    return balance;
}

void User::change_balance(std::string currency, double val) {
    try{
        balance.at(currency) += val;
    } catch (std::exception& e){
        balance.insert({currency, val});
    }
}

std::string User::get_user_id() const{
    return user_id;
}

UserHandler* UserHandler::get_instance() {
    static UserHandler s;
    return &s;
}

void UserHandler::add_user(const std::string& user_id) {
    User new_user{user_id};
    if (users.find(user_id) != users.end()) {
        std::cout << "[INFO] user " << user_id << " already exists"
                  << std::endl;
    } else {
        users.insert({user_id, new_user});
        std::cout << "[INFO] user " << user_id << " added " << std::endl;
    }
}

void UserHandler::update_user(const User& user) {
    if (users.find(user.get_user_id()) != users.end()) {
        std::cout << "[INFO] user " << user.get_user_id() << " updated " << std::endl;
    } else {
        std::cout << "[INFO] user " << user.get_user_id() << " not found "
                  << std::endl;
    }
}

optional<std::reference_wrapper<User>> UserHandler::get_user(const string& user_id) {
    try {
        return users.at(user_id);
    } catch (out_of_range&) {
        std::cout << "[INFO] user " << user_id << " not found " << std::endl;
        return nullopt;
    }
}

map<string, User> UserHandler::get_users() {
    return users;
}

bool UserHandler::verify_user(const std::string& user_id) {
    if (users.find(user_id) != users.end()) {
        return true;
    } else {
        return false;
    }
}
