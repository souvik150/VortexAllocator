cmake_minimum_required(VERSION 3.10)
project(MemoryArenaExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(memory_arena_example 
    src/main.cpp 
    src/user.cpp 
    src/arena.cpp 
    src/short_alloc.cpp
)
