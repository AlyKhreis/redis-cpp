cmake_minimum_required(VERSION 4.2)
project(redis_cpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(redis_cpp
        main.cpp
        src/command.cpp
        src/command.h
        src/prime.cpp
        src/prime.h
        src/hash_table.cpp
        src/hash_table.h
        src/resp.cpp
        src/resp.h
        src/aof.cpp
        src/aof.h
)
