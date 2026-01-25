//
// Created by ADMIN on 1/24/2026.
//

#pragma once
#include <atomic>
#include <string>
#include <thread>
#include "../Core/Bus.h"


class Emulator
{
public:
    static constexpr int WIDTH  = 160;
    static constexpr int HEIGHT = 144;

    Emulator();
    ~Emulator();

    // Lifecycle
    void start();
    void stop();
    void reset();

    // Rom handling
    bool loadCartridge(const std::string& path);
    [[nodiscard]] bool hasRom() const;

    // framebuffer (for renderer)
    uint32_t (&framebuffer)[HEIGHT][WIDTH] = bus.ppu.LCD;

private:
    void run();

    std::atomic<bool> running{false};
    std::thread thread;

    Bus bus;
    std::shared_ptr<Cartridge> cartridge;
};
