cmake_minimum_required(VERSION 3.28)
project(BMPReader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(bmp_reader
        main.cpp
        bmp.cpp
        utils.cpp
)

target_sources(bmp_reader
    PRIVATE
        FILE_SET CXX_MODULES FILES
        errors.cppm
        utils.cppm
        image.cppm
        bmp.cppm
)

if(MSVC)
    target_compile_options(bmp_reader PRIVATE /utf-8)
endif()
