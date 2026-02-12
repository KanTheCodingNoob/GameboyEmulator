#pragma once
#include <cstdint>
#include <vector>
#include <mutex>

class Bus;

class APU {
public:
    explicit APU(Bus* bus);
    ~APU() = default;

    void clock();
    void reset();

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

private:
    Bus* bus;
};
