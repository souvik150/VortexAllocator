#include "user.h"
#include "short_alloc.h"
#include <iostream>
#include <memory>
#include <set>

int main() {
    auto user1 = new User{};
    delete user1;

    auto users = new User[10];
    delete[] users;

    auto user2 = std::make_unique<User>();
    auto user3 = std::make_shared<User>();

    using SmallSet = std::set<int, std::less<int>, ShortAlloc<int, 512>>;

    auto stack_arena = SmallSet::allocator_type::arena_type{};
    auto unique_numbers = SmallSet{stack_arena};

    auto n = int{};
    while (std::cin >> n) unique_numbers.insert(n);

    for (const auto& number : unique_numbers) {
        std::cout << number << '\n';
    }

    return 0;
}
