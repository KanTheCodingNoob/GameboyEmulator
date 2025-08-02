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
    uint8_t LD_r_r(uint8_t &r1, uint8_t &r2); uint8_t LD_r_n(uint8_t &r1); uint8_t LD_r_HLi(uint8_t &r1);
    uint8_t LD_HLi_r(uint8_t &r1); uint8_t LD_rr_nn(uint16_t &rr1); uint8_t POP_rr(uint16_t &rr1);
    uint8_t PUSH_rr(uint16_t &rr1); uint8_t ADD_r(uint8_t &r1); uint8_t ADC_r(uint8_t &r1);
    uint8_t SBC_r(uint8_t &r1); uint8_t CP_r(uint8_t &r1); uint8_t INC_r(uint8_t &r1);
    uint8_t AND_r(uint8_t &r1); uint8_t XOR_r(uint8_t &r1); uint8_t ADD_HL_rr(uint16_t &rr1);
    uint8_t SUB_HL_rr(uint16_t &rr1); uint8_t RL_r(uint8_t &r1); uint8_t RLC_r(uint8_t &r1);
    uint8_t RRC_r(uint8_t &r1); uint8_t SRA_r(uint8_t &r1); uint8_t RR_r(uint8_t &r1); uint8_t SWAP_r(uint8_t &r1);
    uint8_t SLA_r(uint8_t &r1); uint8_t SRL_r(uint8_t &r1); uint8_t BIT_b_r(uint8_t b, uint8_t &r1); uint8_t RES_b_r(uint8_t b, uint8_t &r1);
    uint8_t DEC_r(uint8_t &r1); uint8_t OR_r(uint8_t &r1); uint8_t SET_b_r(uint8_t &r1); uint8_t INC_rr(uint16_t &rr1);
    uint8_t DEC_rr(uint16_t &rr1); uint8_t JR_cc_e(); uint8_t RET_cc(); uint8_t JP_cc_nn(); uint8_t CALL_cc_nn();
    uint8_t RST_n(uint8_t n); uint8_t BIT_b_HLi(uint8_t b); uint8_t RES_b_HLi(uint8_t b);

    // Actual opcode
    uint8_t LD_HLi_n(); uint8_t LD_A_BCi(); uint8_t LD_A_DEi(); uint8_t LD_BCi_A(); uint8_t LD_B_n(/*LD_R_n*/);
    uint8_t LD_DEi_A(); uint8_t LD_A_nni(); uint8_t LD_nni_A(); uint8_t LD_A_HLim(); uint8_t LD_HLim_A(); uint8_t LD_A_HLip();
    uint8_t LD_HLip_A(); uint8_t LDH_A_Ci(); uint8_t LDH_Ci_A(); uint8_t LDH_A_ni(); uint8_t LDH_ni_A(); uint8_t LD_DE_nn(/*LD_rr_nn*/);
    uint8_t LD_nni_SP(); uint8_t LD_SP_HL(); uint8_t LD_BC_nn(/*LD_rr_nn*/); uint8_t INC_BC(/*INC_rr*/); uint8_t DEC_H(/*DEC_r*/);
    uint8_t LD_HL_SP_plus_e(); uint8_t ADD_A_HLi(); uint8_t DEC_B(); uint8_t INC_B(/*INC_r*/); uint8_t INC_C(/*INC_r*/);
    uint8_t ADD_A_n(); uint8_t ADC_A_HLi(); uint8_t ADC_A_n(); uint8_t SUB_r(uint8_t &r1); uint8_t SUB_A_HLi(); uint8_t DEC_D(/*DEC_r*/);
    uint8_t SUB_A_n(); uint8_t SBC_A_HLi(); uint8_t SBC_A_n(); uint8_t SUB_HLi_A(); uint8_t SUB_n_A(); uint8_t LD_E_n(/*LD_r_n*/);
    uint8_t CP_A_HLi(); uint8_t CP_A_n(); uint8_t INC_HLi(); uint8_t ADD_HL_BC(/*ADD_HL_rr*/); uint8_t LD_HL_nn(/*LD_rr_nn*/);
    uint8_t DEC_HLi(); uint8_t AND_A_HLi(); uint8_t AND_A_n();  uint8_t OR_A_HLi(); uint8_t OR_A_n(); uint8_t INC_HL(/*INC_rr*/);
    uint8_t XOR_A_HLi(); uint8_t XOR_A_n(); uint8_t CCF(); uint8_t SCF(); uint8_t DAA(); uint8_t CPL(); uint8_t JR_Z_e(/*JR_cc_e*/);
    uint8_t ADD_SP_e(); uint8_t DEC_BC(/*DEC_rr*/); uint8_t DEC_C(/*DEC_r*/); uint8_t LD_C_n(/*LD_r_n*/); uint8_t LD_A_HLi(/*LD_r_HLi*/);
    uint8_t RLCA(); uint8_t RRCA(); uint8_t RLA(); uint8_t RRA(); uint8_t RLC_HLi(); uint8_t DEC_DE(/*DEC_rr*/); uint8_t DEC_HL(/*DEC_rr*/);
    uint8_t RRC_HLi(); uint8_t RL_HLi(); uint8_t RR_HLi(); uint8_t INC_DE(/*INC_rr*/); uint8_t INC_H(/*INC_r*/);
    uint8_t SLA_HLi(); uint8_t SRA_HLi(); uint8_t SWAP_HLi(); uint8_t INC_D(/*INC_r*/); uint8_t DEC_E(/*DEC_r*/);
    uint8_t INC_E(/*INC_r*/); uint8_t JR_NZ_e(/*JR_rr_e*/); uint8_t LD_H_n(/*LD_r_n*/); uint8_t ADD_HL_HL(/*ADD_rr_rr*/);
    uint8_t SRL_HLi(); uint8_t LD_D_n(/*LD_r_n*/); uint8_t ADD_HL_DE(/*ADD_HL_rr*/);
    uint8_t SET_b_HLi(); uint8_t JP_nn(); uint8_t JP_HL(); uint8_t JR_e(); uint8_t CALL_nn();
    uint8_t RET(); uint8_t RETI(); uint8_t HALT(); uint8_t STOP_n(); uint8_t DI(); uint8_t EI(); uint8_t NOP();
    uint8_t ILLEGAL(); uint8_t INC_L(/*INC_r*/); uint8_t DEC_L(/*DEC_r*/); uint8_t LD_L_n(/*LD_R_n*/); uint8_t JR_NC_e(/*JR_cc_e*/);
    uint8_t LD_SP_nn(/*LD_rr_nn*/); uint8_t LD_HLi_A(/*LD_HLi_r*/); uint8_t INC_SP(/*INC_rr*/); uint8_t JR_C_e(/*JR_cc_e*/);
    uint8_t ADD_HL_SP(/*ADD_rr_rr*/); uint8_t DEC_SP(/*DEC_rr*/); uint8_t INC_A(/*INC_A*/); uint8_t DEC_A(/*DEC_A*/);
    uint8_t LD_A_n(/*LD_r_n*/); uint8_t LD_B_B(/*LD_r_r*/); uint8_t LD_B_C(/*LD_r_r*/); uint8_t LD_B_D(/*LD_r_r*/);
    uint8_t LD_B_E(/*LD_r_r*/); uint8_t LD_B_H(/*LD_r_r*/); uint8_t LD_B_L(/*LD_r_r*/); uint8_t LD_B_HLi(/*LD_r_HLi*/);
    uint8_t LD_B_A(/*LD_r_r*/); uint8_t LD_C_B(/*LD_r_r*/); uint8_t LD_C_C(/*LD_r_r*/); uint8_t LD_C_D(/*LD_r_r*/);
    uint8_t LD_C_E(/*LD_r_r*/); uint8_t LD_C_H(/*LD_r_r*/); uint8_t LD_C_L(/*LD_r_r*/); uint8_t LD_C_HLi(/*LD_r_r*/);
    uint8_t LD_C_A(/*LD_r_r*/); uint8_t LD_D_B(/*LD_r_r*/); uint8_t LD_D_C(/*LD_r_r*/); uint8_t LD_D_D(/*LD_r_r*/);
    uint8_t LD_D_E(/*LD_r_r*/); uint8_t LD_D_H(/*LD_r_r*/); uint8_t LD_D_L(/*LD_r_r*/); uint8_t LD_D_HLi(/*LD_r_HLi*/);
    uint8_t LD_D_A(/*LD_r_r*/); uint8_t LD_E_B(/*LD_r_r*/); uint8_t LD_H_B(/*LD_r_r*/); uint8_t LD_H_C(/*LD_r_r*/);
    uint8_t LD_H_D(/*LD_r_r*/); uint8_t LD_H_E(/*LD_r_r*/); uint8_t LD_H_H(/*LD_r_r*/); uint8_t LD_H_L(/*LD_r_r*/);
    uint8_t LD_H_HLi(/*LD_r_HLi*/); uint8_t LD_E_C(/*LD_r_r*/); uint8_t LD_E_D(/*LD_r_r*/); uint8_t LD_E_E(/*LD_r_r*/);
    uint8_t LD_E_H(/*LD_r_r*/); uint8_t LD_E_L(/*LD_r_r*/); uint8_t LD_E_HLi(/*LD_r_HLi*/); uint8_t LD_E_A(/*LD_r_r*/);
    uint8_t LD_H_A(/*LD_r_r*/); uint8_t LD_L_B(/*LD_r_r*/); uint8_t LD_L_C(/*LD_r_r*/); uint8_t LD_L_D(/*LD_r_r*/);
    uint8_t LD_L_E(/*LD_r_r*/); uint8_t LD_L_H(/*LD_r_r*/); uint8_t LD_L_L(/*LD_r_r*/); uint8_t LD_L_HLi(/*LD_r_HLi*/);
    uint8_t LD_L_A(/*LD_r_r*/); uint8_t LD_HLi_B(/*LD_r_r*/); uint8_t LD_HLi_C(/*LD_r_r*/); uint8_t LD_HLi_D(/*LD_r_r*/);
    uint8_t LD_HLi_E(/*LD_HLi_r*/); uint8_t LD_HLi_H(/*LD_HLi_r*/); uint8_t LD_HLi_L(/*LD_HLi_r*/); uint8_t LD_A_B(/*LD_r_r*/);
    uint8_t LD_A_C(/*LD_r_r*/); uint8_t LD_A_D(/*LD_r_r*/); uint8_t LD_A_E(/*LD_r_r*/); uint8_t LD_A_H(/*LD_r_r*/);
    uint8_t LD_A_L(/*LD_r_r*/); uint8_t LD_A_A(/*LD_r_r*/); uint8_t ADD_A_B(/*ADD_r_r*/);
    uint8_t ADD_A_C(/*ADD_r_r*/); uint8_t ADD_A_D(/*ADD_r_r*/); uint8_t ADD_A_E(/*ADD_r_r*/); uint8_t ADD_A_H(/*ADD_r_r*/);
    uint8_t ADD_A_L(/*ADD_r_r*/); uint8_t ADD_A_A(/*ADD_r_r*/); uint8_t ADC_A_B(/*ADC_r_r*/); uint8_t ADC_A_C(/*ADC_r_r*/);
    uint8_t ADC_A_D(/*ADC_r_r*/); uint8_t ADC_A_E(/*ADC_r_r*/); uint8_t ADC_A_H(/*ADC_r_r*/); uint8_t ADC_A_L(/*ADC_r_r*/);
    uint8_t ADC_A_A(/*ADC_r_r*/); uint8_t SUB_A_B(/*SUB_r_r*/); uint8_t SUB_A_C(/*SUB_r_r*/); uint8_t SUB_A_D(/*SUB_r_r*/);
    uint8_t SUB_A_E(/*SUB_r_r*/); uint8_t SUB_A_H(/*SUB_r_r*/); uint8_t SUB_A_L(/*SUB_r_r*/); uint8_t SUB_A_A(/*SUB_r_r*/);
    uint8_t SBC_A_B(/*SBC_r_r*/); uint8_t SBC_A_C(/*SBC_r_r*/); uint8_t SBC_A_D(/*SBC_r_r*/); uint8_t SBC_A_E(/*SBC_r_r*/);
    uint8_t SBC_A_H(/*SBC_r_r*/); uint8_t SBC_A_L(/*SBC_r_r*/); uint8_t SBC_A_A(/*SBC_r_r*/); uint8_t AND_A_B(/*AND_r_r*/);
    uint8_t AND_A_C(/*AND_r_r*/); uint8_t AND_A_D(/*AND_r_r*/); uint8_t AND_A_E(/*AND_r_r*/); uint8_t AND_A_H(/*AND_r_r*/);
    uint8_t AND_A_L(/*AND_r_r*/); uint8_t AND_A_A(/*AND_r_r*/); uint8_t OR_A_B(/*OR_r_r*/); uint8_t OR_A_C(/*OR_r_r*/);
    uint8_t XOR_A_B(/*XOR_r_r*/); uint8_t XOR_A_C(/*XOR_r_r*/); uint8_t XOR_A_D(/*XOR_r_r*/); uint8_t XOR_A_E(/*XOR_r_r*/);
    uint8_t XOR_A_H(/*XOR_r_r*/); uint8_t XOR_A_L(/*XOR_r_r*/); uint8_t XOR_A_A(/*XOR_r_r*/); uint8_t OR_B_A(/*OR_r_r*/);
    uint8_t OR_A_D(/*OR_r_r*/); uint8_t OR_A_E(/*OR_r_r*/); uint8_t OR_A_H(/*OR_r_r*/); uint8_t OR_A_L(/*OR_r_r*/);
    uint8_t OR_A_A(/*OR_r_r*/); uint8_t CP_A_B(/*CP_r_r*/); uint8_t CP_A_C(/*CP_r_r*/); uint8_t CP_A_D(/*CP_r_r*/);
    uint8_t CP_A_E(/*CP_r_r*/); uint8_t CP_A_H(/*CP_r_r*/); uint8_t CP_A_L(/*CP_r_r*/); uint8_t CP_A_A(/*CP_r_r*/);
    uint8_t RET_NZ(/*RET_cc*/); uint8_t POP_BC(/*POP_rr*/); uint8_t JP_NZ_nn(/*JP_cc_nn*/); uint8_t CALL_NZ_nn(/*CALL_cc_nn*/);
    uint8_t PUSH_BC(/*PUSH_rr*/); uint8_t RST_$00(/*RST_n*/); uint8_t RET_Z(/*RET_cc*/); uint8_t JP_Z_nn(/*JP_cc_nn*/);
    uint8_t CALL_Z_nn(/*CALL_cc_nn*/); uint8_t PREFIX(); uint8_t RST_$08(/*RST_n*/); uint8_t RET_NC(/*RET_cc*/);
    uint8_t POP_DE(/*POP_rr*/); uint8_t JP_NC_nn(/*JP_cc_nn*/); uint8_t CALL_NC_nn(/*CALL_cc_nn*/); uint8_t PUSH_DE(/*PUSH_DE*/);
    uint8_t RST_$10(/*RST_n*/); uint8_t RET_C(/*RET_cc*/); uint8_t JP_C_nn(/*JP_cc_nn*/); uint8_t CALL_C_nn(/*CALL_cc_nn*/);
    uint8_t PUSH_HL(/*PUSH_rr*/); uint8_t RST_$18(/*RST_n*/); uint8_t POP_HL(/*POP_rr*/); uint8_t RST_$20(/*RST_n*/);
    uint8_t RST_$28(/*RST_n*/); uint8_t POP_AF(/*POP_rr*/); uint8_t PUSH_AF(/*PUSH_rr*/); uint8_t RST_$30(/*RST_n*/);
    uint8_t RST_$38(/*RST_n*/); uint8_t RLC_B(); uint8_t RLC_C(); uint8_t RLC_D(); uint8_t RLC_E(); uint8_t RLC_H();
    uint8_t RLC_L(); uint8_t RLC_A(); uint8_t RRC_B(); uint8_t RRC_C(); uint8_t RRC_D(); uint8_t RRC_E();
    uint8_t RRC_H(); uint8_t RRC_L(); uint8_t RRC_A(); uint8_t RL_B(); uint8_t RL_C(); uint8_t RL_D(); uint8_t RL_E();
    uint8_t RL_H(); uint8_t RL_L(); uint8_t RL_A(); uint8_t RR_B(); uint8_t RR_C(); uint8_t RR_D(); uint8_t RR_E();
    uint8_t RR_H(); uint8_t RR_L(); uint8_t RR_A(); uint8_t SLA_B(); uint8_t SLA_C(); uint8_t SLA_D(); uint8_t SLA_E();
    uint8_t SLA_H(); uint8_t SLA_L(); uint8_t SLA_A(); uint8_t SRA_B(); uint8_t SRA_C(); uint8_t SRA_D(); uint8_t SRA_E();
    uint8_t SRA_H(); uint8_t SRA_L(); uint8_t SRA_A(); uint8_t SWAP_B(); uint8_t SWAP_C(); uint8_t SWAP_D(); uint8_t SWAP_E();
    uint8_t SWAP_H(); uint8_t SWAP_L(); uint8_t SWAP_A(); uint8_t SRL_B(); uint8_t SRL_C(); uint8_t SRL_D(); uint8_t SRL_E();
    uint8_t SRL_H(); uint8_t SRL_L(); uint8_t SRL_A(); uint8_t BIT_0_B(); uint8_t BIT_0_C(); uint8_t BIT_0_D(); uint8_t BIT_0_E();
    uint8_t BIT_0_H(); uint8_t BIT_0_L(); uint8_t BIT_0_HLi(); uint8_t BIT_0_A(); uint8_t BIT_1_B(); uint8_t BIT_1_C();
    uint8_t BIT_1_D(); uint8_t BIT_1_E(); uint8_t BIT_1_H(); uint8_t BIT_1_L(); uint8_t BIT_1_HLi(); uint8_t BIT_1_A();
    uint8_t BIT_2_B(); uint8_t BIT_2_C(); uint8_t BIT_2_D(); uint8_t BIT_2_E(); uint8_t BIT_2_H(); uint8_t BIT_2_L();
    uint8_t BIT_2_HLi(); uint8_t BIT_2_A(); uint8_t BIT_3_B(); uint8_t BIT_3_C(); uint8_t BIT_3_D(); uint8_t BIT_3_E();
    uint8_t BIT_3_H(); uint8_t BIT_3_L(); uint8_t BIT_3_HLi(); uint8_t BIT_3_A(); uint8_t BIT_4_B(); uint8_t BIT_4_C();
    uint8_t BIT_4_D(); uint8_t BIT_4_E(); uint8_t BIT_4_H(); uint8_t BIT_4_L(); uint8_t BIT_4_HLi(); uint8_t BIT_4_A();
    uint8_t BIT_5_B(); uint8_t BIT_5_C(); uint8_t BIT_5_D(); uint8_t BIT_5_E(); uint8_t BIT_5_H(); uint8_t BIT_5_L();
    uint8_t BIT_5_HLi(); uint8_t BIT_5_A(); uint8_t BIT_6_B(); uint8_t BIT_6_C(); uint8_t BIT_6_D(); uint8_t BIT_6_E();
    uint8_t BIT_6_H(); uint8_t BIT_6_L(); uint8_t BIT_6_HLi(); uint8_t BIT_6_A(); uint8_t BIT_7_B(); uint8_t BIT_7_C();
    uint8_t BIT_7_D(); uint8_t BIT_7_E(); uint8_t BIT_7_H(); uint8_t BIT_7_L(); uint8_t BIT_7_HLi(); uint8_t BIT_7_A();
    uint8_t RES_0_B(); uint8_t RES_0_C(); uint8_t RES_0_D(); uint8_t RES_0_E(); uint8_t RES_0_H(); uint8_t RES_0_L();
    uint8_t RES_0_HLi(); uint8_t RES_0_A(); uint8_t RES_1_B(); uint8_t RES_1_C(); uint8_t RES_1_D(); uint8_t RES_1_E();
    uint8_t RES_1_H(); uint8_t RES_1_L(); uint8_t RES_1_HLi(); uint8_t RES_1_A(); uint8_t RES_2_B(); uint8_t RES_2_C();
    uint8_t RES_2_D(); uint8_t RES_2_E(); uint8_t RES_2_H(); uint8_t RES_2_L(); uint8_t RES_2_HLi(); uint8_t RES_2_A();
    uint8_t RES_3_B(); uint8_t RES_3_C(); uint8_t RES_3_D(); uint8_t RES_3_E();  uint8_t RES_3_H(); uint8_t RES_3_L();
    uint8_t RES_3_HLi(); uint8_t RES_3_A(); uint8_t RES_4_B(); uint8_t RES_4_C(); uint8_t RES_4_D(); uint8_t RES_4_E();
    uint8_t RES_4_H(); uint8_t RES_4_L(); uint8_t RES_4_HLi(); uint8_t RES_4_A(); uint8_t RES_5_B(); uint8_t RES_5_C();
    uint8_t RES_5_D(); uint8_t RES_5_E(); uint8_t RES_5_H(); uint8_t RES_5_L(); uint8_t RES_5_HLi(); uint8_t RES_5_A();
    uint8_t RES_6_B(); uint8_t RES_6_C(); uint8_t RES_6_D(); uint8_t RES_6_E();  uint8_t RES_6_H(); uint8_t RES_6_L();
    uint8_t RES_6_HLi(); uint8_t RES_6_A(); uint8_t RES_7_B(); uint8_t RES_7_C(); uint8_t RES_7_D(); uint8_t RES_7_E();
    uint8_t RES_7_H(); uint8_t RES_7_L(); uint8_t RES_7_HLi(); uint8_t RES_7_A(); uint8_t SET_0_B(); uint8_t SET_0_C();
    uint8_t SET_0_D(); uint8_t SET_0_E(); uint8_t SET_0_H(); uint8_t SET_0_L(); uint8_t SET_0_HLi(); uint8_t SET_0_A();
    uint8_t SET_1_B(); uint8_t SET_1_C(); uint8_t SET_1_D(); uint8_t SET_1_E(); uint8_t SET_1_H(); uint8_t SET_1_L();
    uint8_t SET_1_HLi(); uint8_t SET_1_A(); uint8_t SET_2_B(); uint8_t SET_2_C(); uint8_t SET_2_D(); uint8_t SET_2_E();
    uint8_t SET_2_H(); uint8_t SET_2_L();  uint8_t SET_2_HLi(); uint8_t SET_2_A();  uint8_t SET_3_B(); uint8_t SET_3_C();
    uint8_t SET_3_D(); uint8_t SET_3_E(); uint8_t SET_3_H(); uint8_t SET_3_L(); uint8_t SET_3_HLi(); uint8_t SET_3_A();
    uint8_t SET_4_B(); uint8_t SET_4_C(); uint8_t SET_4_D(); uint8_t SET_4_E(); uint8_t SET_4_H(); uint8_t SET_4_L();
    uint8_t SET_4_HLi(); uint8_t SET_4_A();  uint8_t SET_5_B(); uint8_t SET_5_C(); uint8_t SET_5_D(); uint8_t SET_5_E();
    uint8_t SET_5_H(); uint8_t SET_5_L(); uint8_t SET_5_HLi(); uint8_t SET_5_A(); uint8_t SET_6_B(); uint8_t SET_6_C();
    uint8_t SET_6_D(); uint8_t SET_6_E();  uint8_t SET_6_H(); uint8_t SET_6_L(); uint8_t SET_6_HLi(); uint8_t SET_6_A();
    uint8_t SET_7_B(); uint8_t SET_7_C(); uint8_t SET_7_D(); uint8_t SET_7_E(); uint8_t SET_7_H(); uint8_t SET_7_L();
    uint8_t SET_7_HLi(); uint8_t SET_7_A();

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
        uint8_t (sm83::*operate)() = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> opcodeTable;
    std::vector<INSTRUCTION> prefixedTable;
};



