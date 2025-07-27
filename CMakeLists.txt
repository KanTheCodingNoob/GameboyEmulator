cmake_minimum_required(VERSION 3.31)
project(GameboyEmu)

set(CMAKE_CXX_STANDARD 23)

add_executable(GameboyEmu main.cpp
        Core/sm83.cpp
        Core/sm83.h
        Core/bus.cpp
        Core/bus.h)
