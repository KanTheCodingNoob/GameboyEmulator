# Game Boy Emulator (DMG-01)

A cycle-accurate Nintendo Game Boy emulator written in C++, 
built to replicate the original DMG-01 hardware at the system level. 
This project focuses on low-level architecture, hardware emulation, 
and performance-aware design.

## Features

### CPU
* Full implementation of the Sharp SM83 CPU (Game Boy’s custom 8-bit processor)
* All official opcodes implemented
* Correct flag behavior (Z, N, H, C)
* Accurate instruction cycle timing
* CB-prefixed instruction set supported

### Memory System
* Accurate 64Kb map of the Game Boy system
* Support ROM banking for games using MBC0, MBC1, MBC2, MBC3 and MBC5
* External RAM (ERAM) support

### Input
* Emulated Joypad register
* Keyboard → Game Boy button mapping

### Graphics (PPU)
* Tile-based rendering system
* Background, Window, and Sprite layers
* Scanline-based rendering
* LCD modes (OAM Search, Drawing, HBlank, VBlank)
* Framebuffer output via SDL

### Audio (APU)
To be implemented

### Save Support
* Emulate battery-backed RAM saving for supported cartridges using .sav file

## Architecture
All components related to Game Boy emulation are centralized through a system bus.
The bus models the original hardware interconnect and exposes the 16-bit address bus 
(0x0000–0xFFFF), allowing each hardware component to communicate through memory-mapped I/O.

## Tech Stack

* Language: C++

* Graphics & Audio: SDL

* Build System: CMake

* Architecture Style: Hardware component simulation

## Project Goals

This project was built to:

* Understand computer architecture

* Practice low-level systems programming

* Learn CPU design & instruction decoding

* Simulate real hardware timing

* Work with memory-mapped I/O

* Render graphics using scanline-based technique

## Testing

I have use these following test to validate my program's accuracy:
* [cpu_instr.gb](https://gbdev.gg8.se/wiki/articles/Test_ROMs#Blargg.27s_tests)  for CPU accuracy
* [dmg_acid2.gb](https://github.com/mattcurrie/dmg-acid2) for PPU accuracy

## Future Work

* APU Implementation
* Web Migration

## How to Build
Requirements

* C++17+ compiler
* CMake

Build (Window)

```bash
git clone https://github.com/KanTheCodingNoob/GameboyEmulator.git
cd GameboyEmulator
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
cmake -S . -B build
cmake --build build
```
The executable will be in the build folder

## Disclaimer
This emulator is for educational purposes. Users must provide their own legally obtained ROMs.
