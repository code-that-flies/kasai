cmake_minimum_required(VERSION 3.17)
project(kasai)

set(CMAKE_CXX_STANDARD 20)

add_executable(kasai main.cpp Primal.cpp Primal.h Prototype.cpp Prototype.h TypedPrototype.cpp TypedPrototype.h Subvector.cpp Subvector.h Tally.cpp Tally.h Table.cpp Table.h list.cpp list.h Feed.cpp Feed.h Splitter.cpp Splitter.h EventEngine.cpp EventEngine.h File.cpp File.h MemberPrototype.cpp MemberPrototype.h Utility.h PrototypeEngine.cpp PrototypeEngine.h Pattern.cpp Pattern.h Query.cpp Query.h Query.cpp Query.h IReversible.cpp IReversible.h MatchmakerUtility.h Util.h Node.cpp Node.h Utility.cpp Thread.cpp Thread.h)
target_link_libraries(kasai)

