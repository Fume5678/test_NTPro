#include <ConHandler.h>
#include <httpparser/request.h>
#include <httpparser/httprequestparser.h>

ConHandler::ConHandler(boost::asio::io_context& io_context)
    : sock(io_context) {}

ConHandler::pointer ConHandler::create(boost::asio::io_context& io_context) {
    return pointer(new ConHandler(io_context));
}

tcp::socket& ConHandler::socket() {
    return sock;
}

void ConHandler::start() {                                          
    sock.async_read_some(
        boost::asio::buffer(buffer, max_length),
        boost::bind(&ConHandler::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void ConHandler::handle_read(const boost::system::error_code& err,
                             size_t bytes_transferred) {
    request_msg = buffer;
    if (!err) {
        cout << "[INFO] Request:\n" << request_msg << endl << endl;
    } else {
        std::cerr << "[ERROR]: " << err.message() << endl << endl;
        sock.close();
    }

    httpparser::Request request;
    httpparser::HttpRequestParser parser;
    httpparser::HttpRequestParser::ParseResult res = parser.parse(request, buffer, buffer + 1024);

    if( res == httpparser::HttpRequestParser::ParsingCompleted )
    {     
        response_msg = req_handler.get_response(request);
    } else {
        response_msg = "HTTP/1.1 400 BadRequest\r\n\r\n";
    }

    sock.async_write_some(
        boost::asio::buffer(response_msg, max_length),
        boost::bind(&ConHandler::handle_write,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void ConHandler::handle_write(const boost::system::error_code& err,
                              size_t bytes_transferred) {
    if (!err) {
        cout << "[INFO] Responce:\n" << response_msg << endl << endl;
    } else {
        std::cerr << "[ERROR]: " << err.message() << endl << endl;
        sock.close();
    }
}
