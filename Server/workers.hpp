#pragma once

#include <cstdint>
#include <queue.hpp>
#include <atomic>
#include <thread>

namespace workers {
    using handler_t = bool(*)(void*, void**);

    struct work_t {
        bool is_critical;

        handler_t handler;

        size_t input_size;
        void* input;

        void** output;
    };


    inline std::vector<std::thread> g_workers;
    inline std::atomic<bool> g_running;

    inline queue<work_t> g_works;

    void queueWork(void* handler, void* input, size_t input_size, void** output = nullptr);

    void workerThread();

    void createWorkers(size_t count);

    void killWorkers();
}