#include <iostream>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // TCP acceptor to listen on port 8080
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "Server is listening on port 8080\n";

        // Infinite loop to accept connections
        for (;;) {
            // Socket to manage the connection
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Connection accepted from: " << socket.remote_endpoint() << "\n";

            // Read data from the client
            boost::asio::streambuf buf;
            boost::asio::read_until(socket, buf, "\n");
            std::string data = boost::asio::buffer_cast<const char*>(buf.data());

            std::cout << "Received from client: " << data << "\n";

            // Send response back to the client
            std::string response = "Hello from server!\n";
            boost::asio::write(socket, boost::asio::buffer(response));

            std::cout << "Response sent to client\n";
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
