//
// Created by anngu on 7/24/2025.
//
#pragma once

#define SM83_H
#include <cstdint>
#include <string>
#include <vector>

#include "Bus.h"
union REGISTER_PAIR {
    struct {
        uint8_t low;
        uint8_t high;
    };
    uint16_t value;
};


class sm83 {
public:
    sm83();
    ~sm83();
    // Register
    REGISTER_PAIR AF{}; // High: A, Low: F
    REGISTER_PAIR BC{}; // High: B, Low: C
    REGISTER_PAIR DE{}; // High: D, Low: E
    REGISTER_PAIR HL{}; // High: H, Low: L

    // Aliases for readability
    uint8_t& F;
    uint8_t& A;
    uint8_t& B;
    uint8_t& C;
    uint8_t& D;
    uint8_t& E;
    uint8_t& H;
    uint8_t& L;

    uint16_t SP = 0x0000;
    uint16_t PC = 0x0000;

    enum FlagRegisters {
        z = (1 << 7),
        n = (1 << 6),
        h = (1 << 5),
        c = (1 << 4)
    };

    void connectBus(Bus *n) {
        bus = n;
    }

    // SM83 Instruction Set
    // i: Indirect
    // m: Decrement
    // p: Increment

    // LD
    void LD_r_r(uint8_t &r1, uint8_t &r2); void LD_r_n(uint8_t &r1); void LD_r_HLi(uint8_t &r1);
    void LD_HLi_r(uint8_t &r1); void LD_HLi_n(); void LD_A_BCi(); void LD_A_DEi(); void LD_BCi_A();
    void LD_DEi_A(); void LD_A_nn(); void LD_nn_A(); void LD_A_HLim(); void LD_HLim_A(); void LD_A_HLip();
    void LD_HLip_A(); void LDH_A_Ci(); void LDH_Ci_A(); void LDH_A_n(); void LDH_n_A();
    void LD_rr_nn(uint16_t &rr1); void LD_nn_SP(); void LD_SP_HL();

    // PUSH

    // Prefixed ($CB $xx)

    void clock();

    uint8_t IR = 0x00;
    uint8_t IE = 0x00;
    uint8_t cycle = 0;

private:
    // Link to bus
    Bus *bus = nullptr;
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    uint8_t getFlags(FlagRegisters f);
    void setFlags(FlagRegisters f);

    struct INSTRUCTION {
        std::string name;
        void (sm83::*operate) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> opcodeTable;
    std::vector<INSTRUCTION> prefixedTable;
};



