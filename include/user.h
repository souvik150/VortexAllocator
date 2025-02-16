#ifndef USER_H
#define USER_H

#include "arena.h"

extern Arena<1024> user_arena;

class User {
public:
    static void* operator new(size_t size) {
        return user_arena.allocate(size);
    }

    static void operator delete(void* p) {
        user_arena.deallocate(static_cast<std::byte*>(p), sizeof(User));
    }

    static void* operator new[](size_t size) {
        return user_arena.allocate(size);
    }

    static void operator delete[](void* p, size_t size) {
        user_arena.deallocate(static_cast<std::byte*>(p), size);
    }

private:
    int id_{};
};

#endif // USER_H
