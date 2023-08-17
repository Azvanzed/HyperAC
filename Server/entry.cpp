#include <iostream>
#include <cstdint>
#include <boost/asio.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <db.hpp>
#include <workers.hpp>
#include <unistd.h>
#include <files.hpp>

#include <packets.hpp>
#include <handlers.hpp>

using boost::asio::ip::tcp;

void acceptRequest(tcp::acceptor& acceptor) {
    acceptor.async_accept([&](const boost::system::error_code&, tcp::socket socket) {
            handlers::Dispatcher(socket);
            socket.close();
            acceptRequest(acceptor);
        }
    );
}

hello, its wcscpy, can you push all the code you did. im bored here and i want to read it

int main() {
    workers::createWorkers(1);

    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8881));

    acceptRequest(acceptor);
    io_service.run();
    return 0;
}
