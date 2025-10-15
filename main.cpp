#include <iostream>
#include <iomanip>

#include "Header/Bus.h"
#include "Header/sm83.h"

int main() {
    std::shared_ptr<Cartridge> ptr(new Cartridge("Tests/IndividualTest/02-interrupts.gb"));
    Bus bus;
    bus.insertCartridge(ptr);
    // std::ofstream log("log.txt");
    // log << std::uppercase << std::hex << std::setfill('0');
    // int i = 0;
    while (true)
    {
        // // Registers
        // log << "A:" << std::setw(2) << static_cast<int>(bus.cpu.A) << " "
        //     << "F:" << std::setw(2) << static_cast<int>(bus.cpu.F) << " "
        //     << "B:" << std::setw(2) << static_cast<int>(bus.cpu.B) << " "
        //     << "C:" << std::setw(2) << static_cast<int>(bus.cpu.C) << " "
        //     << "D:" << std::setw(2) << static_cast<int>(bus.cpu.D) << " "
        //     << "E:" << std::setw(2) << static_cast<int>(bus.cpu.E) << " "
        //     << "H:" << std::setw(2) << static_cast<int>(bus.cpu.H) << " "
        //     << "L:" << std::setw(2) << static_cast<int>(bus.cpu.L) << " ";
        //
        // // Stack Pointer and Program Counter
        // log << "SP:" << std::setw(4) << bus.cpu.SP << " "
        //     << "PC:" << std::setw(4) << bus.cpu.PC << " ";
        //
        // // Next 4 bytes in memory
        // log << "PCMEM:";
        // for (int j = 0; j < 4; j++) {
        //     uint8_t byte = bus.read(bus.cpu.PC + j);
        //     log << std::setw(2) << static_cast<int>(byte);
        //     if (j != 3) log << ",";
        // }
        //
        // log << std::endl;

        bus.clock();
        // i++;
    }
}
