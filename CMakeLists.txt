cmake_minimum_required(VERSION 3.29)
project(mhl
        VERSION 0.0.1)

add_library(mhl STATIC
    src/sys/netlink.cpp
    src/error.cpp
)
target_include_directories(mhl PUBLIC
    inc/
)
set_property(TARGET mhl PROPERTY CXX_STANDARD 23)
set_property(TARGET mhl PROPERTY CXX_EXTENSIONS FALSE)
