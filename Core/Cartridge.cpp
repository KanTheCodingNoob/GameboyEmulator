//
// Created by ADMIN on 10/4/2025.
//

#include "Cartridge.h"


Cartridge::Cartridge(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);

    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open ROM file: " + filename);
    }

    // Get file size
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    // Allocate vector and read
    rom.resize(size);
    if (!ifs.read(reinterpret_cast<char*>(rom.data()), size))
    {
        throw std::runtime_error("Failed to read ROM file: " + filename);
    }

    ifs.close();

    bank00 = &rom[0]; // Assign the pointer to the first 16KiB of Rom, occupy 0000 - 3FFF from bus
    bank01_NN = &rom[0x4000]; // Assign the switchable pointer to the next 16KiB of Rom

    // 0147 - Cartridge type
    // Handling different cartridge type, most notably its mapper
    switch (bank00[0x0147])
    {
    case 0x00:
            mapper = std::make_unique<MBC0>();
            break;
        case 0x01:
            mapper = std::make_unique<MBC1>();
            break;
        default:
            break;
    }

    // 0149 — RAM size
    // Resize external RAM based on how much RAM is present on the cartridge, if any
    uint8_t ramSize = bank00[0x0149];
    switch (ramSize)
    {
        case 0x00:
            eram.resize(0x2000);
            break;
        case 0x01:
            eram.resize(0x800);
            break;
        case 0x02:
            eram.resize(0x2000);
            break;
        case 0x03:
            eram.resize(0x8000);
            break;
        case 0x04:
            eram.resize(0x20000);
            break;
        case 0x05:
            eram.resize(0x10000);
            break;
        default:
            std::cerr << "Unknown RAM size code: 0x"
                  << std::hex << static_cast<int>(ramSize) << std::dec << "\n";
            break;
    }
}

Cartridge::~Cartridge()
= default;

// Return data from the rom
uint8_t Cartridge::readBank00(const uint16_t addr)
{
    return bank00[addr];
}

uint8_t Cartridge::readBank01_nn(const uint16_t addr)
{
    return bank01_NN[addr];
}

// Rom is read only, so the write operation actual purpose is to have the MBC point to the correct bank
void Cartridge::writeToRom(uint16_t addr, uint8_t data)
{
    mapper->MBCWrite(addr, data);
    bank01_NN = &rom[0x4000 * mapper->romBankNum];
}

void Cartridge::writeToRam(uint16_t addr, uint8_t data)
{
    if (mapper->ramEnabled)
    {
        eram[addr - 0xA000] = data;
    }
}


