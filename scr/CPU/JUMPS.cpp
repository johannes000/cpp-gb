#include "../CPU.hpp"

auto CPU::push_r16_reg(const u16 *reg) -> void {
	// Push register pair nn onto stack.
	// Decrement Stack Pointer(SP) twice
	const u8 low = *reg & 0xFFu;
	const u8 high = (*reg & 0xFF00u) >> 8u;
	write(--R_.SP_, high);
	write(--R_.SP_, low);
}

auto CPU::pop_r16_reg(u16 *reg) -> void {
	// Popt 2 u8s vom stack ins reg
	const u8 low = read(R_.SP_++);
	const u16 high = read(R_.SP_++) << 8u;
	*reg = low | high;
}

auto CPU::ret() -> void {
	//  Pop two bytes from stack & jump to that address.
	pop_r16_reg(&R_.PC_);
}

auto CPU::call() -> void {
	// Push address of next cpu_instruction onto stack and then jump to address nn.
	u16 next_inst = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&next_inst);
	R_.PC_ = read_u16_at_pc();
}

auto CPU::opcode0x18() -> u8 { // JR r8
	R_.PC_ += read_i8_at_pc();
	return 0;
}

auto CPU::opcode0x20() -> u8 { // JR NZ r8
	if (!check_flag(Flag::z)) {
		R_.PC_ += read_i8_at_pc();
		current_instruction_info_.cycles_ = 3;
		return 0;
	}
	return 0;
}

auto CPU::opcode0x28() -> u8 { // JR Z r8
	if (check_flag(Flag::z)) {
		R_.PC_ += read_i8_at_pc();
		current_instruction_info_.cycles_ = 3;
		return 0;
	}
	return 0;
}

auto CPU::opcode0x30() -> u8 { // JR NC r8
	if (!check_flag(Flag::c)) {
		R_.PC_ += read_i8_at_pc();
		current_instruction_info_.cycles_ = 3;
		return 0;
	}
	return 0;
}

auto CPU::opcode0x38() -> u8 { // JR C r8
	if (check_flag(Flag::c)) {
		R_.PC_ += read_i8_at_pc();
		current_instruction_info_.cycles_ = 3;
		return 0;
	}
	return 0;
}

auto CPU::opcode0xC0() -> u8 { // RET NZ
	if (!check_flag(Flag::z)) {
		ret();
		current_instruction_info_.cycles_ = 5;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xC8() -> u8 { // RET Z
	if (check_flag(Flag::z)) {
		ret();
		current_instruction_info_.cycles_ = 5;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xD0() -> u8 { // RET NC
	if (!check_flag(Flag::c)) {
		ret();
		current_instruction_info_.cycles_ = 5;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xD8() -> u8 { // RET C
	if (check_flag(Flag::c)) {
		ret();
		current_instruction_info_.cycles_ = 5;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xC2() -> u8 { // JP NZ a16
	if (!check_flag(Flag::z)) {
		R_.PC_ = read_u16_at_pc();
		current_instruction_info_.cycles_ = 4;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xC3() -> u8 { // JP a16
	R_.PC_ = read_u16_at_pc();
	return 1;
}

auto CPU::opcode0xCA() -> u8 { // JP Z a16
	if (check_flag(Flag::z)) {
		R_.PC_ = read_u16_at_pc();
		current_instruction_info_.cycles_ = 4;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xD2() -> u8 { // JP NC a16
	if (!check_flag(Flag::c)) {
		R_.PC_ = read_u16_at_pc();
		current_instruction_info_.cycles_ = 4;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xDA() -> u8 { // JP C a16
	if (check_flag(Flag::c)) {
		R_.PC_ = read_u16_at_pc();
		current_instruction_info_.cycles_ = 4;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xE9() -> u8 { // JP HL
	R_.PC_ = R_.HL_;
	return 1;
}

auto CPU::opcode0xC4() -> u8 { // CALL NZ a16
	if (!check_flag(Flag::z)) {
		call();
		current_instruction_info_.cycles_ = 6;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xCC() -> u8 { // CALL Z a16
	if (check_flag(Flag::z)) {
		call();
		current_instruction_info_.cycles_ = 6;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xCD() -> u8 { // CALL a16
	call();
	return 1;
}

auto CPU::opcode0xD4() -> u8 { // CALL NC a16
	if (!check_flag(Flag::c)) {
		call();
		current_instruction_info_.cycles_ = 6;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xDC() -> u8 { // CALL C a16
	if (check_flag(Flag::c)) {
		call();
		current_instruction_info_.cycles_ = 6;
		return 1;
	}
	return 0;
}

auto CPU::opcode0xDF() -> u8 { // RST 18H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = static_cast<u16>(0x18);
	return 1;
}

auto CPU::opcode0xE7() -> u8 { // RST 20H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x20;
	return 1;
}

auto CPU::opcode0xEF() -> u8 { // RST 28H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x28;
	return 1;
}

auto CPU::opcode0xF7() -> u8 { // RST 30H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x30;
	return 1;
}

auto CPU::opcode0xFF() -> u8 { // RST 38H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x38;
	return 1;
}

auto CPU::opcode0xC9() -> u8 { // RET
	ret();
	return 1;
}

auto CPU::opcode0xD9() -> u8 { // RETI
	IME_ = true;
	ret();
	return 1;
}

auto CPU::opcode0xC7() -> u8 { // RST 00H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x0000;
	return 1;
}

auto CPU::opcode0xCF() -> u8 { // RST 08H

	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x0008;
	return 1;
}

auto CPU::opcode0xD7() -> u8 { // RST 10H
	u16 data = R_.PC_ + current_instruction_info_.bytes_;
	push_r16_reg(&data);
	R_.PC_ = 0x0010;
	return 1;
}

auto CPU::opcode0xC1() -> u8 { // POP BC
	pop_r16_reg(&R_.BC_);
	return 0;
}

auto CPU::opcode0xD1() -> u8 { // POP DE
	pop_r16_reg(&R_.DE_);
	return 0;
}

auto CPU::opcode0xE1() -> u8 { // POP HL
	pop_r16_reg(&R_.HL_);
	return 0;
}

auto CPU::opcode0xF1() -> u8 { // POP AF
	pop_r16_reg(&R_.AF_);
	R_.F_ = R_.F_ & 0xF0u;
	return 0;
}

auto CPU::opcode0xC5() -> u8 { // PUSH BC
	push_r16_reg(&R_.BC_);
	return 0;
}

auto CPU::opcode0xD5() -> u8 { // PUSH DE
	push_r16_reg(&R_.DE_);
	return 0;
}

auto CPU::opcode0xE5() -> u8 { // PUSH HL
	push_r16_reg(&R_.HL_);
	return 0;
}

auto CPU::opcode0xF5() -> u8 { // PUSH AF
	push_r16_reg(&R_.AF_);
	return 0;
}
