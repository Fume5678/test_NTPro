#include <exception>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>

#include <Models.h>
#include <Requests.h>
#include <App.h>

using namespace std;

AppData::AppData(string user_id, string password, string server_addr)
    : user_id{user_id},
      password{password},
      server_addr{server_addr}
{}

App::App() : data{"", "", ""}, requests("") {}

App::App(AppData data) : data(data), requests(data.server_addr) {}

void App::print_help(std::string command = "") {
    if(command == ""){
        std::cout << "\n Help\n";
        std::cout << "/?                  \t" << ": Command list\n";
        std::cout << "/ao <params>[--help] \t" << ": Add new order\n";
        std::cout << "/go                 \t" << ": Get user orders\n";
        std::cout << "/u                  \t" << ": Current user\n";
        std::cout << "/q                  \t" << ": Exit \n";
    }
    if(command == "/ao"){
        std::cout << "\n Add new order. Example:\n";
        std::cout << "/ao --source USD --target RUB --type SELL --value 20 --price 20.123\n";
    }

}

std::map<std::string, std::string> App::parse_command(const std::string& input) {
    map<string, string> params;
    int first_space = input.find(" ");
    first_space = first_space != string::npos ? first_space : input.length();
    string command = input.substr(0, first_space);
    params["command"] = command;

    size_t count = 0;
    string key, buf = "";
    for (size_t i = first_space+1; i <= input.length(); i++) {
        if (i == input.length() || input[i] == ' ') {
            if(!buf.empty()){
                if (auto found = buf.find("--"); found != input.npos) {
                    key = buf.substr(2, input.length());
                    params[key] = "";
                } else if (!key.empty()) {
                    params[key] = buf;
                } else { 
                    throw invalid_argument("Bad input");
                }
            }
            buf = "";
        } else {
            buf += input[i];
        }
    }

    return std::move(params);
}


void App::run() {
    std::cout << "Print /? for help\n";
    bool is_exit = false;

    while(!is_exit){
        std::cout << "\n> ";
        std::string input;
        cin.clear();

        try{
            getline(cin, input);

            std::map<string, string> params(parse_command(input));

            string cmd = params.at("command");
            if(cmd == "/?"){
                print_help();
            } else if(cmd == "/q") {
                is_exit = true;
            } else if(cmd == "/ao"){
                if(params.count("help")){
                    print_help(cmd);
                } else {
                    Order order;
                    order.user_id = data.user_id;
                    order.source  = params.at("source");
                    order.target  = params.at("target");
                    order.type    = params.at("type");
                    order.value   = stod(params.at("value"));
                    order.price   = stod(params.at("price"));

                    requests.POST_add_order(order, data.password);
                }
            }else if(cmd == "/go"){
                auto orders = requests.POST_get_orders(User{data.user_id, data.password});

                for(auto& order : orders){
                    std::cout << order << std::endl;
                }
            }
            else if(cmd == "/u"){

                auto userdetail = requests.POST_get_userdetail(User{data.user_id, data.password});
                if(userdetail.has_value()){
                    std::cout << "Current user: " << userdetail.value().user_id << std::endl;
                    std::cout << "Balance:\n";
                    for(const auto& currency: userdetail.value().balance){
                        std::cout << " {" <<  currency.first << " : " << currency.second << "}" << std::endl;
                    }
                } else {
                    std::cout << "[WARN] User not found" << std::endl;
                }
                
            } else {
                throw std::invalid_argument("Unrecognized command. Try /? for help");
            }

        } catch(std::exception& e){
            cerr << "[ERROR] " << e.what() << std::endl;
        }

    }
}

