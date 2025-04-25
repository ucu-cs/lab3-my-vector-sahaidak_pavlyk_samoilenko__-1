// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef MY_ARRAY_HPP
#define MY_ARRAY_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <compare>
#include <utility>

template<typename T, std::size_t N>
class MyArray {
    T data_[N > 0 ? N : 1];

public:
    MyArray() {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = T();
        }
    }

    template<typename InputIt>
    MyArray(InputIt first, InputIt last) {
            size_t i = 0;
            for (; first != last && i < N; ++first, ++i)
                data_[i] = *first;
            for (; i < N; ++i)
                data_[i] = T();
        }


    MyArray(std::initializer_list<T> init) {
        std::size_t count = std::min(init.size(), N);
        std::copy_n(init.begin(), count, data_);
        for (std::size_t i = count; i < N; ++i) {
            data_[i] = T();
        }
    }

    MyArray(const MyArray& other) {
        for (std::size_t i = 0; i < N; ++i) {
            data_[i] = other.data_[i];
        }
    }

    MyArray(MyArray&& other) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            data_[i] = std::move(other.data_[i]);
        }
    }

    MyArray& operator=(const MyArray& other) {
        if (this != &other) {
            for (std::size_t i = 0; i < N; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    MyArray& operator=(MyArray&& other) noexcept {
        if (this != &other) {
            for (std::size_t i = 0; i < N; ++i) {
                data_[i] = std::move(other.data_[i]);
            }
        }
        return *this;
    }

    ~MyArray() {}

    T& operator[](std::size_t index) { return data_[index]; }
    const T& operator[](std::size_t index) const { return data_[index]; }

    T& at(std::size_t index) {
        if (index >= N) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    const T& at(std::size_t index) const {
        if (index >= N) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    T& front() { return data_[0]; }
    const T& front() const { return data_[0]; }

    T& back() { return data_[N - 1]; }
    const T& back() const { return data_[N - 1]; }

    constexpr std::size_t size() const noexcept { return N; }
    constexpr bool is_empty() const noexcept { return N == 0; }

    T* begin() { return data_; }
    T* end() { return data_ + N; }

    const T* begin() const { return data_; }
    const T* end() const { return data_ + N; }

    const T* cbegin() const { return data_; }
    const T* cend() const { return data_ + N; }

    std::reverse_iterator<T*> rbegin() { return std::reverse_iterator<T*>(end()); }
    std::reverse_iterator<T*> rend() { return std::reverse_iterator<T*>(begin()); }

    std::reverse_iterator<const T*> rbegin() const { return std::reverse_iterator<const T*>(end()); }
    std::reverse_iterator<const T*> rend() const { return std::reverse_iterator<const T*>(begin()); }

    std::reverse_iterator<const T*> rcbegin() const { return std::reverse_iterator<const T*>(cend()); }
    std::reverse_iterator<const T*> rcend() const { return std::reverse_iterator<const T*>(cbegin()); }

    void swap(MyArray& other) noexcept(std::is_nothrow_swappable<T>::value) {
        for (std::size_t i = 0; i < N; ++i) {
            std::swap(data_[i], other.data_[i]);
        }
    }

    bool operator==(const MyArray& other) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (!(data_[i] == other.data_[i]))
                return false;
        }
        return true;
    }

    auto operator<=>(const MyArray& other) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (auto cmp = data_[i] <=> other.data_[i]; cmp != 0) {
                return cmp;
            }
        }
        return std::strong_ordering::equal;
    }
};

#endif // MY_ARRAY_HPP
