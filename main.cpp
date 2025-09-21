#include <iostream>

#include "Core/Bus.h"
#include "Core/sm83.h"

int main() {
    Bus bus;
    bus.loadROM("cpu_instrs.gb");

    // Run until test finishes (naive loop)
    for (int i = 0; i < 10'000'000; i++) {
        bus.cpu->clock();  // one instruction
    }

    std::cout << "\nTest finished.\n";
    return 0;
}