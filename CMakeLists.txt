cmake_minimum_required(VERSION 3.24)
project(Parser)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} src/grammar/grammar.cpp src/include/grammar/grammar.h src/include/grammar/ll1.h src/include/grammar/lr0.h src/grammar/ll1.cc src/grammar/lr0.cc src/include/common/common.h src/common/common.cc)

add_executable(LL1_Parser LL1.cpp)
add_executable(LR0_Parser LR0.cpp)

target_link_libraries(LL1_Parser ${PROJECT_NAME})
target_link_libraries(LR0_Parser ${PROJECT_NAME})


