//
// Created by ADMIN on 1/24/2026.
//

#include "Emulator.h"

Emulator::Emulator() = default;

Emulator::~Emulator()
{
    stop();
}

bool Emulator::hasRom() const
{
    return cartridge != nullptr;
}


bool Emulator::loadCartridge(const std::string& path)
{
    stop();        // power off
    reset();       // clear hardware state

    cartridge = std::make_shared<Cartridge>(path);
    bus.insertCartridge(cartridge);

    return true;
}

void Emulator::start()
{
    if (running || !hasRom())
        return;

    running = true;
    thread = std::thread(&Emulator::run, this);
}

void Emulator::stop()
{
    running = false;

    if (thread.joinable())
        thread.join();
}

void Emulator::reset()
{
    bus.reset();
}

void Emulator::run()
{
    using clock = std::chrono::steady_clock;

    constexpr auto FRAME_DURATION =
        std::chrono::duration_cast<clock::duration>(
            std::chrono::duration<double>(1.0 / 59.7275)
        );

    auto nextFrame = clock::now();

    while (running) {
        constexpr int M_CYCLES_PER_FRAME = 17556;

        for (int i = 0; i < M_CYCLES_PER_FRAME; ++i)
            bus.clock();

        nextFrame += FRAME_DURATION;
        std::this_thread::sleep_until(nextFrame);
    }
}
