cmake_minimum_required(VERSION 3.24)
project(TietoEvryTask)

set(CMAKE_CXX_STANDARD 17)

add_executable(TietoEvryTask main.cpp Grep.cpp Grep.h ThreadPool.cpp ThreadPool.h)
