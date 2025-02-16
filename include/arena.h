#ifndef ARENA_H
#define ARENA_H

#include <cstddef>
#include <cstdint>
#include <new>
#include <iostream>

template <size_t N>
class Arena {
    static constexpr size_t alignment = alignof(std::max_align_t);

public:
    Arena() noexcept : ptr_(buffer_) {}
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    void reset() noexcept { ptr_ = buffer_; }
    static constexpr size_t size() noexcept { return N; }
    size_t used() const noexcept { return static_cast<size_t>(ptr_ - buffer_); }

    std::byte* allocate(size_t n);
    void deallocate(std::byte* p, size_t n) noexcept;

private:
    static size_t align_up(size_t n) noexcept {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }

    bool pointer_in_buffer(const std::byte* p) const noexcept {
        return reinterpret_cast<std::uintptr_t>(p) >= reinterpret_cast<std::uintptr_t>(buffer_) &&
               reinterpret_cast<std::uintptr_t>(p) < reinterpret_cast<std::uintptr_t>(buffer_) + N;
    }

    alignas(alignment) std::byte buffer_[N];
    std::byte* ptr_;
};

template <size_t N>
std::byte* Arena<N>::allocate(size_t n) {
    const auto aligned_n = align_up(n);
    const auto available_bytes = static_cast<size_t>(buffer_ + N - ptr_);

    if (available_bytes >= aligned_n) {
        auto* r = ptr_;
        ptr_ += aligned_n;
        std::cout << "[Arena] Allocated " << aligned_n << " bytes. Used: " << used() << " bytes.\n";
        return r;
    }

    std::cout << "[Arena] Fallback to heap for " << n << " bytes.\n";
    return static_cast<std::byte*>(::operator new(n));
}

template <size_t N>
void Arena<N>::deallocate(std::byte* p, size_t n) noexcept {
    if (pointer_in_buffer(p)) {
        n = align_up(n);
        if (p + n == ptr_) {
            ptr_ = p;
        }
        std::cout << "[Arena] Deallocated " << n << " bytes. Remaining: " << used() << " bytes.\n";
    } else {
        std::cout << "[Arena] Heap deallocation for external memory.\n";
        ::operator delete(p);
    }
}

#endif // ARENA_H
