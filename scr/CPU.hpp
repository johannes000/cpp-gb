#ifndef CPU_H_GUARD
#define CPU_H_GUARD

#include <vector>
#include <map>

#include "MEMORY.hpp"
#include "utl/type.hpp"

enum class Flag {
	z = 7, // Zero Flag
	n = 6, // Add/Sub-Flag
	h = 5, // Half Carry Flag
	c = 4  // Carry Flag
};

struct CPU_Register {
	struct {
		union {
			struct {
				u8 F_;
				u8 A_;
			};
			u16 AF_;
		};
	};

	struct {
		union {
			struct {
				u8 C_;
				u8 B_;
			};
			u16 BC_;
		};
	};

	struct {
		union {
			struct {
				u8 E_;
				u8 D_;
			};
			u16 DE_;
		};
	};

	struct {
		union {
			struct {
				u8 L_;
				u8 H_;
			};
			u16 HL_;
		};
	};

	u16 SP_;
	u16 PC_;
};

class MEMORY;

class CPU {
public:
	explicit CPU(MEMORY *m);

	auto parse_op_code(const u8 &) -> u8;

	auto update() -> int;

	auto inc_pc() -> void;

	inline auto get_next_op_code() -> u8;

	auto is_in_boot() const -> bool;

	auto read(const u16 &addr) -> u8;

	auto write(const u16 &addr, const u8 &data) -> void;

	auto write_u16(const u16 &addr, const u16 &data) const -> void;

	auto init_register(bool) -> void;

private:
	MEMORY *mem_;
	int cycles_ = 0;
	bool in_boot_ = false;

public:
	// REGISTER
	CPU_Register R_{};

	auto dma_transfer(const u8 &data) -> void;

	[[nodiscard]] auto get_pc() const -> u16 { return R_.PC_; }

	[[nodiscard]] auto get_register() const -> const CPU_Register & { return R_; }

	// INTERRUPTS

	// Interrupt Master Enable Flag
	// 0 - Disable all Interrupts
	// 1 - Enable all Interrupts that are enabled in IE Register(FFFF)
	bool IME_ = false;

	bool is_halted_ = false;

	// FFFF - IE - Interrupt Enable (R/W)
	u8 interrupt_enable_register_ = 0x00;
	// FF0F - IF - Interrupt Flag (R/W)
	u8 interrupt_request_register_ = 0x00;

	auto handle_interrupts() -> void;

	auto request_interrupt(Interrupt inter) -> void;

	[[nodiscard]] auto is_IME() const -> bool { return IME_; }

	// FLAGS
	auto set_cpu_flag_if(const Flag &, bool condition) -> void;

	// setze flag auf true
	auto set_flag(const Flag &) -> void;
	auto set_flags(const Flag &, const Flag &) -> void;
	auto set_flags(const Flag &, const Flag &, const Flag &) -> void;

	auto reset_flag(const Flag &) -> void; // setze flag auf false egal was
	auto reset_flags(const Flag &F1, const Flag &F2) -> void;

	auto reset_flags(const Flag &F1, const Flag &F2, const Flag &F3) -> void;

	auto reset_all_flags() -> void;

	auto toggle_flag(const Flag &) -> void; // toggled die flag

	auto check_flag(const Flag &) const -> bool; // gibt flag status wieder

	// TIMER
	auto handle_timer(int t_cycles) -> void;

	// FF04h – DIV – Divider Register (R/W*)
	Register divider_register_{};
	// FF05h – TIMA – Timer Counter (R/W)
	u16 timer_counter_{};
	// FF06h – TMA – Timer Modulo (R/W)
	Register timer_modulo_{};
	// FF07h – TAC – Timer Control (R/W)
	// Bit 2 - Timer Enable (0=Disable, 1=Enable)
	// Bits 1-0 - Main Frequency Divider
	Register timer_control_{};

	int timer_cycle_sum_{};
	int div_cycle_sum_{};

	auto handle_div_register_write() -> void;

	auto handle_time_control_write(const u8 &data) -> void;

	// OPCODES
	struct cpu_instruction {
		const char *mnemonic_{};

		u8 (CPU::*function_)() = nullptr;

		u8 bytes_{};
		u8 cycles_{};
		bool prefixed_{false};
	};

	cpu_instruction current_instruction_info_;
	u8 op_code_{};

	bool prefix_ = false;

	[[nodiscard]] auto get_instruction_at_addr(u16 addr) const -> debug_instruction;

private:
	// Oft benutzte Anweisungen
	auto read_u8_at_pc() -> u8;

	auto read_u16_at_pc() -> u16;

	auto read_u16_sp() -> u16;

	auto read_i8_at_pc() -> s8;

	static auto u8_to_u16(const u8 &data_lb, const u8 &data_hb) -> u16;

	// ARITHMETIC
	auto xor_a_r8(const u8 *reg) -> void;

	auto and_a_r8(const u8 *reg) -> void;

	auto or_r8_r8(const u8 &reg) -> void;

	auto cp_a_r8(const u8 *reg) -> void;

	auto inc_u8_reg(u8 *reg) -> void;

	auto dec_u8_reg(u8 *reg) -> void;

	auto add_a_r8(const u8 *value) -> void;

	auto adc_a_r8(const u8 *value) -> void;

	auto add_r16_r16(u16 *reg, const u16 *value) -> void;

	auto sub_a_r8(const u8 *value) -> void;

	auto sbc_a_r8(const u8 *value) -> void;

	// JUMPS
	auto push_r16_reg(const u16 *reg) -> void;

	auto pop_r16_reg(u16 *reg) -> void;

	auto ret() -> void;

	auto call() -> void;

	// BITS
	auto test_n_bit_set_z(const u8 bit, const u8 *reg) -> void;

	// SHIFTS
	auto rot_right_c_r8(u8 *reg) -> void;

	auto rot_left_c_r8(u8 *reg) -> void;

	auto rot_left_r8(u8 *reg) -> void;

	auto rot_right_r8(u8 *reg) -> void;

	auto shift_left_a_r8(u8 *reg) -> void;

	auto shift_right_a_r8(u8 *reg) -> void;

	auto shift_right_l_r8(u8 *reg) -> void;

	auto swap_r8(u8 *reg) -> void;

	// OPCODES
	const std::vector<cpu_instruction> instructions_ = {
		{"NOP", &CPU::opcode0x00, 1, 1},
		{"LD BC d16", &CPU::opcode0x01, 3, 3},
		{"LD (BC) A", &CPU::opcode0x02, 1, 2},
		{"INC BC", &CPU::opcode0x03, 1, 2},
		{"INC B", &CPU::opcode0x04, 1, 1},
		{"DEC B", &CPU::opcode0x05, 1, 1},
		{"LD B d8", &CPU::opcode0x06, 2, 2},
		{"RLCA", &CPU::opcode0x07, 1, 1},
		{"LD (a16) SP", &CPU::opcode0x08, 3, 5},
		{"ADD HL BC", &CPU::opcode0x09, 1, 2},
		{"LD A (BC)", &CPU::opcode0x0A, 1, 2},
		{"DEC BC", &CPU::opcode0x0B, 1, 2},
		{"INC C", &CPU::opcode0x0C, 1, 1},
		{"DEC C", &CPU::opcode0x0D, 1, 1},
		{"LD C d8", &CPU::opcode0x0E, 2, 2},
		{"RRCA", &CPU::opcode0x0F, 1, 1},
		{"STOP", &CPU::opcode0x10, 1, 1},
		{"LD DE d16", &CPU::opcode0x11, 3, 3},
		{"LD (DE) A", &CPU::opcode0x12, 1, 2},
		{"INC DE", &CPU::opcode0x13, 1, 2},
		{"INC D", &CPU::opcode0x14, 1, 1},
		{"DEC D", &CPU::opcode0x15, 1, 1},
		{"LD D d8", &CPU::opcode0x16, 2, 2},
		{"RLA", &CPU::opcode0x17, 1, 1},
		{"JR r8", &CPU::opcode0x18, 2, 3},
		{"ADD HL DE", &CPU::opcode0x19, 1, 2},
		{"LD A (DE)", &CPU::opcode0x1A, 1, 2},
		{"DEC DE", &CPU::opcode0x1B, 1, 2},
		{"INC E", &CPU::opcode0x1C, 1, 1},
		{"DEC E", &CPU::opcode0x1D, 1, 1},
		{"LD E d8", &CPU::opcode0x1E, 2, 2},
		{"RRA", &CPU::opcode0x1F, 1, 1},
		{"JR NZ r8", &CPU::opcode0x20, 2, 2},
		{"LD HL d16", &CPU::opcode0x21, 3, 3},
		{"LD (HL+) A", &CPU::opcode0x22, 1, 2},
		{"INC HL", &CPU::opcode0x23, 1, 2},
		{"INC H", &CPU::opcode0x24, 1, 1},
		{"DEC H", &CPU::opcode0x25, 1, 1},
		{"LD H d8", &CPU::opcode0x26, 2, 2},
		{"DAA", &CPU::opcode0x27, 1, 1},
		{"JR Z r8", &CPU::opcode0x28, 2, 2},
		{"ADD HL HL", &CPU::opcode0x29, 1, 2},
		{"LD A (HL+)", &CPU::opcode0x2A, 1, 2},
		{"DEC HL", &CPU::opcode0x2B, 1, 2},
		{"INC L", &CPU::opcode0x2C, 1, 1},
		{"DEC L", &CPU::opcode0x2D, 1, 1},
		{"LD L d8", &CPU::opcode0x2E, 2, 2},
		{"CPL", &CPU::opcode0x2F, 1, 1},
		{"JR NC r8", &CPU::opcode0x30, 2, 2},
		{"LD SP d16", &CPU::opcode0x31, 3, 3},
		{"LD (HL-) A", &CPU::opcode0x32, 1, 2},
		{"INC SP", &CPU::opcode0x33, 1, 2},
		{"INC (HL)", &CPU::opcode0x34, 1, 3},
		{"DEC (HL)", &CPU::opcode0x35, 1, 3},
		{"LD (HL) d8", &CPU::opcode0x36, 2, 3},
		{"SCF", &CPU::opcode0x37, 1, 1},
		{"JR C r8", &CPU::opcode0x38, 2, 2},
		{"ADD HL SP", &CPU::opcode0x39, 1, 2},
		{"LD A (HL-)", &CPU::opcode0x3A, 1, 2},
		{"DEC SP", &CPU::opcode0x3B, 1, 2},
		{"INC A", &CPU::opcode0x3C, 1, 1},
		{"DEC A", &CPU::opcode0x3D, 1, 1},
		{"LD A d8", &CPU::opcode0x3E, 2, 2},
		{"CCF", &CPU::opcode0x3F, 1, 1},
		{"LD B B", &CPU::opcode0x40, 1, 1},
		{"LD B C", &CPU::opcode0x41, 1, 1},
		{"LD B D", &CPU::opcode0x42, 1, 1},
		{"LD B E", &CPU::opcode0x43, 1, 1},
		{"LD B H", &CPU::opcode0x44, 1, 1},
		{"LD B L", &CPU::opcode0x45, 1, 1},
		{"LD B (HL)", &CPU::opcode0x46, 1, 2},
		{"LD B A", &CPU::opcode0x47, 1, 1},
		{"LD C B", &CPU::opcode0x48, 1, 1},
		{"LD C C", &CPU::opcode0x49, 1, 1},
		{"LD C D", &CPU::opcode0x4A, 1, 1},
		{"LD C E", &CPU::opcode0x4B, 1, 1},
		{"LD C H", &CPU::opcode0x4C, 1, 1},
		{"LD C L", &CPU::opcode0x4D, 1, 1},
		{"LD C (HL)", &CPU::opcode0x4E, 1, 2},
		{"LD C A", &CPU::opcode0x4F, 1, 1},
		{"LD D B", &CPU::opcode0x50, 1, 1},
		{"LD D C", &CPU::opcode0x51, 1, 1},
		{"LD D D", &CPU::opcode0x52, 1, 1},
		{"LD D E", &CPU::opcode0x53, 1, 1},
		{"LD D H", &CPU::opcode0x54, 1, 1},
		{"LD D L", &CPU::opcode0x55, 1, 1},
		{"LD D (HL)", &CPU::opcode0x56, 1, 2},
		{"LD D A", &CPU::opcode0x57, 1, 1},
		{"LD E B", &CPU::opcode0x58, 1, 1},
		{"LD E C", &CPU::opcode0x59, 1, 1},
		{"LD E D", &CPU::opcode0x5A, 1, 1},
		{"LD E E", &CPU::opcode0x5B, 1, 1},
		{"LD E H", &CPU::opcode0x5C, 1, 1},
		{"LD E L", &CPU::opcode0x5D, 1, 1},
		{"LD E (HL)", &CPU::opcode0x5E, 1, 2},
		{"LD E A", &CPU::opcode0x5F, 1, 1},
		{"LD H B", &CPU::opcode0x60, 1, 1},
		{"LD H C", &CPU::opcode0x61, 1, 1},
		{"LD H D", &CPU::opcode0x62, 1, 1},
		{"LD H E", &CPU::opcode0x63, 1, 1},
		{"LD H H", &CPU::opcode0x64, 1, 1},
		{"LD H L", &CPU::opcode0x65, 1, 1},
		{"LD H (HL)", &CPU::opcode0x66, 1, 2},
		{"LD H A", &CPU::opcode0x67, 1, 1},
		{"LD L B", &CPU::opcode0x68, 1, 1},
		{"LD L C", &CPU::opcode0x69, 1, 1},
		{"LD L D", &CPU::opcode0x6A, 1, 1},
		{"LD L E", &CPU::opcode0x6B, 1, 1},
		{"LD L H", &CPU::opcode0x6C, 1, 1},
		{"LD L L", &CPU::opcode0x6D, 1, 1},
		{"LD L (HL)", &CPU::opcode0x6E, 1, 2},
		{"LD L A", &CPU::opcode0x6F, 1, 1},
		{"LD (HL) B", &CPU::opcode0x70, 1, 2},
		{"LD (HL) C", &CPU::opcode0x71, 1, 2},
		{"LD (HL) D", &CPU::opcode0x72, 1, 2},
		{"LD (HL) E", &CPU::opcode0x73, 1, 2},
		{"LD (HL) H", &CPU::opcode0x74, 1, 2},
		{"LD (HL) L", &CPU::opcode0x75, 1, 2},
		{"HALT", &CPU::opcode0x76, 1, 1},
		{"LD (HL) A", &CPU::opcode0x77, 1, 2},
		{"LD A B", &CPU::opcode0x78, 1, 1},
		{"LD A C", &CPU::opcode0x79, 1, 1},
		{"LD A D", &CPU::opcode0x7A, 1, 1},
		{"LD A E", &CPU::opcode0x7B, 1, 1},
		{"LD A H", &CPU::opcode0x7C, 1, 1},
		{"LD A L", &CPU::opcode0x7D, 1, 1},
		{"LD A (HL)", &CPU::opcode0x7E, 1, 2},
		{"LD A A", &CPU::opcode0x7F, 1, 1},
		{"ADD A B", &CPU::opcode0x80, 1, 1},
		{"ADD A C", &CPU::opcode0x81, 1, 1},
		{"ADD A D", &CPU::opcode0x82, 1, 1},
		{"ADD A E", &CPU::opcode0x83, 1, 1},
		{"ADD A H", &CPU::opcode0x84, 1, 1},
		{"ADD A L", &CPU::opcode0x85, 1, 1},
		{"ADD A (HL)", &CPU::opcode0x86, 1, 2},
		{"ADD A A", &CPU::opcode0x87, 1, 1},
		{"ADC A B", &CPU::opcode0x88, 1, 1},
		{"ADC A C", &CPU::opcode0x89, 1, 1},
		{"ADC A D", &CPU::opcode0x8A, 1, 1},
		{"ADC A E", &CPU::opcode0x8B, 1, 1},
		{"ADC A H", &CPU::opcode0x8C, 1, 1},
		{"ADC A L", &CPU::opcode0x8D, 1, 1},
		{"ADC A (HL)", &CPU::opcode0x8E, 1, 2},
		{"ADC A A", &CPU::opcode0x8F, 1, 1},
		{"SUB B", &CPU::opcode0x90, 1, 1},
		{"SUB C", &CPU::opcode0x91, 1, 1},
		{"SUB D", &CPU::opcode0x92, 1, 1},
		{"SUB E", &CPU::opcode0x93, 1, 1},
		{"SUB H", &CPU::opcode0x94, 1, 1},
		{"SUB L", &CPU::opcode0x95, 1, 1},
		{"SUB (HL)", &CPU::opcode0x96, 1, 2},
		{"SUB A", &CPU::opcode0x97, 1, 1},
		{"SBC A B", &CPU::opcode0x98, 1, 1},
		{"SBC A C", &CPU::opcode0x99, 1, 1},
		{"SBC A D", &CPU::opcode0x9A, 1, 1},
		{"SBC A E", &CPU::opcode0x9B, 1, 1},
		{"SBC A H", &CPU::opcode0x9C, 1, 1},
		{"SBC A L", &CPU::opcode0x9D, 1, 1},
		{"SBC A (HL)", &CPU::opcode0x9E, 1, 2},
		{"SBC A A", &CPU::opcode0x9F, 1, 1},
		{"AND B", &CPU::opcode0xA0, 1, 1},
		{"AND C", &CPU::opcode0xA1, 1, 1},
		{"AND D", &CPU::opcode0xA2, 1, 1},
		{"AND E", &CPU::opcode0xA3, 1, 1},
		{"AND H", &CPU::opcode0xA4, 1, 1},
		{"AND L", &CPU::opcode0xA5, 1, 1},
		{"AND (HL)", &CPU::opcode0xA6, 1, 2},
		{"AND A", &CPU::opcode0xA7, 1, 1},
		{"XOR B", &CPU::opcode0xA8, 1, 1},
		{"XOR C", &CPU::opcode0xA9, 1, 1},
		{"XOR D", &CPU::opcode0xAA, 1, 1},
		{"XOR E", &CPU::opcode0xAB, 1, 1},
		{"XOR H", &CPU::opcode0xAC, 1, 1},
		{"XOR L", &CPU::opcode0xAD, 1, 1},
		{"XOR (HL)", &CPU::opcode0xAE, 1, 2},
		{"XOR A", &CPU::opcode0xAF, 1, 1},
		{"OR B", &CPU::opcode0xB0, 1, 1},
		{"OR C", &CPU::opcode0xB1, 1, 1},
		{"OR D", &CPU::opcode0xB2, 1, 1},
		{"OR E", &CPU::opcode0xB3, 1, 1},
		{"OR H", &CPU::opcode0xB4, 1, 1},
		{"OR L", &CPU::opcode0xB5, 1, 1},
		{"OR HL", &CPU::opcode0xB6, 1, 2},
		{"OR A", &CPU::opcode0xB7, 1, 1},
		{"CP B", &CPU::opcode0xB8, 1, 1},
		{"CP C", &CPU::opcode0xB9, 1, 1},
		{"CP D", &CPU::opcode0xBA, 1, 1},
		{"CP E", &CPU::opcode0xBB, 1, 1},
		{"CP H", &CPU::opcode0xBC, 1, 1},
		{"CP L", &CPU::opcode0xBD, 1, 1},
		{"CP HL", &CPU::opcode0xBE, 1, 2},
		{"CP A", &CPU::opcode0xBF, 1, 1},
		{"RET NZ", &CPU::opcode0xC0, 1, 2},
		{"POP BC", &CPU::opcode0xC1, 1, 3},
		{"JP NZ a16", &CPU::opcode0xC2, 3, 3},
		{"JP a16", &CPU::opcode0xC3, 3, 4},
		{"CALL NZ a16", &CPU::opcode0xC4, 3, 3},
		{"PUSH BC", &CPU::opcode0xC5, 1, 4},
		{"ADD A d8", &CPU::opcode0xC6, 2, 2},
		{"RST 00H", &CPU::opcode0xC7, 1, 4},
		{"RET Z", &CPU::opcode0xC8, 1, 2},
		{"RET", &CPU::opcode0xC9, 1, 4},
		{"JP Z a16", &CPU::opcode0xCA, 3, 3},
		{"PREFIX", &CPU::opcode0xCB, 1, 1},
		{"CALL Z a16", &CPU::opcode0xCC, 3, 3},
		{"CALL a16", &CPU::opcode0xCD, 3, 6},
		{"ADC A d8", &CPU::opcode0xCE, 2, 2},
		{"RST 08H", &CPU::opcode0xCF, 1, 4},
		{"RET NC", &CPU::opcode0xD0, 1, 2},
		{"POP DE", &CPU::opcode0xD1, 1, 3},
		{"JP NC a16", &CPU::opcode0xD2, 3, 3},
		{"ILLEGAL_D3", &CPU::opcode0xD3, 1, 1},
		{"CALL NC a16", &CPU::opcode0xD4, 3, 3},
		{"PUSH DE", &CPU::opcode0xD5, 1, 4},
		{"SUB d8", &CPU::opcode0xD6, 2, 2},
		{"RST 10H", &CPU::opcode0xD7, 1, 4},
		{"RET C", &CPU::opcode0xD8, 1, 2},
		{"RETI", &CPU::opcode0xD9, 1, 4},
		{"JP C a16", &CPU::opcode0xDA, 3, 3},
		{"ILLEGAL_DB", &CPU::opcode0xDB, 1, 1},
		{"CALL C a16", &CPU::opcode0xDC, 3, 3},
		{"ILLEGAL_DD", &CPU::opcode0xDD, 1, 1},
		{"SBC A d8", &CPU::opcode0xDE, 2, 2},
		{"RST 18H", &CPU::opcode0xDF, 1, 4},
		{"LDH (a8) A", &CPU::opcode0xE0, 2, 3},
		{"POP HL", &CPU::opcode0xE1, 1, 3},
		{"LD (C) A", &CPU::opcode0xE2, 1, 2},
		{"ILLEGAL_E3", &CPU::opcode0xE3, 1, 1},
		{"ILLEGAL_E4", &CPU::opcode0xE4, 1, 1},
		{"PUSH HL", &CPU::opcode0xE5, 1, 4},
		{"AND d8", &CPU::opcode0xE6, 2, 2},
		{"RST 20H", &CPU::opcode0xE7, 1, 4},
		{"ADD SP r8", &CPU::opcode0xE8, 2, 4},
		{"JP (HL)", &CPU::opcode0xE9, 1, 1},
		{"LD (a16) A", &CPU::opcode0xEA, 3, 4},
		{"ILLEGAL_EB", &CPU::opcode0xEB, 1, 1},
		{"ILLEGAL_EC", &CPU::opcode0xEC, 1, 1},
		{"ILLEGAL_ED", &CPU::opcode0xED, 1, 1},
		{"XOR d8", &CPU::opcode0xEE, 2, 2},
		{"RST 28H", &CPU::opcode0xEF, 1, 4},
		{"LDH A (a8)", &CPU::opcode0xF0, 2, 3},
		{"POP AF", &CPU::opcode0xF1, 1, 3},
		{"LD A (C)", &CPU::opcode0xF2, 1, 2},
		{"DI", &CPU::opcode0xF3, 1, 1},
		{"ILLEGAL_F4", &CPU::opcode0xF4, 1, 1},
		{"PUSH AF", &CPU::opcode0xF5, 1, 4},
		{"OR d8", &CPU::opcode0xF6, 2, 2},
		{"RST 30H", &CPU::opcode0xF7, 1, 4},
		{"LD HL SP", &CPU::opcode0xF8, 2, 3},
		{"LD SP HL", &CPU::opcode0xF9, 1, 2},
		{"LD A (a16)", &CPU::opcode0xFA, 3, 4},
		{"EI", &CPU::opcode0xFB, 1, 1},
		{"ILLEGAL_FC", &CPU::opcode0xFC, 1, 1},
		{"ILLEGAL_FD", &CPU::opcode0xFD, 1, 1},
		{"CP d8", &CPU::opcode0xFE, 2, 2},
		{"RST 38H", &CPU::opcode0xFF, 1, 4},
	};

private:
	const std::vector<cpu_instruction> prefixedinstructions_ = {
		{"RLC B", &CPU::prefixedopcode0x00, 2, 2},
		{"RLC C", &CPU::prefixedopcode0x01, 2, 2},
		{"RLC D", &CPU::prefixedopcode0x02, 2, 2},
		{"RLC E", &CPU::prefixedopcode0x03, 2, 2},
		{"RLC H", &CPU::prefixedopcode0x04, 2, 2},
		{"RLC L", &CPU::prefixedopcode0x05, 2, 2},
		{"RLC HL", &CPU::prefixedopcode0x06, 2, 4},
		{"RLC A", &CPU::prefixedopcode0x07, 2, 2},
		{"RRC B", &CPU::prefixedopcode0x08, 2, 2},
		{"RRC C", &CPU::prefixedopcode0x09, 2, 2},
		{"RRC D", &CPU::prefixedopcode0x0A, 2, 2},
		{"RRC E", &CPU::prefixedopcode0x0B, 2, 2},
		{"RRC H", &CPU::prefixedopcode0x0C, 2, 2},
		{"RRC L", &CPU::prefixedopcode0x0D, 2, 2},
		{"RRC HL", &CPU::prefixedopcode0x0E, 2, 4},
		{"RRC A", &CPU::prefixedopcode0x0F, 2, 2},
		{"RL B", &CPU::prefixedopcode0x10, 2, 2},
		{"RL C", &CPU::prefixedopcode0x11, 2, 2},
		{"RL D", &CPU::prefixedopcode0x12, 2, 2},
		{"RL E", &CPU::prefixedopcode0x13, 2, 2},
		{"RL H", &CPU::prefixedopcode0x14, 2, 2},
		{"RL L", &CPU::prefixedopcode0x15, 2, 2},
		{"RL HL", &CPU::prefixedopcode0x16, 2, 4},
		{"RL A", &CPU::prefixedopcode0x17, 2, 2},
		{"RR B", &CPU::prefixedopcode0x18, 2, 2},
		{"RR C", &CPU::prefixedopcode0x19, 2, 2},
		{"RR D", &CPU::prefixedopcode0x1A, 2, 2},
		{"RR E", &CPU::prefixedopcode0x1B, 2, 2},
		{"RR H", &CPU::prefixedopcode0x1C, 2, 2},
		{"RR L", &CPU::prefixedopcode0x1D, 2, 2},
		{"RR HL", &CPU::prefixedopcode0x1E, 2, 4},
		{"RR A", &CPU::prefixedopcode0x1F, 2, 2},
		{"SLA B", &CPU::prefixedopcode0x20, 2, 2},
		{"SLA C", &CPU::prefixedopcode0x21, 2, 2},
		{"SLA D", &CPU::prefixedopcode0x22, 2, 2},
		{"SLA E", &CPU::prefixedopcode0x23, 2, 2},
		{"SLA H", &CPU::prefixedopcode0x24, 2, 2},
		{"SLA L", &CPU::prefixedopcode0x25, 2, 2},
		{"SLA HL", &CPU::prefixedopcode0x26, 2, 4},
		{"SLA A", &CPU::prefixedopcode0x27, 2, 2},
		{"SRA B", &CPU::prefixedopcode0x28, 2, 2},
		{"SRA C", &CPU::prefixedopcode0x29, 2, 2},
		{"SRA D", &CPU::prefixedopcode0x2A, 2, 2},
		{"SRA E", &CPU::prefixedopcode0x2B, 2, 2},
		{"SRA H", &CPU::prefixedopcode0x2C, 2, 2},
		{"SRA L", &CPU::prefixedopcode0x2D, 2, 2},
		{"SRA HL", &CPU::prefixedopcode0x2E, 2, 4},
		{"SRA A", &CPU::prefixedopcode0x2F, 2, 2},
		{"SWAP B", &CPU::prefixedopcode0x30, 2, 2},
		{"SWAP C", &CPU::prefixedopcode0x31, 2, 2},
		{"SWAP D", &CPU::prefixedopcode0x32, 2, 2},
		{"SWAP E", &CPU::prefixedopcode0x33, 2, 2},
		{"SWAP H", &CPU::prefixedopcode0x34, 2, 2},
		{"SWAP L", &CPU::prefixedopcode0x35, 2, 2},
		{"SWAP HL", &CPU::prefixedopcode0x36, 2, 4},
		{"SWAP A", &CPU::prefixedopcode0x37, 2, 2},
		{"SRL B", &CPU::prefixedopcode0x38, 2, 2},
		{"SRL C", &CPU::prefixedopcode0x39, 2, 2},
		{"SRL D", &CPU::prefixedopcode0x3A, 2, 2},
		{"SRL E", &CPU::prefixedopcode0x3B, 2, 2},
		{"SRL H", &CPU::prefixedopcode0x3C, 2, 2},
		{"SRL L", &CPU::prefixedopcode0x3D, 2, 2},
		{"SRL HL", &CPU::prefixedopcode0x3E, 2, 4},
		{"SRL A", &CPU::prefixedopcode0x3F, 2, 2},
		{"BIT 0 B", &CPU::prefixedopcode0x40, 2, 2},
		{"BIT 0 C", &CPU::prefixedopcode0x41, 2, 2},
		{"BIT 0 D", &CPU::prefixedopcode0x42, 2, 2},
		{"BIT 0 E", &CPU::prefixedopcode0x43, 2, 2},
		{"BIT 0 H", &CPU::prefixedopcode0x44, 2, 2},
		{"BIT 0 L", &CPU::prefixedopcode0x45, 2, 2},
		{"BIT 0 HL", &CPU::prefixedopcode0x46, 2, 3},
		{"BIT 0 A", &CPU::prefixedopcode0x47, 2, 2},
		{"BIT 1 B", &CPU::prefixedopcode0x48, 2, 2},
		{"BIT 1 C", &CPU::prefixedopcode0x49, 2, 2},
		{"BIT 1 D", &CPU::prefixedopcode0x4A, 2, 2},
		{"BIT 1 E", &CPU::prefixedopcode0x4B, 2, 2},
		{"BIT 1 H", &CPU::prefixedopcode0x4C, 2, 2},
		{"BIT 1 L", &CPU::prefixedopcode0x4D, 2, 2},
		{"BIT 1 HL", &CPU::prefixedopcode0x4E, 2, 3},
		{"BIT 1 A", &CPU::prefixedopcode0x4F, 2, 2},
		{"BIT 2 B", &CPU::prefixedopcode0x50, 2, 2},
		{"BIT 2 C", &CPU::prefixedopcode0x51, 2, 2},
		{"BIT 2 D", &CPU::prefixedopcode0x52, 2, 2},
		{"BIT 2 E", &CPU::prefixedopcode0x53, 2, 2},
		{"BIT 2 H", &CPU::prefixedopcode0x54, 2, 2},
		{"BIT 2 L", &CPU::prefixedopcode0x55, 2, 2},
		{"BIT 2 HL", &CPU::prefixedopcode0x56, 2, 3},
		{"BIT 2 A", &CPU::prefixedopcode0x57, 2, 2},
		{"BIT 3 B", &CPU::prefixedopcode0x58, 2, 2},
		{"BIT 3 C", &CPU::prefixedopcode0x59, 2, 2},
		{"BIT 3 D", &CPU::prefixedopcode0x5A, 2, 2},
		{"BIT 3 E", &CPU::prefixedopcode0x5B, 2, 2},
		{"BIT 3 H", &CPU::prefixedopcode0x5C, 2, 2},
		{"BIT 3 L", &CPU::prefixedopcode0x5D, 2, 2},
		{"BIT 3 HL", &CPU::prefixedopcode0x5E, 2, 3},
		{"BIT 3 A", &CPU::prefixedopcode0x5F, 2, 2},
		{"BIT 4 B", &CPU::prefixedopcode0x60, 2, 2},
		{"BIT 4 C", &CPU::prefixedopcode0x61, 2, 2},
		{"BIT 4 D", &CPU::prefixedopcode0x62, 2, 2},
		{"BIT 4 E", &CPU::prefixedopcode0x63, 2, 2},
		{"BIT 4 H", &CPU::prefixedopcode0x64, 2, 2},
		{"BIT 4 L", &CPU::prefixedopcode0x65, 2, 2},
		{"BIT 4 HL", &CPU::prefixedopcode0x66, 2, 3},
		{"BIT 4 A", &CPU::prefixedopcode0x67, 2, 2},
		{"BIT 5 B", &CPU::prefixedopcode0x68, 2, 2},
		{"BIT 5 C", &CPU::prefixedopcode0x69, 2, 2},
		{"BIT 5 D", &CPU::prefixedopcode0x6A, 2, 2},
		{"BIT 5 E", &CPU::prefixedopcode0x6B, 2, 2},
		{"BIT 5 H", &CPU::prefixedopcode0x6C, 2, 2},
		{"BIT 5 L", &CPU::prefixedopcode0x6D, 2, 2},
		{"BIT 5 HL", &CPU::prefixedopcode0x6E, 2, 3},
		{"BIT 5 A", &CPU::prefixedopcode0x6F, 2, 2},
		{"BIT 6 B", &CPU::prefixedopcode0x70, 2, 2},
		{"BIT 6 C", &CPU::prefixedopcode0x71, 2, 2},
		{"BIT 6 D", &CPU::prefixedopcode0x72, 2, 2},
		{"BIT 6 E", &CPU::prefixedopcode0x73, 2, 2},
		{"BIT 6 H", &CPU::prefixedopcode0x74, 2, 2},
		{"BIT 6 L", &CPU::prefixedopcode0x75, 2, 2},
		{"BIT 6 HL", &CPU::prefixedopcode0x76, 2, 3},
		{"BIT 6 A", &CPU::prefixedopcode0x77, 2, 2},
		{"BIT 7 B", &CPU::prefixedopcode0x78, 2, 2},
		{"BIT 7 C", &CPU::prefixedopcode0x79, 2, 2},
		{"BIT 7 D", &CPU::prefixedopcode0x7A, 2, 2},
		{"BIT 7 E", &CPU::prefixedopcode0x7B, 2, 2},
		{"BIT 7 H", &CPU::prefixedopcode0x7C, 2, 2},
		{"BIT 7 L", &CPU::prefixedopcode0x7D, 2, 2},
		{"BIT 7 HL", &CPU::prefixedopcode0x7E, 2, 3},
		{"BIT 7 A", &CPU::prefixedopcode0x7F, 2, 2},
		{"RES 0 B", &CPU::prefixedopcode0x80, 2, 2},
		{"RES 0 C", &CPU::prefixedopcode0x81, 2, 2},
		{"RES 0 D", &CPU::prefixedopcode0x82, 2, 2},
		{"RES 0 E", &CPU::prefixedopcode0x83, 2, 2},
		{"RES 0 H", &CPU::prefixedopcode0x84, 2, 2},
		{"RES 0 L", &CPU::prefixedopcode0x85, 2, 2},
		{"RES 0 HL", &CPU::prefixedopcode0x86, 2, 4},
		{"RES 0 A", &CPU::prefixedopcode0x87, 2, 2},
		{"RES 1 B", &CPU::prefixedopcode0x88, 2, 2},
		{"RES 1 C", &CPU::prefixedopcode0x89, 2, 2},
		{"RES 1 D", &CPU::prefixedopcode0x8A, 2, 2},
		{"RES 1 E", &CPU::prefixedopcode0x8B, 2, 2},
		{"RES 1 H", &CPU::prefixedopcode0x8C, 2, 2},
		{"RES 1 L", &CPU::prefixedopcode0x8D, 2, 2},
		{"RES 1 HL", &CPU::prefixedopcode0x8E, 2, 4},
		{"RES 1 A", &CPU::prefixedopcode0x8F, 2, 2},
		{"RES 2 B", &CPU::prefixedopcode0x90, 2, 2},
		{"RES 2 C", &CPU::prefixedopcode0x91, 2, 2},
		{"RES 2 D", &CPU::prefixedopcode0x92, 2, 2},
		{"RES 2 E", &CPU::prefixedopcode0x93, 2, 2},
		{"RES 2 H", &CPU::prefixedopcode0x94, 2, 2},
		{"RES 2 L", &CPU::prefixedopcode0x95, 2, 2},
		{"RES 2 HL", &CPU::prefixedopcode0x96, 2, 4},
		{"RES 2 A", &CPU::prefixedopcode0x97, 2, 2},
		{"RES 3 B", &CPU::prefixedopcode0x98, 2, 2},
		{"RES 3 C", &CPU::prefixedopcode0x99, 2, 2},
		{"RES 3 D", &CPU::prefixedopcode0x9A, 2, 2},
		{"RES 3 E", &CPU::prefixedopcode0x9B, 2, 2},
		{"RES 3 H", &CPU::prefixedopcode0x9C, 2, 2},
		{"RES 3 L", &CPU::prefixedopcode0x9D, 2, 2},
		{"RES 3 HL", &CPU::prefixedopcode0x9E, 2, 4},
		{"RES 3 A", &CPU::prefixedopcode0x9F, 2, 2},
		{"RES 4 B", &CPU::prefixedopcode0xA0, 2, 2},
		{"RES 4 C", &CPU::prefixedopcode0xA1, 2, 2},
		{"RES 4 D", &CPU::prefixedopcode0xA2, 2, 2},
		{"RES 4 E", &CPU::prefixedopcode0xA3, 2, 2},
		{"RES 4 H", &CPU::prefixedopcode0xA4, 2, 2},
		{"RES 4 L", &CPU::prefixedopcode0xA5, 2, 2},
		{"RES 4 HL", &CPU::prefixedopcode0xA6, 2, 4},
		{"RES 4 A", &CPU::prefixedopcode0xA7, 2, 2},
		{"RES 5 B", &CPU::prefixedopcode0xA8, 2, 2},
		{"RES 5 C", &CPU::prefixedopcode0xA9, 2, 2},
		{"RES 5 D", &CPU::prefixedopcode0xAA, 2, 2},
		{"RES 5 E", &CPU::prefixedopcode0xAB, 2, 2},
		{"RES 5 H", &CPU::prefixedopcode0xAC, 2, 2},
		{"RES 5 L", &CPU::prefixedopcode0xAD, 2, 2},
		{"RES 5 HL", &CPU::prefixedopcode0xAE, 2, 4},
		{"RES 5 A", &CPU::prefixedopcode0xAF, 2, 2},
		{"RES 6 B", &CPU::prefixedopcode0xB0, 2, 2},
		{"RES 6 C", &CPU::prefixedopcode0xB1, 2, 2},
		{"RES 6 D", &CPU::prefixedopcode0xB2, 2, 2},
		{"RES 6 E", &CPU::prefixedopcode0xB3, 2, 2},
		{"RES 6 H", &CPU::prefixedopcode0xB4, 2, 2},
		{"RES 6 L", &CPU::prefixedopcode0xB5, 2, 2},
		{"RES 6 HL", &CPU::prefixedopcode0xB6, 2, 4},
		{"RES 6 A", &CPU::prefixedopcode0xB7, 2, 2},
		{"RES 7 B", &CPU::prefixedopcode0xB8, 2, 2},
		{"RES 7 C", &CPU::prefixedopcode0xB9, 2, 2},
		{"RES 7 D", &CPU::prefixedopcode0xBA, 2, 2},
		{"RES 7 E", &CPU::prefixedopcode0xBB, 2, 2},
		{"RES 7 H", &CPU::prefixedopcode0xBC, 2, 2},
		{"RES 7 L", &CPU::prefixedopcode0xBD, 2, 2},
		{"RES 7 HL", &CPU::prefixedopcode0xBE, 2, 4},
		{"RES 7 A", &CPU::prefixedopcode0xBF, 2, 2},
		{"SET 0 B", &CPU::prefixedopcode0xC0, 2, 2},
		{"SET 0 C", &CPU::prefixedopcode0xC1, 2, 2},
		{"SET 0 D", &CPU::prefixedopcode0xC2, 2, 2},
		{"SET 0 E", &CPU::prefixedopcode0xC3, 2, 2},
		{"SET 0 H", &CPU::prefixedopcode0xC4, 2, 2},
		{"SET 0 L", &CPU::prefixedopcode0xC5, 2, 2},
		{"SET 0 HL", &CPU::prefixedopcode0xC6, 2, 4},
		{"SET 0 A", &CPU::prefixedopcode0xC7, 2, 2},
		{"SET 1 B", &CPU::prefixedopcode0xC8, 2, 2},
		{"SET 1 C", &CPU::prefixedopcode0xC9, 2, 2},
		{"SET 1 D", &CPU::prefixedopcode0xCA, 2, 2},
		{"SET 1 E", &CPU::prefixedopcode0xCB, 2, 2},
		{"SET 1 H", &CPU::prefixedopcode0xCC, 2, 2},
		{"SET 1 L", &CPU::prefixedopcode0xCD, 2, 2},
		{"SET 1 HL", &CPU::prefixedopcode0xCE, 2, 4},
		{"SET 1 A", &CPU::prefixedopcode0xCF, 2, 2},
		{"SET 2 B", &CPU::prefixedopcode0xD0, 2, 2},
		{"SET 2 C", &CPU::prefixedopcode0xD1, 2, 2},
		{"SET 2 D", &CPU::prefixedopcode0xD2, 2, 2},
		{"SET 2 E", &CPU::prefixedopcode0xD3, 2, 2},
		{"SET 2 H", &CPU::prefixedopcode0xD4, 2, 2},
		{"SET 2 L", &CPU::prefixedopcode0xD5, 2, 2},
		{"SET 2 HL", &CPU::prefixedopcode0xD6, 2, 4},
		{"SET 2 A", &CPU::prefixedopcode0xD7, 2, 2},
		{"SET 3 B", &CPU::prefixedopcode0xD8, 2, 2},
		{"SET 3 C", &CPU::prefixedopcode0xD9, 2, 2},
		{"SET 3 D", &CPU::prefixedopcode0xDA, 2, 2},
		{"SET 3 E", &CPU::prefixedopcode0xDB, 2, 2},
		{"SET 3 H", &CPU::prefixedopcode0xDC, 2, 2},
		{"SET 3 L", &CPU::prefixedopcode0xDD, 2, 2},
		{"SET 3 HL", &CPU::prefixedopcode0xDE, 2, 4},
		{"SET 3 A", &CPU::prefixedopcode0xDF, 2, 2},
		{"SET 4 B", &CPU::prefixedopcode0xE0, 2, 2},
		{"SET 4 C", &CPU::prefixedopcode0xE1, 2, 2},
		{"SET 4 D", &CPU::prefixedopcode0xE2, 2, 2},
		{"SET 4 E", &CPU::prefixedopcode0xE3, 2, 2},
		{"SET 4 H", &CPU::prefixedopcode0xE4, 2, 2},
		{"SET 4 L", &CPU::prefixedopcode0xE5, 2, 2},
		{"SET 4 HL", &CPU::prefixedopcode0xE6, 2, 4},
		{"SET 4 A", &CPU::prefixedopcode0xE7, 2, 2},
		{"SET 5 B", &CPU::prefixedopcode0xE8, 2, 2},
		{"SET 5 C", &CPU::prefixedopcode0xE9, 2, 2},
		{"SET 5 D", &CPU::prefixedopcode0xEA, 2, 2},
		{"SET 5 E", &CPU::prefixedopcode0xEB, 2, 2},
		{"SET 5 H", &CPU::prefixedopcode0xEC, 2, 2},
		{"SET 5 L", &CPU::prefixedopcode0xED, 2, 2},
		{"SET 5 HL", &CPU::prefixedopcode0xEE, 2, 4},
		{"SET 5 A", &CPU::prefixedopcode0xEF, 2, 2},
		{"SET 6 B", &CPU::prefixedopcode0xF0, 2, 2},
		{"SET 6 C", &CPU::prefixedopcode0xF1, 2, 2},
		{"SET 6 D", &CPU::prefixedopcode0xF2, 2, 2},
		{"SET 6 E", &CPU::prefixedopcode0xF3, 2, 2},
		{"SET 6 H", &CPU::prefixedopcode0xF4, 2, 2},
		{"SET 6 L", &CPU::prefixedopcode0xF5, 2, 2},
		{"SET 6 HL", &CPU::prefixedopcode0xF6, 2, 4},
		{"SET 6 A", &CPU::prefixedopcode0xF7, 2, 2},
		{"SET 7 B", &CPU::prefixedopcode0xF8, 2, 2},
		{"SET 7 C", &CPU::prefixedopcode0xF9, 2, 2},
		{"SET 7 D", &CPU::prefixedopcode0xFA, 2, 2},
		{"SET 7 E", &CPU::prefixedopcode0xFB, 2, 2},
		{"SET 7 H", &CPU::prefixedopcode0xFC, 2, 2},
		{"SET 7 L", &CPU::prefixedopcode0xFD, 2, 2},
		{"SET 7 HL", &CPU::prefixedopcode0xFE, 2, 4},
		{"SET 7 A", &CPU::prefixedopcode0xFF, 2, 2}};

	auto opcode0x00() -> u8; // NOP
	auto opcode0x01() -> u8; // LD BC d16
	auto opcode0x02() -> u8; // LD BC A
	auto opcode0x03() -> u8; // INC BC
	auto opcode0x04() -> u8; // INC B
	auto opcode0x05() -> u8; // DEC B
	auto opcode0x06() -> u8; // LD B d8
	auto opcode0x07() -> u8; // RLCA
	auto opcode0x08() -> u8; // LD a16 SP
	auto opcode0x09() -> u8; // ADD HL BC
	auto opcode0x0A() -> u8; // LD A BC
	auto opcode0x0B() -> u8; // DEC BC
	auto opcode0x0C() -> u8; // INC C
	auto opcode0x0D() -> u8; // DEC C
	auto opcode0x0E() -> u8; // LD C d8
	auto opcode0x0F() -> u8; // RRCA
	auto opcode0x10() -> u8; // STOP
	auto opcode0x11() -> u8; // LD DE d16
	auto opcode0x12() -> u8; // LD DE A
	auto opcode0x13() -> u8; // INC DE
	auto opcode0x14() -> u8; // INC D
	auto opcode0x15() -> u8; // DEC D
	auto opcode0x16() -> u8; // LD D d8
	auto opcode0x17() -> u8; // RLA
	auto opcode0x18() -> u8; // JR r8
	auto opcode0x19() -> u8; // ADD HL DE
	auto opcode0x1A() -> u8; // LD A DE
	auto opcode0x1B() -> u8; // DEC DE
	auto opcode0x1C() -> u8; // INC E
	auto opcode0x1D() -> u8; // DEC E
	auto opcode0x1E() -> u8; // LD E d8
	auto opcode0x1F() -> u8; // RRA
	auto opcode0x20() -> u8; // JR NZ r8
	auto opcode0x21() -> u8; // LD HL d16
	auto opcode0x22() -> u8; // LD HL A
	auto opcode0x23() -> u8; // INC HL
	auto opcode0x24() -> u8; // INC H
	auto opcode0x25() -> u8; // DEC H
	auto opcode0x26() -> u8; // LD H d8
	auto opcode0x27() -> u8; // DAA
	auto opcode0x28() -> u8; // JR Z r8
	auto opcode0x29() -> u8; // ADD HL HL
	auto opcode0x2A() -> u8; // LD A HL
	auto opcode0x2B() -> u8; // DEC HL
	auto opcode0x2C() -> u8; // INC L
	auto opcode0x2D() -> u8; // DEC L
	auto opcode0x2E() -> u8; // LD L d8
	auto opcode0x2F() -> u8; // CPL
	auto opcode0x30() -> u8; // JR NC r8
	auto opcode0x31() -> u8; // LD SP d16
	auto opcode0x32() -> u8; // LD HL A
	auto opcode0x33() -> u8; // INC SP
	auto opcode0x34() -> u8; // INC HL
	auto opcode0x35() -> u8; // DEC HL
	auto opcode0x36() -> u8; // LD HL d8
	auto opcode0x37() -> u8; // SCF
	auto opcode0x38() -> u8; // JR C r8
	auto opcode0x39() -> u8; // ADD HL SP
	auto opcode0x3A() -> u8; // LD A HL
	auto opcode0x3B() -> u8;

	// DEC SP
	auto opcode0x3C() -> u8; // INC A
	auto opcode0x3D() -> u8; // DEC A
	auto opcode0x3E() -> u8; // LD A d8
	auto opcode0x3F() -> u8; // CCF
	auto opcode0x40() -> u8; // LD B B
	auto opcode0x41() -> u8; // LD B C
	auto opcode0x42() -> u8; // LD B D
	auto opcode0x43() -> u8; // LD B E
	auto opcode0x44() -> u8; // LD B H
	auto opcode0x45() -> u8; // LD B L
	auto opcode0x46() -> u8; // LD B HL
	auto opcode0x47() -> u8; // LD B A
	auto opcode0x48() -> u8; // LD C B
	auto opcode0x49() -> u8; // LD C C
	auto opcode0x4A() -> u8; // LD C D
	auto opcode0x4B() -> u8; // LD C E
	auto opcode0x4C() -> u8; // LD C H
	auto opcode0x4D() -> u8; // LD C L
	auto opcode0x4E() -> u8; // LD C HL
	auto opcode0x4F() -> u8; // LD C A
	auto opcode0x50() -> u8; // LD D B
	auto opcode0x51() -> u8; // LD D C
	auto opcode0x52() -> u8; // LD D D
	auto opcode0x53() -> u8; // LD D E
	auto opcode0x54() -> u8; // LD D H
	auto opcode0x55() -> u8; // LD D L
	auto opcode0x56() -> u8; // LD D HL
	auto opcode0x57() -> u8; // LD D A
	auto opcode0x58() -> u8; // LD E B
	auto opcode0x59() -> u8; // LD E C
	auto opcode0x5A() -> u8; // LD E D
	auto opcode0x5B() -> u8; // LD E E
	auto opcode0x5C() -> u8; // LD E H
	auto opcode0x5D() -> u8; // LD E L
	auto opcode0x5E() -> u8; // LD E HL
	auto opcode0x5F() -> u8; // LD E A
	auto opcode0x60() -> u8; // LD H B
	auto opcode0x61() -> u8; // LD H C
	auto opcode0x62() -> u8; // LD H D
	auto opcode0x63() -> u8; // LD H E
	auto opcode0x64() -> u8; // LD H H
	auto opcode0x65() -> u8; // LD H L
	auto opcode0x66() -> u8; // LD H HL
	auto opcode0x67() -> u8; // LD H A
	auto opcode0x68() -> u8; // LD L B
	auto opcode0x69() -> u8; // LD L C
	auto opcode0x6A() -> u8; // LD L D
	auto opcode0x6B() -> u8; // LD L E
	auto opcode0x6C() -> u8; // LD L H
	auto opcode0x6D() -> u8; // LD L L
	auto opcode0x6E() -> u8; // LD L HL
	auto opcode0x6F() -> u8; // LD L A
	auto opcode0x70() -> u8; // LD HL B
	auto opcode0x71() -> u8; // LD HL C
	auto opcode0x72() -> u8; // LD HL D
	auto opcode0x73() -> u8; // LD HL E
	auto opcode0x74() -> u8; // LD HL H
	auto opcode0x75() -> u8; // LD HL L
	auto opcode0x76() -> u8; // HALT
	auto opcode0x77() -> u8; // LD HL A
	auto opcode0x78() -> u8; // LD A B
	auto opcode0x79() -> u8; // LD A C
	auto opcode0x7A() -> u8; // LD A D
	auto opcode0x7B() -> u8; // LD A E
	auto opcode0x7C() -> u8; // LD A H
	auto opcode0x7D() -> u8; // LD A L
	auto opcode0x7E() -> u8; // LD A HL
	auto opcode0x7F() -> u8; // LD A A
	auto opcode0x80() -> u8; // ADD A B
	auto opcode0x81() -> u8; // ADD A C
	auto opcode0x82() -> u8; // ADD A D
	auto opcode0x83() -> u8; // ADD A E
	auto opcode0x84() -> u8; // ADD A H
	auto opcode0x85() -> u8; // ADD A L
	auto opcode0x86() -> u8; // ADD A HL
	auto opcode0x87() -> u8; // ADD A A
	auto opcode0x88() -> u8; // ADC A B
	auto opcode0x89() -> u8; // ADC A C
	auto opcode0x8A() -> u8; // ADC A D
	auto opcode0x8B() -> u8; // ADC A E
	auto opcode0x8C() -> u8; // ADC A H
	auto opcode0x8D() -> u8; // ADC A L
	auto opcode0x8E() -> u8; // ADC A HL
	auto opcode0x8F() -> u8; // ADC A A
	auto opcode0x90() -> u8; // SUB B
	auto opcode0x91() -> u8; // SUB C
	auto opcode0x92() -> u8; // SUB D
	auto opcode0x93() -> u8; // SUB E
	auto opcode0x94() -> u8; // SUB H
	auto opcode0x95() -> u8; // SUB L
	auto opcode0x96() -> u8; // SUB HL
	auto opcode0x97() -> u8; // SUB A
	auto opcode0x98() -> u8; // SBC A B
	auto opcode0x99() -> u8; // SBC A C
	auto opcode0x9A() -> u8; // SBC A D
	auto opcode0x9B() -> u8; // SBC A E
	auto opcode0x9C() -> u8; // SBC A H
	auto opcode0x9D() -> u8; // SBC A L
	auto opcode0x9E() -> u8; // SBC A HL
	auto opcode0x9F() -> u8; // SBC A A
	auto opcode0xA0() -> u8; // AND B
	auto opcode0xA1() -> u8; // AND C
	auto opcode0xA2() -> u8; // AND D
	auto opcode0xA3() -> u8; // AND E
	auto opcode0xA4() -> u8; // AND H
	auto opcode0xA5() -> u8; // AND L
	auto opcode0xA6() -> u8; // AND HL
	auto opcode0xA7() -> u8; // AND A
	auto opcode0xA8() -> u8; // XOR B
	auto opcode0xA9() -> u8; // XOR C
	auto opcode0xAA() -> u8; // XOR D
	auto opcode0xAB() -> u8; // XOR E
	auto opcode0xAC() -> u8; // XOR H
	auto opcode0xAD() -> u8; // XOR L
	auto opcode0xAE() -> u8; // XOR HL
	auto opcode0xAF() -> u8; // XOR A
	auto opcode0xB0() -> u8; // OR B
	auto opcode0xB1() -> u8; // OR C
	auto opcode0xB2() -> u8; // OR D
	auto opcode0xB3() -> u8; // OR E
	auto opcode0xB4() -> u8; // OR H
	auto opcode0xB5() -> u8; // OR L
	auto opcode0xB6() -> u8; // OR HL
	auto opcode0xB7() -> u8; // OR A
	auto opcode0xB8() -> u8; // CP B
	auto opcode0xB9() -> u8; // CP C
	auto opcode0xBA() -> u8; // CP D
	auto opcode0xBB() -> u8; // CP E
	auto opcode0xBC() -> u8; // CP H
	auto opcode0xBD() -> u8; // CP L
	auto opcode0xBE() -> u8; // CP HL
	auto opcode0xBF() -> u8; // CP A
	auto opcode0xC0() -> u8; // RET NZ
	auto opcode0xC1() -> u8; // POP BC
	auto opcode0xC2() -> u8; // JP NZ a16
	auto opcode0xC3() -> u8; // JP a16
	auto opcode0xC4() -> u8; // CALL NZ a16
	auto opcode0xC5() -> u8; // PUSH BC
	auto opcode0xC6() -> u8; // ADD A d8
	auto opcode0xC7() -> u8; // RST 00H
	auto opcode0xC8() -> u8; // RET Z
	auto opcode0xC9() -> u8; // RET
	auto opcode0xCA() -> u8; // JP Z a16
	auto opcode0xCB() -> u8; // PREFIX
	auto opcode0xCC() -> u8; // CALL Z a16
	auto opcode0xCD() -> u8; // CALL a16
	auto opcode0xCE() -> u8; // ADC A d8
	auto opcode0xCF() -> u8; // RST 08H
	auto opcode0xD0() -> u8; // RET NC
	auto opcode0xD1() -> u8; // POP DE
	auto opcode0xD2() -> u8; // JP NC a16
	auto opcode0xD3() -> u8; // ILLEGAL_D3
	auto opcode0xD4() -> u8; // CALL NC a16
	auto opcode0xD5() -> u8; // PUSH DE
	auto opcode0xD6() -> u8; // SUB d8
	auto opcode0xD7() -> u8; // RST 10H
	auto opcode0xD8() -> u8; // RET C
	auto opcode0xD9() -> u8; // RETI
	auto opcode0xDA() -> u8; // JP C a16
	auto opcode0xDB() -> u8; // ILLEGAL_DB
	auto opcode0xDC() -> u8; // CALL C a16
	auto opcode0xDD() -> u8; // ILLEGAL_DD
	auto opcode0xDE() -> u8; // SBC A d8
	auto opcode0xDF() -> u8; // RST 18H
	auto opcode0xE0() -> u8; // LDH a8 A
	auto opcode0xE1() -> u8; // POP HL
	auto opcode0xE2() -> u8; // LD C A
	auto opcode0xE3() -> u8; // ILLEGAL_E3
	auto opcode0xE4() -> u8; // ILLEGAL_E4
	auto opcode0xE5() -> u8; // PUSH HL
	auto opcode0xE6() -> u8; // AND d8
	auto opcode0xE7() -> u8; // RST 20H
	auto opcode0xE8() -> u8; // ADD SP r8
	auto opcode0xE9() -> u8; // JP HL
	auto opcode0xEA() -> u8; // LD a16 A
	auto opcode0xEB() -> u8; // ILLEGAL_EB
	auto opcode0xEC() -> u8; // ILLEGAL_EC
	auto opcode0xED() -> u8; // ILLEGAL_ED
	auto opcode0xEE() -> u8; // XOR d8
	auto opcode0xEF() -> u8; // RST 28H
	auto opcode0xF0() -> u8; // LDH A a8
	auto opcode0xF1() -> u8; // POP AF
	auto opcode0xF2() -> u8; // LD A C
	auto opcode0xF3() -> u8; // DI
	auto opcode0xF4() -> u8; // ILLEGAL_F4
	auto opcode0xF5() -> u8; // PUSH AF
	auto opcode0xF6() -> u8; // OR d8
	auto opcode0xF7() -> u8; // RST 30H
	auto opcode0xF8() -> u8; // LD HL SP
	auto opcode0xF9() -> u8; // LD SP HL
	auto opcode0xFA() -> u8; // LD A a16
	auto opcode0xFB() -> u8; // EI
	auto opcode0xFC() -> u8; // ILLEGAL_FC
	auto opcode0xFD() -> u8; // ILLEGAL_FD
	auto opcode0xFE() -> u8; // CP d8
	auto opcode0xFF() -> u8; // RST 38H

	// refixed opcodes
	auto prefixedopcode0x00() -> u8; // RLC B
	auto prefixedopcode0x01() -> u8; // RLC C
	auto prefixedopcode0x02() -> u8; // RLC D
	auto prefixedopcode0x03() -> u8; // RLC E
	auto prefixedopcode0x04() -> u8; // RLC H
	auto prefixedopcode0x05() -> u8; // RLC L
	auto prefixedopcode0x06() -> u8; // RLC HL
	auto prefixedopcode0x07() -> u8; // RLC A
	auto prefixedopcode0x08() -> u8; // RRC B
	auto prefixedopcode0x09() -> u8; // RRC C
	auto prefixedopcode0x0A() -> u8; // RRC D
	auto prefixedopcode0x0B() -> u8; // RRC E
	auto prefixedopcode0x0C() -> u8; // RRC H
	auto prefixedopcode0x0D() -> u8; // RRC L
	auto prefixedopcode0x0E() -> u8; // RRC HL
	auto prefixedopcode0x0F() -> u8; // RRC A
	auto prefixedopcode0x10() -> u8; // RL B
	auto prefixedopcode0x11() -> u8; // RL C
	auto prefixedopcode0x12() -> u8; // RL D
	auto prefixedopcode0x13() -> u8; // RL E
	auto prefixedopcode0x14() -> u8; // RL H
	auto prefixedopcode0x15() -> u8; // RL L
	auto prefixedopcode0x16() -> u8; // RL HL
	auto prefixedopcode0x17() -> u8; // RL A
	auto prefixedopcode0x18() -> u8; // RR B
	auto prefixedopcode0x19() -> u8; // RR C
	auto prefixedopcode0x1A() -> u8; // RR D
	auto prefixedopcode0x1B() -> u8; // RR E
	auto prefixedopcode0x1C() -> u8; // RR H
	auto prefixedopcode0x1D() -> u8; // RR L
	auto prefixedopcode0x1E() -> u8; // RR HL
	auto prefixedopcode0x1F() -> u8; // RR A
	auto prefixedopcode0x20() -> u8; // SLA B
	auto prefixedopcode0x21() -> u8; // SLA C
	auto prefixedopcode0x22() -> u8; // SLA D
	auto prefixedopcode0x23() -> u8; // SLA E
	auto prefixedopcode0x24() -> u8; // SLA H
	auto prefixedopcode0x25() -> u8; // SLA L
	auto prefixedopcode0x26() -> u8; // SLA HL
	auto prefixedopcode0x27() -> u8; // SLA A
	auto prefixedopcode0x28() -> u8; // SRA B
	auto prefixedopcode0x29() -> u8; // SRA C
	auto prefixedopcode0x2A() -> u8; // SRA D
	auto prefixedopcode0x2B() -> u8; // SRA E
	auto prefixedopcode0x2C() -> u8; // SRA H
	auto prefixedopcode0x2D() -> u8; // SRA L
	auto prefixedopcode0x2E() -> u8; // SRA HL
	auto prefixedopcode0x2F() -> u8; // SRA A
	auto prefixedopcode0x30() -> u8; // SWAP B
	auto prefixedopcode0x31() -> u8; // SWAP C
	auto prefixedopcode0x32() -> u8; // SWAP D
	auto prefixedopcode0x33() -> u8; // SWAP E
	auto prefixedopcode0x34() -> u8; // SWAP H
	auto prefixedopcode0x35() -> u8; // SWAP L
	auto prefixedopcode0x36() -> u8; // SWAP HL
	auto prefixedopcode0x37() -> u8; // SWAP A
	auto prefixedopcode0x38() -> u8; // SRL B
	auto prefixedopcode0x39() -> u8; // SRL C
	auto prefixedopcode0x3A() -> u8; // SRL D
	auto prefixedopcode0x3B() -> u8; // SRL E
	auto prefixedopcode0x3C() -> u8; // SRL H
	auto prefixedopcode0x3D() -> u8; // SRL L
	auto prefixedopcode0x3E() -> u8; // SRL HL
	auto prefixedopcode0x3F() -> u8; // SRL A
	auto prefixedopcode0x40() -> u8; // BIT 0 B
	auto prefixedopcode0x41() -> u8; // BIT 0 C
	auto prefixedopcode0x42() -> u8; // BIT 0 D
	auto prefixedopcode0x43() -> u8; // BIT 0 E
	auto prefixedopcode0x44() -> u8; // BIT 0 H
	auto prefixedopcode0x45() -> u8; // BIT 0 L
	auto prefixedopcode0x46() -> u8; // BIT 0 HL
	auto prefixedopcode0x47() -> u8; // BIT 0 A
	auto prefixedopcode0x48() -> u8; // BIT 1 B
	auto prefixedopcode0x49() -> u8; // BIT 1 C
	auto prefixedopcode0x4A() -> u8; // BIT 1 D
	auto prefixedopcode0x4B() -> u8; // BIT 1 E
	auto prefixedopcode0x4C() -> u8; // BIT 1 H
	auto prefixedopcode0x4D() -> u8; // BIT 1 L
	auto prefixedopcode0x4E() -> u8; // BIT 1 HL
	auto prefixedopcode0x4F() -> u8; // BIT 1 A
	auto prefixedopcode0x50() -> u8; // BIT 2 B
	auto prefixedopcode0x51() -> u8; // BIT 2 C
	auto prefixedopcode0x52() -> u8; // BIT 2 D
	auto prefixedopcode0x53() -> u8; // BIT 2 E
	auto prefixedopcode0x54() -> u8; // BIT 2 H
	auto prefixedopcode0x55() -> u8; // BIT 2 L
	auto prefixedopcode0x56() -> u8; // BIT 2 HL
	auto prefixedopcode0x57() -> u8; // BIT 2 A
	auto prefixedopcode0x58() -> u8; // BIT 3 B
	auto prefixedopcode0x59() -> u8; // BIT 3 C
	auto prefixedopcode0x5A() -> u8; // BIT 3 D
	auto prefixedopcode0x5B() -> u8; // BIT 3 E
	auto prefixedopcode0x5C() -> u8; // BIT 3 H
	auto prefixedopcode0x5D() -> u8; // BIT 3 L
	auto prefixedopcode0x5E() -> u8; // BIT 3 HL
	auto prefixedopcode0x5F() -> u8; // BIT 3 A
	auto prefixedopcode0x60() -> u8; // BIT 4 B
	auto prefixedopcode0x61() -> u8; // BIT 4 C
	auto prefixedopcode0x62() -> u8; // BIT 4 D
	auto prefixedopcode0x63() -> u8; // BIT 4 E
	auto prefixedopcode0x64() -> u8; // BIT 4 H
	auto prefixedopcode0x65() -> u8; // BIT 4 L
	auto prefixedopcode0x66() -> u8; // BIT 4 HL
	auto prefixedopcode0x67() -> u8; // BIT 4 A
	auto prefixedopcode0x68() -> u8; // BIT 5 B
	auto prefixedopcode0x69() -> u8; // BIT 5 C
	auto prefixedopcode0x6A() -> u8; // BIT 5 D
	auto prefixedopcode0x6B() -> u8; // BIT 5 E
	auto prefixedopcode0x6C() -> u8; // BIT 5 H
	auto prefixedopcode0x6D() -> u8; // BIT 5 L
	auto prefixedopcode0x6E() -> u8; // BIT 5 HL
	auto prefixedopcode0x6F() -> u8; // BIT 5 A
	auto prefixedopcode0x70() -> u8; // BIT 6 B
	auto prefixedopcode0x71() -> u8; // BIT 6 C
	auto prefixedopcode0x72() -> u8; // BIT 6 D
	auto prefixedopcode0x73() -> u8; // BIT 6 E
	auto prefixedopcode0x74() -> u8; // BIT 6 H
	auto prefixedopcode0x75() -> u8; // BIT 6 L
	auto prefixedopcode0x76() -> u8; // BIT 6 HL
	auto prefixedopcode0x77() -> u8; // BIT 6 A
	auto prefixedopcode0x78() -> u8; // BIT 7 B
	auto prefixedopcode0x79() -> u8; // BIT 7 C
	auto prefixedopcode0x7A() -> u8; // BIT 7 D
	auto prefixedopcode0x7B() -> u8; // BIT 7 E
	auto prefixedopcode0x7C() -> u8; // BIT 7 H
	auto prefixedopcode0x7D() -> u8; // BIT 7 L
	auto prefixedopcode0x7E() -> u8; // BIT 7 HL
	auto prefixedopcode0x7F() -> u8; // BIT 7 A
	auto prefixedopcode0x80() -> u8; // RES 0 B
	auto prefixedopcode0x81() -> u8; // RES 0 C
	auto prefixedopcode0x82() -> u8; // RES 0 D
	auto prefixedopcode0x83() -> u8; // RES 0 E
	auto prefixedopcode0x84() -> u8; // RES 0 H
	auto prefixedopcode0x85() -> u8; // RES 0 L
	auto prefixedopcode0x86() -> u8; // RES 0 HL
	auto prefixedopcode0x87() -> u8; // RES 0 A
	auto prefixedopcode0x88() -> u8; // RES 1 B
	auto prefixedopcode0x89() -> u8; // RES 1 C
	auto prefixedopcode0x8A() -> u8; // RES 1 D
	auto prefixedopcode0x8B() -> u8; // RES 1 E
	auto prefixedopcode0x8C() -> u8; // RES 1 H
	auto prefixedopcode0x8D() -> u8; // RES 1 L
	auto prefixedopcode0x8E() -> u8; // RES 1 HL
	auto prefixedopcode0x8F() -> u8; // RES 1 A
	auto prefixedopcode0x90() -> u8; // RES 2 B
	auto prefixedopcode0x91() -> u8; // RES 2 C
	auto prefixedopcode0x92() -> u8; // RES 2 D
	auto prefixedopcode0x93() -> u8; // RES 2 E
	auto prefixedopcode0x94() -> u8; // RES 2 H
	auto prefixedopcode0x95() -> u8; // RES 2 L
	auto prefixedopcode0x96() -> u8; // RES 2 HL
	auto prefixedopcode0x97() -> u8; // RES 2 A
	auto prefixedopcode0x98() -> u8; // RES 3 B
	auto prefixedopcode0x99() -> u8; // RES 3 C
	auto prefixedopcode0x9A() -> u8; // RES 3 D
	auto prefixedopcode0x9B() -> u8; // RES 3 E
	auto prefixedopcode0x9C() -> u8; // RES 3 H
	auto prefixedopcode0x9D() -> u8; // RES 3 L
	auto prefixedopcode0x9E() -> u8; // RES 3 HL
	auto prefixedopcode0x9F() -> u8; // RES 3 A
	auto prefixedopcode0xA0() -> u8; // RES 4 B
	auto prefixedopcode0xA1() -> u8; // RES 4 C
	auto prefixedopcode0xA2() -> u8; // RES 4 D
	auto prefixedopcode0xA3() -> u8; // RES 4 E
	auto prefixedopcode0xA4() -> u8; // RES 4 H
	auto prefixedopcode0xA5() -> u8; // RES 4 L
	auto prefixedopcode0xA6() -> u8; // RES 4 HL
	auto prefixedopcode0xA7() -> u8; // RES 4 A
	auto prefixedopcode0xA8() -> u8; // RES 5 B
	auto prefixedopcode0xA9() -> u8; // RES 5 C
	auto prefixedopcode0xAA() -> u8; // RES 5 D
	auto prefixedopcode0xAB() -> u8; // RES 5 E
	auto prefixedopcode0xAC() -> u8; // RES 5 H
	auto prefixedopcode0xAD() -> u8; // RES 5 L
	auto prefixedopcode0xAE() -> u8; // RES 5 HL
	auto prefixedopcode0xAF() -> u8; // RES 5 A
	auto prefixedopcode0xB0() -> u8; // RES 6 B
	auto prefixedopcode0xB1() -> u8; // RES 6 C
	auto prefixedopcode0xB2() -> u8; // RES 6 D
	auto prefixedopcode0xB3() -> u8; // RES 6 E
	auto prefixedopcode0xB4() -> u8; // RES 6 H
	auto prefixedopcode0xB5() -> u8; // RES 6 L
	auto prefixedopcode0xB6() -> u8; // RES 6 HL
	auto prefixedopcode0xB7() -> u8; // RES 6 A
	auto prefixedopcode0xB8() -> u8; // RES 7 B
	auto prefixedopcode0xB9() -> u8; // RES 7 C
	auto prefixedopcode0xBA() -> u8; // RES 7 D
	auto prefixedopcode0xBB() -> u8; // RES 7 E
	auto prefixedopcode0xBC() -> u8; // RES 7 H
	auto prefixedopcode0xBD() -> u8; // RES 7 L
	auto prefixedopcode0xBE() -> u8; // RES 7 HL
	auto prefixedopcode0xBF() -> u8; // RES 7 A
	auto prefixedopcode0xC0() -> u8; // SET 0 B
	auto prefixedopcode0xC1() -> u8; // SET 0 C
	auto prefixedopcode0xC2() -> u8; // SET 0 D
	auto prefixedopcode0xC3() -> u8; // SET 0 E
	auto prefixedopcode0xC4() -> u8; // SET 0 H
	auto prefixedopcode0xC5() -> u8; // SET 0 L
	auto prefixedopcode0xC6() -> u8; // SET 0 HL
	auto prefixedopcode0xC7() -> u8; // SET 0 A
	auto prefixedopcode0xC8() -> u8; // SET 1 B
	auto prefixedopcode0xC9() -> u8; // SET 1 C
	auto prefixedopcode0xCA() -> u8; // SET 1 D
	auto prefixedopcode0xCB() -> u8; // SET 1 E
	auto prefixedopcode0xCC() -> u8; // SET 1 H
	auto prefixedopcode0xCD() -> u8; // SET 1 L
	auto prefixedopcode0xCE() -> u8; // SET 1 HL
	auto prefixedopcode0xCF() -> u8; // SET 1 A
	auto prefixedopcode0xD0() -> u8; // SET 2 B
	auto prefixedopcode0xD1() -> u8; // SET 2 C
	auto prefixedopcode0xD2() -> u8; // SET 2 D
	auto prefixedopcode0xD3() -> u8; // SET 2 E
	auto prefixedopcode0xD4() -> u8; // SET 2 H
	auto prefixedopcode0xD5() -> u8; // SET 2 L
	auto prefixedopcode0xD6() -> u8; // SET 2 HL
	auto prefixedopcode0xD7() -> u8; // SET 2 A
	auto prefixedopcode0xD8() -> u8; // SET 3 B
	auto prefixedopcode0xD9() -> u8; // SET 3 C
	auto prefixedopcode0xDA() -> u8; // SET 3 D
	auto prefixedopcode0xDB() -> u8; // SET 3 E
	auto prefixedopcode0xDC() -> u8; // SET 3 H
	auto prefixedopcode0xDD() -> u8; // SET 3 L
	auto prefixedopcode0xDE() -> u8; // SET 3 HL
	auto prefixedopcode0xDF() -> u8; // SET 3 A
	auto prefixedopcode0xE0() -> u8; // SET 4 B
	auto prefixedopcode0xE1() -> u8; // SET 4 C
	auto prefixedopcode0xE2() -> u8; // SET 4 D
	auto prefixedopcode0xE3() -> u8; // SET 4 E
	auto prefixedopcode0xE4() -> u8; // SET 4 H
	auto prefixedopcode0xE5() -> u8; // SET 4 L
	auto prefixedopcode0xE6() -> u8; // SET 4 HL
	auto prefixedopcode0xE7() -> u8; // SET 4 A
	auto prefixedopcode0xE8() -> u8; // SET 5 B
	auto prefixedopcode0xE9() -> u8; // SET 5 C
	auto prefixedopcode0xEA() -> u8; // SET 5 D
	auto prefixedopcode0xEB() -> u8; // SET 5 E
	auto prefixedopcode0xEC() -> u8; // SET 5 H
	auto prefixedopcode0xED() -> u8; // SET 5 L
	auto prefixedopcode0xEE() -> u8; // SET 5 HL
	auto prefixedopcode0xEF() -> u8; // SET 5 A
	auto prefixedopcode0xF0() -> u8; // SET 6 B
	auto prefixedopcode0xF1() -> u8; // SET 6 C
	auto prefixedopcode0xF2() -> u8; // SET 6 D
	auto prefixedopcode0xF3() -> u8; // SET 6 E
	auto prefixedopcode0xF4() -> u8; // SET 6 H
	auto prefixedopcode0xF5() -> u8; // SET 6 L
	auto prefixedopcode0xF6() -> u8; // SET 6 HL
	auto prefixedopcode0xF7() -> u8; // SET 6 A
	auto prefixedopcode0xF8() -> u8; // SET 7 B
	auto prefixedopcode0xF9() -> u8; // SET 7 C
	auto prefixedopcode0xFA() -> u8; // SET 7 D
	auto prefixedopcode0xFB() -> u8; // SET 7 E
	auto prefixedopcode0xFC() -> u8; // SET 7 H
	auto prefixedopcode0xFD() -> u8; // SET 7 L
	auto prefixedopcode0xFE() -> u8; // SET 7 HL
	auto prefixedopcode0xFF() -> u8; // SET 7 A
};

#endif // !CPU_H_GUARD
