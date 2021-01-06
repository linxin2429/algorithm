cmake_minimum_required(VERSION 3.15)
project(algorithm)

set(CMAKE_CXX_STANDARD 11)

add_executable(algorithm main.cpp FUNDAMENTALS/BinarySearch.h utils/utils.h FUNDAMENTALS/Bag.h FUNDAMENTALS/UF.h FUNDAMENTALS/random.h SORTING/sort.h SORTING/PQ.h)