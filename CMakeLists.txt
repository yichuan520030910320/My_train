cmake_minimum_required(VERSION 3.10.2)
project(code)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_executable(code Order.cpp
        Hash.hpp
        Date.hpp
        includes.hpp
        Train.hpp
        User.hpp
        FileOperator.hpp
        bplustree.hpp
        utility.hpp
        hashmap.h
        lru.h
        )