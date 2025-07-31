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
    // Core methods that the opcode method is going to use to reduce code repetition
    void LD_r_r(uint8_t &r1, uint8_t &r2); void LD_r_n(uint8_t &r1); void LD_r_HLi(uint8_t &r1);
    void LD_HLi_r(uint8_t &r1); void LD_rr_nn(uint16_t &rr1); void POP_rr(uint16_t &rr1);
    void PUSH_rr(uint16_t &rr1); void ADD_r(uint8_t &r1); void ADC_r(uint8_t &r1);
    void SBC_r(uint8_t &r1); void CP_r(uint8_t &r1); void INC_r(uint8_t &r1);
    void AND_r(uint8_t &r1); void XOR_r(uint8_t &r1); void ADD_HL_rr(uint16_t &rr1);
    void SUB_HL_rr(uint16_t &rr1); void RL_r(uint8_t &r1); void RLC_r(uint8_t &r1);
    void RRC_r(uint8_t &r1); void SRA_r(uint8_t &r1); void RR_r(uint8_t &r1); void SWAP_r(uint8_t &r1);
    void SLA_r(uint8_t &r1); void SRL_r(uint8_t &r1); void BIT_b_r(uint8_t &r1); void RES_b_r(uint8_t &r1);
    void DEC_r(uint8_t &r1); void OR_r(uint8_t &r1); void SET_b_r(uint8_t &r1); void INC_rr(uint16_t &rr1);
    void DEC_rr(uint16_t &rr1); void JR_cc_e();

    // Actual opcode
    void LD_HLi_n(); void LD_A_BCi(); void LD_A_DEi(); void LD_BCi_A(); void LD_B_n(/*LD_R_n*/);
    void LD_DEi_A(); void LD_A_nn(); void LD_nn_A(); void LD_A_HLim(); void LD_HLim_A(); void LD_A_HLip();
    void LD_HLip_A(); void LDH_A_Ci(); void LDH_Ci_A(); void LDH_A_n(); void LDH_n_A(); void LD_DE_nn(/*LD_rr_nn*/);
    void LD_nni_SP(); void LD_SP_HL(); void LD_BC_nn(/*LD_rr_nn*/); void INC_BC(/*INC_rr*/); void DEC_H(/*DEC_r*/);
    void LD_HL_SP_plus_e(); void ADD_HLi(); void DEC_B(); void INC_B(/*INC_r*/); void INC_C(/*INC_r*/);
    void ADD_n(); void ADC_HLi(); void ADC_n(); void SUB_r(uint8_t &r1); void SUB_HLi(); void DEC_D(/*DEC_r*/);
    void SUB_n(); void SBC_HLi(); void SBC_n(); void SUB_HLi_A(); void SUB_n_A(); void LD_E_n(/*LD_r_n*/);
    void CP_HLi(); void CP_n(); void INC_HLi(); void ADD_HL_BC(/*ADD_HL_rr*/); void LD_HL_nn(/*LD_rr_nn*/);
    void DEC_HLi(); void AND_HLi(); void AND_n();  void OR_HLi(); void OR_n(); void INC_HL(/*INC_rr*/);
    void XOR_HLi(); void XOR_n(); void CCF(); void SCF(); void DAA(); void CPL(); void JR_Z_e(/*JR_cc_e*/);
    void ADD_SP_e(); void DEC_BC(/*DEC_rr*/); void DEC_C(/*DEC_r*/); void LD_C_n(/*LD_r_n*/); void LD_A_HLi(/*LD_r_HLi*/);
    void RLCA(); void RRCA(); void RLA(); void RRA(); void RLC_HLi(); void DEC_DE(/*DEC_rr*/); void DEC_HL(/*DEC_rr*/);
    void RRC_HLi(); void RL_HLi(); void RR_HLi(); void INC_DE(/*INC_rr*/); void INC_H(/*INC_r*/);
    void SLA_HLi(); void SRA_HLi(); void SWAP_HLi(); void INC_D(/*INC_r*/); void DEC_E(/*DEC_r*/);
    void INC_E(/*INC_r*/); void JR_NZ_e(/*JR_rr_e*/); void LD_H_n(/*LD_r_n*/); void ADD_HL_HL(/*ADD_rr_rr*/);
    void SRL_HLi(); void BIT_b_HLi(); void RES_b_HLi(); void LD_D_n(/*LD_r_n*/); void ADD_HL_DE(/*ADD_HL_rr*/);
    void SET_b_HLi(); void JPnn(); void JP_HL(); void JP_cc_nn(); void JR_e(); void CALL_nn(); void CALL_cc_nn();
    void RET(); void RET_cc(); void RETI(); void RST_n(); void HALT(); void STOP_n(); void DI(); void EI(); void NOP();
    void ILLEGAL();

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
        void (sm83::*operate)() = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> opcodeTable;
    std::vector<INSTRUCTION> prefixedTable;
};



