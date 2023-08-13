#include "server.hpp"
#include <iostream>
#include <boost/asio.hpp>
int server::start() {
    using boost::asio::ip::tcp;
    try {
        boost::asio::io_context io_context;

        // Resolver to convert host and port to a TCP endpoint
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("hyper-ac.com", "8080");

        // TCP socket
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server at 127.0.0.1:8080\n";

        // Send a request to the server
        std::string request = "Hello from client!\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        // Read the response from the server
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\n");
        std::string response_data = boost::asio::buffer_cast<const char*>(response.data());

        std::cout << "Received from server: " << response_data << "\n";
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
