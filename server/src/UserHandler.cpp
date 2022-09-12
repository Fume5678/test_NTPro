#include <UserHandler.h>

using namespace std;

UserHandler* UserHandler::get_instance() {
    static UserHandler s;
    return &s;
}

void UserHandler::add_user(const User& user) {
    try{
        users.at(user.user_id);
        std::cout << "[INFO] user " << user.user_id << " already exists" << std::endl;
    } catch (out_of_range&) {
        user.balance = 500.0;
        users.insert({user.user_id, user});
        std::cout << "[INFO] user " << user.user_id << " added " << std::endl;
    }
}

void UserHandler::update_user(const User& user) {
    try{
        users.at(user.user_id) = user;
        std::cout << "[INFO] user " << user.user_id << " updated " << std::endl;
    } catch (out_of_range&) {
        std::cout << "[INFO] user " << user.user_id << " not found " << std::endl;
    }
}

optional<User> UserHandler::get_user(const string& user_id) {
    try{
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
    try{
        users.at(user_id);
        return true;
    } catch (out_of_range&) {
        return false;
    }
}

