cmake_minimum_required(VERSION 3.17)
project(kasai)

set(CMAKE_CXX_STANDARD 20)

add_executable(kasai main.cpp Primal.cpp Primal.h Prototype.cpp Prototype.h typed_Prototype.cpp typed_Prototype.h Substring.cpp Substring.h Tally.cpp Tally.h Table.cpp Table.h list.cpp list.h Feed.cpp Feed.h Splitter.cpp Splitter.h Event.cpp Event.h File.cpp File.h member_Prototype.cpp member_Prototype.h Utility.h Prototype_Engine.cpp Prototype_Engine.h Pattern.cpp Pattern.h Query.cpp Query.h StrQuery.cpp StrQuery.h)
target_link_libraries(kasai)

