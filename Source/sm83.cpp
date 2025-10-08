//
// Created by anngu on 7/24/2025.
//
#include "../Header/sm83.h"
#include "../Header/Bus.h"


sm83::sm83(): F(AF_PAIR.low), A(AF_PAIR.high), B(BC_PAIR.high), C(BC_PAIR.low),
              D(DE_PAIR.high), E(DE_PAIR.low), H(HL_PAIR.high), L(HL_PAIR.low),
              AF(AF_PAIR.value), BC(BC_PAIR.value), DE(DE_PAIR.value), HL(HL_PAIR.value)
{
   opcodeTable = {
    {"NOP", &sm83::NOP, 4},
    {"LD BC n16", &sm83::LD_BC_nn, 12},
    {"LD [BC] A", &sm83::LD_BCi_A, 8},
    {"INC BC", &sm83::INC_BC, 8},
    {"INC B", &sm83::INC_B, 4},
    {"DEC B", &sm83::DEC_B, 4},
    {"LD B n8", &sm83::LD_B_n, 8},
    {"RLCA", &sm83::RLCA, 4},
    {"LD [a16] SP", &sm83::LD_nni_SP, 20},
    {"ADD HL BC", &sm83::ADD_HL_BC, 8},
    {"LD A [BC]", &sm83::LD_A_BCi, 8},
    {"DEC BC", &sm83::DEC_BC, 8},
    {"INC C", &sm83::INC_C, 4},
    {"DEC C", &sm83::DEC_C, 4},
    {"LD C n8", &sm83::LD_C_n, 8},
    {"RRCA", &sm83::RRCA, 4},
    {"STOP n8", &sm83::STOP_n, 4},
    {"LD DE n16", &sm83::LD_DE_nn, 12},
    {"LD [DE] A", &sm83::LD_DEi_A, 8},
    {"INC DE", &sm83::INC_DE, 8},
    {"INC D", &sm83::INC_D, 4},
    {"DEC D", &sm83::DEC_D, 4},
    {"LD D n8", &sm83::LD_D_n, 8},
    {"RLA", &sm83::RLA, 4},
    {"JR e8", &sm83::JR_e, 12},
    {"ADD HL DE", &sm83::ADD_HL_DE, 8},
    {"LD A [DE]", &sm83::LD_A_DEi, 8},
    {"DEC DE", &sm83::DEC_DE, 8},
    {"INC E", &sm83::INC_E, 4},
    {"DEC E", &sm83::DEC_E, 4},
    {"LD E n8", &sm83::LD_E_n, 8},
    {"RRA", &sm83::RRA, 4},
    {"JR NZ e8", &sm83::JR_NZ_e, 12},
    {"LD HL n16", &sm83::LD_HL_nn, 12},
    {"LD [HL+] A", &sm83::LD_HLip_A, 8},
    {"INC HL", &sm83::INC_HL, 8},
    {"INC H", &sm83::INC_H, 4},
    {"DEC H", &sm83::DEC_H, 4},
    {"LD H n8", &sm83::LD_H_n, 8},
    {"DAA", &sm83::DAA, 4},
    {"JR Z e8", &sm83::JR_Z_e, 12},
    {"ADD HL HL", &sm83::ADD_HL_HL, 8},
    {"LD A [HL+]", &sm83::LD_A_HLip, 8},
    {"DEC HL", &sm83::DEC_HL, 8},
    {"INC L", &sm83::INC_L, 4},
    {"DEC L", &sm83::DEC_L, 4},
    {"LD L n8", &sm83::LD_L_n, 8},
    {"CPL", &sm83::CPL, 4},
    {"JR NC e8", &sm83::JR_NC_e, 12},
    {"LD SP n16", &sm83::LD_SP_nn, 12},
    {"LD [HL-] A", &sm83::LD_HLim_A, 8},
    {"INC SP", &sm83::INC_SP, 8},
    {"INC [HL]", &sm83::INC_HLi, 12},
    {"DEC [HL]", &sm83::DEC_HLi, 12},
    {"LD HL n8", &sm83::LD_HLi_n, 12},
    {"SCF", &sm83::SCF, 4},
    {"JR C e8", &sm83::JR_C_e, 12},
    {"ADD HL SP", &sm83::ADD_HL_SP, 8},
    {"LD A [HL-]", &sm83::LD_A_HLim, 8},
    {"DEC SP", &sm83::DEC_SP, 8},
    {"INC A", &sm83::INC_A, 4},
    {"DEC A", &sm83::DEC_A, 4},
    {"LD A n8", &sm83::LD_A_n, 8},
    {"CCF", &sm83::CCF, 4},
    {"LD B B", &sm83::LD_B_B, 4},
    {"LD B C", &sm83::LD_B_C, 4},
    {"LD B D", &sm83::LD_B_D, 4},
    {"LD B E", &sm83::LD_B_E, 4},
    {"LD B H", &sm83::LD_B_H, 4},
    {"LD B L", &sm83::LD_B_L, 4},
    {"LD B [HL]", &sm83::LD_B_HLi, 8},
    {"LD B A", &sm83::LD_B_A, 4},
    {"LD C B", &sm83::LD_C_B, 4},
    {"LD C C", &sm83::LD_C_C, 4},
    {"LD C D", &sm83::LD_C_D, 4},
    {"LD C E", &sm83::LD_C_E, 4},
    {"LD C H", &sm83::LD_C_H, 4},
    {"LD C L", &sm83::LD_C_L, 4},
    {"LD C [HL]", &sm83::LD_C_HLi, 8},
    {"LD C A", &sm83::LD_C_A, 4},
    {"LD D B", &sm83::LD_D_B, 4},
    {"LD D C", &sm83::LD_D_C, 4},
    {"LD D D", &sm83::LD_D_D, 4},
    {"LD D E", &sm83::LD_D_E, 4},
    {"LD D H", &sm83::LD_D_H, 4},
    {"LD D L", &sm83::LD_D_L, 4},
    {"LD D [HL]", &sm83::LD_D_HLi, 8},
    {"LD D A", &sm83::LD_D_A, 4},
    {"LD E B", &sm83::LD_E_B, 4},
    {"LD E C", &sm83::LD_E_C, 4},
    {"LD E D", &sm83::LD_E_D, 4},
    {"LD E E", &sm83::LD_E_E, 4},
    {"LD E H", &sm83::LD_E_H, 4},
    {"LD E L", &sm83::LD_E_L, 4},
    {"LD E [HL]", &sm83::LD_E_HLi, 8},
    {"LD E A", &sm83::LD_E_A, 4},
    {"LD H B", &sm83::LD_H_B, 4},
    {"LD H C", &sm83::LD_H_C, 4},
    {"LD H D", &sm83::LD_H_D, 4},
    {"LD H E", &sm83::LD_H_E, 4},
    {"LD H H", &sm83::LD_H_H, 4},
    {"LD H L", &sm83::LD_H_L, 4},
    {"LD H [HL]", &sm83::LD_H_HLi, 8},
    {"LD H A", &sm83::LD_H_A, 4},
    {"LD L B", &sm83::LD_L_B, 4},
    {"LD L C", &sm83::LD_L_C, 4},
    {"LD L D", &sm83::LD_L_D, 4},
    {"LD L E", &sm83::LD_L_E, 4},
    {"LD L H", &sm83::LD_L_H, 4},
    {"LD L L", &sm83::LD_L_L, 4},
    {"LD L [HL]", &sm83::LD_L_HLi, 8},
    {"LD L A", &sm83::LD_L_A, 4},
    {"LD [HL] B", &sm83::LD_HLi_B, 8},
    {"LD [HL] C", &sm83::LD_HLi_C, 8},
    {"LD [HL] D", &sm83::LD_HLi_D, 8},
    {"LD [HL] E", &sm83::LD_HLi_E, 8},
    {"LD [HL] H", &sm83::LD_HLi_H, 8},
    {"LD [HL] L", &sm83::LD_HLi_L, 8},
    {"HALT", &sm83::HALT, 4},
    {"LD HL A", &sm83::LD_HLi_A, 8},
    {"LD A B", &sm83::LD_A_B, 4},
    {"LD A C", &sm83::LD_A_C, 4},
    {"LD A D", &sm83::LD_A_D, 4},
    {"LD A E", &sm83::LD_A_E, 4},
    {"LD A H", &sm83::LD_A_H, 4},
    {"LD A L", &sm83::LD_A_L, 4},
    {"LD A [HL]", &sm83::LD_A_HLi, 8},
    {"LD A A", &sm83::LD_A_A, 4},
    {"ADD A B", &sm83::ADD_A_B, 4},
    {"ADD A C", &sm83::ADD_A_C, 4},
    {"ADD A D", &sm83::ADD_A_D, 4},
    {"ADD A E", &sm83::ADD_A_E, 4},
    {"ADD A H", &sm83::ADD_A_H, 4},
    {"ADD A L", &sm83::ADD_A_L, 4},
    {"ADD A [HL]", &sm83::ADD_A_HLi, 8},
    {"ADD A A", &sm83::ADD_A_A, 4},
    {"ADC A B", &sm83::ADC_A_B, 4},
    {"ADC A C", &sm83::ADC_A_C, 4},
    {"ADC A D", &sm83::ADC_A_D, 4},
    {"ADC A E", &sm83::ADC_A_E, 4},
    {"ADC A H", &sm83::ADC_A_H, 4},
    {"ADC A L", &sm83::ADC_A_L, 4},
    {"ADC A HL", &sm83::ADC_A_HLi, 8},
    {"ADC A A", &sm83::ADC_A_A, 4},
    {"SUB A B", &sm83::SUB_A_B, 4},
    {"SUB A C", &sm83::SUB_A_C, 4},
    {"SUB A D", &sm83::SUB_A_D, 4},
    {"SUB A E", &sm83::SUB_A_E, 4},
    {"SUB A H", &sm83::SUB_A_H, 4},
    {"SUB A L", &sm83::SUB_A_L, 4},
    {"SUB A HL", &sm83::SUB_A_HLi, 8},
    {"SUB A A", &sm83::SUB_A_A, 4},
    {"SBC A B", &sm83::SBC_A_B, 4},
    {"SBC A C", &sm83::SBC_A_C, 4},
    {"SBC A D", &sm83::SBC_A_D, 4},
    {"SBC A E", &sm83::SBC_A_E, 4},
    {"SBC A H", &sm83::SBC_A_H, 4},
    {"SBC A L", &sm83::SBC_A_L, 4},
    {"SBC A HL", &sm83::SBC_A_HLi, 8},
    {"SBC A A", &sm83::SBC_A_A, 4},
    {"AND A B", &sm83::AND_A_B, 4},
    {"AND A C", &sm83::AND_A_C, 4},
    {"AND A D", &sm83::AND_A_D, 4},
    {"AND A E", &sm83::AND_A_E, 4},
    {"AND A H", &sm83::AND_A_H, 4},
    {"AND A L", &sm83::AND_A_L, 4},
    {"AND A HL", &sm83::AND_A_HLi, 8},
    {"AND A A", &sm83::AND_A_A, 4},
    {"XOR A B", &sm83::XOR_A_B, 4},
    {"XOR A C", &sm83::XOR_A_C, 4},
    {"XOR A D", &sm83::XOR_A_D, 4},
    {"XOR A E", &sm83::XOR_A_E, 4},
    {"XOR A H", &sm83::XOR_A_H, 4},
    {"XOR A L", &sm83::XOR_A_L, 4},
    {"XOR A HL", &sm83::XOR_A_HLi, 8},
    {"XOR A A", &sm83::XOR_A_A, 4},
    {"OR A B", &sm83::OR_A_B, 4},
    {"OR A C", &sm83::OR_A_C, 4},
    {"OR A D", &sm83::OR_A_D, 4},
    {"OR A E", &sm83::OR_A_E, 4},
    {"OR A H", &sm83::OR_A_H, 4},
    {"OR A L", &sm83::OR_A_L, 4},
    {"OR A HL", &sm83::OR_A_HLi, 8},
    {"OR A A", &sm83::OR_A_A, 4},
    {"CP A B", &sm83::CP_A_B, 4},
    {"CP A C", &sm83::CP_A_C, 4},
    {"CP A D", &sm83::CP_A_D, 4},
    {"CP A E", &sm83::CP_A_E, 4},
    {"CP A H", &sm83::CP_A_H, 4},
    {"CP A L", &sm83::CP_A_L, 4},
    {"CP A HL", &sm83::CP_A_HLi, 8},
    {"CP A A", &sm83::CP_A_A, 4},
    {"RET NZ", &sm83::RET_NZ, 20},
    {"POP BC", &sm83::POP_BC, 12},
    {"JP NZ a16", &sm83::JP_NZ_nn, 16},
    {"JP a16", &sm83::JP_nn, 16},
    {"CALL NZ a16", &sm83::CALL_NZ_nn, 24},
    {"PUSH BC", &sm83::PUSH_BC, 16},
    {"ADD A n8", &sm83::ADD_A_n, 8},
    {"RST $00", &sm83::RST_$00, 16},
    {"RET Z", &sm83::RET_Z, 20},
    {"RET", &sm83::RET, 16},
    {"JP Z a16", &sm83::JP_Z_nn, 16},
    {"PREFIX", &sm83::PREFIX, 4},
    {"CALL Z a16", &sm83::CALL_Z_nn, 24},
    {"CALL a16", &sm83::CALL_nn, 24},
    {"ADC A n8", &sm83::ADC_A_n, 8},
    {"RST $08", &sm83::RST_$08, 16},
    {"RET NC", &sm83::RET_NC, 20},
    {"POP DE", &sm83::POP_DE, 12},
    {"JP NC a16", &sm83::JP_NC_nn, 16},
    {"ILLEGAL_D3", &sm83::ILLEGAL, 4},
    {"CALL NC a16", &sm83::CALL_NC_nn, 24},
    {"PUSH DE", &sm83::PUSH_DE, 16},
    {"SUB A n8", &sm83::SUB_A_n, 8},
    {"RST $10", &sm83::RST_$10, 16},
    {"RET C", &sm83::RET_C, 20},
    {"RETI", &sm83::RETI, 16},
    {"JP C a16", &sm83::JP_C_nn, 16},
    {"ILLEGAL_DB", &sm83::ILLEGAL, 4},
    {"CALL C a16", &sm83::CALL_C_nn, 24},
    {"ILLEGAL_DD", &sm83::ILLEGAL, 4},
    {"SBC A n8", &sm83::SBC_A_n, 8},
    {"RST $18", &sm83::RST_$18, 16},
    {"LDH a8 A", &sm83::LDH_ni_A, 12},
    {"POP HL", &sm83::POP_HL, 12},
    {"LDH C A", &sm83::LDH_Ci_A, 8},
    {"ILLEGAL_E3", &sm83::ILLEGAL, 4},
    {"ILLEGAL_E4", &sm83::ILLEGAL, 4},
    {"PUSH HL", &sm83::PUSH_HL, 16},
    {"AND A n8", &sm83::AND_A_n, 8},
    {"RST $20", &sm83::RST_$20, 16},
    {"ADD SP e8", &sm83::ADD_SP_e, 16},
    {"JP HL", &sm83::JP_HL, 4},
    {"LD a16 A", &sm83::LD_nni_A, 16},
    {"ILLEGAL_EB", &sm83::ILLEGAL, 4},
    {"ILLEGAL_EC", &sm83::ILLEGAL, 4},
    {"ILLEGAL_ED", &sm83::ILLEGAL, 4},
    {"XOR A n8", &sm83::XOR_A_n, 8},
    {"RST $28", &sm83::RST_$28, 16},
    {"LDH A a8", &sm83::LDH_A_ni, 12},
    {"POP AF", &sm83::POP_AF, 12},
    {"LDH A C", &sm83::LDH_A_Ci, 8},
    {"DI", &sm83::DI, 4},
    {"ILLEGAL_F4", &sm83::ILLEGAL, 4},
    {"PUSH AF", &sm83::PUSH_AF, 16},
    {"OR A n8", &sm83::OR_A_n, 8},
    {"RST $30", &sm83::RST_$30, 16},
    {"LD HL SP e8", &sm83::LD_HL_SP_plus_e, 12},
    {"LD SP HL", &sm83::LD_SP_HL, 8},
    {"LD A a16", &sm83::LD_A_nni, 16},
    {"EI", &sm83::EI, 4},
    {"ILLEGAL_FC", &sm83::ILLEGAL, 4},
    {"ILLEGAL_FD", &sm83::ILLEGAL, 4},
    {"CP A n8", &sm83::CP_A_n, 8},
    {"RST $38", &sm83::RST_$38, 16}
};
    prefixedTable = {{"RLC B", &sm83::RLC_B, 8},
    {"RLC C", &sm83::RLC_C, 8},
    {"RLC D", &sm83::RLC_D, 8},
    {"RLC E", &sm83::RLC_E, 8},
    {"RLC H", &sm83::RLC_H, 8},
    {"RLC L", &sm83::RLC_L, 8},
    {"RLC HL", &sm83::RLC_HLi, 16},
    {"RLC A", &sm83::RLC_A, 8},
    {"RRC B", &sm83::RRC_B, 8},
    {"RRC C", &sm83::RRC_C, 8},
    {"RRC D", &sm83::RRC_D, 8},
    {"RRC E", &sm83::RRC_E, 8},
    {"RRC H", &sm83::RRC_H, 8},
    {"RRC L", &sm83::RRC_L, 8},
    {"RRC HL", &sm83::RRC_HLi, 16},
    {"RRC A", &sm83::RRC_A, 8},
    {"RL B", &sm83::RL_B, 8},
    {"RL C", &sm83::RL_C, 8},
    {"RL D", &sm83::RL_D, 8},
    {"RL E", &sm83::RL_E, 8},
    {"RL H", &sm83::RL_H, 8},
    {"RL L", &sm83::RL_L, 8},
    {"RL HL", &sm83::RL_HLi, 16},
    {"RL A", &sm83::RL_A, 8},
    {"RR B", &sm83::RR_B, 8},
    {"RR C", &sm83::RR_C, 8},
    {"RR D", &sm83::RR_D, 8},
    {"RR E", &sm83::RR_E, 8},
    {"RR H", &sm83::RR_H, 8},
    {"RR L", &sm83::RR_L, 8},
    {"RR HL", &sm83::RR_HLi, 16},
    {"RR A", &sm83::RR_A, 8},
    {"SLA B", &sm83::SLA_B, 8},
    {"SLA C", &sm83::SLA_C, 8},
    {"SLA D", &sm83::SLA_D, 8},
    {"SLA E", &sm83::SLA_E, 8},
    {"SLA H", &sm83::SLA_H, 8},
    {"SLA L", &sm83::SLA_L, 8},
    {"SLA HL", &sm83::SLA_HLi, 16},
    {"SLA A", &sm83::SLA_A, 8},
    {"SRA B", &sm83::SRA_B, 8},
    {"SRA C", &sm83::SRA_C, 8},
    {"SRA D", &sm83::SRA_D, 8},
    {"SRA E", &sm83::SRA_E, 8},
    {"SRA H", &sm83::SRA_H, 8},
    {"SRA L", &sm83::SRA_L, 8},
    {"SRA HL", &sm83::SRA_HLi, 16},
    {"SRA A", &sm83::SRA_A, 8},
    {"SWAP B", &sm83::SWAP_B, 8},
    {"SWAP C", &sm83::SWAP_C, 8},
    {"SWAP D", &sm83::SWAP_D, 8},
    {"SWAP E", &sm83::SWAP_E, 8},
    {"SWAP H", &sm83::SWAP_H, 8},
    {"SWAP L", &sm83::SWAP_L, 8},
    {"SWAP HL", &sm83::SWAP_HLi, 16},
    {"SWAP A", &sm83::SWAP_A, 8},
    {"SRL B", &sm83::SRL_B, 8},
    {"SRL C", &sm83::SRL_C, 8},
    {"SRL D", &sm83::SRL_D, 8},
    {"SRL E", &sm83::SRL_E, 8},
    {"SRL H", &sm83::SRL_H, 8},
    {"SRL L", &sm83::SRL_L, 8},
    {"SRL HL", &sm83::SRL_HLi, 16},
    {"SRL A", &sm83::SRL_A, 8},
    {"BIT 0 B", &sm83::BIT_0_B, 8},
    {"BIT 0 C", &sm83::BIT_0_C, 8},
    {"BIT 0 D", &sm83::BIT_0_D, 8},
    {"BIT 0 E", &sm83::BIT_0_E, 8},
    {"BIT 0 H", &sm83::BIT_0_H, 8},
    {"BIT 0 L", &sm83::BIT_0_L, 8},
    {"BIT 0 HL", &sm83::BIT_0_HLi, 12},
    {"BIT 0 A", &sm83::BIT_0_A, 8},
    {"BIT 1 B", &sm83::BIT_1_B, 8},
    {"BIT 1 C", &sm83::BIT_1_C, 8},
    {"BIT 1 D", &sm83::BIT_1_D, 8},
    {"BIT 1 E", &sm83::BIT_1_E, 8},
    {"BIT 1 H", &sm83::BIT_1_H, 8},
    {"BIT 1 L", &sm83::BIT_1_L, 8},
    {"BIT 1 HL", &sm83::BIT_1_HLi, 12},
    {"BIT 1 A", &sm83::BIT_1_A, 8},
    {"BIT 2 B", &sm83::BIT_2_B, 8},
    {"BIT 2 C", &sm83::BIT_2_C, 8},
    {"BIT 2 D", &sm83::BIT_2_D, 8},
    {"BIT 2 E", &sm83::BIT_2_E, 8},
    {"BIT 2 H", &sm83::BIT_2_H, 8},
    {"BIT 2 L", &sm83::BIT_2_L, 8},
    {"BIT 2 HL", &sm83::BIT_2_HLi, 12},
    {"BIT 2 A", &sm83::BIT_2_A, 8},
    {"BIT 3 B", &sm83::BIT_3_B, 8},
    {"BIT 3 C", &sm83::BIT_3_C, 8},
    {"BIT 3 D", &sm83::BIT_3_D, 8},
    {"BIT 3 E", &sm83::BIT_3_E, 8},
    {"BIT 3 H", &sm83::BIT_3_H, 8},
    {"BIT 3 L", &sm83::BIT_3_L, 8},
    {"BIT 3 HL", &sm83::BIT_3_HLi, 12},
    {"BIT 3 A", &sm83::BIT_3_A, 8},
    {"BIT 4 B", &sm83::BIT_4_B, 8},
    {"BIT 4 C", &sm83::BIT_4_C, 8},
    {"BIT 4 D", &sm83::BIT_4_D, 8},
    {"BIT 4 E", &sm83::BIT_4_E, 8},
    {"BIT 4 H", &sm83::BIT_4_H, 8},
    {"BIT 4 L", &sm83::BIT_4_L, 8},
    {"BIT 4 HL", &sm83::BIT_4_HLi, 12},
    {"BIT 4 A", &sm83::BIT_4_A, 8},
    {"BIT 5 B", &sm83::BIT_5_B, 8},
    {"BIT 5 C", &sm83::BIT_5_C, 8},
    {"BIT 5 D", &sm83::BIT_5_D, 8},
    {"BIT 5 E", &sm83::BIT_5_E, 8},
    {"BIT 5 H", &sm83::BIT_5_H, 8},
    {"BIT 5 L", &sm83::BIT_5_L, 8},
    {"BIT 5 HL", &sm83::BIT_5_HLi, 12},
    {"BIT 5 A", &sm83::BIT_5_A, 8},
    {"BIT 6 B", &sm83::BIT_6_B, 8},
    {"BIT 6 C", &sm83::BIT_6_C, 8},
    {"BIT 6 D", &sm83::BIT_6_D, 8},
    {"BIT 6 E", &sm83::BIT_6_E, 8},
    {"BIT 6 H", &sm83::BIT_6_H, 8},
    {"BIT 6 L", &sm83::BIT_6_L, 8},
    {"BIT 6 HL", &sm83::BIT_6_HLi, 12},
    {"BIT 6 A", &sm83::BIT_6_A, 8},
    {"BIT 7 B", &sm83::BIT_7_B, 8},
    {"BIT 7 C", &sm83::BIT_7_C, 8},
    {"BIT 7 D", &sm83::BIT_7_D, 8},
    {"BIT 7 E", &sm83::BIT_7_E, 8},
    {"BIT 7 H", &sm83::BIT_7_H, 8},
    {"BIT 7 L", &sm83::BIT_7_L, 8},
    {"BIT 7 HL", &sm83::BIT_7_HLi, 12},
    {"BIT 7 A", &sm83::BIT_7_A, 8},
    {"RES 0 B", &sm83::RES_0_B, 8},
    {"RES 0 C", &sm83::RES_0_C, 8},
    {"RES 0 D", &sm83::RES_0_D, 8},
    {"RES 0 E", &sm83::RES_0_E, 8},
    {"RES 0 H", &sm83::RES_0_H, 8},
    {"RES 0 L", &sm83::RES_0_L, 8},
    {"RES 0 HL", &sm83::RES_0_HLi, 16},
    {"RES 0 A", &sm83::RES_0_A, 8},
    {"RES 1 B", &sm83::RES_1_B, 8},
    {"RES 1 C", &sm83::RES_1_C, 8},
    {"RES 1 D", &sm83::RES_1_D, 8},
    {"RES 1 E", &sm83::RES_1_E, 8},
    {"RES 1 H", &sm83::RES_1_H, 8},
    {"RES 1 L", &sm83::RES_1_L, 8},
    {"RES 1 HL", &sm83::RES_1_HLi, 16},
    {"RES 1 A", &sm83::RES_1_A, 8},
    {"RES 2 B", &sm83::RES_2_B, 8},
    {"RES 2 C", &sm83::RES_2_C, 8},
    {"RES 2 D", &sm83::RES_2_D, 8},
    {"RES 2 E", &sm83::RES_2_E, 8},
    {"RES 2 H", &sm83::RES_2_H, 8},
    {"RES 2 L", &sm83::RES_2_L, 8},
    {"RES 2 HL", &sm83::RES_2_HLi, 16},
    {"RES 2 A", &sm83::RES_2_A, 8},
    {"RES 3 B", &sm83::RES_3_B, 8},
    {"RES 3 C", &sm83::RES_3_C, 8},
    {"RES 3 D", &sm83::RES_3_D, 8},
    {"RES 3 E", &sm83::RES_3_E, 8},
    {"RES 3 H", &sm83::RES_3_H, 8},
    {"RES 3 L", &sm83::RES_3_L, 8},
    {"RES 3 HL", &sm83::RES_3_HLi, 16},
    {"RES 3 A", &sm83::RES_3_A, 8},
    {"RES 4 B", &sm83::RES_4_B, 8},
    {"RES 4 C", &sm83::RES_4_C, 8},
    {"RES 4 D", &sm83::RES_4_D, 8},
    {"RES 4 E", &sm83::RES_4_E, 8},
    {"RES 4 H", &sm83::RES_4_H, 8},
    {"RES 4 L", &sm83::RES_4_L, 8},
    {"RES 4 HL", &sm83::RES_4_HLi, 16},
    {"RES 4 A", &sm83::RES_4_A, 8},
    {"RES 5 B", &sm83::RES_5_B, 8},
    {"RES 5 C", &sm83::RES_5_C, 8},
    {"RES 5 D", &sm83::RES_5_D, 8},
    {"RES 5 E", &sm83::RES_5_E, 8},
    {"RES 5 H", &sm83::RES_5_H, 8},
    {"RES 5 L", &sm83::RES_5_L, 8},
    {"RES 5 HL", &sm83::RES_5_HLi, 16},
    {"RES 5 A", &sm83::RES_5_A, 8},
    {"RES 6 B", &sm83::RES_6_B, 8},
    {"RES 6 C", &sm83::RES_6_C, 8},
    {"RES 6 D", &sm83::RES_6_D, 8},
    {"RES 6 E", &sm83::RES_6_E, 8},
    {"RES 6 H", &sm83::RES_6_H, 8},
    {"RES 6 L", &sm83::RES_6_L, 8},
    {"RES 6 HL", &sm83::RES_6_HLi, 16},
    {"RES 6 A", &sm83::RES_6_A, 8},
    {"RES 7 B", &sm83::RES_7_B, 8},
    {"RES 7 C", &sm83::RES_7_C, 8},
    {"RES 7 D", &sm83::RES_7_D, 8},
    {"RES 7 E", &sm83::RES_7_E, 8},
    {"RES 7 H", &sm83::RES_7_H, 8},
    {"RES 7 L", &sm83::RES_7_L, 8},
    {"RES 7 HL", &sm83::RES_7_HLi, 16},
    {"RES 7 A", &sm83::RES_7_A, 8},
    {"SET 0 B", &sm83::SET_0_B, 8},
    {"SET 0 C", &sm83::SET_0_C, 8},
    {"SET 0 D", &sm83::SET_0_D, 8},
    {"SET 0 E", &sm83::SET_0_E, 8},
    {"SET 0 H", &sm83::SET_0_H, 8},
    {"SET 0 L", &sm83::SET_0_L, 8},
    {"SET 0 HL", &sm83::SET_0_HLi, 16},
    {"SET 0 A", &sm83::SET_0_A, 8},
    {"SET 1 B", &sm83::SET_1_B, 8},
    {"SET 1 C", &sm83::SET_1_C, 8},
    {"SET 1 D", &sm83::SET_1_D, 8},
    {"SET 1 E", &sm83::SET_1_E, 8},
    {"SET 1 H", &sm83::SET_1_H, 8},
    {"SET 1 L", &sm83::SET_1_L, 8},
    {"SET 1 HL", &sm83::SET_1_HLi, 16},
    {"SET 1 A", &sm83::SET_1_A, 8},
    {"SET 2 B", &sm83::SET_2_B, 8},
    {"SET 2 C", &sm83::SET_2_C, 8},
    {"SET 2 D", &sm83::SET_2_D, 8},
    {"SET 2 E", &sm83::SET_2_E, 8},
    {"SET 2 H", &sm83::SET_2_H, 8},
    {"SET 2 L", &sm83::SET_2_L, 8},
    {"SET 2 HL", &sm83::SET_2_HLi, 16},
    {"SET 2 A", &sm83::SET_2_A, 8},
    {"SET 3 B", &sm83::SET_3_B, 8},
    {"SET 3 C", &sm83::SET_3_C, 8},
    {"SET 3 D", &sm83::SET_3_D, 8},
    {"SET 3 E", &sm83::SET_3_E, 8},
    {"SET 3 H", &sm83::SET_3_H, 8},
    {"SET 3 L", &sm83::SET_3_L, 8},
    {"SET 3 HL", &sm83::SET_3_HLi, 16},
    {"SET 3 A", &sm83::SET_3_A, 8},
    {"SET 4 B", &sm83::SET_4_B, 8},
    {"SET 4 C", &sm83::SET_4_C, 8},
    {"SET 4 D", &sm83::SET_4_D, 8},
    {"SET 4 E", &sm83::SET_4_E, 8},
    {"SET 4 H", &sm83::SET_4_H, 8},
    {"SET 4 L", &sm83::SET_4_L, 8},
    {"SET 4 HL", &sm83::SET_4_HLi, 16},
    {"SET 4 A", &sm83::SET_4_A, 8},
    {"SET 5 B", &sm83::SET_5_B, 8},
    {"SET 5 C", &sm83::SET_5_C, 8},
    {"SET 5 D", &sm83::SET_5_D, 8},
    {"SET 5 E", &sm83::SET_5_E, 8},
    {"SET 5 H", &sm83::SET_5_H, 8},
    {"SET 5 L", &sm83::SET_5_L, 8},
    {"SET 5 HL", &sm83::SET_5_HLi, 16},
    {"SET 5 A", &sm83::SET_5_A, 8},
    {"SET 6 B", &sm83::SET_6_B, 8},
    {"SET 6 C", &sm83::SET_6_C, 8},
    {"SET 6 D", &sm83::SET_6_D, 8},
    {"SET 6 E", &sm83::SET_6_E, 8},
    {"SET 6 H", &sm83::SET_6_H, 8},
    {"SET 6 L", &sm83::SET_6_L, 8},
    {"SET 6 HL", &sm83::SET_6_HLi, 16},
    {"SET 6 A", &sm83::SET_6_A, 8},
    {"SET 7 B", &sm83::SET_7_B, 8},
    {"SET 7 C", &sm83::SET_7_C, 8},
    {"SET 7 D", &sm83::SET_7_D, 8},
    {"SET 7 E", &sm83::SET_7_E, 8},
    {"SET 7 H", &sm83::SET_7_H, 8},
    {"SET 7 L", &sm83::SET_7_L, 8},
    {"SET 7 HL", &sm83::SET_7_HLi, 16},
    {"SET 7 A", &sm83::SET_7_A, 8}
};
}

sm83::~sm83() = default;

uint8_t sm83::read(uint16_t addr) {
    return bus->read(addr);
}

void sm83::write(uint16_t addr, uint8_t data) {
    bus->write(addr, data);
}

void sm83::clock() {
    // // Handle HALT state
    // if (halted) {
    //     IF = read(0xFF0F);
    //     if ((IE & IF) != 0) {
    //         // Wake up on any pending interrupt
    //         halted = false;
    //     } else {
    //         return; // Stay halted
    //     }
    // }
    //
    // if (stopped) {
    //     // Check Joypad input (P1 register at 0xFF00)
    //     uint8_t joy = read(0xFF00);
    //     if ((joy & 0x0F) != 0x0F) {
    //         // One of the buttons pressed, wake up
    //         stopped = false;
    //     } else {
    //         return; // stay stopped
    //     }
    // }

    // if (cycle == 0) {
        IR = read(PC); // Read Opcode
        PC++; // Increment program counter
        cycle = opcodeTable[IR].cycles / 4; // Set cycles
        const uint8_t additionalCycles = (this->*opcodeTable[IR].operate)();
        cycle -= additionalCycles;

        checkInterrupts();
    // }
    // cycle--;
}

void sm83::checkInterrupts() {
    IF = read(0xFF0F);
    uint8_t fired = IE & IF;

    if (IME && fired) {
        halted = false;  // wake up
        IME = false;     // disable further interrupts

        for (int i = 0; i < 5; i++) {
            if (fired & (1 << i)) {
                write(0xFF0F, IF & ~(1 << i)); // clear IF bit

                // Push PC onto stack
                SP--;
                write(SP, (PC >> 8) & 0xFF);
                SP--;
                write(SP, PC & 0xFF);

                static const uint16_t vectors[5] = {0x40, 0x48, 0x50, 0x58, 0x60};
                PC = vectors[i];
                break;
            }
        }
    }

    // Apply delayed EI
    if (IME_next) {
        IME = true;
        IME_next = false;
    }
}

void sm83::setFlags(const FlagRegisters f, const bool v) const {
    if (v) {
        F |= f;
    } else {
        F &= ~f;
    }
}

uint8_t sm83::getFlags(const FlagRegisters f) const {
    return ((F & f) > 0) ? 1 : 0;
}


// Opcode implementation

uint8_t sm83::LD_r_r(uint8_t &r1, const uint8_t &r2) {
    r1 = r2;
    return 0;
}

uint8_t sm83::LD_r_n(uint8_t &r1) {
    r1 = read(PC);
    PC++;
    return 0;
}

uint8_t sm83::LD_HLi_r(const uint8_t &r1){
    write(HL, r1);
    return 0;
}

uint8_t sm83::LD_r_HLi(uint8_t &r1) {
    r1 = read(HL);
    return 0;
}

uint8_t sm83::LD_HLi_n() {
    write(HL, read(PC));
    PC++;
    return 0;
}

uint8_t sm83::LD_A_BCi() {
    A = read(BC);
    return 0;
}

uint8_t sm83::LD_A_DEi() {
    A = read(DE);
    return 0;
}

uint8_t sm83::LD_BCi_A() {
    write(BC, A);
    return 0;
}

uint8_t sm83::LD_DEi_A() {
    write(DE, A);
    return 0;
}

uint8_t sm83::LD_A_nni() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t W = read(PC);
    PC++;
    const uint16_t addr = (static_cast<uint16_t>(W) << 8) | static_cast<uint16_t>(Z);
    A = read(addr);
    return 0;
}

uint8_t sm83::LD_nni_A() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t W = read(PC);
    PC++;
    const uint16_t addr = static_cast<uint16_t>(W) << 8 | static_cast<uint16_t>(Z);
    write(addr, A);
    return 0;
}

uint8_t sm83::LDH_A_Ci() {
    const uint16_t addr = static_cast<uint16_t>(0xFF) << 8 | static_cast<uint16_t>(C);
    A = read(addr);
    return 0;
}

uint8_t sm83::LDH_Ci_A() {
    const uint16_t addr = static_cast<uint16_t>(0xFF) << 8 | static_cast<uint16_t>(C);
    write(addr, A);
    return 0;
}

uint8_t sm83::LDH_A_ni() {
    const uint8_t Z = read(PC);
    PC++;
    const uint16_t addr = static_cast<uint16_t>(0xFF) << 8 | static_cast<uint16_t>(Z);
    A = read(addr);
    return 0;
}

uint8_t sm83::LDH_ni_A() {
    const uint8_t Z = read(PC);
    PC++;
    const uint16_t addr = static_cast<uint16_t>(0xFF) << 8 | static_cast<uint16_t>(Z);
    write(addr, A);
    return 0;
}

uint8_t sm83::LD_A_HLim() {
    A = read(HL);
    HL--;
    return 0;
}

uint8_t sm83::LD_HLim_A() {
    write(HL, A);
    HL--;
    return 0;
}

uint8_t sm83::LD_A_HLip() {
    A = read(HL);
    HL++;
    return 0;
}

uint8_t sm83::LD_HLip_A() {
    write(HL, A);
    HL++;
    return 0;
}

uint8_t sm83::LD_rr_nn(uint16_t &rr1) {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t W = read(PC);
    PC++;
    const uint16_t data = static_cast<uint16_t>(W) << 8 | static_cast<uint16_t>(Z);
    rr1 = data;
    return 0;
}

uint8_t sm83::LD_nni_SP() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t W = read(PC);
    PC++;
    const uint16_t addr = static_cast<uint16_t>(W) << 8 | static_cast<uint16_t>(Z);
    write(addr, SP);
    return 0;
}

uint8_t sm83::LD_SP_HL() {
    SP = HL;
    return 0;
}

uint8_t sm83::PUSH_rr(const uint16_t &rr1) {
    SP--;
    write(SP, static_cast<uint8_t>(rr1 >> 8));
    SP--;
    write(SP, static_cast<uint8_t>(rr1));
    return 0;
}

uint8_t sm83::POP_rr(uint16_t &rr1) {
    const uint8_t Z = read(SP);
    SP++;
    const uint8_t W = read(SP);
    SP++;
    rr1 = static_cast<uint16_t>(W) << 8 | Z;
    return 0;
}

uint8_t sm83::LD_HL_SP_plus_e() {
    const auto e = static_cast<int8_t>(read(PC));
    PC++;
    const uint16_t result = SP + e;
    HL = result;
    setFlags(z, false);
    setFlags(n, false);
    setFlags(h, (SP & 0xF) + (e & 0xF) > 0xF);
    setFlags(c, static_cast<uint16_t>((SP & 0xFF) + e) > 0x00FF);
    return 0;
}

uint8_t sm83::ADD_A_r(const uint8_t &r1) {
    const uint8_t result = A + r1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (A & 0xF) + (r1 & 0xF) > 0xF);  // half-carry: bit 3 → 4
    setFlags(c, static_cast<uint16_t>(A) + r1 > 0xFF);  // carry: bit 7 → 8
    A = result;
    return 0;
}

uint8_t sm83::ADD_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = A + Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (A & 0xF) + (Z & 0xF) > 0xF);
    setFlags(c, static_cast<uint16_t>(A) + Z > 0xFF);
    A = result;
    return 0;
}

uint8_t sm83::ADD_A_n() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A + Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (A & 0xF) + (Z & 0xF) > 0xF);
    setFlags(c, static_cast<uint16_t>(A) + Z > 0xFF);
    A = result;
    return 0;
}

uint8_t sm83::ADC_A_r(const uint8_t &r1) {
    const uint8_t flag = getFlags(c);
    const uint8_t result = A + r1 + flag;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (A & 0xF) + (r1 & 0xF) + flag > 0xF);
    setFlags(c, static_cast<uint16_t>(A) + r1 + flag > 0xFF);
    A = result;
    return 0;
}

uint8_t sm83::ADC_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t flag = getFlags(c);
    const uint8_t result = A + Z + flag;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (A & 0xF) + (Z & 0xF) + flag > 0xF);
    setFlags(c, static_cast<uint16_t>(A) + Z + flag > 0xFF);
    A = result;
    return 0;
}

uint8_t sm83::ADC_A_n() {
    const uint8_t flag = getFlags(c);
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A + Z + flag;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (A & 0xF) + (Z & 0xF) + flag > 0xF);
    setFlags(c, static_cast<uint16_t>(A) + Z + flag > 0xFF);
    A = result;
    return 0;
}

uint8_t sm83::SUB_A_r(const uint8_t &r1) {
    const uint8_t result = A - r1;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < (r1 & 0xF));
    setFlags(c, A < r1);
    A = result;
    return 0;
}

uint8_t sm83::SUB_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = A - Z;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < (Z & 0xF));
    setFlags(c, A < Z);
    A = result;
    return 0;
}

uint8_t sm83::SUB_A_n() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A - Z;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < (Z & 0xF));
    setFlags(c, A < Z);
    A = result;
    return 0;
}

uint8_t sm83::SBC_A_r(const uint8_t &r1) {
    const uint8_t flag = getFlags(c);
    const uint8_t result = A - r1 - flag;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < ((r1 + flag & 0xF)));
    setFlags(c, A < static_cast<uint16_t>(r1) + flag);
    A = result;
    return 0;
}

uint8_t sm83::SBC_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t flag = getFlags(c);
    const uint8_t result = A - Z - flag;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < ((Z + flag) & 0xF));
    setFlags(c, A < static_cast<uint16_t>(Z) + flag);
    A = result;
    return 0;
}

uint8_t sm83::SBC_A_n() {
    const uint8_t flag = getFlags(c);
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A - Z - flag;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < ((Z+flag) & 0xF));
    setFlags(c, A < static_cast<uint16_t>(Z) + flag);
    A = result;
    return 0;
}

uint8_t sm83::CP_A_r(const uint8_t &r1) {
    const uint8_t result = A - r1;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < (r1 & 0xF));
    setFlags(c, A < r1);
    return 0;
}

uint8_t sm83::CP_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = A - Z;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < (Z & 0xF));
    setFlags(c, A < Z);
    return 0;
}

uint8_t sm83::CP_A_n() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A - Z;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (A & 0xF) < (Z & 0xF));
    setFlags(c, A < Z);
    return 0;
}

uint8_t sm83::INC_r(uint8_t &r1) {
    const uint8_t result = r1 + 1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (r1 & 0xF) + 1 > 0xF);
    r1 = result;
    return 0;
}

uint8_t sm83::INC_HLi() {
    uint8_t Z = read(HL);
    const uint8_t result = Z + 1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, (Z & 0xF) + 1 > 0xF);
    write(HL, result);
    return 0;
}

uint8_t sm83::DEC_A_r(uint8_t &r1) {
    const uint8_t result = r1 - 1;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (r1 & 0xF) == 0);
    r1 = result;
    return 0;
}

uint8_t sm83::DEC_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = Z - 1;
    setFlags(z, result == 0);
    setFlags(n, true);
    setFlags(h, (Z & 0xF) == 0);
    write(HL, result);
    return 0;
}

uint8_t sm83::AND_A_r(const uint8_t &r1) {
    const uint8_t result = A & r1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, true);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::AND_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = A & Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, true);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::AND_A_n() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A & Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, true);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::OR_A_r(const uint8_t &r1) {
    const uint8_t result = A | r1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::OR_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = A | Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::OR_A_n() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A | Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::XOR_A_r(const uint8_t &r1) {
    const uint8_t result = A ^ r1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::XOR_A_HLi() {
    const uint8_t Z = read(HL);
    const uint8_t result = A ^ Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::XOR_A_n() {
    const uint8_t Z = read(PC);
    PC++;
    const uint8_t result = A ^ Z;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    A = result;
    return 0;
}

uint8_t sm83::CCF() {
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, ~getFlags(c));
    return 0;
}

uint8_t sm83::SCF() {
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, true);
    return 0;
}

uint8_t sm83::DAA() {
    uint8_t result = 0;
    bool carry = getFlags(c);
    if (getFlags(n) == 1) {
        uint8_t adjustment = 0;
        if (getFlags(h) == 1) {
            adjustment = adjustment + 0x06;
        }
        if (getFlags(c) == 1) {
            adjustment = adjustment + 0x60;
        }
        result = A - adjustment;
        A = result;
    } else {
        uint8_t adjustment = 0;
        if (getFlags(h) == 1 || (A & 0x0F) > 0x09) {
            adjustment = adjustment + 0x06;
        }
        if (getFlags(c) == 1 || A > 0x99) {
            adjustment = adjustment + 0x60;
            carry = true;
        }
        result = A + adjustment;
        A = result;
    }
    setFlags(z, A == 0);
    setFlags(h, false);
    setFlags(c, carry);
    return 0;
}

uint8_t sm83::CPL() {
    A = ~A;
    setFlags(n, true);
    setFlags(h, true);
    return 0;
}

uint8_t sm83::INC_rr(uint16_t &rr1) {
    rr1 = rr1 + 1;
    return 0;
}

uint8_t sm83::DEC_rr(uint16_t &rr1) {
    rr1 = rr1 - 1;
    return 0;
}

uint8_t sm83::ADD_HL_rr(const uint16_t &rr1) {
    uint16_t result = HL + rr1;
    setFlags(n, false);
    setFlags(h, (HL & 0xFFF) + (rr1 & 0xFFF) > 0xFFF);
    setFlags(c, result < HL);
    HL = result;
    return 0;
}

uint8_t sm83::ADD_SP_e() {
    const auto e = static_cast<int8_t>(read(PC));
    PC++;
    const uint16_t result = SP + e;
    setFlags(z, false);
    setFlags(n, false);
    setFlags(h, ((SP ^ e ^ result) & 0x10) == 0x10);
    setFlags(c, ((SP ^ e ^ result) & 0x100) == 0x100);
    SP = result;
    return 0;
}

uint8_t sm83::PREFIX() {
    IR = read(PC);
    PC++;

    return (this->*prefixedTable[IR].operate)();
}

uint8_t sm83::RLCA() {
    const uint8_t b7 = (A & 0b10000000) >> 7;
    A = (A << 1) | b7;
    setFlags(z, false);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    return 0;
}

uint8_t sm83::RRCA() {
    const uint8_t b0 = A & 0x01;
    A = (A >> 1) | (b0 << 7);
    setFlags(z, false);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    return 0;
}

uint8_t sm83::RLA() {
    const uint8_t b7 = (A & 0b10000000) >> 7;
    A = (A << 1) | getFlags(c);
    setFlags(z, false);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    return 0;
}

uint8_t sm83::RRA() {
    const uint8_t b0 = A & 0x01;
    A = (A >> 1) | (getFlags(c) << 7);
    setFlags(z, false);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    return 0;
}

uint8_t sm83::RLC_r(uint8_t &r1) {
    const uint8_t b7 = (r1 & 0b10000000) >> 7;
    const uint8_t result = (r1 << 1) | b7;
    r1 = result;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    return 0;
}

uint8_t sm83::RLC_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b7 = (data & 0b10000000) >> 7;
    const uint8_t result = (data << 1) | b7;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::RRC_r(uint8_t &r1) {
    const uint8_t b0 = r1 & 0x01;
    r1 = (r1 >> 1) | (b0 << 7);
    setFlags(z, r1 == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    return 0;
}

uint8_t sm83::RRC_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b0 = data & 0x01;
    const uint8_t result = (data >> 1) | (b0 << 7);
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::RL_r(uint8_t &r1) {
    const uint8_t b7 = (r1 & 0b10000000) >> 7;
    r1 = (r1 << 1) | getFlags(c);
    setFlags(z, r1 == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    return 0;
}

uint8_t sm83::RL_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b7 = (data & 0b10000000) >> 7;
    const uint8_t result = (data << 1) | getFlags(c);
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::RR_r(uint8_t &r1) {
    const uint8_t b0 = r1 & 0x01;
    r1 = (r1 >> 1) | (getFlags(c) << 7);
    setFlags(z, r1 == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    return 0;
}

uint8_t sm83::RR_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b0 = data & 0x01;
    const uint8_t result = (data >> 1) | (getFlags(c) << 7);
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::SLA_r(uint8_t &r1) {
    const uint8_t b7 = r1 & 0b10000000 >> 7;
    r1 = r1 << 1;
    setFlags(z, r1 == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    return 0;
}

uint8_t sm83::SLA_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b7 = (data & 0b10000000) >> 7;
    const uint8_t result = data << 1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b7 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::SRA_r(uint8_t &r1) {
    const uint8_t b0 = r1 & 0x01;
    const uint8_t b7 = r1 & 0b10000000;
    r1 = (r1 >> 1) | b7;
    setFlags(z, r1 == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    return 0;
}

uint8_t sm83::SRA_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b0 = data & 0x01;
    const uint8_t result = data >> 1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::SWAP_r(uint8_t &r1) {
    const uint8_t firstHalf = r1 << 4;
    const uint8_t secondHalf = r1 >> 4;
    const uint8_t result = firstHalf | secondHalf;
    r1 = result;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    return 0;
}

uint8_t sm83::SWAP_HLi() {
    const uint8_t data = read(HL);
    const uint8_t firstHalf = data << 4;
    const uint8_t secondHalf = data >> 4;
    const uint8_t result = firstHalf | secondHalf;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, false);
    write(HL, result);
    return 0;
}

uint8_t sm83::SRL_r(uint8_t &r1) {
    const uint8_t b0 = r1 & 0x01;
    const uint8_t result = (r1 >> 1);
    r1 = result;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    return 0;
}

uint8_t sm83::SRL_HLi() {
    const uint8_t data = read(HL);
    const uint8_t b0 = data & 0x01;
    const uint8_t result = data >> 1;
    setFlags(z, result == 0);
    setFlags(n, false);
    setFlags(h, false);
    setFlags(c, b0 == 0x01);
    write(HL, result);
    return 0;
}

uint8_t sm83::BIT_b_r(const uint8_t b, const uint8_t &r1) {
    const uint8_t mask = 1 << b;
    const bool bitIsZero = (r1 & mask) == 0;
    setFlags(z, bitIsZero);
    setFlags(n, false);
    setFlags(h, true);
    return 0;
}

uint8_t sm83::BIT_b_HLi(const uint8_t b) {
    const uint8_t mask = 1 << b;
    const bool bitIsZero = (read(HL) & mask) == 0;
    setFlags(z, bitIsZero);
    setFlags(n, false);
    setFlags(h, true);
    return 0;
}

uint8_t sm83::RES_b_r(const uint8_t b, uint8_t &r1) {
    const uint8_t mask = 1 << b;
    const uint8_t result = r1 & ~mask;
    r1 = result;
    return 0;
}

uint8_t sm83::RES_b_HLi(const uint8_t b) {
    const uint8_t data = read(HL);
    const uint8_t mask = 1 << b;
    const uint8_t result = data & ~mask;
    write(HL, result);
    return 0;
}

uint8_t sm83::SET_b_r(const uint8_t b, uint8_t &r1) {
    const uint8_t mask = 1 << b;
    const uint8_t result = r1 | mask;
    r1 = result;
    return 0;
}

uint8_t sm83::SET_b_HLi(const uint8_t b) {
    const uint8_t data = read(HL);
    const uint8_t mask = 1 << b;
    const uint8_t result = data | mask;
    write(HL, result);
    return 0;
}

uint8_t sm83::JP_nn() {
    const uint8_t nn_lsb = read(PC++);
    const uint8_t nn_msb = read(PC++);
    const uint16_t nn = nn_msb << 8 | nn_lsb;
    PC = nn;
    return 0;
}

uint8_t sm83::JP_HL() {
    PC = HL;
    return 0;
}

uint8_t sm83::JP_cc_nn(const FlagRegisters f, const bool v) {
    const uint8_t nn_lsb = read(PC++);
    const uint8_t nn_msb = read(PC++);
    const uint16_t nn = nn_msb << 8 | nn_lsb;
    if (getFlags(f) == v) {
        PC = nn;
        return 0;
    }
    return 1;
}

uint8_t sm83::JR_e() {
    const auto e = static_cast<int8_t>(read(PC++));
    PC = static_cast<uint16_t>(PC + e);
    return 0;
}

uint8_t sm83::JR_cc_e(const FlagRegisters flag, const bool v) {
    const auto e = static_cast<int8_t>(read(PC++));
    if (getFlags(flag) == v) {
        PC = static_cast<uint16_t>(PC + e);
        return 0;
    }
    return 1;
}

uint8_t sm83::CALL_nn() {
    const uint8_t nn_lsb = read(PC++);
    const uint8_t nn_msb = read(PC++);
    const uint16_t nn = nn_msb << 8 | nn_lsb;
    SP--;
    write(SP, (PC >> 8) & 0xFF);
    SP--;
    write(SP, PC & 0xFF);
    PC = nn;
    return 0;
}

uint8_t sm83::CALL_cc_nn(const FlagRegisters f, const bool v) {
    const uint8_t nn_lsb = read(PC++);
    const uint8_t nn_msb = read(PC++);
    const uint16_t nn = nn_msb << 8 | nn_lsb;
    if (getFlags(f) == v) {
        SP--;
        write(SP, (PC >> 8) & 0xFF);
        SP--;
        write(SP, PC & 0xFF);
        PC = nn;
        return 0;
    }
    return 3;
}

uint8_t sm83::RET() {
    const uint8_t nn_lsb = read(SP++);
    const uint8_t nn_msb = read(SP++);
    PC = nn_msb << 8 | nn_lsb;
    return 0;
}

uint8_t sm83::RET_cc(const FlagRegisters f, const bool v) {
    if (getFlags(f) == v) {
        const uint8_t nn_lsb = read(SP++);
        const uint8_t nn_msb = read(SP++);
        PC = nn_msb << 8 | nn_lsb;
        return 0;
    }
    return 3;
}

uint8_t sm83::RETI() {
    const uint8_t nn_lsb = read(SP++);
    const uint8_t nn_msb = read(SP++);
    PC = nn_msb << 8 | nn_lsb;
    IME = true;
    return 0;
}

uint8_t sm83::RST_n(const uint8_t n) {
    SP--;
    write(SP, (PC >> 8) & 0xFF);
    SP--;
    write(SP, PC & 0xFF);
    PC = static_cast<uint16_t>(n);
    return 0;
}

uint8_t sm83::HALT() {
    IF = read(0xFF0F); // Interrupt Flag register
    uint8_t pending = IE & IF;

    if (!IME && pending == 0) {
        // HALT bug condition: IME=0, no interrupt pending
        halt_bug = true;
        halted = false;
    } else {
        // Normal HALT: CPU suspends until an interrupt is requested
        halted = true;
    }

    return 0; // HALT itself takes 1 machine cycle
}

uint8_t sm83::STOP_n() {
    // Read and discard the following byte
    uint8_t unused = read(PC++);

    // Handle STOP behavior
    stopped = true;
    return 0;
}

uint8_t sm83::DI() {
    IME = false;
    return 0;
}

uint8_t sm83::EI() {
    IME = true;
    return 0;
}

uint8_t sm83::NOP() {
    return 0;
}

uint8_t sm83::ILLEGAL() {
    return 0;
}

uint8_t sm83::LD_B_n(/*LD_R_n*/) {
    return LD_r_n(B);
}

uint8_t sm83::LD_DE_nn(/*LD_rr_nn*/) {
    return LD_rr_nn(DE);
}

uint8_t sm83::LD_BC_nn() {
    return LD_rr_nn(BC);
}

uint8_t sm83::INC_BC() {
    return INC_rr(BC);
}

uint8_t sm83::DEC_H() {
    return DEC_A_r(H);
}

uint8_t sm83::DEC_B() {
    return DEC_A_r(B);
}

uint8_t sm83::INC_B() {
    return INC_r(B);
}

uint8_t sm83::INC_C() {
    return INC_r(C);
}

uint8_t sm83::DEC_D() {
    return DEC_A_r(D);
}

uint8_t sm83::LD_E_n() {
    return LD_r_n(E);
}

uint8_t sm83::ADD_HL_BC() {
    return ADD_HL_rr(BC);
}

uint8_t sm83::LD_HL_nn() {
    return LD_rr_nn(HL);
}

uint8_t sm83::INC_HL() {
    return INC_rr(HL);
}

uint8_t sm83::JR_Z_e() {
    return JR_cc_e(z, true);
}

uint8_t sm83::DEC_BC() {
    return DEC_rr(BC);
}

uint8_t sm83::DEC_C() {
    return DEC_A_r(C);
}

uint8_t sm83::LD_C_n() {
    return LD_r_n(C);
}

uint8_t sm83::LD_A_HLi() {
    return LD_r_HLi(A);
}

uint8_t sm83::DEC_DE() {
    return DEC_rr(DE);
}

uint8_t sm83::DEC_HL() {
    return DEC_rr(HL);
}

uint8_t sm83::INC_DE() {
    return INC_rr(DE);
}

uint8_t sm83::INC_H() {
    return INC_r(H);
}

uint8_t sm83::INC_D() {
    return INC_r(D);
}

uint8_t sm83::DEC_E() {
    return DEC_A_r(E);
}

uint8_t sm83::INC_E() {
    return INC_r(E);
}

uint8_t sm83::JR_NZ_e() {
    return JR_cc_e(z, false);
}

uint8_t sm83::LD_H_n() {
    return LD_r_n(H);
}

uint8_t sm83::ADD_HL_HL() {
    return ADD_HL_rr(HL);
}

uint8_t sm83::LD_D_n() {
    return LD_r_n(D);
}

uint8_t sm83::ADD_HL_DE() {
    return ADD_HL_rr(DE);
}

uint8_t sm83::INC_L() {
    return INC_r(L);
}

uint8_t sm83::DEC_L() {
    return DEC_A_r(L);
}

uint8_t sm83::LD_L_n() {
    return LD_r_n(L);
}

uint8_t sm83::JR_NC_e() {
    return JR_cc_e(c, false);
}

uint8_t sm83::LD_SP_nn() {
    return LD_rr_nn(SP);
}

uint8_t sm83::LD_HLi_A() {
    return LD_HLi_r(A);
}

uint8_t sm83::INC_SP() {
    return INC_rr(SP);
}

uint8_t sm83::JR_C_e() {
    return JR_cc_e(c, true);
}

uint8_t sm83::ADD_HL_SP() {
    return ADD_HL_rr(SP);
}

uint8_t sm83::DEC_SP() {
    return DEC_rr(SP);
}

uint8_t sm83::INC_A() {
    return INC_r(A);
}

uint8_t sm83::DEC_A() {
    return DEC_A_r(A);
}

uint8_t sm83::LD_A_n() {
    return LD_r_n(A);
}

uint8_t sm83::LD_B_B() {
    return LD_r_r(B, B);
}

uint8_t sm83::LD_B_C() {
    return LD_r_r(B, C);
}

uint8_t sm83::LD_B_D() {
    return LD_r_r(B, D);
}

uint8_t sm83::LD_B_E() {
    return LD_r_r(B, E);
}

uint8_t sm83::LD_B_H() {
    return LD_r_r(B, H);
}

uint8_t sm83::LD_B_L() {
    return LD_r_r(B, L);
}

uint8_t sm83::LD_B_HLi() {
    return LD_r_HLi(B);
}

uint8_t sm83::LD_B_A() {
    return LD_r_r(B, A);
}

uint8_t sm83::LD_C_B() {
    return LD_r_r(C, B);
}

uint8_t sm83::LD_C_C() {
    return LD_r_r(C, C);
}

uint8_t sm83::LD_C_D() {
    return LD_r_r(C, D);
}

uint8_t sm83::LD_C_E() {
    return LD_r_r(C, E);
}

uint8_t sm83::LD_C_H() {
    return LD_r_r(C, H);
}

uint8_t sm83::LD_C_L() {
    return LD_r_r(C, L);
}

uint8_t sm83::LD_C_HLi() {
    return LD_r_HLi(C);
}

uint8_t sm83::LD_C_A() {
    return LD_r_r(C, A);
}

uint8_t sm83::LD_D_B() {
    return LD_r_r(D, B);
}

uint8_t sm83::LD_D_C() {
    return LD_r_r(D, C);
}

uint8_t sm83::LD_D_D() {
    return LD_r_r(D, D);
}

uint8_t sm83::LD_D_E() {
    return LD_r_r(D, E);
}

uint8_t sm83::LD_D_H() {
    return LD_r_r(D, H);
}

uint8_t sm83::LD_D_L() {
    return LD_r_r(D, L);
}

uint8_t sm83::LD_D_HLi() {
    return LD_r_HLi(D);
}

uint8_t sm83::LD_D_A() {
    return LD_r_r(D, A);
}

uint8_t sm83::LD_E_B() {
    return LD_r_r(E, B);
}

uint8_t sm83::LD_H_B() {
    return LD_r_r(H, B);
}

uint8_t sm83::LD_H_C() {
    return LD_r_r(H, C);
}

uint8_t sm83::LD_H_D() {
    return LD_r_r(H, D);
}

uint8_t sm83::LD_H_E() {
    return LD_r_r(H, E);
}

uint8_t sm83::LD_H_H() {
    return LD_r_r(H, H);
}

uint8_t sm83::LD_H_L() {
    return LD_r_r(H, L);
}

uint8_t sm83::LD_H_HLi() {
    return LD_r_HLi(H);
}

uint8_t sm83::LD_E_C() {
    return LD_r_r(E, C);
}

uint8_t sm83::LD_E_D() {
    return LD_r_r(E, D);
}

uint8_t sm83::LD_E_E() {
    return LD_r_r(E, E);
}

uint8_t sm83::LD_E_H() {
    return LD_r_r(E, H);
}

uint8_t sm83::LD_E_L() {
    return LD_r_r(E, L);
}

uint8_t sm83::LD_E_HLi() {
    return LD_r_HLi(E);
}

uint8_t sm83::LD_E_A() {
    return LD_r_r(E, A);
}

uint8_t sm83::LD_H_A() {
    return LD_r_r(H, A);
}

uint8_t sm83::LD_L_B() {
    return LD_r_r(L, B);
}

uint8_t sm83::LD_L_C() {
    return LD_r_r(L, C);
}

uint8_t sm83::LD_L_D() {
    return LD_r_r(L, D);
}

uint8_t sm83::LD_L_E() {
    return LD_r_r(L, E);
}

uint8_t sm83::LD_L_H() {
    return LD_r_r(L, H);
}

uint8_t sm83::LD_L_L() {
    return LD_r_r(L, L);
}

uint8_t sm83::LD_L_HLi() {
    return LD_r_HLi(L);
}

uint8_t sm83::LD_L_A() {
    return LD_r_r(L, A);
}

uint8_t sm83::LD_HLi_B() {
    return LD_HLi_r(B);
}

uint8_t sm83::LD_HLi_C() {
    return LD_HLi_r(C);
}

uint8_t sm83::LD_HLi_D() {
    return LD_HLi_r(D);
}

uint8_t sm83::LD_HLi_E() {
    return LD_HLi_r(E);
}

uint8_t sm83::LD_HLi_H() {
    return LD_HLi_r(H);
}

uint8_t sm83::LD_HLi_L() {
    return LD_HLi_r(L);
}

uint8_t sm83::LD_A_B() {
    return LD_r_r(A, B);
}

uint8_t sm83::LD_A_C() {
    return LD_r_r(A, C);
}

uint8_t sm83::LD_A_D() {
    return LD_r_r(A, D);
}

uint8_t sm83::LD_A_E() {
    return LD_r_r(A, E);
}

uint8_t sm83::LD_A_H() {
    return LD_r_r(A, H);
}

uint8_t sm83::LD_A_L() {
    return LD_r_r(A, L);
}

uint8_t sm83::LD_A_A() {
    return LD_r_r(A, A);
}

uint8_t sm83::ADD_A_B() {
    return ADD_A_r(B);
}

uint8_t sm83::ADD_A_C() {
    return ADD_A_r(C);
}

uint8_t sm83::ADD_A_D() {
    return ADD_A_r(D);
}

uint8_t sm83::ADD_A_E() {
    return ADD_A_r(E);
}

uint8_t sm83::ADD_A_H() {
    return ADD_A_r(H);
}

uint8_t sm83::ADD_A_L() {
    return ADD_A_r(L);
}

uint8_t sm83::ADD_A_A() {
    return ADD_A_r(A);
}

uint8_t sm83::ADC_A_B() {
    return ADC_A_r(B);
}

uint8_t sm83::ADC_A_C() {
    return ADC_A_r(C);
}

uint8_t sm83::ADC_A_D() {
    return ADC_A_r(D);
}

uint8_t sm83::ADC_A_E() {
    return ADC_A_r(E);
}

uint8_t sm83::ADC_A_H() {
    return ADC_A_r(H);
}

uint8_t sm83::ADC_A_L() {
    return ADC_A_r(L);
}

uint8_t sm83::ADC_A_A() {
    return ADC_A_r(A);
}

uint8_t sm83::SUB_A_B() {
    return SUB_A_r(B);
}

uint8_t sm83::SUB_A_C() {
    return SUB_A_r(C);
}

uint8_t sm83::SUB_A_D() {
    return SUB_A_r(D);
}

uint8_t sm83::SUB_A_E() {
    return SUB_A_r(E);
}

uint8_t sm83::SUB_A_H() {
    return SUB_A_r(H);
}

uint8_t sm83::SUB_A_L() {
    return SUB_A_r(L);
}

uint8_t sm83::SUB_A_A() {
    return SUB_A_r(A);
}

uint8_t sm83::SBC_A_B() {
    return SBC_A_r(B);
}

uint8_t sm83::SBC_A_C() {
    return SBC_A_r(C);
}

uint8_t sm83::SBC_A_D() {
    return SBC_A_r(D);
}

uint8_t sm83::SBC_A_E() {
    return SBC_A_r(E);
}

uint8_t sm83::SBC_A_H() {
    return SBC_A_r(H);
}

uint8_t sm83::SBC_A_L() {
    return SBC_A_r(L);
}

uint8_t sm83::SBC_A_A() {
    return SBC_A_r(A);
}

uint8_t sm83::AND_A_B() {
    return AND_A_r(B);
}

uint8_t sm83::AND_A_C() {
    return AND_A_r(C);
}

uint8_t sm83::AND_A_D() {
    return AND_A_r(D);
}

uint8_t sm83::AND_A_E() {
    return AND_A_r(E);
}

uint8_t sm83::AND_A_H() {
    return AND_A_r(H);
}

uint8_t sm83::AND_A_L() {
    return AND_A_r(L);
}

uint8_t sm83::AND_A_A() {
    return AND_A_r(A);
}

uint8_t sm83::OR_A_B() {
    return OR_A_r(B);
}

uint8_t sm83::OR_A_C() {
    return OR_A_r(C);
}

uint8_t sm83::XOR_A_B() {
    return XOR_A_r(B);
}

uint8_t sm83::XOR_A_C() {
    return XOR_A_r(C);
}

uint8_t sm83::XOR_A_D() {
    return XOR_A_r(D);
}

uint8_t sm83::XOR_A_E() {
    return XOR_A_r(E);
}

uint8_t sm83::XOR_A_H() {
    return XOR_A_r(H);
}

uint8_t sm83::XOR_A_L() {
    return XOR_A_r(L);
}

uint8_t sm83::XOR_A_A() {
    return XOR_A_r(A);
}

uint8_t sm83::OR_B_A() {
    return OR_A_r(B);
}

uint8_t sm83::OR_A_D() {
    return OR_A_r(D);
}

uint8_t sm83::OR_A_E() {
    return OR_A_r(E);
}

uint8_t sm83::OR_A_H() {
    return OR_A_r(H);
}

uint8_t sm83::OR_A_L() {
    return OR_A_r(L);
}

uint8_t sm83::OR_A_A() {
    return OR_A_r(A);
}

uint8_t sm83::CP_A_B() {
    return CP_A_r(B);
}

uint8_t sm83::CP_A_C() {
    return CP_A_r(C);
}

uint8_t sm83::CP_A_D() {
    return CP_A_r(D);
}

uint8_t sm83::CP_A_E() {
    return CP_A_r(E);
}

uint8_t sm83::CP_A_H() {
    return CP_A_r(H);
}

uint8_t sm83::CP_A_L() {
    return CP_A_r(L);
}

uint8_t sm83::CP_A_A() {
    return CP_A_r(A);
}

uint8_t sm83::RET_NZ() {
    return RET_cc(z, false);
}

uint8_t sm83::POP_BC() {
    return POP_rr(BC);
}

uint8_t sm83::JP_NZ_nn() {
    return JP_cc_nn(z, false);
}

uint8_t sm83::CALL_NZ_nn() {
    return CALL_cc_nn(z, false);
}

uint8_t sm83::PUSH_BC() {
    return PUSH_rr(BC);
}

uint8_t sm83::RST_$00() {
    return RST_n(0x00);
}

uint8_t sm83::RET_Z() {
    return RET_cc(z, true);
}

uint8_t sm83::JP_Z_nn() {
    return JP_cc_nn(z, true);
}

uint8_t sm83::CALL_Z_nn() {
    return CALL_cc_nn(z, true);
}

uint8_t sm83::RST_$08() {
    return RST_n(0x08);
}

uint8_t sm83::RET_NC() {
    return RET_cc(c, false);
}

uint8_t sm83::POP_DE() {
    return POP_rr(DE);
}

uint8_t sm83::JP_NC_nn() {
    return JP_cc_nn(c, false);
}

uint8_t sm83::CALL_NC_nn() {
    return CALL_cc_nn(c, false);
}

uint8_t sm83::PUSH_DE() {
    return PUSH_rr(DE);
}

uint8_t sm83::RST_$10() {
    return RST_n(0x10);
}

uint8_t sm83::RET_C() {
    return RET_cc(c, true);
}

uint8_t sm83::JP_C_nn() {
    return JP_cc_nn(c, true);
}

uint8_t sm83::CALL_C_nn() {
    return CALL_cc_nn(c, true);
}

uint8_t sm83::PUSH_HL() {
    return PUSH_rr(HL);
}

uint8_t sm83::RST_$18() {
    return RST_n(0x18);
}

uint8_t sm83::POP_HL() {
    return POP_rr(HL);
}

uint8_t sm83::RST_$20() {
    return RST_n(0x20);
}

uint8_t sm83::RST_$28() {
    return RST_n(0x28);
}

uint8_t sm83::POP_AF() {
    uint8_t low = read(SP++);
    uint8_t high = read(SP++);
    AF = (high << 8) | (low & 0xF0); // lower nibble forced to 0
    return 0;
}

uint8_t sm83::PUSH_AF() {
    return PUSH_rr(AF);
}

uint8_t sm83::RST_$30() {
    return RST_n(0x30);
}

uint8_t sm83::RST_$38() {
    return RST_n(0x38);
}

uint8_t sm83::RLC_B() {
    return RLC_r(B);
}

uint8_t sm83::RLC_C() {
    return RLC_r(C);
}

uint8_t sm83::RLC_D() {
    return RLC_r(D);
}

uint8_t sm83::RLC_E() {
    return RLC_r(E);
}

uint8_t sm83::RLC_H() {
    return RLC_r(H);
}

uint8_t sm83::RLC_L() {
    return RLC_r(L);
}

uint8_t sm83::RLC_A() {
    return RLC_r(A);
}

uint8_t sm83::RRC_B() {
    return RRC_r(B);
}

uint8_t sm83::RRC_C() {
    return RRC_r(C);
}

uint8_t sm83::RRC_D() {
    return RRC_r(D);
}

uint8_t sm83::RRC_E() {
    return RRC_r(E);
}

uint8_t sm83::RRC_H() {
    return RRC_r(H);
}

uint8_t sm83::RRC_L() {
    return RRC_r(L);
}

uint8_t sm83::RRC_A() {
    return RRC_r(A);
}

uint8_t sm83::RL_B() {
    return RL_r(B);
}

uint8_t sm83::RL_C() {
    return RL_r(C);
}

uint8_t sm83::RL_D() {
    return RL_r(D);
}

uint8_t sm83::RL_E() {
    return RL_r(E);
}

uint8_t sm83::RL_H() {
    return RL_r(H);
}

uint8_t sm83::RL_L() {
    return RL_r(L);
}

uint8_t sm83::RL_A() {
    return RL_r(A);
}

uint8_t sm83::RR_B() {
    return RR_r(B);
}

uint8_t sm83::RR_C() {
    return RR_r(C);
}

uint8_t sm83::RR_D() {
    return RR_r(D);
}

uint8_t sm83::RR_E() {
    return RR_r(E);
}

uint8_t sm83::RR_H() {
    return RR_r(H);
}

uint8_t sm83::RR_L() {
    return RR_r(L);
}

uint8_t sm83::RR_A() {
    return RR_r(A);
}

uint8_t sm83::SLA_B() {
    return SLA_r(B);
}

uint8_t sm83::SLA_C() {
    return SLA_r(C);
}

uint8_t sm83::SLA_D() {
    return SLA_r(D);
}

uint8_t sm83::SLA_E() {
    return SLA_r(E);
}

uint8_t sm83::SLA_H() {
    return SLA_r(H);
}

uint8_t sm83::SLA_L() {
    return SLA_r(L);
}

uint8_t sm83::SLA_A() {
    return SLA_r(A);
}

uint8_t sm83::SRA_B() {
    return SRA_r(B);
}

uint8_t sm83::SRA_C() {
    return SRA_r(C);
}

uint8_t sm83::SRA_D() {
    return SRA_r(D);
}

uint8_t sm83::SRA_E() {
    return SRA_r(E);
}

uint8_t sm83::SRA_H() {
    return SRA_r(H);
}

uint8_t sm83::SRA_L() {
    return SRA_r(L);
}

uint8_t sm83::SRA_A() {
    return SRA_r(A);
}

uint8_t sm83::SWAP_B() {
    return SWAP_r(B);
}

uint8_t sm83::SWAP_C() {
    return SWAP_r(C);
}

uint8_t sm83::SWAP_D() {
    return SWAP_r(D);
}

uint8_t sm83::SWAP_E() {
    return SWAP_r(E);
}

uint8_t sm83::SWAP_H() {
    return SWAP_r(H);
}

uint8_t sm83::SWAP_L() {
    return SWAP_r(L);
}

uint8_t sm83::SWAP_A() {
    return SWAP_r(A);
}

uint8_t sm83::SRL_B() {
    return SRL_r(B);
}

uint8_t sm83::SRL_C() {
    return SRL_r(C);
}

uint8_t sm83::SRL_D() {
    return SRL_r(D);
}

uint8_t sm83::SRL_E() {
    return SRL_r(E);
}

uint8_t sm83::SRL_H() {
    return SRL_r(H);
}

uint8_t sm83::SRL_L() {
    return SRL_r(L);
}

uint8_t sm83::SRL_A() {
    return SRL_r(A);
}

uint8_t sm83::BIT_0_B() {
    return BIT_b_r(0, B);
}

uint8_t sm83::BIT_0_C() {
    return BIT_b_r(0, C);
}

uint8_t sm83::BIT_0_D() {
    return BIT_b_r(0, D);
}

uint8_t sm83::BIT_0_E() {
    return BIT_b_r(0, E);
}

uint8_t sm83::BIT_0_H() {
    return BIT_b_r(0, H);
}

uint8_t sm83::BIT_0_L() {
    return BIT_b_r(0, L);
}

uint8_t sm83::BIT_0_HLi() {
    return BIT_b_HLi(0);
}

uint8_t sm83::BIT_0_A() {
    return BIT_b_r(0, A);
}

uint8_t sm83::BIT_1_B() {
    return BIT_b_r(1, B);
}

uint8_t sm83::BIT_1_C() {
    return BIT_b_r(1, C);
}

uint8_t sm83::BIT_1_D() {
    return BIT_b_r(1, D);
}

uint8_t sm83::BIT_1_E() {
    return BIT_b_r(1, E);
}

uint8_t sm83::BIT_1_H() {
    return BIT_b_r(1, H);
}

uint8_t sm83::BIT_1_L() {
    return BIT_b_r(1, L);
}

uint8_t sm83::BIT_1_HLi() {
    return BIT_b_HLi(1);
}

uint8_t sm83::BIT_1_A() {
    return BIT_b_r(1, A);
}

uint8_t sm83::BIT_2_B() {
    return BIT_b_r(2, B);
}

uint8_t sm83::BIT_2_C() {
    return BIT_b_r(2, C);
}

uint8_t sm83::BIT_2_D() {
    return BIT_b_r(2, D);
}

uint8_t sm83::BIT_2_E() {
    return BIT_b_r(2, E);
}

uint8_t sm83::BIT_2_H() {
    return BIT_b_r(2, H);
}

uint8_t sm83::BIT_2_L() {
    return BIT_b_r(2, L);
}

uint8_t sm83::BIT_2_HLi() {
    return BIT_b_HLi(2);
}

uint8_t sm83::BIT_2_A() {
    return BIT_b_r(2, A);
}

uint8_t sm83::BIT_3_B() {
    return BIT_b_r(3, B);
}

uint8_t sm83::BIT_3_C() {
    return BIT_b_r(3, C);
}

uint8_t sm83::BIT_3_D() {
    return BIT_b_r(3, D);
}

uint8_t sm83::BIT_3_E() {
    return BIT_b_r(3, E);
}

uint8_t sm83::BIT_3_H() {
    return BIT_b_r(3, H);
}

uint8_t sm83::BIT_3_L() {
    return BIT_b_r(3, L);
}

uint8_t sm83::BIT_3_HLi() {
    return BIT_b_HLi(3);
}

uint8_t sm83::BIT_3_A() {
    return BIT_b_r(3, A);
}

uint8_t sm83::BIT_4_B() {
    return BIT_b_r(4, B);
}

uint8_t sm83::BIT_4_C() {
    return BIT_b_r(4, C);
}

uint8_t sm83::BIT_4_D() {
    return BIT_b_r(4, D);
}

uint8_t sm83::BIT_4_E() {
    return BIT_b_r(4, E);
}

uint8_t sm83::BIT_4_H() {
    return BIT_b_r(4, H);
}

uint8_t sm83::BIT_4_L() {
    return BIT_b_r(4, L);
}

uint8_t sm83::BIT_4_HLi() {
    return BIT_b_HLi(4);
}

uint8_t sm83::BIT_4_A() {
    return BIT_b_r(4, A);
}

uint8_t sm83::BIT_5_B() {
    return BIT_b_r(5, B);
}

uint8_t sm83::BIT_5_C() {
    return BIT_b_r(5, C);
}

uint8_t sm83::BIT_5_D() {
    return BIT_b_r(5, D);
}

uint8_t sm83::BIT_5_E() {
    return BIT_b_r(5, E);
}

uint8_t sm83::BIT_5_H() {
    return BIT_b_r(5, H);
}

uint8_t sm83::BIT_5_L() {
    return BIT_b_r(5, L);
}

uint8_t sm83::BIT_5_HLi() {
    return BIT_b_HLi(5);
}

uint8_t sm83::BIT_5_A() {
    return BIT_b_r(5, A);
}

uint8_t sm83::BIT_6_B() {
    return BIT_b_r(6, B);
}

uint8_t sm83::BIT_6_C() {
    return BIT_b_r(6, C);
}

uint8_t sm83::BIT_6_D() {
    return BIT_b_r(6, D);
}

uint8_t sm83::BIT_6_E() {
    return BIT_b_r(6, E);
}

uint8_t sm83::BIT_6_H() {
    return BIT_b_r(6, H);
}

uint8_t sm83::BIT_6_L() {
    return BIT_b_r(6, L);
}

uint8_t sm83::BIT_6_HLi() {
    return BIT_b_HLi(6);
}

uint8_t sm83::BIT_6_A() {
    return BIT_b_r(6, A);
}

uint8_t sm83::BIT_7_B() {
    return BIT_b_r(7, B);
}

uint8_t sm83::BIT_7_C() {
    return BIT_b_r(7, C);
}

uint8_t sm83::BIT_7_D() {
    return BIT_b_r(7, D);
}

uint8_t sm83::BIT_7_E() {
    return BIT_b_r(7, E);
}

uint8_t sm83::BIT_7_H() {
    return BIT_b_r(7, H);
}

uint8_t sm83::BIT_7_L() {
    return BIT_b_r(7, L);
}

uint8_t sm83::BIT_7_HLi() {
    return BIT_b_HLi(7);
}

uint8_t sm83::BIT_7_A() {
    return BIT_b_r(7, A);
}

uint8_t sm83::RES_0_B() {
    return RES_b_r(0, B);
}

uint8_t sm83::RES_0_C() {
    return RES_b_r(0, C);
}

uint8_t sm83::RES_0_D() {
    return RES_b_r(0, D);
}

uint8_t sm83::RES_0_E() {
    return RES_b_r(0, E);
}

uint8_t sm83::RES_0_H() {
    return RES_b_r(0, H);
}

uint8_t sm83::RES_0_L() {
    return RES_b_r(0, L);
}

uint8_t sm83::RES_0_HLi() {
    return RES_b_HLi(0);
}

uint8_t sm83::RES_0_A() {
    return RES_b_r(0, A);
}

uint8_t sm83::RES_1_B() {
    return RES_b_r(1, B);
}

uint8_t sm83::RES_1_C() {
    return RES_b_r(1, C);
}

uint8_t sm83::RES_1_D() {
    return RES_b_r(1, D);
}

uint8_t sm83::RES_1_E() {
    return RES_b_r(1, E);
}

uint8_t sm83::RES_1_H() {
    return RES_b_r(1, H);
}

uint8_t sm83::RES_1_L() {
    return RES_b_r(1, L);
}

uint8_t sm83::RES_1_HLi() {
    return RES_b_HLi(1);
}

uint8_t sm83::RES_1_A() {
    return RES_b_r(1, A);
}

uint8_t sm83::RES_2_B() {
    return RES_b_r(2, B);
}

uint8_t sm83::RES_2_C() {
    return RES_b_r(2, C);
}

uint8_t sm83::RES_2_D() {
    return RES_b_r(2, D);
}

uint8_t sm83::RES_2_E() {
    return RES_b_r(2, E);
}

uint8_t sm83::RES_2_H() {
    return RES_b_r(2, H);
}

uint8_t sm83::RES_2_L() {
    return RES_b_r(2, L);
}

uint8_t sm83::RES_2_HLi() {
    return RES_b_HLi(2);
}

uint8_t sm83::RES_2_A() {
    return RES_b_r(2, A);
}

uint8_t sm83::RES_3_B() {
    return RES_b_r(3, B);
}

uint8_t sm83::RES_3_C() {
    return RES_b_r(3, C);
}

uint8_t sm83::RES_3_D() {
    return RES_b_r(3, D);
}

uint8_t sm83::RES_3_E() {
    return RES_b_r(3, E);
}

uint8_t sm83::RES_3_H() {
    return RES_b_r(3, H);
}

uint8_t sm83::RES_3_L() {
    return RES_b_r(3, L);
}

uint8_t sm83::RES_3_HLi() {
    return RES_b_HLi(3);
}

uint8_t sm83::RES_3_A() {
    return RES_b_r(3, A);
}

uint8_t sm83::RES_4_B() {
    return RES_b_r(4, B);
}

uint8_t sm83::RES_4_C() {
    return RES_b_r(4, C);
}

uint8_t sm83::RES_4_D() {
    return RES_b_r(4, D);
}

uint8_t sm83::RES_4_E() {
    return RES_b_r(4, E);
}

uint8_t sm83::RES_4_H() {
    return RES_b_r(4, H);
}

uint8_t sm83::RES_4_L() {
    return RES_b_r(4, L);
}

uint8_t sm83::RES_4_HLi() {
    return RES_b_HLi(4);
}

uint8_t sm83::RES_4_A() {
    return RES_b_r(4, A);
}

uint8_t sm83::RES_5_B() {
    return RES_b_r(5, B);
}

uint8_t sm83::RES_5_C() {
    return RES_b_r(5, C);
}

uint8_t sm83::RES_5_D() {
    return RES_b_r(5, D);
}

uint8_t sm83::RES_5_E() {
    return RES_b_r(5, E);
}

uint8_t sm83::RES_5_H() {
    return RES_b_r(5, H);
}

uint8_t sm83::RES_5_L() {
    return RES_b_r(5, L);
}

uint8_t sm83::RES_5_HLi() {
    return RES_b_HLi(5);
}

uint8_t sm83::RES_5_A() {
    return RES_b_r(5, A);
}

uint8_t sm83::RES_6_B() {
    return RES_b_r(6, B);
}

uint8_t sm83::RES_6_C() {
    return RES_b_r(6, C);
}

uint8_t sm83::RES_6_D() {
    return RES_b_r(6, D);
}

uint8_t sm83::RES_6_E() {
    return RES_b_r(6, E);
}

uint8_t sm83::RES_6_H() {
    return RES_b_r(6, H);
}

uint8_t sm83::RES_6_L() {
    return RES_b_r(6, L);
}

uint8_t sm83::RES_6_HLi() {
    return RES_b_HLi(6);
}

uint8_t sm83::RES_6_A() {
    return RES_b_r(6, A);
}

uint8_t sm83::RES_7_B() {
    return RES_b_r(7, B);
}

uint8_t sm83::RES_7_C() {
    return RES_b_r(7, C);
}

uint8_t sm83::RES_7_D() {
    return RES_b_r(7, D);
}

uint8_t sm83::RES_7_E() {
    return RES_b_r(7, E);
}

uint8_t sm83::RES_7_H() {
    return RES_b_r(7, H);
}

uint8_t sm83::RES_7_L() {
    return RES_b_r(7, L);
}

uint8_t sm83::RES_7_HLi() {
    return RES_b_HLi(7);
}

uint8_t sm83::RES_7_A() {
    return RES_b_r(7, A);
}

uint8_t sm83::SET_0_B() {
    return SET_b_r(0, B);

}

uint8_t sm83::SET_0_C() {
    return SET_b_r(0, C);
}

uint8_t sm83::SET_0_D() {
    return SET_b_r(0, D);
}

uint8_t sm83::SET_0_E() {
    return SET_b_r(0, E);
}

uint8_t sm83::SET_0_H() {
    return SET_b_r(0, H);
}

uint8_t sm83::SET_0_L() {
    return SET_b_r(0, L);
}

uint8_t sm83::SET_0_HLi() {
    return SET_b_HLi(0);
}

uint8_t sm83::SET_0_A() {
    return SET_b_r(0, A);
}

uint8_t sm83::SET_1_B() {
    return SET_b_r(1, B);
}

uint8_t sm83::SET_1_C() {
    return SET_b_r(1, C);
}

uint8_t sm83::SET_1_D() {
    return SET_b_r(1, D);
}

uint8_t sm83::SET_1_E() {
    return SET_b_r(1, E);
}

uint8_t sm83::SET_1_H() {
    return SET_b_r(1, H);
}

uint8_t sm83::SET_1_L() {
    return SET_b_r(1, L);
}

uint8_t sm83::SET_1_HLi() {
    return SET_b_HLi(1);
}

uint8_t sm83::SET_1_A() {
    return SET_b_r(1, A);
}

uint8_t sm83::SET_2_B() {
    return SET_b_r(2, B);
}

uint8_t sm83::SET_2_C() {
    return SET_b_r(2, C);
}

uint8_t sm83::SET_2_D() {
    return SET_b_r(2, D);
}

uint8_t sm83::SET_2_E() {
    return SET_b_r(2, E);
}

uint8_t sm83::SET_2_H() {
    return SET_b_r(2, H);
}

uint8_t sm83::SET_2_L() {
    return SET_b_r(2, L);
}

uint8_t sm83::SET_2_HLi() {
    return SET_b_HLi(2);
}

uint8_t sm83::SET_2_A() {
    return SET_b_r(2, A);
}

uint8_t sm83::SET_3_B() {
    return SET_b_r(3, B);
}

uint8_t sm83::SET_3_C() {
    return SET_b_r(3, C);
}

uint8_t sm83::SET_3_D() {
    return SET_b_r(3, D);
}

uint8_t sm83::SET_3_E() {
    return SET_b_r(3, E);
}

uint8_t sm83::SET_3_H() {
    return SET_b_r(3, H);
}

uint8_t sm83::SET_3_L() {
    return SET_b_r(3, L);
}

uint8_t sm83::SET_3_HLi() {
    return SET_b_HLi(3);
}

uint8_t sm83::SET_3_A() {
    return SET_b_r(3, A);
}

uint8_t sm83::SET_4_B() {
    return SET_b_r(4, B);
}

uint8_t sm83::SET_4_C() {
    return SET_b_r(4, C);
}

uint8_t sm83::SET_4_D() {
    return SET_b_r(4, D);
}

uint8_t sm83::SET_4_E() {
    return SET_b_r(4, E);
}

uint8_t sm83::SET_4_H() {
    return SET_b_r(4, H);
}

uint8_t sm83::SET_4_L() {
    return SET_b_r(4, L);
}

uint8_t sm83::SET_4_HLi() {
    return SET_b_HLi(4);
}

uint8_t sm83::SET_4_A() {
    return SET_b_r(4, A);
}

uint8_t sm83::SET_5_B() {
    return SET_b_r(5, B);
}

uint8_t sm83::SET_5_C() {
    return SET_b_r(5, C);
}

uint8_t sm83::SET_5_D() {
    return SET_b_r(5, D);
}

uint8_t sm83::SET_5_E() {
    return SET_b_r(5, E);
}

uint8_t sm83::SET_5_H() {
    return SET_b_r(5, H);
}

uint8_t sm83::SET_5_L() {
    return SET_b_r(5, L);
}

uint8_t sm83::SET_5_HLi() {
    return SET_b_HLi(5);
}

uint8_t sm83::SET_5_A() {
    return SET_b_r(5, A);
}

uint8_t sm83::SET_6_B() {
    return SET_b_r(6, B);
}

uint8_t sm83::SET_6_C() {
    return SET_b_r(6, C);
}

uint8_t sm83::SET_6_D() {
    return SET_b_r(6, D);
}

uint8_t sm83::SET_6_E() {
    return SET_b_r(6, E);
}

uint8_t sm83::SET_6_H() {
    return SET_b_r(6, H);
}

uint8_t sm83::SET_6_L() {
    return SET_b_r(6, L);
}

uint8_t sm83::SET_6_HLi() {
    return SET_b_HLi(6);
}

uint8_t sm83::SET_6_A() {
    return SET_b_r(6, A);
}

uint8_t sm83::SET_7_B() {
    return SET_b_r(7, B);
}

uint8_t sm83::SET_7_C() {
    return SET_b_r(7, C);
}

uint8_t sm83::SET_7_D() {
    return SET_b_r(7, D);
}

uint8_t sm83::SET_7_E() {
    return SET_b_r(7, E);
}

uint8_t sm83::SET_7_H() {
    return SET_b_r(7, H);
}

uint8_t sm83::SET_7_L() {
    return SET_b_r(7, L);
}

uint8_t sm83::SET_7_HLi() {
    return SET_b_HLi(7);
}

uint8_t sm83::SET_7_A() {
    return SET_b_r(7, A);
}