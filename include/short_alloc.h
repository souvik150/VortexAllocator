#ifndef SHORT_ALLOC_H
#define SHORT_ALLOC_H

#include "arena.h"
#include <memory>

template <class T, size_t N>
struct ShortAlloc {
  using value_type = T;
  using arena_type = Arena<N>;

  ShortAlloc(const ShortAlloc&) = default;
  ShortAlloc& operator=(const ShortAlloc&) = default;

  ShortAlloc(arena_type& arena) noexcept : arena_{&arena} {}

  template <class U>
  ShortAlloc(const ShortAlloc<U, N>& other) noexcept : arena_{other.arena_} {}

  template <class U> struct rebind {
    using other = ShortAlloc<U, N>;
  };

  auto allocate(size_t n) -> T* {
    return reinterpret_cast<T*>(arena_->allocate(n*sizeof(T)));
  }

  auto deallocate(T *p, size_t n) noexcept -> void {
    arena_ -> deallocate(reinterpret_cast<std::byte*>(p), n*sizeof(T));
  }

  template<class U, size_t M>
  auto operator==(const ShortAlloc<U, M>& other) const noexcept {
    return N==M && arena_ == other.arena_;
  }

  template<class U, size_t M>
  auto operator!=(const ShortAlloc<U, M>& other) const noexcept {
    return !(*this == other);
  }

  template<class U, size_t M> friend struct ShortAlloc;

private:
  arena_type* arena_;
};

#endif // SHORT_ALLOC_H
