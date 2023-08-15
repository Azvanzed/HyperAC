#include <iostream>
#include <cstdint>
#include <boost/asio.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <db.hpp>
#include <storage.hpp>

#include <thread>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
    tcp::socket socket_;
    std::string data_;

public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        // Begin asynchronous operations here, like reading from the socket.
    }
};

class Server {
    tcp::acceptor acceptor_;
    tcp::socket socket_;

    void waitForClientConnection() {
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket_))->start();
            }
            waitForClientConnection();
            });
    }

public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), socket_(io_context) {
        waitForClientConnection();
    }
};

void initializeThreadPool(boost::asio::io_context& io_context, std::vector<std::thread>& thread_pool) {
    for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        thread_pool.emplace_back([&io_context]() { io_context.run(); });
    }
}

int main() {
    // Initialize database
    db::g_con = sql::mysql::get_driver_instance()->connect("tcp://127.0.0.1:3306", "name", "name");
    db::g_con->setSchema("HyperAC");
    boost::asio::io_context io_context;
    Server s(io_context, 8080);
    std::vector<std::thread> thread_pool;
    initializeThreadPool(io_context, thread_pool);
    for (auto& t : thread_pool) {
        t.join();
    }

    delete db::g_con;
    return 0;
}
