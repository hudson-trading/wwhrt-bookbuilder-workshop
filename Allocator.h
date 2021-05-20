#pragma once

#include <memory>
#include <array>
#include <set>

// This memory allocator can be used as a template parameter to
// various C++ stdlib datastructures. We've provided a default
// implementation below that can be modified if desired.
template <typename T> struct Allocator {
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ssize_t;

    Allocator() {}

    [[nodiscard]] T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) { ::operator delete(p); }
};

template <class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) {
    return true;
}
template <class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) {
    return false;
}
