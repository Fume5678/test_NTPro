#include <UserHandler.h>
#include <exception>
#include <iostream>

using namespace std;

User::User(std::string user_id, std::string password)
    : user_id{user_id}, password{password} {}

double User::get_balance_by_curr(std::string currency) const {
    try {
        return balance.at(currency);
    } catch (std::exception& e) {
        return 0.0;
    }
}

User::BalanceMap& User::get_balance() {
    return balance;
}

void User::change_balance(std::string currency, float val) {
    try {
        balance.at(currency) += val;
    } catch (std::out_of_range&) {
        balance.insert({currency, val});
    } catch (std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }

    return;
}

std::string User::get_user_id() const {
    return user_id;
}

std::string User::get_password() const {
    return password;
}

std::shared_ptr<UserHandler> UserHandler::get_instance() {
    static std::shared_ptr<UserHandler> s{new UserHandler};
    return s;
}

void UserHandler::add_user(User&& user) {
    if (users.find(user.get_user_id()) != users.end()) {
        std::cout << "[INFO] user " << user.get_user_id() << " already exists"
                  << std::endl;
    } else {
        users.insert({user.get_user_id(), user});
        std::cout << "[INFO] user " << user.get_user_id() << " added "
                  << std::endl;
    }
}

optional<std::reference_wrapper<User>>
    UserHandler::get_user(const string& user_id) {
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

bool UserHandler::verify_user(const User& user) {
    auto found_user = users.find(user.get_user_id());
    if (found_user != users.end()) {
        if (found_user->second.get_password() == user.get_password()) {
            return true;
        }
        return false;
    } else {
        return false;
    }
}
