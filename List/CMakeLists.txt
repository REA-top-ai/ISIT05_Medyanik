cmake_minimum_required(VERSION 3.28)
project(LinkedList LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(list_lib)

target_sources(list_lib
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS lib/include
        FILES lib/include/List.cppm
    PRIVATE
        lib/src/List.cpp
)

add_executable(list_demo main.cpp)
target_link_libraries(list_demo PRIVATE list_lib)

if(MSVC)
    target_compile_options(list_lib PRIVATE /utf-8)
    target_compile_options(list_demo PRIVATE /utf-8)
endif()
