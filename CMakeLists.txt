cmake_minimum_required(VERSION 3.31)
project(GameboyEmu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(GameboyEmu main.cpp
        Core/sm83.cpp
        Core/sm83.h
        Core/Bus.cpp
        Core/Bus.h
        Core/Cartridge.cpp
        Core/Cartridge.h
        Core/MBC.cpp
        Core/MBC.h
        Core/MBC1.cpp
        Core/MBC1.h
        Core/Joypad.cpp
        Core/Joypad.h
        Core/Timer.cpp
        Core/Timer.h
        Core/Interrupt.cpp
        Core/Interrupt.h
        Core/IO.cpp
        Core/IO.h
        Core/PPU.cpp
        Core/PPU.h
        Core/MBC0.cpp
        Core/MBC0.h
        App/SDLDisplay.cpp
        App/SDLDisplay.h
        App/Emulator.cpp
        App/Emulator.h
        App/InputMapper.cpp
        App/InputMapper.h
)

add_custom_command(TARGET GameboyEmu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/TestRoms"
        "$<TARGET_FILE_DIR:GameboyEmu>/TestRoms"
)

target_link_libraries(GameboyEmu PRIVATE SDL3::SDL3)