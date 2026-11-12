cmake_minimum_required(VERSION 3.28)

project(MyProjects LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(bmp_reader
        BMPReader/main.cpp
        BMPReader/bmp.cpp
        BMPReader/utils.cpp
)

target_sources(bmp_reader
        PRIVATE
        FILE_SET CXX_MODULES FILES
        BMPReader/errors.cppm
        BMPReader/utils.cppm
        BMPReader/image.cppm
        BMPReader/bmp.cppm
)


add_executable(list_demo
        List/main.cpp
        List/lib/src/List.cpp
)

target_sources(list_demo
        PRIVATE
        FILE_SET CXX_MODULES
        BASE_DIRS List/lib/include
        FILES List/lib/include/List.cppm
)


if(MSVC)
    target_compile_options(bmp_reader PRIVATE /utf-8)
    target_compile_options(list_demo PRIVATE /utf-8)
endif()
