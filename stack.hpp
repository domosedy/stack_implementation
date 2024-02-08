#ifndef STACK_HPP_
#define STACK_HPP_

#include <memory>
#include <stdexcept>
#include <utility>

template <typename T, typename Alloc = std::allocator<T>>
class stack {
    T *m_data;
    Alloc m_alloc{};
    std::size_t m_capacity;
    std::size_t m_size;

    void swap(stack &other) {
        std::swap(m_data, other.m_data);
        std::swap(m_alloc, other.m_alloc);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
    }

public:
    stack() : m_data(nullptr), m_size(0), m_capacity(0) {
    }

    stack(const stack &other)
        : m_alloc(other.m_alloc),
          m_capacity(other.m_capacity),
          m_size(other.m_size) {
        m_data = m_alloc.allocate(m_size);

        for (std::size_t i = 0; i < other.m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    stack(stack &&other)
        : m_data(std::exchange(other.m_data, nullptr)),
          m_capacity(std::exchange(other.m_capacity, 0)),
          m_size(std::exchange(other.m_size, 0)),
          m_alloc(std::move(other.m_alloc)) {
    }

    stack &operator=(const stack &other) {
        if (this != &other) {
            stack tmp(other);
            swap(tmp);
        }

        return *this;
    }

    stack &operator=(stack &&other) {
        if (this != &other) {
            stack tmp(std::move(other));
            swap(tmp);
        }

        return *this;
    }

    ~stack() {
        m_alloc.deallocate(m_data, m_capacity);
    }

    void push(T new_element) {
        if (m_capacity == 0) {
            m_data = m_alloc.allocate(1);
            m_size = 0;
            m_capacity = 1;
        }

        if (m_size == m_capacity) {
            auto new_data = m_alloc.allocate(m_capacity * 2);
            for (std::size_t i = 0; i < m_size; ++i) {
                new_data[i] = std::move(m_data[i]);
            }

            m_alloc.deallocate(m_data, m_capacity);
            m_data = new_data;
            m_capacity *= 2;
        }

        m_data[m_size] = std::move(new_element);
        ++m_size;
    }

    T pop() {
        if (m_size == 0) {
            throw std::out_of_range("Stack is empty");
        }

        auto value = std::move(m_data[m_size - 1]);
        --m_size;

        return value;
    }

    T top() const {
        if (m_size == 0) {
            throw std::out_of_range("Stack is empty");
        }

        return m_data[m_size - 1];
    }

    std::size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }
};

#endif