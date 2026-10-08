cmake_minimum_required(VERSION 4.3)
project(ISIT05_Medyanik)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ISIT05_Medyanik
        bmp.cpp
        bmp.h
        bmp_header.cpp
        main.cpp
        utils.cpp
        utils.h)
