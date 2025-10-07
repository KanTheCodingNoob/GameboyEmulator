cmake_minimum_required(VERSION 3.31)
project(GameboyEmu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

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
        Header/MBC1.h
        )

add_custom_command(TARGET GameboyEmu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/cpu_instrs.gb"
        "$<TARGET_FILE_DIR:GameboyEmu>/cpu_instrs.gb"
)

target_link_libraries(GameboyEmu PRIVATE SDL3::SDL3)