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
    // 0147 - Cartridge type
    // Handling different cartridge type, most notably its mapper
    switch (static_cast<Type>(rom[CARTRIDGE_TYPE_ADDR]))
    {
        case Type::ROM_ONLY:
            mapper = std::make_unique<MBC0>(rom, eram);
            break;
        case Type::MBC1:
        case Type::MBC1_RAM:
        case Type::MBC1_RAM_BATTERY:
            mapper = std::make_unique<MBC1>(rom, eram);
            break;
        case Type::MBC2:
        case Type::MBC2_BATTERY:
            mapper = std::make_unique<MBC2>(rom, eram);
            break;
        case Type::MBC3:
        case Type::MBC3_RAM:
        case Type::MBC3_RAM_BATTERY:
        case Type::MBC3_TIMER_BATTERY:
        case Type::MBC3_TIMER_RAM_BATTERY:
            mapper = std::make_unique<MBC3>(rom, eram);
            break;
        default:
            throw std::runtime_error("Unsupported cartridge type 0x" + std::to_string(rom[CARTRIDGE_TYPE_ADDR]));
    }

    // 0149 — RAM size
    // Resize external RAM based on how much RAM is present on the cartridge, if any
    uint8_t ramSize = rom[RAM_SIZE_ADDR];
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


uint8_t Cartridge::read(const uint16_t addr)
{
    return mapper->read(addr);
}

void Cartridge::write(const uint16_t addr, const uint8_t data)
{
    mapper->write(addr, data);
}