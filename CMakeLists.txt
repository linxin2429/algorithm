cmake_minimum_required(VERSION 3.15)
project(algorithm)

set(CMAKE_CXX_STANDARD 11)

add_executable(algorithm main.cpp FUNDAMENTALS/BinarySearch.h utils/utils.h FUNDAMENTALS/Bag.h FUNDAMENTALS/UF.h FUNDAMENTALS/random.h SORTING/sort.h SORTING/PQ.h SEARCHING/ST.h SEARCHING/HashST.h SEARCHING/Tree.h GRAPHS/Graph.h GRAPHS/Digraph.h GRAPHS/EdgeWeightedGraph.h GRAPHS/SP.h STRINGS/stringsort.h STRINGS/Trie.h)