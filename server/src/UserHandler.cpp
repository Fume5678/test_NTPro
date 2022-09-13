#include <UserHandler.h>
#include <iostream>

using namespace std;

UserHandler* UserHandler::get_instance() {
    static UserHandler s;
    return &s;
}


void UserHandler::add_user(const std::string& user_id) {
    User new_user = {user_id, 500.0};
    if(users.find(user_id) != users.end()) {
        std::cout << "[INFO] user " << user_id << " already exists" << std::endl;
    } else {
        users.insert({user_id, new_user});
        std::cout << "[INFO] user " << user_id << " added " << std::endl;
    }
}

void UserHandler::update_user(const User& user) {
    if( users.find(user.user_id) != users.end() ) {
        std::cout << "[INFO] user " << user.user_id << " updated " << std::endl;
    } else {
        std::cout << "[INFO] user " << user.user_id << " not found " << std::endl;
    }
}

optional<User> UserHandler::get_user(const string& user_id) const{
    try{
        return users.at(user_id);
    } catch (out_of_range&) {
        std::cout << "[INFO] user " << user_id << " not found " << std::endl;
        return nullopt;
    }
}

map<string, User> UserHandler::get_users(){
    return users;
}

bool UserHandler::verify_user(const std::string& user_id) {
    if(users.find(user_id) != users.end()) {
        return true;
    } else {
        return false;
    }
}

