#include <iomanip>

#include "Header/Bus.h"
#include "Header/sm83.h"

int main() {
    std::shared_ptr<Cartridge> ptr(new Cartridge("TestRoms/cpu_instrs.gb"));
    Bus bus;
    bus.insertCartridge(ptr);
    while (true)
    {
        bus.clock();
    }
}
