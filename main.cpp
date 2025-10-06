#include <iostream>

#include "Header/Bus.h"
#include "Header/sm83.h"

int main() {
    std::shared_ptr<Cartridge> ptr(new Cartridge("cpu_instrs.gb"));
    Bus bus;
    bus.insertCartridge(ptr);
    for (int i = 0; i < 100000000; i++)
    {
        bus.cpu.clock();
    }

    return 0;
}