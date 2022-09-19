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
            return 1;
        } 

        if(vm.count("user") && vm.count("pass")){
            std::cout << "User: " << vm["user"].as<std::string>() << " " << vm["pass"].as<std::string>();
        } else if(vm.count("add-user") && vm.count("pass")){
            std::cout << "User: " << vm["add-user"].as<std::string>() << " " << vm["pass"].as<std::string>();
        }
    } catch(std::exception&){
        std::cout << "Unrecongized. Try --help\n";
        return 1;
    }

    return 0;
}