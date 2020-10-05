cmake_minimum_required(VERSION 3.17)
project(kasai)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT "/usr/local/include/boost")

find_package(Boost 1.63.0 COMPONENTS serialization)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(kasai main.cpp Primal.cpp Primal.h Prototype.cpp Prototype.h typed_Prototype.cpp typed_Prototype.h int_Prototype.cpp int_Prototype.h float_Prototype.cpp float_Prototype.h string_Prototype.cpp string_Prototype.h Substring.cpp Substring.h Tally.cpp Tally.h Pattern.cpp Pattern.h Table.cpp Table.h list.cpp list.h)
    target_link_libraries(kasai ${Boost_LIBRARIES})
endif()
