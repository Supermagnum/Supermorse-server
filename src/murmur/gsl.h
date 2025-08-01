// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_GSL_H_
#define MUMBLE_MURMUR_GSL_H_

#include <array>
#include <cstddef>
#include <vector>

namespace gsl {

/**
 * @brief A simplified implementation of gsl::span
 * 
 * This is a minimal implementation of the Guidelines Support Library's span type,
 * which is a non-owning view of a contiguous sequence. It's similar to std::span
 * in C++20 but available for earlier C++ versions.
 */
template <typename T>
class span {
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using size_type = std::size_t;
    using iterator = pointer;
    using const_iterator = const T*;

    // Default constructor
    constexpr span() noexcept : data_(nullptr), size_(0) {}

    // Constructor from pointer and size
    constexpr span(pointer ptr, size_type count) noexcept : data_(ptr), size_(count) {}

    // Constructor from array
    template <std::size_t N>
    constexpr span(T (&arr)[N]) noexcept : data_(arr), size_(N) {}

    // Constructor from std::array
    template <std::size_t N>
    constexpr span(std::array<T, N>& arr) noexcept : data_(arr.data()), size_(N) {}

    // Constructor from vector
    constexpr span(std::vector<T>& v) noexcept : data_(v.data()), size_(v.size()) {}

    // Access methods
    constexpr reference operator[](size_type idx) const { return data_[idx]; }
    constexpr pointer data() const noexcept { return data_; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    // Iterator methods
    constexpr iterator begin() const noexcept { return data_; }
    constexpr iterator end() const noexcept { return data_ + size_; }
    constexpr const_iterator cbegin() const noexcept { return data_; }
    constexpr const_iterator cend() const noexcept { return data_ + size_; }

private:
    pointer data_;
    size_type size_;
};

} // namespace gsl

#endif // MUMBLE_MURMUR_GSL_H_