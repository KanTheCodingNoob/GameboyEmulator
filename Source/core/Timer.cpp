//
// Created by ADMIN on 10/11/2025.
//

#include "../../Header/core/Timer.h"

#include "../../Header/core/Bus.h"

Timer::Timer(Bus *bus): bus(bus){}

Timer::~Timer()
= default;

void Timer::tick()
{
    DIVCounter++;
    // DIV Increment after 64 cycles
    if (DIVCounter >= DIVIncrementCycle)
    {
        // Direct access since writing to the DIV register through the bus reset it to 0
        bus->io.directIOWrite(DIVLocation, bus->io.directIORead(DIVLocation));
        DIVCounter = 0;
    }

    if ((bus->read(TACLocation) & 0x4) > 0)
    {
        TIMACounter++;

        // TIMA Increment after TIMAIncrementCycle cycles
        if (TIMACounter >= TIMAIncrementCycle)
        {
            TIMACounter = 0;
            const uint8_t initialValue = bus->io.directIORead(TIMALocation);
            bus->io.directIOWrite(TIMALocation, initialValue + 1);
            if (initialValue == 0xFF)
            {
                bus->interrupt.requestInterrupt(2);
                bus->io.directIOWrite(TIMALocation, bus->io.directIORead(TMALocation)); // Reset TIMA to the value of TMA
            }
        }
    }
}

// Custom behavior for the Timer register in terms of writing to it
void Timer::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
        case 0xFF04:
            bus->io.directIOWrite(addr, 0);
            DIVCounter = 0;
            break;
        case 0xFF07:
            bus->io.directIOWrite(addr, data);
            // If bit 2 of TAC register is set, TIME is enabled to increment
            if ((data & 0x04) != 0)
            {
                // Determine the increment cycle based on the first 2 bit of TAC
                switch (data & 0x3)
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
            }
            break;
        default:
            bus->io.directIOWrite(addr, data);
    }
}


