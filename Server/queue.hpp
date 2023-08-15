#pragma once

#include <cstdint>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class queue {
private:
    std::deque<T> m_queue;
    mutable std::mutex m_mtx;
    std::condition_variable m_cond;

public:

    void Push(const T& value, bool front = false) {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (front) {
            m_queue.push_front(value);
        }
        else {
            m_queue.push_back(value);
        }

        m_cond.notify_one();
    }

    size_t getSize() const {
        std::lock_guard<std::mutex> lock(m_mtx);
        return m_queue.size();
    }

    bool Pop(T& value) {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cond.wait(lock, [this]() {
                return !m_queue.empty();
            }
        );

        if (!m_queue.empty()) {
            value = m_queue.front();
            m_queue.pop_front();
            return true;
        }

        return false;
    }
};
