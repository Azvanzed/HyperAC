#pragma once

#include <cstdint>

template <typename T>
class vector {
private:
    T* m_storage;
    size_t m_size;
    size_t m_capacity;

public:
    vector() : m_storage(nullptr), m_size(0), m_capacity(0) {}

    ~vector() {
        Clear();
        if (m_storage) {
            mmu::Free(m_storage);
        }
    }

    T* Reserve() {
        if (m_size >= m_capacity) {
            m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
            T* new_storage = (T*)mmu::allocateEx(m_capacity * sizeof(T));
            if (m_storage) {
                crt::memcpy(new_storage, m_storage, m_size * sizeof(T));
                mmu::Free(m_storage);
            }

            m_storage = new_storage;
        }

        return &m_storage[m_size++];
    }

    size_t getCapacity() const {
        return m_capacity;
    }

    size_t getSize() const {
        return m_size;
    }

    bool isEmpty() const {
        return m_size == 0;
    }

    T* getStorage() {
        return m_storage;
    }

    T* pushBack(const T& value) {
        T* element = Reserve();
        *element = value;
        return element;
    }

    void Pop() {
        if (m_size > 0) {
            --m_size;
            m_storage[m_size].~T();
        }
    }

    void Clear() {
        for (size_t i = 0; i < m_size; ++i) {
            m_storage[i].~T();
        }
        m_size = 0;
    }

    T& operator[](size_t index) {
        return m_storage[index];
    }

    size_t Find(const T& element) {
        for (size_t i = 0; i < getSize(); ++i) {
            if (m_storage[i] == element) {
                return i;
            }
        }

        return -1;
    }

    void Delete(size_t index) {
        if (index < m_size) {
            m_storage[index].~T();
            for (size_t i = index; i < m_size - 1; ++i) {
                m_storage[i] = m_storage[i + 1];
            }
            --m_size;
        }
    }
};