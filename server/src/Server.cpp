#include <Server.h>

void Server::start_accept() {
    // socket
    ConHandler::pointer connection = ConHandler::create(io_context_);

    std::cout << "[INFO] Ready for connection" << std::endl;

    // asynchronous accept operation and wait for a new connection.
    acceptor_.async_accept(connection->socket(),
                           boost::bind(&Server::handle_accept,
                                       this,
                                       connection,
                                       boost::asio::placeholders::error));
}

Server::Server(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 1234)) {

    start_accept();
}

void Server::handle_accept(ConHandler::pointer             connection,
                           const boost::system::error_code& err) {
    if (!err) {
        connection->start();
    }
    start_accept();
}