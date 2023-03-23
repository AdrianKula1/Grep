cmake_minimum_required(VERSION 3.24)
project(Grep)

set(CMAKE_CXX_STANDARD 17)

add_executable(Grep main.cpp classes/Grep.cpp classes/Grep.h classes/ThreadPool.cpp classes/ThreadPool.h)
