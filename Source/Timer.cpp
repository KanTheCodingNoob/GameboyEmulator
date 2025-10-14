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
    // DIV Increment after 64 cycles
    if (bus->systemClockCounter % 64 == 0)
    {
        bus->IORegisters[DIVLocation]++;
    }

    // If bit 2 of TAC register is set, TIME is enabled to increment
    if (bus->IORegisters[TACLocation] & 0x7 >= 0x4)
    {
        uint16_t TIMAIncrementCycle;
        // Determine the increment cycle based on the first 2 bit of TAC
        switch (bus->IORegisters[TACLocation] & 0x3)
        {
        case 0:
            TIMAIncrementCycle = 256;
            break;
        case 1:
            TIMAIncrementCycle = 4;
            break;
        case 2:
            TIMAIncrementCycle = 16;
            break;
        case 3:
            TIMAIncrementCycle = 64;
            break;
        default:
            TIMAIncrementCycle = 256;
        }

        // TIMA Increment after TIMAIncrementCycle cycles
        if (bus->systemClockCounter % TIMAIncrementCycle == 0)
        {
            uint8_t initialValue = bus->read(TIMALocation);
            if (initialValue == 0xFF)
            {
                bus->interrupt.interruptHandler(2);
                bus->IORegisters[TIMALocation] = bus->IORegisters[TMALocation]; // Reset TIMA to the value of TMA
            } else
            {
                bus->IORegisters[TIMALocation]++;
            }
        }
    }
}

