#include <iostream>
#include <cstdint>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <db.hpp>
#include <workers.hpp>
#include <unistd.h>

void lowTest() {
    printf("low [%i]\n", gettid());
}

void highTest() {
    printf("high [%i]\n", gettid());
}

int main() {
    printf("%i\n", std::thread::hardware_concurrency());

    workers::createWorkers(10);

    for (size_t i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            workers::queueWork((void*)&highTest, nullptr, 0);
        }
        else {
            workers::queueWork((void*)&lowTest, nullptr, 0);
        }
    }

    workers::killWorkers();

    return 0;
}