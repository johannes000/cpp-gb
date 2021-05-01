#include "../CPU.h"

void CPU::push_r16_reg(const u16 *reg) {
	// Push register pair nn onto stack.
	// Decrement Stack Pointer(SP) twice
	const u8 low = *reg & 0xFFu;
	const u8 high = (*reg & 0xFF00u) >> 8u;
	write(--R.SP, high);
	write(--R.SP, low);
}

void CPU::pop_r16_reg(u16 *reg) {
	// Popt 2 u8s vom stack ins reg
	const u8 low = read(R.SP++);
	const u16 high = read(R.SP++) << 8u;
	*reg = low | high;
}

void CPU::ret() {
	//  Pop two bytes from stack & jump to that address.
	pop_r16_reg(&R.PC);
}

void CPU::call() {
	// Push address of next cpu_instruction onto stack and then jump to address nn.
	u16 next_inst = R.PC + current_instruction_info.bytes;
	push_r16_reg(&next_inst);
	R.PC = read_u16_at_pc();
}

u8 CPU::opcode0x18() { // JR r8
	R.PC += read_i8_at_pc();
	return 0;
}

u8 CPU::opcode0x20() { // JR NZ r8
	if (!check_flag(Flag::z)) {
		R.PC += read_i8_at_pc();
		current_instruction_info.cycles = 3;
		return 0;
	}
	return 0;
}

u8 CPU::opcode0x28() { // JR Z r8
	if (check_flag(Flag::z)) {
		R.PC += read_i8_at_pc();
		current_instruction_info.cycles = 3;
		return 0;
	}
	return 0;
}

u8 CPU::opcode0x30() { // JR NC r8
	if (!check_flag(Flag::c)) {
		R.PC += read_i8_at_pc();
		current_instruction_info.cycles = 3;
		return 0;
	}
	return 0;
}

u8 CPU::opcode0x38() { // JR C r8
	if (check_flag(Flag::c)) {
		R.PC += read_i8_at_pc();
		current_instruction_info.cycles = 3;
		return 0;
	}
	return 0;
}

u8 CPU::opcode0xC0() { // RET NZ
	if (!check_flag(Flag::z)) {
		ret();
		current_instruction_info.cycles = 5;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xC8() { // RET Z
	if (check_flag(Flag::z)) {
		ret();
		current_instruction_info.cycles = 5;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xD0() { // RET NC
	if (!check_flag(Flag::c)) {
		ret();
		current_instruction_info.cycles = 5;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xD8() { // RET C
	if (check_flag(Flag::c)) {
		ret();
		current_instruction_info.cycles = 5;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xC2() { // JP NZ a16
	if (!check_flag(Flag::z)) {
		R.PC = read_u16_at_pc();
		current_instruction_info.cycles = 4;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xC3() { // JP a16
	R.PC = read_u16_at_pc();
	return 1;
}

u8 CPU::opcode0xCA() { // JP Z a16
	if (check_flag(Flag::z)) {
		R.PC = read_u16_at_pc();
		current_instruction_info.cycles = 4;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xD2() { // JP NC a16
	if (!check_flag(Flag::c)) {
		R.PC = read_u16_at_pc();
		current_instruction_info.cycles = 4;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xDA() { // JP C a16
	if (check_flag(Flag::c)) {
		R.PC = read_u16_at_pc();
		current_instruction_info.cycles = 4;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xE9() { // JP HL
	R.PC = R.HL;
	return 1;
}

u8 CPU::opcode0xC4() { // CALL NZ a16
	if (!check_flag(Flag::z)) {
		call();
		current_instruction_info.cycles = 6;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xCC() { // CALL Z a16
	if (check_flag(Flag::z)) {
		call();
		current_instruction_info.cycles = 6;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xCD() { // CALL a16
	call();
	return 1;
}

u8 CPU::opcode0xD4() { // CALL NC a16
	if (!check_flag(Flag::c)) {
		call();
		current_instruction_info.cycles = 6;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xDC() { // CALL C a16
	if (check_flag(Flag::c)) {
		call();
		current_instruction_info.cycles = 6;
		return 1;
	}
	return 0;
}

u8 CPU::opcode0xDF() { // RST 18H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = static_cast<u16>(0x18);
	return 1;
}

u8 CPU::opcode0xE7() { // RST 20H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x20;
	return 1;
}

u8 CPU::opcode0xEF() { // RST 28H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x28;
	return 1;
}

u8 CPU::opcode0xF7() { // RST 30H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x30;
	return 1;
}

u8 CPU::opcode0xFF() { // RST 38H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x38;
	return 1;
}

u8 CPU::opcode0xC9() { // RET
	ret();
	return 1;
}

u8 CPU::opcode0xD9() { // RETI
	IME = true;
	ret();
	return 1;
}

u8 CPU::opcode0xC7() { // RST 00H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x0000;
	return 1;
}

u8 CPU::opcode0xCF() { // RST 08H

	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x0008;
	return 1;
}

u8 CPU::opcode0xD7() { // RST 10H
	u16 data = R.PC + current_instruction_info.bytes;
	push_r16_reg(&data);
	R.PC = 0x0010;
	return 1;
}

u8 CPU::opcode0xC1() { // POP BC
	pop_r16_reg(&R.BC);
	return 0;
}

u8 CPU::opcode0xD1() { // POP DE
	pop_r16_reg(&R.DE);
	return 0;
}

u8 CPU::opcode0xE1() { // POP HL
	pop_r16_reg(&R.HL);
	return 0;
}

u8 CPU::opcode0xF1() { // POP AF
	pop_r16_reg(&R.AF);
	R.F = R.F & 0xF0u;
	return 0;
}

u8 CPU::opcode0xC5() { // PUSH BC
	push_r16_reg(&R.BC);
	return 0;
}

u8 CPU::opcode0xD5() { // PUSH DE
	push_r16_reg(&R.DE);
	return 0;
}

u8 CPU::opcode0xE5() { // PUSH HL
	push_r16_reg(&R.HL);
	return 0;
}

u8 CPU::opcode0xF5() { // PUSH AF
	push_r16_reg(&R.AF);
	return 0;
}
