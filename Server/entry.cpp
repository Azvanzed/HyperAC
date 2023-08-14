#include <iostream>
#include <cstdint>
#include <boost/asio.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <db.hpp>

using boost::asio::ip::tcp;

int main() {
    db::g_con = sql::mysql::get_driver_instance()->connect("tcp://127.0.0.1:3306", "wcscpy", "gahgah123");
    db::g_con->setSchema("HyperAC");

    std::vector<db::player_t> players;
    printf("find: %i\n", db::addPlayer("the best player id ever", { "hehe", "hoho", "behe", "niggi" }));
    //db::addPlayer("the best player id ever", { "the best hwid!", "hello world"});

    // INSERT INTO `players`(`id`, `hwid`, `last_play`, `flags`) VALUES ('', '', '', '')

    delete db::g_con;
    return 0;
}