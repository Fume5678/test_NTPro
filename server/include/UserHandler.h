#ifndef USERHANDLER_H
#define USERHANDLER_H
#include <map>
#include <string>
#include <optional>
#include <map>

class User {
public:
    using BalanceMap  = std::map<std::string, double>;

    User() = default;
    User(std::string user_id);

    std::string get_user_id() const;
    double get_balance_by_curr(std::string currency) const;
    BalanceMap& get_balance();
    void change_balance(std::string currency, double balance);

private:
    std::string user_id;
    BalanceMap balance; 
};


class UserHandler {
public:
    using UserMap = std::map<std::string, User>;

    static UserHandler* get_instance();
    void add_user(const std::string& user_id);
    void update_user(const User& user);
    std::optional<std::reference_wrapper<User>> get_user(const std::string& user_id);
    std::map<std::string, User> get_users();

    bool verify_user(const std::string& user_id);

private:
    mutable UserMap users;
};

#endif // USERHANDLER_H
