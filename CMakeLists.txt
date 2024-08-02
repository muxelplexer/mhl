cmake_minimum_required(VERSION 3.29)
project(mhl
        VERSION 0.0.1)

add_library(mhl INTERFACE)
target_include_directories(mhl INTERFACE
    inc/
)
