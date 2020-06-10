#pragma once
#ifndef CPU_H_GUARD
#define CPU_H_GUARD

#include <iostream>
#include <vector>    
#include <array>


#include "type.h"
#include "MEMORY.h"
class MEMORY;

class CPU {
public:
	CPU();
	CPU(MEMORY*);

	MEMORY* mem = nullptr;

	u8 parseOPCode(u8&);
	void update();
	void incPC();
	u8 getNextOPCode();

private:
	//REGISTER
	Register R;
	void initRegister();

	//FLAGS
public:
	void set_flag_if(const Flag&, bool condition);
	void set_flag(const Flag&);								//setze flag auf true egal was
	void set_flags(const Flag&, const Flag&);				//setze flag auf true egal was
	void set_flags(const Flag&, const Flag&, const Flag&);	//setze flag auf true egal was
	void set_all_flags();

	void reset_flag(const Flag&);							//setze flag auf false egal was
	void reset_flags(const Flag& F1, const Flag& F2);
	void reset_flags(const Flag& F1, const Flag& F2, const Flag& F3);
	void reset_all_flags();

	void toggle_flag(const Flag&);							//toggled die flag
	void toggle_flags(const Flag& F1, const Flag& F2);
	void toggle_flags(const Flag& F1, const Flag& F2, const Flag& F3);
	void toggle_all_flags();

	bool check_flag(const Flag&);							//gibt flag status wieder

	void set_bit_if(u8& value, const u8 bit, bool condition);
	void set_bit_if(u16& value, const u16 bit, bool condition);
	void set_bit(u8& value, const u8 bit);
	void set_bit(u16& value, const u16 bit);
	void reset_bit(u8& value, const u8 bit);
	void reset_bit(u16& value, const u16 bit);
	void toggle_bit(u8& value, const u8 bit);
	void toggle_bit(u16& value, const u16 bit);
	bool check_bit(const u8& value, const u8 bit);
	bool check_bit(const u16& value, const u16 bit);


	//OPCODES
private:

	struct instruction {
		const char* mnemonic = "";
		u8(CPU::* function)(void) = nullptr;
		u8 bytes = 0;
		u8 cycles = 0;
	};
	instruction instruction_info;
	u8 OPCode;
	bool prefix = false;

	//oft benutzte Anweisugen
	u8 read_u8_at_pc();
	u16 read_u16_at_pc();
	u16 read_u16_sp();

	i8 read_i8_at_pc();

	u16 u8_to_u16(const u8& data_lb, const u8& data_hb);


	// ARITHMETIC
	void xor_r8_r8(u8* data1, const u8* data2);
	void and_r8_r8(u8* reg1, const u8* reg2);
	void or_r8_r8(u8* reg1, const u8* reg2);

	void cp_r8_r8(const u8* data1, const u8* data2);

	void inc_u8_reg(u8* reg);
	void dec_u8_reg(u8* reg);

	void add_r8_r8(u8* reg, u8* value);
	void adc_r8_r8(u8* reg, u8* value);

	void add_r16_r16(u16* reg, u16* value);

	void sub_r8_r8(u8* reg, u8* value);
	void sbc_r8_r8(u8* reg, u8* value);

	// JUMPS
	void push_r16_reg(u16* reg);
	void pop_r16_reg(u16* reg);
	void ret();
	void call();

	// BITS
	void test_n_bit_set_z(const u8 bit, const u8* reg);

	// SHIFTS
	void rot_right_c_r8(u8* reg);
	void rot_left_c_r8(u8* reg);
	void rot_left_r8(u8* reg);
	void rot_right_r8(u8* reg);
	void shift_left_a_r8(u8* reg);
	void shift_right_a_r8(u8* reg);
	void shift_right_l_r8(u8* reg);
	void swap_r8(u8* reg);

	//OPCODES
	const std::vector<instruction> instructions = {
	   {"NOP", &CPU::opcode0x00, 1, 1},{"LD BC d16", &CPU::opcode0x01, 3, 3},{"LD BC A", &CPU::opcode0x02, 1, 2},{"INC BC", &CPU::opcode0x03, 1, 2},{"INC B", &CPU::opcode0x04, 1, 1},{"DEC B", &CPU::opcode0x05, 1, 1},{"LD B d8", &CPU::opcode0x06, 2, 2},{"RLCA", &CPU::opcode0x07, 1, 1},{"LD a16 SP", &CPU::opcode0x08, 3, 5},{"ADD HL BC", &CPU::opcode0x09, 1, 2},{"LD A BC", &CPU::opcode0x0A, 1, 2},{"DEC BC", &CPU::opcode0x0B, 1, 2},{"INC C", &CPU::opcode0x0C, 1, 1},{"DEC C", &CPU::opcode0x0D, 1, 1},{"LD C d8", &CPU::opcode0x0E, 2, 2},{"RRCA", &CPU::opcode0x0F, 1, 1},
	   {"STOP", &CPU::opcode0x10, 1, 1},{"LD DE d16", &CPU::opcode0x11, 3, 3},{"LD DE A", &CPU::opcode0x12, 1, 2},{"INC DE", &CPU::opcode0x13, 1, 2},{"INC D", &CPU::opcode0x14, 1, 1},{"DEC D", &CPU::opcode0x15, 1, 1},{"LD D d8", &CPU::opcode0x16, 2, 2},{"RLA", &CPU::opcode0x17, 1, 1},{"JR r8", &CPU::opcode0x18, 2, 3},{"ADD HL DE", &CPU::opcode0x19, 1, 2},{"LD A DE", &CPU::opcode0x1A, 1, 2},{"DEC DE", &CPU::opcode0x1B, 1, 2},{"INC E", &CPU::opcode0x1C, 1, 1},{"DEC E", &CPU::opcode0x1D, 1, 1},{"LD E d8", &CPU::opcode0x1E, 2, 2},{"RRA", &CPU::opcode0x1F, 1, 1},
	   {"JR NZ r8", &CPU::opcode0x20, 2, 3},{"LD HL d16", &CPU::opcode0x21, 3, 3},{"LD HL A", &CPU::opcode0x22, 1, 2},{"INC HL", &CPU::opcode0x23, 1, 2},{"INC H", &CPU::opcode0x24, 1, 1},{"DEC H", &CPU::opcode0x25, 1, 1},{"LD H d8", &CPU::opcode0x26, 2, 2},{"DAA", &CPU::opcode0x27, 1, 1},{"JR Z r8", &CPU::opcode0x28, 2, 3},{"ADD HL HL", &CPU::opcode0x29, 1, 2},{"LD A HL", &CPU::opcode0x2A, 1, 2},{"DEC HL", &CPU::opcode0x2B, 1, 2},{"INC L", &CPU::opcode0x2C, 1, 1},{"DEC L", &CPU::opcode0x2D, 1, 1},{"LD L d8", &CPU::opcode0x2E, 2, 2},{"CPL", &CPU::opcode0x2F, 1, 1},
	   {"JR NC r8", &CPU::opcode0x30, 2, 3},{"LD SP d16", &CPU::opcode0x31, 3, 3},{"LD HL A", &CPU::opcode0x32, 1, 2},{"INC SP", &CPU::opcode0x33, 1, 2},{"INC HL", &CPU::opcode0x34, 1, 3},{"DEC HL", &CPU::opcode0x35, 1, 3},{"LD HL d8", &CPU::opcode0x36, 2, 3},{"SCF", &CPU::opcode0x37, 1, 1},{"JR C r8", &CPU::opcode0x38, 2, 3},{"ADD HL SP", &CPU::opcode0x39, 1, 2},{"LD A HL", &CPU::opcode0x3A, 1, 2},{"DEC SP", &CPU::opcode0x3B, 1, 2},{"INC A", &CPU::opcode0x3C, 1, 1},{"DEC A", &CPU::opcode0x3D, 1, 1},{"LD A d8", &CPU::opcode0x3E, 2, 2},{"CCF", &CPU::opcode0x3F, 1, 1},
	   {"LD B B", &CPU::opcode0x40, 1, 1},{"LD B C", &CPU::opcode0x41, 1, 1},{"LD B D", &CPU::opcode0x42, 1, 1},{"LD B E", &CPU::opcode0x43, 1, 1},{"LD B H", &CPU::opcode0x44, 1, 1},{"LD B L", &CPU::opcode0x45, 1, 1},{"LD B HL", &CPU::opcode0x46, 1, 2},{"LD B A", &CPU::opcode0x47, 1, 1},{"LD C B", &CPU::opcode0x48, 1, 1},{"LD C C", &CPU::opcode0x49, 1, 1},{"LD C D", &CPU::opcode0x4A, 1, 1},{"LD C E", &CPU::opcode0x4B, 1, 1},{"LD C H", &CPU::opcode0x4C, 1, 1},{"LD C L", &CPU::opcode0x4D, 1, 1},{"LD C HL", &CPU::opcode0x4E, 1, 2},{"LD C A", &CPU::opcode0x4F, 1, 1},
	   {"LD D B", &CPU::opcode0x50, 1, 1},{"LD D C", &CPU::opcode0x51, 1, 1},{"LD D D", &CPU::opcode0x52, 1, 1},{"LD D E", &CPU::opcode0x53, 1, 1},{"LD D H", &CPU::opcode0x54, 1, 1},{"LD D L", &CPU::opcode0x55, 1, 1},{"LD D HL", &CPU::opcode0x56, 1, 2},{"LD D A", &CPU::opcode0x57, 1, 1},{"LD E B", &CPU::opcode0x58, 1, 1},{"LD E C", &CPU::opcode0x59, 1, 1},{"LD E D", &CPU::opcode0x5A, 1, 1},{"LD E E", &CPU::opcode0x5B, 1, 1},{"LD E H", &CPU::opcode0x5C, 1, 1},{"LD E L", &CPU::opcode0x5D, 1, 1},{"LD E HL", &CPU::opcode0x5E, 1, 2},{"LD E A", &CPU::opcode0x5F, 1, 1},
	   {"LD H B", &CPU::opcode0x60, 1, 1},{"LD H C", &CPU::opcode0x61, 1, 1},{"LD H D", &CPU::opcode0x62, 1, 1},{"LD H E", &CPU::opcode0x63, 1, 1},{"LD H H", &CPU::opcode0x64, 1, 1},{"LD H L", &CPU::opcode0x65, 1, 1},{"LD H HL", &CPU::opcode0x66, 1, 2},{"LD H A", &CPU::opcode0x67, 1, 1},{"LD L B", &CPU::opcode0x68, 1, 1},{"LD L C", &CPU::opcode0x69, 1, 1},{"LD L D", &CPU::opcode0x6A, 1, 1},{"LD L E", &CPU::opcode0x6B, 1, 1},{"LD L H", &CPU::opcode0x6C, 1, 1},{"LD L L", &CPU::opcode0x6D, 1, 1},{"LD L HL", &CPU::opcode0x6E, 1, 2},{"LD L A", &CPU::opcode0x6F, 1, 1},
	   {"LD HL B", &CPU::opcode0x70, 1, 2},{"LD HL C", &CPU::opcode0x71, 1, 2},{"LD HL D", &CPU::opcode0x72, 1, 2},{"LD HL E", &CPU::opcode0x73, 1, 2},{"LD HL H", &CPU::opcode0x74, 1, 2},{"LD HL L", &CPU::opcode0x75, 1, 2},{"HALT", &CPU::opcode0x76, 1, 1},{"LD HL A", &CPU::opcode0x77, 1, 2},{"LD A B", &CPU::opcode0x78, 1, 1},{"LD A C", &CPU::opcode0x79, 1, 1},{"LD A D", &CPU::opcode0x7A, 1, 1},{"LD A E", &CPU::opcode0x7B, 1, 1},{"LD A H", &CPU::opcode0x7C, 1, 1},{"LD A L", &CPU::opcode0x7D, 1, 1},{"LD A HL", &CPU::opcode0x7E, 1, 2},{"LD A A", &CPU::opcode0x7F, 1, 1},
	   {"ADD A B", &CPU::opcode0x80, 1, 1},{"ADD A C", &CPU::opcode0x81, 1, 1},{"ADD A D", &CPU::opcode0x82, 1, 1},{"ADD A E", &CPU::opcode0x83, 1, 1},{"ADD A H", &CPU::opcode0x84, 1, 1},{"ADD A L", &CPU::opcode0x85, 1, 1},{"ADD A HL", &CPU::opcode0x86, 1, 2},{"ADD A A", &CPU::opcode0x87, 1, 1},{"ADC A B", &CPU::opcode0x88, 1, 1},{"ADC A C", &CPU::opcode0x89, 1, 1},{"ADC A D", &CPU::opcode0x8A, 1, 1},{"ADC A E", &CPU::opcode0x8B, 1, 1},{"ADC A H", &CPU::opcode0x8C, 1, 1},{"ADC A L", &CPU::opcode0x8D, 1, 1},{"ADC A HL", &CPU::opcode0x8E, 1, 2},{"ADC A A", &CPU::opcode0x8F, 1, 1},
	   {"SUB B", &CPU::opcode0x90, 1, 1},{"SUB C", &CPU::opcode0x91, 1, 1},{"SUB D", &CPU::opcode0x92, 1, 1},{"SUB E", &CPU::opcode0x93, 1, 1},{"SUB H", &CPU::opcode0x94, 1, 1},{"SUB L", &CPU::opcode0x95, 1, 1},{"SUB HL", &CPU::opcode0x96, 1, 2},{"SUB A", &CPU::opcode0x97, 1, 1},{"SBC A B", &CPU::opcode0x98, 1, 1},{"SBC A C", &CPU::opcode0x99, 1, 1},{"SBC A D", &CPU::opcode0x9A, 1, 1},{"SBC A E", &CPU::opcode0x9B, 1, 1},{"SBC A H", &CPU::opcode0x9C, 1, 1},{"SBC A L", &CPU::opcode0x9D, 1, 1},{"SBC A HL", &CPU::opcode0x9E, 1, 2},{"SBC A A", &CPU::opcode0x9F, 1, 1},
	   {"AND B", &CPU::opcode0xA0, 1, 1},{"AND C", &CPU::opcode0xA1, 1, 1},{"AND D", &CPU::opcode0xA2, 1, 1},{"AND E", &CPU::opcode0xA3, 1, 1},{"AND H", &CPU::opcode0xA4, 1, 1},{"AND L", &CPU::opcode0xA5, 1, 1},{"AND HL", &CPU::opcode0xA6, 1, 2},{"AND A", &CPU::opcode0xA7, 1, 1},{"XOR B", &CPU::opcode0xA8, 1, 1},{"XOR C", &CPU::opcode0xA9, 1, 1},{"XOR D", &CPU::opcode0xAA, 1, 1},{"XOR E", &CPU::opcode0xAB, 1, 1},{"XOR H", &CPU::opcode0xAC, 1, 1},{"XOR L", &CPU::opcode0xAD, 1, 1},{"XOR HL", &CPU::opcode0xAE, 1, 2},{"XOR A", &CPU::opcode0xAF, 1, 1},
	   {"OR B", &CPU::opcode0xB0, 1, 1},{"OR C", &CPU::opcode0xB1, 1, 1},{"OR D", &CPU::opcode0xB2, 1, 1},{"OR E", &CPU::opcode0xB3, 1, 1},{"OR H", &CPU::opcode0xB4, 1, 1},{"OR L", &CPU::opcode0xB5, 1, 1},{"OR HL", &CPU::opcode0xB6, 1, 2},{"OR A", &CPU::opcode0xB7, 1, 1},{"CP B", &CPU::opcode0xB8, 1, 1},{"CP C", &CPU::opcode0xB9, 1, 1},{"CP D", &CPU::opcode0xBA, 1, 1},{"CP E", &CPU::opcode0xBB, 1, 1},{"CP H", &CPU::opcode0xBC, 1, 1},{"CP L", &CPU::opcode0xBD, 1, 1},{"CP HL", &CPU::opcode0xBE, 1, 2},{"CP A", &CPU::opcode0xBF, 1, 1},
	   {"RET NZ", &CPU::opcode0xC0, 1, 5},{"POP BC", &CPU::opcode0xC1, 1, 3},{"JP NZ a16", &CPU::opcode0xC2, 3, 4},{"JP a16", &CPU::opcode0xC3, 3, 4},{"CALL NZ a16", &CPU::opcode0xC4, 3, 6},{"PUSH BC", &CPU::opcode0xC5, 1, 4},{"ADD A d8", &CPU::opcode0xC6, 2, 2},{"RST 00H", &CPU::opcode0xC7, 1, 4},{"RET Z", &CPU::opcode0xC8, 1, 5},{"RET", &CPU::opcode0xC9, 1, 4},{"JP Z a16", &CPU::opcode0xCA, 3, 4},{"PREFIX", &CPU::opcode0xCB, 1, 1},{"CALL Z a16", &CPU::opcode0xCC, 3, 6},{"CALL a16", &CPU::opcode0xCD, 3, 6},{"ADC A d8", &CPU::opcode0xCE, 2, 2},{"RST 08H", &CPU::opcode0xCF, 1, 4},
	   {"RET NC", &CPU::opcode0xD0, 1, 5},{"POP DE", &CPU::opcode0xD1, 1, 3},{"JP NC a16", &CPU::opcode0xD2, 3, 4},{"ILLEGAL_D3", &CPU::opcode0xD3, 1, 1},{"CALL NC a16", &CPU::opcode0xD4, 3, 6},{"PUSH DE", &CPU::opcode0xD5, 1, 4},{"SUB d8", &CPU::opcode0xD6, 2, 2},{"RST 10H", &CPU::opcode0xD7, 1, 4},{"RET C", &CPU::opcode0xD8, 1, 5},{"RETI", &CPU::opcode0xD9, 1, 4},{"JP C a16", &CPU::opcode0xDA, 3, 4},{"ILLEGAL_DB", &CPU::opcode0xDB, 1, 1},{"CALL C a16", &CPU::opcode0xDC, 3, 6},{"ILLEGAL_DD", &CPU::opcode0xDD, 1, 1},{"SBC A d8", &CPU::opcode0xDE, 2, 2},{"RST 18H", &CPU::opcode0xDF, 1, 4},
	   {"LDH a8 A", &CPU::opcode0xE0, 2, 3},{"POP HL", &CPU::opcode0xE1, 1, 3},{"LD C A", &CPU::opcode0xE2, 1, 2},{"ILLEGAL_E3", &CPU::opcode0xE3, 1, 1},{"ILLEGAL_E4", &CPU::opcode0xE4, 1, 1},{"PUSH HL", &CPU::opcode0xE5, 1, 4},{"AND d8", &CPU::opcode0xE6, 2, 2},{"RST 20H", &CPU::opcode0xE7, 1, 4},{"ADD SP r8", &CPU::opcode0xE8, 2, 4},{"JP HL", &CPU::opcode0xE9, 1, 1},{"LD a16 A", &CPU::opcode0xEA, 3, 4},{"ILLEGAL_EB", &CPU::opcode0xEB, 1, 1},{"ILLEGAL_EC", &CPU::opcode0xEC, 1, 1},{"ILLEGAL_ED", &CPU::opcode0xED, 1, 1},{"XOR d8", &CPU::opcode0xEE, 2, 2},{"RST 28H", &CPU::opcode0xEF, 1, 4},
	   {"LDH A a8", &CPU::opcode0xF0, 2, 3},{"POP AF", &CPU::opcode0xF1, 1, 3},{"LD A C", &CPU::opcode0xF2, 1, 2},{"DI", &CPU::opcode0xF3, 1, 1},{"ILLEGAL_F4", &CPU::opcode0xF4, 1, 1},{"PUSH AF", &CPU::opcode0xF5, 1, 4},{"OR d8", &CPU::opcode0xF6, 2, 2},{"RST 30H", &CPU::opcode0xF7, 1, 4},{"LD HL SP", &CPU::opcode0xF8, 2, 3},{"LD SP HL", &CPU::opcode0xF9, 1, 2},{"LD A a16", &CPU::opcode0xFA, 3, 4},{"EI", &CPU::opcode0xFB, 1, 1},{"ILLEGAL_FC", &CPU::opcode0xFC, 1, 1},{"ILLEGAL_FD", &CPU::opcode0xFD, 1, 1},{"CP d8", &CPU::opcode0xFE, 2, 2},{"RST 38H", &CPU::opcode0xFF, 1, 4}
	};
	const std::vector<instruction> prefixedinstructions = {
		{"RLC B", &CPU::prefixedopcode0x00, 2, 2},{"RLC C", &CPU::prefixedopcode0x01, 2, 2},{"RLC D", &CPU::prefixedopcode0x02, 2, 2},{"RLC E", &CPU::prefixedopcode0x03, 2, 2},{"RLC H", &CPU::prefixedopcode0x04, 2, 2},{"RLC L", &CPU::prefixedopcode0x05, 2, 2},{"RLC HL", &CPU::prefixedopcode0x06, 2, 4},{"RLC A", &CPU::prefixedopcode0x07, 2, 2},{"RRC B", &CPU::prefixedopcode0x08, 2, 2},{"RRC C", &CPU::prefixedopcode0x09, 2, 2},{"RRC D", &CPU::prefixedopcode0x0A, 2, 2},{"RRC E", &CPU::prefixedopcode0x0B, 2, 2},{"RRC H", &CPU::prefixedopcode0x0C, 2, 2},{"RRC L", &CPU::prefixedopcode0x0D, 2, 2},{"RRC HL", &CPU::prefixedopcode0x0E, 2, 4},{"RRC A", &CPU::prefixedopcode0x0F, 2, 2},
		{"RL B", &CPU::prefixedopcode0x10, 2, 2},{"RL C", &CPU::prefixedopcode0x11, 2, 2},{"RL D", &CPU::prefixedopcode0x12, 2, 2},{"RL E", &CPU::prefixedopcode0x13, 2, 2},{"RL H", &CPU::prefixedopcode0x14, 2, 2},{"RL L", &CPU::prefixedopcode0x15, 2, 2},{"RL HL", &CPU::prefixedopcode0x16, 2, 4},{"RL A", &CPU::prefixedopcode0x17, 2, 2},{"RR B", &CPU::prefixedopcode0x18, 2, 2},{"RR C", &CPU::prefixedopcode0x19, 2, 2},{"RR D", &CPU::prefixedopcode0x1A, 2, 2},{"RR E", &CPU::prefixedopcode0x1B, 2, 2},{"RR H", &CPU::prefixedopcode0x1C, 2, 2},{"RR L", &CPU::prefixedopcode0x1D, 2, 2},{"RR HL", &CPU::prefixedopcode0x1E, 2, 4},{"RR A", &CPU::prefixedopcode0x1F, 2, 2},
		{"SLA B", &CPU::prefixedopcode0x20, 2, 2},{"SLA C", &CPU::prefixedopcode0x21, 2, 2},{"SLA D", &CPU::prefixedopcode0x22, 2, 2},{"SLA E", &CPU::prefixedopcode0x23, 2, 2},{"SLA H", &CPU::prefixedopcode0x24, 2, 2},{"SLA L", &CPU::prefixedopcode0x25, 2, 2},{"SLA HL", &CPU::prefixedopcode0x26, 2, 4},{"SLA A", &CPU::prefixedopcode0x27, 2, 2},{"SRA B", &CPU::prefixedopcode0x28, 2, 2},{"SRA C", &CPU::prefixedopcode0x29, 2, 2},{"SRA D", &CPU::prefixedopcode0x2A, 2, 2},{"SRA E", &CPU::prefixedopcode0x2B, 2, 2},{"SRA H", &CPU::prefixedopcode0x2C, 2, 2},{"SRA L", &CPU::prefixedopcode0x2D, 2, 2},{"SRA HL", &CPU::prefixedopcode0x2E, 2, 4},{"SRA A", &CPU::prefixedopcode0x2F, 2, 2},
		{"SWAP B", &CPU::prefixedopcode0x30, 2, 2},{"SWAP C", &CPU::prefixedopcode0x31, 2, 2},{"SWAP D", &CPU::prefixedopcode0x32, 2, 2},{"SWAP E", &CPU::prefixedopcode0x33, 2, 2},{"SWAP H", &CPU::prefixedopcode0x34, 2, 2},{"SWAP L", &CPU::prefixedopcode0x35, 2, 2},{"SWAP HL", &CPU::prefixedopcode0x36, 2, 4},{"SWAP A", &CPU::prefixedopcode0x37, 2, 2},{"SRL B", &CPU::prefixedopcode0x38, 2, 2},{"SRL C", &CPU::prefixedopcode0x39, 2, 2},{"SRL D", &CPU::prefixedopcode0x3A, 2, 2},{"SRL E", &CPU::prefixedopcode0x3B, 2, 2},{"SRL H", &CPU::prefixedopcode0x3C, 2, 2},{"SRL L", &CPU::prefixedopcode0x3D, 2, 2},{"SRL HL", &CPU::prefixedopcode0x3E, 2, 4},{"SRL A", &CPU::prefixedopcode0x3F, 2, 2},
		{"BIT 0 B", &CPU::prefixedopcode0x40, 2, 2},{"BIT 0 C", &CPU::prefixedopcode0x41, 2, 2},{"BIT 0 D", &CPU::prefixedopcode0x42, 2, 2},{"BIT 0 E", &CPU::prefixedopcode0x43, 2, 2},{"BIT 0 H", &CPU::prefixedopcode0x44, 2, 2},{"BIT 0 L", &CPU::prefixedopcode0x45, 2, 2},{"BIT 0 HL", &CPU::prefixedopcode0x46, 2, 3},{"BIT 0 A", &CPU::prefixedopcode0x47, 2, 2},{"BIT 1 B", &CPU::prefixedopcode0x48, 2, 2},{"BIT 1 C", &CPU::prefixedopcode0x49, 2, 2},{"BIT 1 D", &CPU::prefixedopcode0x4A, 2, 2},{"BIT 1 E", &CPU::prefixedopcode0x4B, 2, 2},{"BIT 1 H", &CPU::prefixedopcode0x4C, 2, 2},{"BIT 1 L", &CPU::prefixedopcode0x4D, 2, 2},{"BIT 1 HL", &CPU::prefixedopcode0x4E, 2, 3},{"BIT 1 A", &CPU::prefixedopcode0x4F, 2, 2},
		{"BIT 2 B", &CPU::prefixedopcode0x50, 2, 2},{"BIT 2 C", &CPU::prefixedopcode0x51, 2, 2},{"BIT 2 D", &CPU::prefixedopcode0x52, 2, 2},{"BIT 2 E", &CPU::prefixedopcode0x53, 2, 2},{"BIT 2 H", &CPU::prefixedopcode0x54, 2, 2},{"BIT 2 L", &CPU::prefixedopcode0x55, 2, 2},{"BIT 2 HL", &CPU::prefixedopcode0x56, 2, 3},{"BIT 2 A", &CPU::prefixedopcode0x57, 2, 2},{"BIT 3 B", &CPU::prefixedopcode0x58, 2, 2},{"BIT 3 C", &CPU::prefixedopcode0x59, 2, 2},{"BIT 3 D", &CPU::prefixedopcode0x5A, 2, 2},{"BIT 3 E", &CPU::prefixedopcode0x5B, 2, 2},{"BIT 3 H", &CPU::prefixedopcode0x5C, 2, 2},{"BIT 3 L", &CPU::prefixedopcode0x5D, 2, 2},{"BIT 3 HL", &CPU::prefixedopcode0x5E, 2, 3},{"BIT 3 A", &CPU::prefixedopcode0x5F, 2, 2},
		{"BIT 4 B", &CPU::prefixedopcode0x60, 2, 2},{"BIT 4 C", &CPU::prefixedopcode0x61, 2, 2},{"BIT 4 D", &CPU::prefixedopcode0x62, 2, 2},{"BIT 4 E", &CPU::prefixedopcode0x63, 2, 2},{"BIT 4 H", &CPU::prefixedopcode0x64, 2, 2},{"BIT 4 L", &CPU::prefixedopcode0x65, 2, 2},{"BIT 4 HL", &CPU::prefixedopcode0x66, 2, 3},{"BIT 4 A", &CPU::prefixedopcode0x67, 2, 2},{"BIT 5 B", &CPU::prefixedopcode0x68, 2, 2},{"BIT 5 C", &CPU::prefixedopcode0x69, 2, 2},{"BIT 5 D", &CPU::prefixedopcode0x6A, 2, 2},{"BIT 5 E", &CPU::prefixedopcode0x6B, 2, 2},{"BIT 5 H", &CPU::prefixedopcode0x6C, 2, 2},{"BIT 5 L", &CPU::prefixedopcode0x6D, 2, 2},{"BIT 5 HL", &CPU::prefixedopcode0x6E, 2, 3},{"BIT 5 A", &CPU::prefixedopcode0x6F, 2, 2},
		{"BIT 6 B", &CPU::prefixedopcode0x70, 2, 2},{"BIT 6 C", &CPU::prefixedopcode0x71, 2, 2},{"BIT 6 D", &CPU::prefixedopcode0x72, 2, 2},{"BIT 6 E", &CPU::prefixedopcode0x73, 2, 2},{"BIT 6 H", &CPU::prefixedopcode0x74, 2, 2},{"BIT 6 L", &CPU::prefixedopcode0x75, 2, 2},{"BIT 6 HL", &CPU::prefixedopcode0x76, 2, 3},{"BIT 6 A", &CPU::prefixedopcode0x77, 2, 2},{"BIT 7 B", &CPU::prefixedopcode0x78, 2, 2},{"BIT 7 C", &CPU::prefixedopcode0x79, 2, 2},{"BIT 7 D", &CPU::prefixedopcode0x7A, 2, 2},{"BIT 7 E", &CPU::prefixedopcode0x7B, 2, 2},{"BIT 7 H", &CPU::prefixedopcode0x7C, 2, 2},{"BIT 7 L", &CPU::prefixedopcode0x7D, 2, 2},{"BIT 7 HL", &CPU::prefixedopcode0x7E, 2, 3},{"BIT 7 A", &CPU::prefixedopcode0x7F, 2, 2},
		{"RES 0 B", &CPU::prefixedopcode0x80, 2, 2},{"RES 0 C", &CPU::prefixedopcode0x81, 2, 2},{"RES 0 D", &CPU::prefixedopcode0x82, 2, 2},{"RES 0 E", &CPU::prefixedopcode0x83, 2, 2},{"RES 0 H", &CPU::prefixedopcode0x84, 2, 2},{"RES 0 L", &CPU::prefixedopcode0x85, 2, 2},{"RES 0 HL", &CPU::prefixedopcode0x86, 2, 4},{"RES 0 A", &CPU::prefixedopcode0x87, 2, 2},{"RES 1 B", &CPU::prefixedopcode0x88, 2, 2},{"RES 1 C", &CPU::prefixedopcode0x89, 2, 2},{"RES 1 D", &CPU::prefixedopcode0x8A, 2, 2},{"RES 1 E", &CPU::prefixedopcode0x8B, 2, 2},{"RES 1 H", &CPU::prefixedopcode0x8C, 2, 2},{"RES 1 L", &CPU::prefixedopcode0x8D, 2, 2},{"RES 1 HL", &CPU::prefixedopcode0x8E, 2, 4},{"RES 1 A", &CPU::prefixedopcode0x8F, 2, 2},
		{"RES 2 B", &CPU::prefixedopcode0x90, 2, 2},{"RES 2 C", &CPU::prefixedopcode0x91, 2, 2},{"RES 2 D", &CPU::prefixedopcode0x92, 2, 2},{"RES 2 E", &CPU::prefixedopcode0x93, 2, 2},{"RES 2 H", &CPU::prefixedopcode0x94, 2, 2},{"RES 2 L", &CPU::prefixedopcode0x95, 2, 2},{"RES 2 HL", &CPU::prefixedopcode0x96, 2, 4},{"RES 2 A", &CPU::prefixedopcode0x97, 2, 2},{"RES 3 B", &CPU::prefixedopcode0x98, 2, 2},{"RES 3 C", &CPU::prefixedopcode0x99, 2, 2},{"RES 3 D", &CPU::prefixedopcode0x9A, 2, 2},{"RES 3 E", &CPU::prefixedopcode0x9B, 2, 2},{"RES 3 H", &CPU::prefixedopcode0x9C, 2, 2},{"RES 3 L", &CPU::prefixedopcode0x9D, 2, 2},{"RES 3 HL", &CPU::prefixedopcode0x9E, 2, 4},{"RES 3 A", &CPU::prefixedopcode0x9F, 2, 2},
		{"RES 4 B", &CPU::prefixedopcode0xA0, 2, 2},{"RES 4 C", &CPU::prefixedopcode0xA1, 2, 2},{"RES 4 D", &CPU::prefixedopcode0xA2, 2, 2},{"RES 4 E", &CPU::prefixedopcode0xA3, 2, 2},{"RES 4 H", &CPU::prefixedopcode0xA4, 2, 2},{"RES 4 L", &CPU::prefixedopcode0xA5, 2, 2},{"RES 4 HL", &CPU::prefixedopcode0xA6, 2, 4},{"RES 4 A", &CPU::prefixedopcode0xA7, 2, 2},{"RES 5 B", &CPU::prefixedopcode0xA8, 2, 2},{"RES 5 C", &CPU::prefixedopcode0xA9, 2, 2},{"RES 5 D", &CPU::prefixedopcode0xAA, 2, 2},{"RES 5 E", &CPU::prefixedopcode0xAB, 2, 2},{"RES 5 H", &CPU::prefixedopcode0xAC, 2, 2},{"RES 5 L", &CPU::prefixedopcode0xAD, 2, 2},{"RES 5 HL", &CPU::prefixedopcode0xAE, 2, 4},{"RES 5 A", &CPU::prefixedopcode0xAF, 2, 2},
		{"RES 6 B", &CPU::prefixedopcode0xB0, 2, 2},{"RES 6 C", &CPU::prefixedopcode0xB1, 2, 2},{"RES 6 D", &CPU::prefixedopcode0xB2, 2, 2},{"RES 6 E", &CPU::prefixedopcode0xB3, 2, 2},{"RES 6 H", &CPU::prefixedopcode0xB4, 2, 2},{"RES 6 L", &CPU::prefixedopcode0xB5, 2, 2},{"RES 6 HL", &CPU::prefixedopcode0xB6, 2, 4},{"RES 6 A", &CPU::prefixedopcode0xB7, 2, 2},{"RES 7 B", &CPU::prefixedopcode0xB8, 2, 2},{"RES 7 C", &CPU::prefixedopcode0xB9, 2, 2},{"RES 7 D", &CPU::prefixedopcode0xBA, 2, 2},{"RES 7 E", &CPU::prefixedopcode0xBB, 2, 2},{"RES 7 H", &CPU::prefixedopcode0xBC, 2, 2},{"RES 7 L", &CPU::prefixedopcode0xBD, 2, 2},{"RES 7 HL", &CPU::prefixedopcode0xBE, 2, 4},{"RES 7 A", &CPU::prefixedopcode0xBF, 2, 2},
		{"SET 0 B", &CPU::prefixedopcode0xC0, 2, 2},{"SET 0 C", &CPU::prefixedopcode0xC1, 2, 2},{"SET 0 D", &CPU::prefixedopcode0xC2, 2, 2},{"SET 0 E", &CPU::prefixedopcode0xC3, 2, 2},{"SET 0 H", &CPU::prefixedopcode0xC4, 2, 2},{"SET 0 L", &CPU::prefixedopcode0xC5, 2, 2},{"SET 0 HL", &CPU::prefixedopcode0xC6, 2, 4},{"SET 0 A", &CPU::prefixedopcode0xC7, 2, 2},{"SET 1 B", &CPU::prefixedopcode0xC8, 2, 2},{"SET 1 C", &CPU::prefixedopcode0xC9, 2, 2},{"SET 1 D", &CPU::prefixedopcode0xCA, 2, 2},{"SET 1 E", &CPU::prefixedopcode0xCB, 2, 2},{"SET 1 H", &CPU::prefixedopcode0xCC, 2, 2},{"SET 1 L", &CPU::prefixedopcode0xCD, 2, 2},{"SET 1 HL", &CPU::prefixedopcode0xCE, 2, 4},{"SET 1 A", &CPU::prefixedopcode0xCF, 2, 2},
		{"SET 2 B", &CPU::prefixedopcode0xD0, 2, 2},{"SET 2 C", &CPU::prefixedopcode0xD1, 2, 2},{"SET 2 D", &CPU::prefixedopcode0xD2, 2, 2},{"SET 2 E", &CPU::prefixedopcode0xD3, 2, 2},{"SET 2 H", &CPU::prefixedopcode0xD4, 2, 2},{"SET 2 L", &CPU::prefixedopcode0xD5, 2, 2},{"SET 2 HL", &CPU::prefixedopcode0xD6, 2, 4},{"SET 2 A", &CPU::prefixedopcode0xD7, 2, 2},{"SET 3 B", &CPU::prefixedopcode0xD8, 2, 2},{"SET 3 C", &CPU::prefixedopcode0xD9, 2, 2},{"SET 3 D", &CPU::prefixedopcode0xDA, 2, 2},{"SET 3 E", &CPU::prefixedopcode0xDB, 2, 2},{"SET 3 H", &CPU::prefixedopcode0xDC, 2, 2},{"SET 3 L", &CPU::prefixedopcode0xDD, 2, 2},{"SET 3 HL", &CPU::prefixedopcode0xDE, 2, 4},{"SET 3 A", &CPU::prefixedopcode0xDF, 2, 2},
		{"SET 4 B", &CPU::prefixedopcode0xE0, 2, 2},{"SET 4 C", &CPU::prefixedopcode0xE1, 2, 2},{"SET 4 D", &CPU::prefixedopcode0xE2, 2, 2},{"SET 4 E", &CPU::prefixedopcode0xE3, 2, 2},{"SET 4 H", &CPU::prefixedopcode0xE4, 2, 2},{"SET 4 L", &CPU::prefixedopcode0xE5, 2, 2},{"SET 4 HL", &CPU::prefixedopcode0xE6, 2, 4},{"SET 4 A", &CPU::prefixedopcode0xE7, 2, 2},{"SET 5 B", &CPU::prefixedopcode0xE8, 2, 2},{"SET 5 C", &CPU::prefixedopcode0xE9, 2, 2},{"SET 5 D", &CPU::prefixedopcode0xEA, 2, 2},{"SET 5 E", &CPU::prefixedopcode0xEB, 2, 2},{"SET 5 H", &CPU::prefixedopcode0xEC, 2, 2},{"SET 5 L", &CPU::prefixedopcode0xED, 2, 2},{"SET 5 HL", &CPU::prefixedopcode0xEE, 2, 4},{"SET 5 A", &CPU::prefixedopcode0xEF, 2, 2},
		{"SET 6 B", &CPU::prefixedopcode0xF0, 2, 2},{"SET 6 C", &CPU::prefixedopcode0xF1, 2, 2},{"SET 6 D", &CPU::prefixedopcode0xF2, 2, 2},{"SET 6 E", &CPU::prefixedopcode0xF3, 2, 2},{"SET 6 H", &CPU::prefixedopcode0xF4, 2, 2},{"SET 6 L", &CPU::prefixedopcode0xF5, 2, 2},{"SET 6 HL", &CPU::prefixedopcode0xF6, 2, 4},{"SET 6 A", &CPU::prefixedopcode0xF7, 2, 2},{"SET 7 B", &CPU::prefixedopcode0xF8, 2, 2},{"SET 7 C", &CPU::prefixedopcode0xF9, 2, 2},{"SET 7 D", &CPU::prefixedopcode0xFA, 2, 2},{"SET 7 E", &CPU::prefixedopcode0xFB, 2, 2},{"SET 7 H", &CPU::prefixedopcode0xFC, 2, 2},{"SET 7 L", &CPU::prefixedopcode0xFD, 2, 2},{"SET 7 HL", &CPU::prefixedopcode0xFE, 2, 4},{"SET 7 A", &CPU::prefixedopcode0xFF, 2, 2}
	};

	u8 opcode0x00();	// NOP
	u8 opcode0x01();	// LD BC d16
	u8 opcode0x02();	// LD BC A
	u8 opcode0x03();	// INC BC
	u8 opcode0x04();	// INC B
	u8 opcode0x05();	// DEC B
	u8 opcode0x06();	// LD B d8
	u8 opcode0x07();	// RLCA
	u8 opcode0x08();	// LD a16 SP
	u8 opcode0x09();	// ADD HL BC
	u8 opcode0x0A();	// LD A BC
	u8 opcode0x0B();	// DEC BC
	u8 opcode0x0C();	// INC C
	u8 opcode0x0D();	// DEC C
	u8 opcode0x0E();	// LD C d8
	u8 opcode0x0F();	// RRCA
	u8 opcode0x10();	// STOP
	u8 opcode0x11();	// LD DE d16
	u8 opcode0x12();	// LD DE A
	u8 opcode0x13();	// INC DE
	u8 opcode0x14();	// INC D
	u8 opcode0x15();	// DEC D
	u8 opcode0x16();	// LD D d8
	u8 opcode0x17();	// RLA
	u8 opcode0x18();	// JR r8
	u8 opcode0x19();	// ADD HL DE
	u8 opcode0x1A();	// LD A DE
	u8 opcode0x1B();	// DEC DE
	u8 opcode0x1C();	// INC E
	u8 opcode0x1D();	// DEC E
	u8 opcode0x1E();	// LD E d8
	u8 opcode0x1F();	// RRA
	u8 opcode0x20();	// JR NZ r8
	u8 opcode0x21();	// LD HL d16
	u8 opcode0x22();	// LD HL A
	u8 opcode0x23();	// INC HL
	u8 opcode0x24();	// INC H
	u8 opcode0x25();	// DEC H
	u8 opcode0x26();	// LD H d8
	u8 opcode0x27();	// DAA
	u8 opcode0x28();	// JR Z r8
	u8 opcode0x29();	// ADD HL HL
	u8 opcode0x2A();	// LD A HL
	u8 opcode0x2B();	// DEC HL
	u8 opcode0x2C();	// INC L
	u8 opcode0x2D();	// DEC L
	u8 opcode0x2E();	// LD L d8
	u8 opcode0x2F();	// CPL
	u8 opcode0x30();	// JR NC r8
	u8 opcode0x31();	// LD SP d16
	u8 opcode0x32();	// LD HL A
	u8 opcode0x33();	// INC SP
	u8 opcode0x34();	// INC HL
	u8 opcode0x35();	// DEC HL
	u8 opcode0x36();	// LD HL d8
	u8 opcode0x37();	// SCF
	u8 opcode0x38();	// JR C r8
	u8 opcode0x39();	// ADD HL SP
	u8 opcode0x3A();	// LD A HL
	u8 opcode0x3B();
	// DEC SP
	u8 opcode0x3C();	// INC A
	u8 opcode0x3D();	// DEC A
	u8 opcode0x3E();	// LD A d8
	u8 opcode0x3F();	// CCF
	u8 opcode0x40();	// LD B B
	u8 opcode0x41();	// LD B C
	u8 opcode0x42();	// LD B D
	u8 opcode0x43();	// LD B E
	u8 opcode0x44();	// LD B H
	u8 opcode0x45();	// LD B L
	u8 opcode0x46();	// LD B HL
	u8 opcode0x47();	// LD B A
	u8 opcode0x48();	// LD C B
	u8 opcode0x49();	// LD C C
	u8 opcode0x4A();	// LD C D
	u8 opcode0x4B();	// LD C E
	u8 opcode0x4C();	// LD C H
	u8 opcode0x4D();	// LD C L
	u8 opcode0x4E();	// LD C HL
	u8 opcode0x4F();	// LD C A
	u8 opcode0x50();	// LD D B
	u8 opcode0x51();	// LD D C
	u8 opcode0x52();	// LD D D
	u8 opcode0x53();	// LD D E
	u8 opcode0x54();	// LD D H
	u8 opcode0x55();	// LD D L
	u8 opcode0x56();	// LD D HL
	u8 opcode0x57();	// LD D A
	u8 opcode0x58();	// LD E B
	u8 opcode0x59();	// LD E C
	u8 opcode0x5A();	// LD E D
	u8 opcode0x5B();	// LD E E
	u8 opcode0x5C();	// LD E H
	u8 opcode0x5D();	// LD E L
	u8 opcode0x5E();	// LD E HL
	u8 opcode0x5F();	// LD E A
	u8 opcode0x60();	// LD H B
	u8 opcode0x61();	// LD H C
	u8 opcode0x62();	// LD H D
	u8 opcode0x63();	// LD H E
	u8 opcode0x64();	// LD H H
	u8 opcode0x65();	// LD H L
	u8 opcode0x66();	// LD H HL
	u8 opcode0x67();	// LD H A
	u8 opcode0x68();	// LD L B
	u8 opcode0x69();	// LD L C
	u8 opcode0x6A();	// LD L D
	u8 opcode0x6B();	// LD L E
	u8 opcode0x6C();	// LD L H
	u8 opcode0x6D();	// LD L L
	u8 opcode0x6E();	// LD L HL
	u8 opcode0x6F();	// LD L A
	u8 opcode0x70();	// LD HL B
	u8 opcode0x71();	// LD HL C
	u8 opcode0x72();	// LD HL D
	u8 opcode0x73();	// LD HL E
	u8 opcode0x74();	// LD HL H
	u8 opcode0x75();	// LD HL L
	u8 opcode0x76();	// HALT
	u8 opcode0x77();	// LD HL A
	u8 opcode0x78();	// LD A B
	u8 opcode0x79();	// LD A C
	u8 opcode0x7A();	// LD A D
	u8 opcode0x7B();	// LD A E
	u8 opcode0x7C();	// LD A H
	u8 opcode0x7D();	// LD A L
	u8 opcode0x7E();	// LD A HL
	u8 opcode0x7F();	// LD A A
	u8 opcode0x80();	// ADD A B
	u8 opcode0x81();	// ADD A C
	u8 opcode0x82();	// ADD A D
	u8 opcode0x83();	// ADD A E
	u8 opcode0x84();	// ADD A H
	u8 opcode0x85();	// ADD A L
	u8 opcode0x86();	// ADD A HL
	u8 opcode0x87();	// ADD A A
	u8 opcode0x88();	// ADC A B
	u8 opcode0x89();	// ADC A C
	u8 opcode0x8A();	// ADC A D
	u8 opcode0x8B();	// ADC A E
	u8 opcode0x8C();	// ADC A H
	u8 opcode0x8D();	// ADC A L
	u8 opcode0x8E();	// ADC A HL
	u8 opcode0x8F();	// ADC A A
	u8 opcode0x90();	// SUB B
	u8 opcode0x91();	// SUB C
	u8 opcode0x92();	// SUB D
	u8 opcode0x93();	// SUB E
	u8 opcode0x94();	// SUB H
	u8 opcode0x95();	// SUB L
	u8 opcode0x96();	// SUB HL
	u8 opcode0x97();	// SUB A
	u8 opcode0x98();	// SBC A B
	u8 opcode0x99();	// SBC A C
	u8 opcode0x9A();	// SBC A D
	u8 opcode0x9B();	// SBC A E
	u8 opcode0x9C();	// SBC A H
	u8 opcode0x9D();	// SBC A L
	u8 opcode0x9E();	// SBC A HL
	u8 opcode0x9F();	// SBC A A
	u8 opcode0xA0();	// AND B
	u8 opcode0xA1();	// AND C
	u8 opcode0xA2();	// AND D
	u8 opcode0xA3();	// AND E
	u8 opcode0xA4();	// AND H
	u8 opcode0xA5();	// AND L
	u8 opcode0xA6();	// AND HL
	u8 opcode0xA7();	// AND A
	u8 opcode0xA8();	// XOR B
	u8 opcode0xA9();	// XOR C
	u8 opcode0xAA();	// XOR D
	u8 opcode0xAB();	// XOR E
	u8 opcode0xAC();	// XOR H
	u8 opcode0xAD();	// XOR L
	u8 opcode0xAE();	// XOR HL
	u8 opcode0xAF();	// XOR A
	u8 opcode0xB0();	// OR B
	u8 opcode0xB1();	// OR C
	u8 opcode0xB2();	// OR D
	u8 opcode0xB3();	// OR E
	u8 opcode0xB4();	// OR H
	u8 opcode0xB5();	// OR L
	u8 opcode0xB6();	// OR HL
	u8 opcode0xB7();	// OR A
	u8 opcode0xB8();	// CP B
	u8 opcode0xB9();	// CP C
	u8 opcode0xBA();	// CP D
	u8 opcode0xBB();	// CP E
	u8 opcode0xBC();	// CP H
	u8 opcode0xBD();	// CP L
	u8 opcode0xBE();	// CP HL
	u8 opcode0xBF();	// CP A
	u8 opcode0xC0();	// RET NZ
	u8 opcode0xC1();	// POP BC
	u8 opcode0xC2();	// JP NZ a16
	u8 opcode0xC3();	// JP a16
	u8 opcode0xC4();	// CALL NZ a16
	u8 opcode0xC5();	// PUSH BC
	u8 opcode0xC6();	// ADD A d8
	u8 opcode0xC7();	// RST 00H
	u8 opcode0xC8();	// RET Z
	u8 opcode0xC9();	// RET
	u8 opcode0xCA();	// JP Z a16
	u8 opcode0xCB();	// PREFIX
	u8 opcode0xCC();	// CALL Z a16
	u8 opcode0xCD();	// CALL a16
	u8 opcode0xCE();	// ADC A d8
	u8 opcode0xCF();	// RST 08H
	u8 opcode0xD0();	// RET NC
	u8 opcode0xD1();	// POP DE
	u8 opcode0xD2();	// JP NC a16
	u8 opcode0xD3();	// ILLEGAL_D3
	u8 opcode0xD4();	// CALL NC a16
	u8 opcode0xD5();	// PUSH DE
	u8 opcode0xD6();	// SUB d8
	u8 opcode0xD7();	// RST 10H
	u8 opcode0xD8();	// RET C
	u8 opcode0xD9();	// RETI
	u8 opcode0xDA();	// JP C a16
	u8 opcode0xDB();	// ILLEGAL_DB
	u8 opcode0xDC();	// CALL C a16
	u8 opcode0xDD();	// ILLEGAL_DD
	u8 opcode0xDE();	// SBC A d8
	u8 opcode0xDF();	// RST 18H
	u8 opcode0xE0();	// LDH a8 A
	u8 opcode0xE1();	// POP HL
	u8 opcode0xE2();	// LD C A
	u8 opcode0xE3();	// ILLEGAL_E3
	u8 opcode0xE4();	// ILLEGAL_E4
	u8 opcode0xE5();	// PUSH HL
	u8 opcode0xE6();	// AND d8
	u8 opcode0xE7();	// RST 20H
	u8 opcode0xE8();	// ADD SP r8
	u8 opcode0xE9();	// JP HL
	u8 opcode0xEA();	// LD a16 A
	u8 opcode0xEB();	// ILLEGAL_EB
	u8 opcode0xEC();	// ILLEGAL_EC
	u8 opcode0xED();	// ILLEGAL_ED
	u8 opcode0xEE();	// XOR d8
	u8 opcode0xEF();	// RST 28H
	u8 opcode0xF0();	// LDH A a8
	u8 opcode0xF1();	// POP AF
	u8 opcode0xF2();	// LD A C
	u8 opcode0xF3();	// DI
	u8 opcode0xF4();	// ILLEGAL_F4
	u8 opcode0xF5();	// PUSH AF
	u8 opcode0xF6();	// OR d8
	u8 opcode0xF7();	// RST 30H
	u8 opcode0xF8();	// LD HL SP
	u8 opcode0xF9();	// LD SP HL
	u8 opcode0xFA();	// LD A a16
	u8 opcode0xFB();	// EI
	u8 opcode0xFC();	// ILLEGAL_FC
	u8 opcode0xFD();	// ILLEGAL_FD
	u8 opcode0xFE();	// CP d8
	u8 opcode0xFF();	// RST 38H

	//refixed opcodes
	u8 prefixedopcode0x00();	// RLC B
	u8 prefixedopcode0x01();	// RLC C
	u8 prefixedopcode0x02();	// RLC D
	u8 prefixedopcode0x03();	// RLC E
	u8 prefixedopcode0x04();	// RLC H
	u8 prefixedopcode0x05();	// RLC L
	u8 prefixedopcode0x06();	// RLC HL
	u8 prefixedopcode0x07();	// RLC A
	u8 prefixedopcode0x08();	// RRC B
	u8 prefixedopcode0x09();	// RRC C
	u8 prefixedopcode0x0A();	// RRC D
	u8 prefixedopcode0x0B();	// RRC E
	u8 prefixedopcode0x0C();	// RRC H
	u8 prefixedopcode0x0D();	// RRC L
	u8 prefixedopcode0x0E();	// RRC HL
	u8 prefixedopcode0x0F();	// RRC A
	u8 prefixedopcode0x10();	// RL B
	u8 prefixedopcode0x11();	// RL C
	u8 prefixedopcode0x12();	// RL D
	u8 prefixedopcode0x13();	// RL E
	u8 prefixedopcode0x14();	// RL H
	u8 prefixedopcode0x15();	// RL L
	u8 prefixedopcode0x16();	// RL HL
	u8 prefixedopcode0x17();	// RL A
	u8 prefixedopcode0x18();	// RR B
	u8 prefixedopcode0x19();	// RR C
	u8 prefixedopcode0x1A();	// RR D
	u8 prefixedopcode0x1B();	// RR E
	u8 prefixedopcode0x1C();	// RR H
	u8 prefixedopcode0x1D();	// RR L
	u8 prefixedopcode0x1E();	// RR HL
	u8 prefixedopcode0x1F();	// RR A
	u8 prefixedopcode0x20();	// SLA B
	u8 prefixedopcode0x21();	// SLA C
	u8 prefixedopcode0x22();	// SLA D
	u8 prefixedopcode0x23();	// SLA E
	u8 prefixedopcode0x24();	// SLA H
	u8 prefixedopcode0x25();	// SLA L
	u8 prefixedopcode0x26();	// SLA HL
	u8 prefixedopcode0x27();	// SLA A
	u8 prefixedopcode0x28();	// SRA B
	u8 prefixedopcode0x29();	// SRA C
	u8 prefixedopcode0x2A();	// SRA D
	u8 prefixedopcode0x2B();	// SRA E
	u8 prefixedopcode0x2C();	// SRA H
	u8 prefixedopcode0x2D();	// SRA L
	u8 prefixedopcode0x2E();	// SRA HL
	u8 prefixedopcode0x2F();	// SRA A
	u8 prefixedopcode0x30();	// SWAP B
	u8 prefixedopcode0x31();	// SWAP C
	u8 prefixedopcode0x32();	// SWAP D
	u8 prefixedopcode0x33();	// SWAP E
	u8 prefixedopcode0x34();	// SWAP H
	u8 prefixedopcode0x35();	// SWAP L
	u8 prefixedopcode0x36();	// SWAP HL
	u8 prefixedopcode0x37();	// SWAP A
	u8 prefixedopcode0x38();	// SRL B
	u8 prefixedopcode0x39();	// SRL C
	u8 prefixedopcode0x3A();	// SRL D
	u8 prefixedopcode0x3B();	// SRL E
	u8 prefixedopcode0x3C();	// SRL H
	u8 prefixedopcode0x3D();	// SRL L
	u8 prefixedopcode0x3E();	// SRL HL
	u8 prefixedopcode0x3F();	// SRL A
	u8 prefixedopcode0x40();	// BIT 0 B
	u8 prefixedopcode0x41();	// BIT 0 C
	u8 prefixedopcode0x42();	// BIT 0 D
	u8 prefixedopcode0x43();	// BIT 0 E
	u8 prefixedopcode0x44();	// BIT 0 H
	u8 prefixedopcode0x45();	// BIT 0 L
	u8 prefixedopcode0x46();	// BIT 0 HL
	u8 prefixedopcode0x47();	// BIT 0 A
	u8 prefixedopcode0x48();	// BIT 1 B
	u8 prefixedopcode0x49();	// BIT 1 C
	u8 prefixedopcode0x4A();	// BIT 1 D
	u8 prefixedopcode0x4B();	// BIT 1 E
	u8 prefixedopcode0x4C();	// BIT 1 H
	u8 prefixedopcode0x4D();	// BIT 1 L
	u8 prefixedopcode0x4E();	// BIT 1 HL
	u8 prefixedopcode0x4F();	// BIT 1 A
	u8 prefixedopcode0x50();	// BIT 2 B
	u8 prefixedopcode0x51();	// BIT 2 C
	u8 prefixedopcode0x52();	// BIT 2 D
	u8 prefixedopcode0x53();	// BIT 2 E
	u8 prefixedopcode0x54();	// BIT 2 H
	u8 prefixedopcode0x55();	// BIT 2 L
	u8 prefixedopcode0x56();	// BIT 2 HL
	u8 prefixedopcode0x57();	// BIT 2 A
	u8 prefixedopcode0x58();	// BIT 3 B
	u8 prefixedopcode0x59();	// BIT 3 C
	u8 prefixedopcode0x5A();	// BIT 3 D
	u8 prefixedopcode0x5B();	// BIT 3 E
	u8 prefixedopcode0x5C();	// BIT 3 H
	u8 prefixedopcode0x5D();	// BIT 3 L
	u8 prefixedopcode0x5E();	// BIT 3 HL
	u8 prefixedopcode0x5F();	// BIT 3 A
	u8 prefixedopcode0x60();	// BIT 4 B
	u8 prefixedopcode0x61();	// BIT 4 C
	u8 prefixedopcode0x62();	// BIT 4 D
	u8 prefixedopcode0x63();	// BIT 4 E
	u8 prefixedopcode0x64();	// BIT 4 H
	u8 prefixedopcode0x65();	// BIT 4 L
	u8 prefixedopcode0x66();	// BIT 4 HL
	u8 prefixedopcode0x67();	// BIT 4 A
	u8 prefixedopcode0x68();	// BIT 5 B
	u8 prefixedopcode0x69();	// BIT 5 C
	u8 prefixedopcode0x6A();	// BIT 5 D
	u8 prefixedopcode0x6B();	// BIT 5 E
	u8 prefixedopcode0x6C();	// BIT 5 H
	u8 prefixedopcode0x6D();	// BIT 5 L
	u8 prefixedopcode0x6E();	// BIT 5 HL
	u8 prefixedopcode0x6F();	// BIT 5 A
	u8 prefixedopcode0x70();	// BIT 6 B
	u8 prefixedopcode0x71();	// BIT 6 C
	u8 prefixedopcode0x72();	// BIT 6 D
	u8 prefixedopcode0x73();	// BIT 6 E
	u8 prefixedopcode0x74();	// BIT 6 H
	u8 prefixedopcode0x75();	// BIT 6 L
	u8 prefixedopcode0x76();	// BIT 6 HL
	u8 prefixedopcode0x77();	// BIT 6 A
	u8 prefixedopcode0x78();	// BIT 7 B
	u8 prefixedopcode0x79();	// BIT 7 C
	u8 prefixedopcode0x7A();	// BIT 7 D
	u8 prefixedopcode0x7B();	// BIT 7 E
	u8 prefixedopcode0x7C();	// BIT 7 H
	u8 prefixedopcode0x7D();	// BIT 7 L
	u8 prefixedopcode0x7E();	// BIT 7 HL
	u8 prefixedopcode0x7F();	// BIT 7 A
	u8 prefixedopcode0x80();	// RES 0 B
	u8 prefixedopcode0x81();	// RES 0 C
	u8 prefixedopcode0x82();	// RES 0 D
	u8 prefixedopcode0x83();	// RES 0 E
	u8 prefixedopcode0x84();	// RES 0 H
	u8 prefixedopcode0x85();	// RES 0 L
	u8 prefixedopcode0x86();	// RES 0 HL
	u8 prefixedopcode0x87();	// RES 0 A
	u8 prefixedopcode0x88();	// RES 1 B
	u8 prefixedopcode0x89();	// RES 1 C
	u8 prefixedopcode0x8A();	// RES 1 D
	u8 prefixedopcode0x8B();	// RES 1 E
	u8 prefixedopcode0x8C();	// RES 1 H
	u8 prefixedopcode0x8D();	// RES 1 L
	u8 prefixedopcode0x8E();	// RES 1 HL
	u8 prefixedopcode0x8F();	// RES 1 A
	u8 prefixedopcode0x90();	// RES 2 B
	u8 prefixedopcode0x91();	// RES 2 C
	u8 prefixedopcode0x92();	// RES 2 D
	u8 prefixedopcode0x93();	// RES 2 E
	u8 prefixedopcode0x94();	// RES 2 H
	u8 prefixedopcode0x95();	// RES 2 L
	u8 prefixedopcode0x96();	// RES 2 HL
	u8 prefixedopcode0x97();	// RES 2 A
	u8 prefixedopcode0x98();	// RES 3 B
	u8 prefixedopcode0x99();	// RES 3 C
	u8 prefixedopcode0x9A();	// RES 3 D
	u8 prefixedopcode0x9B();	// RES 3 E
	u8 prefixedopcode0x9C();	// RES 3 H
	u8 prefixedopcode0x9D();	// RES 3 L
	u8 prefixedopcode0x9E();	// RES 3 HL
	u8 prefixedopcode0x9F();	// RES 3 A
	u8 prefixedopcode0xA0();	// RES 4 B
	u8 prefixedopcode0xA1();	// RES 4 C
	u8 prefixedopcode0xA2();	// RES 4 D
	u8 prefixedopcode0xA3();	// RES 4 E
	u8 prefixedopcode0xA4();	// RES 4 H
	u8 prefixedopcode0xA5();	// RES 4 L
	u8 prefixedopcode0xA6();	// RES 4 HL
	u8 prefixedopcode0xA7();	// RES 4 A
	u8 prefixedopcode0xA8();	// RES 5 B
	u8 prefixedopcode0xA9();	// RES 5 C
	u8 prefixedopcode0xAA();	// RES 5 D
	u8 prefixedopcode0xAB();	// RES 5 E
	u8 prefixedopcode0xAC();	// RES 5 H
	u8 prefixedopcode0xAD();	// RES 5 L
	u8 prefixedopcode0xAE();	// RES 5 HL
	u8 prefixedopcode0xAF();	// RES 5 A
	u8 prefixedopcode0xB0();	// RES 6 B
	u8 prefixedopcode0xB1();	// RES 6 C
	u8 prefixedopcode0xB2();	// RES 6 D
	u8 prefixedopcode0xB3();	// RES 6 E
	u8 prefixedopcode0xB4();	// RES 6 H
	u8 prefixedopcode0xB5();	// RES 6 L
	u8 prefixedopcode0xB6();	// RES 6 HL
	u8 prefixedopcode0xB7();	// RES 6 A
	u8 prefixedopcode0xB8();	// RES 7 B
	u8 prefixedopcode0xB9();	// RES 7 C
	u8 prefixedopcode0xBA();	// RES 7 D
	u8 prefixedopcode0xBB();	// RES 7 E
	u8 prefixedopcode0xBC();	// RES 7 H
	u8 prefixedopcode0xBD();	// RES 7 L
	u8 prefixedopcode0xBE();	// RES 7 HL
	u8 prefixedopcode0xBF();	// RES 7 A
	u8 prefixedopcode0xC0();	// SET 0 B
	u8 prefixedopcode0xC1();	// SET 0 C
	u8 prefixedopcode0xC2();	// SET 0 D
	u8 prefixedopcode0xC3();	// SET 0 E
	u8 prefixedopcode0xC4();	// SET 0 H
	u8 prefixedopcode0xC5();	// SET 0 L
	u8 prefixedopcode0xC6();	// SET 0 HL
	u8 prefixedopcode0xC7();	// SET 0 A
	u8 prefixedopcode0xC8();	// SET 1 B
	u8 prefixedopcode0xC9();	// SET 1 C
	u8 prefixedopcode0xCA();	// SET 1 D
	u8 prefixedopcode0xCB();	// SET 1 E
	u8 prefixedopcode0xCC();	// SET 1 H
	u8 prefixedopcode0xCD();	// SET 1 L
	u8 prefixedopcode0xCE();	// SET 1 HL
	u8 prefixedopcode0xCF();	// SET 1 A
	u8 prefixedopcode0xD0();	// SET 2 B
	u8 prefixedopcode0xD1();	// SET 2 C
	u8 prefixedopcode0xD2();	// SET 2 D
	u8 prefixedopcode0xD3();	// SET 2 E
	u8 prefixedopcode0xD4();	// SET 2 H
	u8 prefixedopcode0xD5();	// SET 2 L
	u8 prefixedopcode0xD6();	// SET 2 HL
	u8 prefixedopcode0xD7();	// SET 2 A
	u8 prefixedopcode0xD8();	// SET 3 B
	u8 prefixedopcode0xD9();	// SET 3 C
	u8 prefixedopcode0xDA();	// SET 3 D
	u8 prefixedopcode0xDB();	// SET 3 E
	u8 prefixedopcode0xDC();	// SET 3 H
	u8 prefixedopcode0xDD();	// SET 3 L
	u8 prefixedopcode0xDE();	// SET 3 HL
	u8 prefixedopcode0xDF();	// SET 3 A
	u8 prefixedopcode0xE0();	// SET 4 B
	u8 prefixedopcode0xE1();	// SET 4 C
	u8 prefixedopcode0xE2();	// SET 4 D
	u8 prefixedopcode0xE3();	// SET 4 E
	u8 prefixedopcode0xE4();	// SET 4 H
	u8 prefixedopcode0xE5();	// SET 4 L
	u8 prefixedopcode0xE6();	// SET 4 HL
	u8 prefixedopcode0xE7();	// SET 4 A
	u8 prefixedopcode0xE8();	// SET 5 B
	u8 prefixedopcode0xE9();	// SET 5 C
	u8 prefixedopcode0xEA();	// SET 5 D
	u8 prefixedopcode0xEB();	// SET 5 E
	u8 prefixedopcode0xEC();	// SET 5 H
	u8 prefixedopcode0xED();	// SET 5 L
	u8 prefixedopcode0xEE();	// SET 5 HL
	u8 prefixedopcode0xEF();	// SET 5 A
	u8 prefixedopcode0xF0();	// SET 6 B
	u8 prefixedopcode0xF1();	// SET 6 C
	u8 prefixedopcode0xF2();	// SET 6 D
	u8 prefixedopcode0xF3();	// SET 6 E
	u8 prefixedopcode0xF4();	// SET 6 H
	u8 prefixedopcode0xF5();	// SET 6 L
	u8 prefixedopcode0xF6();	// SET 6 HL
	u8 prefixedopcode0xF7();	// SET 6 A
	u8 prefixedopcode0xF8();	// SET 7 B
	u8 prefixedopcode0xF9();	// SET 7 C
	u8 prefixedopcode0xFA();	// SET 7 D
	u8 prefixedopcode0xFB();	// SET 7 E
	u8 prefixedopcode0xFC();	// SET 7 H
	u8 prefixedopcode0xFD();	// SET 7 L
	u8 prefixedopcode0xFE();	// SET 7 HL
	u8 prefixedopcode0xFF();	// SET 7 A

};
#endif // !CPU_H_GUARD
