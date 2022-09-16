#ifndef USERHANDLER_H
#define USERHANDLER_H
#include <map>
#include <string>
#include <optional>
#include <memory>
#include <map>

class User {
public:
    using BalanceMap  = std::map<std::string, float>;

    User() = default;
    User(std::string user_id, std::string password);

    std::string get_user_id() const;
    std::string get_password() const;
    double get_balance_by_curr(std::string currency) const;
    BalanceMap& get_balance();
    void change_balance(std::string currency, float balance);

    friend bool operator==(const User& lhs, const User& rhs) {
        return lhs.user_id == rhs.user_id;
    }

private:
    std::string user_id;
    std::string password;
    BalanceMap balance; 
};


class UserHandler {
public:
    using UserMap = std::map<std::string, User>;

    static std::shared_ptr<UserHandler> get_instance();
    void add_user(User&& user);
    std::optional<std::reference_wrapper<User>> get_user(const std::string& user_id);
    std::map<std::string, User> get_users();

    bool verify_user(const User& user);

private:
    mutable UserMap users;
};

#endif // USERHANDLER_H
