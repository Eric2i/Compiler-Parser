cmake_minimum_required(VERSION 3.24)
project(Parser)
set(CMAKE_CXX_STANDARD 11)

# Add source files
set(SOURCES
        LL1.cpp
        LR0.cpp
        SLR1.cpp
        main.cpp
        )

# Add library files
set(LIBRARY_SOURCES
        src/grammar/grammar.cc
        src/grammar/ll1.cc
        src/grammar/lr0.cc
        src/grammar/slr1.cc
        src/common/common.cc
        )

# Add include directories
set(INCLUDE_DIRS
        src/include/third_lib
        src/include/grammar
        src/include/common
        )

# Build library
add_library(${PROJECT_NAME} ${LIBRARY_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# Build executables
foreach (SOURCE_FILE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
    target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME})
endforeach ()