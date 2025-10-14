#include <iostream>
#include <iomanip>

#include "Header/Bus.h"
#include "Header/sm83.h"

int main() {
    std::shared_ptr<Cartridge> ptr(new Cartridge("Tests/IndividualTest/02-interrupts.gb"));
    Bus bus;
    bus.insertCartridge(ptr);
    while (true)
    {
        bus.cpu.clock();
    }
}