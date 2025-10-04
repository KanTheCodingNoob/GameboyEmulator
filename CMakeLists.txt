cmake_minimum_required(VERSION 3.31)
project(GameboyEmu)

set(CMAKE_CXX_STANDARD 23)

add_executable(GameboyEmu main.cpp
        Source/sm83.cpp
        Header/sm83.h
        Source/bus.cpp
        Header/Bus.h
        Source/Cartridge.cpp
        Header/Cartridge.h
        Source/MBC.cpp
        Header/MBC.h
        Source/MBC1.cpp
        Header/MBC1.h)
