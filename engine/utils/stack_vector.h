//
// Created by Neirokan on 14.02.2024.
//

#ifndef UTILS_STACK_VECTOR_H
#define UTILS_STACK_VECTOR_H

#include <cstdint>
#include <utility>
#include <stdexcept>

// Kinda std::vector, but with fixed max capacity and allocated on stack
template<class T, size_t N>
class stack_vector {
public:
    // Just trying to be compatible with standard vector
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = pointer;
    using const_iterator = const_pointer;

private:
    size_type _size = 0;
    alignas(value_type) uint8_t _buff[N * sizeof(value_type)];
std::vector<int> a;
public:
    stack_vector() = default;

    stack_vector(const stack_vector& other) : _size(other._size) {
        pointer arr1 = reinterpret_cast<pointer>(_buff);
        pointer arr2 = reinterpret_cast<pointer>(_buff);
        for (size_type i = 0; i < _size; i++) {
            arr1[i] = value_type(arr2[i]);
        }
    };

    ~stack_vector() {
        clear();
    }

    template <class... Args>
    constexpr decltype(auto) emplace_back(Args&&... args) {
#ifndef NDEBUG
        if (_size == N) throw std::bad_alloc();
#endif
        uint8_t* address = _buff + _size * sizeof(value_type);
        const_pointer result = new(address) value_type(std::forward<Args>(args)...);
        _size++;
        return *result;
    }

    void push_back(const_reference value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    void pop_back() {
#ifndef NDEBUG
        if (empty()) throw std::domain_error("Stack vector underflow");
#endif
        pointer it = reinterpret_cast<pointer>(_buff) + --_size;
        it->~T();
    }

    void clear() {
        pointer it = reinterpret_cast<pointer>(_buff) + _size;
        while (_size-- > 0) {
            (--it)->~T();
        }
    }

    [[nodiscard]] inline reference operator[](size_t index) {
#ifndef NDEBUG
        if (index >= _size) throw std::out_of_range("Out of range of stack vector");
#endif
        return *(reinterpret_cast<pointer>(_buff) + index);
    }

    [[nodiscard]] inline const_reference operator[](size_t index) const {
#ifndef NDEBUG
        if (index >= _size) throw std::out_of_range("Out of range of stack vector");
#endif
        return *(reinterpret_cast<pointer>(_buff) + index);
    }

    [[nodiscard]] inline bool empty() const noexcept { return _size == 0; }
    [[nodiscard]] inline size_type size() const noexcept { return _size; }
    [[nodiscard]] inline size_type capacity() const noexcept { return N; }

    [[nodiscard]] inline iterator begin() noexcept { return reinterpret_cast<pointer>(_buff); }
    [[nodiscard]] inline const_iterator begin() const noexcept { return reinterpret_cast<const_pointer>(_buff); }

    [[nodiscard]] inline iterator end() noexcept { return begin() + _size; }
    [[nodiscard]] inline const_iterator end() const noexcept { return begin() + _size; }

    [[nodiscard]] inline reference front() { return (*this)[0]; }
    [[nodiscard]] inline const_reference front() const { return (*this)[0]; }

    [[nodiscard]] inline reference back() { return (*this)[_size - 1]; }
    [[nodiscard]] inline const_reference back() const { return (*this)[_size - 1]; }
};

#endif //UTILS_STACK_VECTOR_H
