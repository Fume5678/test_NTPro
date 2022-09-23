/*
    Client
*/
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <exception>
#include <iostream>
#include <boost/program_options.hpp>
#include <string>

#include <App.h>

// using namespace boost::asio;
// using ip::tcp;
// using std::string;
// using std::cout;
// using std::endl;

namespace po = boost::program_options;

const std::string SERVER_ADDR = "127.0.0.1:1234";

int main(int ac, char** av) {
    po::options_description desc("User auth");
    desc.add_options()
        ("help", "produce help message")
        ("user,u", po::value<std::string>(), "username for sign in")
        ("add-user,U", po::value<std::string>(), "username for sign up")
        ("pass,p", po::value<std::string>(), "user password")
    ;
    po::variables_map vm;

    try{
        if(ac < 2){
            throw std::invalid_argument("No arguments");
        }

        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if(vm.count("help")){
            std::cout << desc << "\n";
            return 0;
        } 

        Requests requester(SERVER_ADDR);
        std::string user_id, password;
        if(vm.count("user") && vm.count("pass")){  
            
            if(!requester.POST_verify_user(User{vm["user"].as<std::string>(), vm["pass"].as<std::string>()})){
                throw std::invalid_argument("Failed authorize");
            }
            
            std::cout << "Login as " << vm["user"].as<std::string>() << std::endl;
            App app(AppData{vm["user"].as<string>(), vm["pass"].as<string>(), SERVER_ADDR});
            app.run();

            return 0;
        } else if(vm.count("add-user") && vm.count("pass")){
            std::cout << requester.POST_add_user(
                User{vm["add-user"].as<string>(), vm["pass"].as<string>()}) << std::endl;
            return 0;
        }
        throw std::invalid_argument("Invalid arguments");
    } catch(std::exception& e){
        std::cerr << "[ERROR] " << e.what() << std::endl;
        std::cerr << "Unrecongized. Try --help\n";
        return 1;
    }

    return 0;
}

// /ao --source USD --target RUB --type SELL --value 20 --price 20
// /ao --source USD --target RUB --type BUY --value 10 --price 24