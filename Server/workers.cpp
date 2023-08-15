#include <workers.hpp>

void workers::queueWork(void* handler, void* input, size_t input_size, void* output, size_t* output_size) {
    work_t work;
    work.handler = (handler_t)handler;
    work.is_critical = output != nullptr;
    work.input = input;
    work.input_size = input_size;
    work.output = output;
    work.output_size = output_size;

    g_works.Push(work, work.is_critical);
    while (work.is_critical && *output_size == 0);
}

void workers::workerThread() {
    while (g_works.getSize() > 0) {
        work_t work;
        while (g_works.Pop(work) ) {
            work.handler(work.input, work.output, work.output_size);
        }
    }
}

void workers::createWorkers(size_t count) {
    g_running.store(true);

    for (size_t i = 0; i < count; ++i) {
        g_workers.push_back(std::thread(&workerThread));
    }
}

void workers::killWorkers() {
    g_running.store(false);

    for (std::thread& worker : g_workers) {
        worker.join();
    }
}