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
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if(vm.count("help")){
            std::cout << desc << "\n";
            //std::cout << "Example auth:\n client -u nickname -p password\n";
            return 0;
        } 

        std::string user_id, password;
        if(vm.count("user") && vm.count("pass")){  
            std::cout << "Login as " << vm["user"].as<std::string>();

            App app(AppData{vm["user"].as<string>(), vm["pass"].as<string>()});
            app.run();
            return 0;
        } else if(vm.count("add-user") && vm.count("pass")){
            std::cout << "User added: " << vm["add-user"].as<std::string>() << " " << vm["pass"].as<std::string>();
            return 0;
        }
    } catch(std::exception& e){
        std::cerr << "[ERROR] " << e.what() << std::endl;
        std::cerr << "Unrecongized. Try --help\n";
        return 1;
    }

    return 0;
}