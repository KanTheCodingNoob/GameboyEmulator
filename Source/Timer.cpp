//
// Created by ADMIN on 10/11/2025.
//

#include "../Header/Timer.h"

#include "../Header/Bus.h"

Timer::Timer()
= default;

Timer::~Timer()
= default;

void Timer::tick()
{
    DIVCounter++;
    // DIV Increment after 64 cycles
    if (DIVCounter >= 64)
    {
        // Direct access since writing to the DIV register through the bus reset it to 0
        bus->IORegisters[DIVLocation - 0xFF00]++;
        DIVCounter = 0;
    }

    TIMACounter++;

    // TIMA Increment after TIMAIncrementCycle cycles
    if (TIMACounter >= TIMAIncrementCycle)
    {
        TIMACounter = 0;
        uint8_t initialValue = bus->read(TIMALocation);
        bus->write(TIMALocation, initialValue + 1);
        if (initialValue == 0xFF)
        {
            bus->interrupt.interruptHandler(2);
            bus->write(TIMALocation, bus->read(TMALocation)); // Reset TIMA to the value of TMA
        }
    }
}

// Custom behavior for the Timer register in terms of writing to it
void Timer::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
        case 0xFF04:
            bus->IORegisters[addr - 0xFF00] = 0;
            break;
        case 0xFF07:
            bus->IORegisters[addr - 0xFF00] = data;
            // If bit 2 of TAC register is set, TIME is enabled to increment
            if ((bus->read(TACLocation) & 0x04) != 0)
            {
                // Determine the increment cycle based on the first 2 bit of TAC
                switch (bus->read(TACLocation) & 0x3)
                {
                case 0:
                    TIMAIncrementCycle = 255;
                    break;
                case 1:
                    TIMAIncrementCycle = 3;
                    break;
                case 2:
                    TIMAIncrementCycle = 15;
                    break;
                case 3:
                    TIMAIncrementCycle = 63;
                    break;
                default:
                    TIMAIncrementCycle = 255;
                }
            }
        default:
            bus->IORegisters[addr - 0xFF00] = data;
    }
}


