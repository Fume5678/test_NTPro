#include <ctime>
#include <iostream>
#include <string>
#include <chrono>
#include <boost/asio.hpp>

#include <Server.h>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;


int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context io_context;
        Server                  server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << endl;
    }
    return 0;
}