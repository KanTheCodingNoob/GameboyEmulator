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
    REGISTER_PAIR AF_PAIR{}; // High: A, Low: F
    REGISTER_PAIR BC_PAIR{}; // High: B, Low: C
    REGISTER_PAIR DE_PAIR{}; // High: D, Low: E
    REGISTER_PAIR HL_PAIR{}; // High: H, Low: L

    // Aliases for readability
    uint8_t& F;
    uint8_t& A;
    uint8_t& B;
    uint8_t& C;
    uint8_t& D;
    uint8_t& E;
    uint8_t& H;
    uint8_t& L;
    uint16_t& AF;
    uint16_t& BC;
    uint16_t& DE;
    uint16_t& HL;

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

    void LD_r_r(uint8_t &r1, uint8_t &r2); void LD_r_n(uint8_t &r1); void LD_r_HLi(uint8_t &r1);
    void LD_HLi_r(uint8_t &r1); void LD_HLi_n(); void LD_A_BCi(); void LD_A_DEi(); void LD_BCi_A();
    void LD_DEi_A(); void LD_A_nn(); void LD_nn_A(); void LD_A_HLim(); void LD_HLim_A(); void LD_A_HLip();
    void LD_HLip_A(); void LDH_A_Ci(); void LDH_Ci_A(); void LDH_A_n(); void LDH_n_A();
    void LD_rr_nn(uint16_t &rr1); void LD_nn_SP(); void LD_SP_HL(); void PUSH_rr(uint16_t &rr1);
    void POP_rr(uint16_t &rr1); void LD_HL_SP_plus_e(); void ADD_r(uint8_t &r1); void ADD_HLi();
    void ADD_n(); void ADC_r(uint8_t &r1); void ADC_HLi(); void ADC_n(); void SUB_r(uint8_t &r1); void SUB_HLi();
    void SUB_n(); void SBC_r(uint8_t &r1); void SBC_HLi(); void SBC_n(); void SUB_HLi_A(); void SUB_n_A();
    void CP_r(uint8_t &r1); void CP_HLi(); void CP_n(); void INC_r(uint8_t &r1); void INC_HLi(); void DEC_r(uint8_t &r1);
    void DEC_HLi(); void AND_r(uint8_t &r1); void AND_HLi(); void AND_n(); void OR_r(uint8_t &r1); void OR_HLi(); void OR_n();
    void XOR_r(uint8_t &r1); void XOR_HLi(); void XOR_n(); void CCF(); void SCF(); void DAA(); void CPL(); void INC_rr();
    void DEC_rr(); void ADD_HL_rr(uint16_t &rr1); void ADD_SP_e(); void SUB_HL_rr(uint16_t &rr1);
    void RLCA(); void RRCA(); void RLA(); void RRA(); void RLC_r(uint8_t &r1); void RLC_HLi(); void RRC_r(uint8_t &r1);
    void RRC_HLi(); void RL_r(uint8_t &r1); void RL_HLi(); void RR_r(uint8_t &r1); void RR_HLi(); void SLA_r(uint8_t &r1);
    void SLA_HLi(); void SRA_r(uint8_t &r1); void SRA_HLi(); void SWAP_r(uint8_t &r1); void SWAP_HLi(); void SRL_r(uint8_t &r1);
    void SRL_HLi(); void BIT_b_r(uint8_t &r1); void BIT_b_HLi(); void RES_b_r(uint8_t &r1); void RES_b_HLi(); void SET_b_r(uint8_t &r1);
    void SET_b_HLi(); void JPnn(); void JP_HL(); void JP_cc_nn(); void JR_e(); void JR_cc_e(); void CALL_nn(); void CALL_cc_nn();
    void RET(); void RET_cc(); void RETI(); void RST_n(); void HALT(); void STOP(); void DI(); void EI(); void NOP();

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



