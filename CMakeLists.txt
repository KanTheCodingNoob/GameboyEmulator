cmake_minimum_required(VERSION 3.31)
project(GameboyEmu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(GameboyEmu main.cpp
        Source/core/sm83.cpp
        Header/core/sm83.h
        Source/core/Bus.cpp
        Header/core/Bus.h
        Source/core/Cartridge.cpp
        Header/core/Cartridge.h
        Source/core/MBC.cpp
        Header/core/MBC.h
        Source/core/MBC1.cpp
        Header/core/MBC1.h
        Source/core/Joypad.cpp
        Header/core/Joypad.h
        Source/core/Timer.cpp
        Header/core/Timer.h
        Source/core/Interrupt.cpp
        Header/core/Interrupt.h
        Source/core/IO.cpp
        Header/core/IO.h
        Source/core/PPU.cpp
        Header/core/PPU.h
        Source/core/MBC0.cpp
        Header/core/MBC0.h
)

add_custom_command(TARGET GameboyEmu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/TestRoms"
        "$<TARGET_FILE_DIR:GameboyEmu>/TestRoms"
)

target_link_libraries(GameboyEmu PRIVATE SDL3::SDL3)