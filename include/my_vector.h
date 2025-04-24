#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <initializer_list>
#include <compare>
#include <concepts>
#include <cstddef>
#include <new>
#include <utility>

template<typename T>
class MyVector {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    T* data_;
    size_type size_;
    size_type capacity_;

public:
    MyVector() noexcept : data_(nullptr), size_(0), capacity_(0) {}


    MyVector(size_type count, const T& value)
        : data_(nullptr), size_(0), capacity_(0) {
        reserve(count);
        for (size_type i = 0; i < count; ++i)
            new (&data_[i]) T(value);
        size_ = count;
    }


    template<std::input_iterator InputIt>
    MyVector(InputIt first, InputIt last)
        : data_(nullptr), size_(0), capacity_(0) {
        auto dist = std::distance(first, last);
        size_type count = static_cast<size_type>(dist);
        reserve(count);
        for (auto it = first; it != last; ++it)
            new (&data_[size_++]) T(*it);
    }


    MyVector(std::initializer_list<T> init)
        : data_(nullptr), size_(0), capacity_(0) {
        reserve(init.size());
        for (const auto& v : init)
            new (&data_[size_++]) T(v);
    }


    MyVector(const MyVector& other)
        : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; ++i)
            new (&data_[i]) T(other.data_[i]);
        size_ = other.size_;
    }


    MyVector(MyVector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }


    ~MyVector() {
        clear();
        ::operator delete(data_);
    }


    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            data_ = nullptr;
            size_ = capacity_ = 0;
            reserve(other.size_);
            for (size_type i = 0; i < other.size_; ++i)
                new (&data_[i]) T(other.data_[i]);
            size_ = other.size_;
        }
        return *this;
    }


    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    reference operator[](size_type index) noexcept {
        return data_[index];
    }
    const_reference operator[](size_type index) const noexcept {
        return data_[index];
    }

    reference at(size_type index) {
        if (index >= size_) throw std::out_of_range("MyVector::at");
        return data_[index];
    }
    const_reference at(size_type index) const {
        if (index >= size_) throw std::out_of_range("MyVector::at");
        return data_[index];
    }

    reference front() {
        if (is_empty()) throw std::out_of_range("MyVector::front");
        return data_[0];
    }
    const_reference front() const {
        if (is_empty()) throw std::out_of_range("MyVector::front");
        return data_[0];
    }

    reference back() {
        if (is_empty()) throw std::out_of_range("MyVector::back");
        return *(end() - 1);
    }

    const_reference back() const {
        if (is_empty()) throw std::out_of_range("MyVector::back");
        return *(end() - 1);
    }

    iterator begin() noexcept
    {
        return iterator(data_);
    }

    iterator end() noexcept
    {
        return iterator(data_+size_);
    }
    const_iterator begin() const noexcept { return data_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    bool is_empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }

    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        pointer new_data = static_cast<pointer>(::operator new(new_cap * sizeof(T)));
        for (size_type i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move_if_noexcept(data_[i]));
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_cap;
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            pointer new_data = nullptr;
            if (size_) {
                new_data = static_cast<pointer>(::operator new(size_ * sizeof(T)));
                for (size_type i = 0; i < size_; ++i) {
                    new (&new_data[i]) T(std::move_if_noexcept(data_[i]));
                    data_[i].~T();
                }
            }
            ::operator delete(data_);
            data_ = new_data;
            capacity_ = size_;
        }
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i)
            data_[i].~T();
        size_ = 0;
    }

    void resize(size_type count, const T& value = T()) {
        if (count < size_) {
            for (size_type i = count; i < size_; ++i)
                data_[i].~T();
        } else if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i)
                new (&data_[i]) T(value);
        }
        size_ = count;
    }

    void swap(MyVector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    iterator insert(const_iterator pos, const T& value) {
        size_type idx = pos - data_;
        if (pos < data_ || pos > end()) throw std::out_of_range("insert");
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
        for (size_type i = size_; i > idx; --i) {
            new (&data_[i]) T(std::move_if_noexcept(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (&data_[idx]) T(value);
        ++size_;
        return data_ + idx;
    }

    template<std::input_iterator InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type idx = pos - data_;
        size_type count = static_cast<size_type>(std::distance(first, last));
        if (pos < data_ || pos > end()) throw std::out_of_range("insert range");
        if (size_ + count > capacity_) reserve(std::max(capacity_ * 2, size_ + count));
        for (size_type i = size_; i > idx; --i) {
            new (&data_[i + count - 1]) T(std::move_if_noexcept(data_[i - 1]));
            data_[i - 1].~T();
        }
        for (auto it = first; it != last; ++it)
            new (&data_[idx++]) T(*it);
        size_ += count;
        return data_ + (idx - count);
    }

    iterator erase(const_iterator pos) {
        if (pos < data_ || pos >= end()) return end();
        size_type idx = pos - data_;
        data_[idx].~T();
        for (size_type i = idx; i < size_ - 1; ++i) {
            new (&data_[i]) T(std::move_if_noexcept(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
        return data_ + idx;
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first < data_ || last > end() || first > last) return end();
        size_type idx = first - data_;
        size_type count = last - first;
        for (size_type i = 0; i < count; ++i)
            data_[idx + i].~T();
        for (size_type i = idx + count; i < size_; ++i) {
            new (&data_[i - count]) T(std::move_if_noexcept(data_[i]));
            data_[i].~T();
        }
        size_ -= count;
        return data_ + idx;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
        new (&data_[size_]) T(value);
        ++size_;
    }

    void push_back(T&& value)
    {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
        new (&data_[size_]) T(std::move(value));
        ++size_;
    }

    void pop_back() noexcept {
        if (is_empty()) return;
        data_[size_ - 1].~T();
        --size_;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
        new (&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
    }

    auto operator<=>(const MyVector& other) const {
        return std::lexicographical_compare_three_way(
            begin(), end(), other.begin(), other.end());
    }

    bool operator==(const MyVector& other) const {
        if (size_ != other.size_) return false;
        for (size_type i = 0; i < size_; ++i)
            if (!(data_[i] == other.data_[i])) return false;
        return true;
    }
};

#endif // MY_VECTOR_H
