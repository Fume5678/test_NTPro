#include "Models.h"
#include <App.h>
#include <exception>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>

using namespace std;


AppData::AppData(std::string user_id, std::string password)
    : user_id{user_id},
      password{password}
{}

App::App() : data{"", ""} {}

App::App(AppData data): data(data) {}

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
                }
            } else if(cmd == "/u"){
                std::cout << " Current user: " << data.user_id << std::endl;
            } else {
                throw std::invalid_argument("Unrecognized command. Try /? for help");
            }

        } catch(std::exception& e){
            cerr << "[ERROR] " << e.what() << std::endl;
        }

    }
}

