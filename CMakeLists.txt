cmake_minimum_required(VERSION 3.17)
project(kasai)

set(CMAKE_CXX_STANDARD 20)

add_executable(kasai main.cpp Primal.cpp Primal.h Prototype.cpp Prototype.h typed_Prototype.cpp typed_Prototype.h Substring.cpp Substring.h Tally.cpp Tally.h Pattern.cpp Pattern.h Table.cpp Table.h list.cpp list.h)
target_link_libraries(kasai)

