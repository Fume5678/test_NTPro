#ifndef APP_H
#define APP_H

#include <map>
#include <string>
#include <Models.h>
#include <Requests.h>

class AppData{
public:
    std::string user_id;
    std::string password;

    AppData() = default;
    AppData(std::string user_id, std::string password);
};

class App {
    void print_help(std::string command);
    void command_handler();

    std::map<std::string, std::string> parse_command(const std::string& cmd);

    // Commands
    bool add_order();
    bool print_orders();
    // bool get_orders();
public:
    App();
    App(AppData data);

    void run();

private:
    AppData data;
    Requests requests;
};

#endif // APP_H
