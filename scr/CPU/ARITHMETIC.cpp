#include "../CPU.hpp"

auto CPU::xor_a_r8(const u8 *reg) -> void {
	// XOR data 1 und data 2 und speicher das ergebnis in data 1
	R_.A_ ^= *reg;
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
	reset_flags(Flag::n, Flag::h, Flag::c);
}

auto CPU::and_a_r8(const u8 *reg) -> void {
	// AND A und reg 2 und speicher das ergebnis in A
	R_.A_ &= *reg;
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
	reset_flags(Flag::n, Flag::c);
	set_flag(Flag::h);
}

auto CPU::or_r8_r8(const u8 &reg) -> void {
	// OR reg 1 und reg 2 und Speicher das Ergebnis in data 1
	reset_all_flags();
	R_.A_ |= reg;
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
}

auto CPU::cp_a_r8(const u8 *reg) -> void {
	// Subtract the value in data2 from data1 and set flags accordingly, but don't store the result. This is useful for ComParing values.
	set_flag(Flag::n);
	set_cpu_flag_if(Flag::h, ((R_.A_ & 0xfu) < (*reg & 0xfu)));
	set_cpu_flag_if(Flag::c, R_.A_ < *reg);
	set_cpu_flag_if(Flag::z, R_.A_ == *reg);
}

auto CPU::inc_u8_reg(u8 *reg) -> void {
	//  Increment register.
	u8 result = *reg + 1;

	reset_flag(Flag::n);

	set_cpu_flag_if(Flag::h, ((*reg & 0xFu) + 0x1 > 0xF));
	set_cpu_flag_if(Flag::z, result == 0);
	*reg = result;
}

auto CPU::dec_u8_reg(u8 *reg) -> void {
	// Decrement register
	u8 result = *reg - 1;

	set_flag(Flag::n);
	set_cpu_flag_if(Flag::h, (static_cast<s8>((*reg & 0xFu) - 0x1) < 0));
	set_cpu_flag_if(Flag::z, result == 0);
	*reg = result;
}

auto CPU::add_a_r8(const u8 *value) -> void {
	const unsigned int result = R_.A_ + *value;
	reset_flag(Flag::n);
	set_cpu_flag_if(Flag::c, ((R_.A_ ^ *value ^ result) & 0x100u) != 0);
	set_cpu_flag_if(Flag::h, ((R_.A_ ^ *value ^ result) & 0x10u) != 0);
	R_.A_ = static_cast<u8>(result);
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
}

auto CPU::adc_a_r8(const u8 *value) -> void {
	// Add n + Carry flag to A
	u8 c;
	check_flag(Flag::c) ? c = 1 : c = 0;
	int result = R_.A_ + *value + c;
	reset_flag(Flag::n);
	set_cpu_flag_if(Flag::c, result > 0xFF);
	set_cpu_flag_if(Flag::h, ((R_.A_ & 0x0Fu) + (*value & 0x0Fu) + c) > 0x0F);
	R_.A_ = static_cast<u8>(result);
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
}

auto CPU::add_r16_r16(u16 *reg, const u16 *value) -> void {
	const unsigned int result = *reg + *value;
	reset_flag(Flag::n);
	set_cpu_flag_if(Flag::h, (*reg & 0xfffu) + (*value & 0xfffu) > 0xfffu);
	set_cpu_flag_if(Flag::c, (result & 0x10000u) != 0);
	*reg = result;
}

auto CPU::sub_a_r8(const u8 *value) -> void {

	set_flag(Flag::n);
	set_cpu_flag_if(Flag::h, (((R_.A_ & 0x0Fu) < (*value & 0x0Fu))));
	set_cpu_flag_if(Flag::c, (R_.A_ & 0xFFu) < (*value & 0xFFu));

	R_.A_ -= *value;
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
}

auto CPU::sbc_a_r8(const u8 *value) -> void {
	u8 c;
	check_flag(Flag::c) ? c = 1 : c = 0;
	int result = R_.A_ - *value - c;
	set_flag(Flag::n);
	set_cpu_flag_if(Flag::c, result < 0);
	set_cpu_flag_if(Flag::h, ((R_.A_ & 0x0F) - (*value & 0x0F) - c) < 0);
	R_.A_ = static_cast<u8>(result);
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
}

auto CPU::opcode0x3C() -> u8 { // INC A
	inc_u8_reg(&R_.A_);
	return 0;
}

auto CPU::opcode0x04() -> u8 { // INC B
	inc_u8_reg(&R_.B_);
	return 0;
}

auto CPU::opcode0x0C() -> u8 { // INC C
	inc_u8_reg(&R_.C_);
	return 0;
}

auto CPU::opcode0x14() -> u8 { // INC D
	inc_u8_reg(&R_.D_);
	return 0;
}

auto CPU::opcode0x1C() -> u8 { // INC E
	inc_u8_reg(&R_.E_);
	return 0;
}

auto CPU::opcode0x24() -> u8 { // INC H
	inc_u8_reg(&R_.H_);
	return 0;
}

auto CPU::opcode0x2C() -> u8 { // INC L
	inc_u8_reg(&R_.L_);
	return 0;
}

auto CPU::opcode0x34() -> u8 { // INC (HL)
	u8 data = read(R_.HL_);
	inc_u8_reg(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::opcode0x13() -> u8 { // INC DE
	R_.DE_++;
	return 0;
}

auto CPU::opcode0x03() -> u8 { // INC BC
	R_.BC_++;
	return 0;
}

auto CPU::opcode0x23() -> u8 { // INC HL
	R_.HL_++;
	return 0;
}

auto CPU::opcode0x33() -> u8 { // INC SP
	R_.SP_++;
	return 0;
}

auto CPU::opcode0x05() -> u8 { // DEC B
	dec_u8_reg(&R_.B_);
	return 0;
}

auto CPU::opcode0x0D() -> u8 { // DEC C
	dec_u8_reg(&R_.C_);
	return 0;
}

auto CPU::opcode0x15() -> u8 { // DEC D
	dec_u8_reg(&R_.D_);
	return 0;
}

auto CPU::opcode0x1D() -> u8 { // DEC E
	dec_u8_reg(&R_.E_);
	return 0;
}

auto CPU::opcode0x25() -> u8 { // DEC H
	dec_u8_reg(&R_.H_);
	return 0;
}

auto CPU::opcode0x3D() -> u8 { // DEC A
	dec_u8_reg(&R_.A_);
	return 0;
}

auto CPU::opcode0x2D() -> u8 { // DEC L
	dec_u8_reg(&R_.L_);
	return 0;
}

auto CPU::opcode0x35() -> u8 { // DEC (HL)
	u8 data = read(R_.HL_);
	dec_u8_reg(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::opcode0x0B() -> u8 { // DEC BC
	R_.BC_--;
	return 0;
}

auto CPU::opcode0x1B() -> u8 { // DEC DE
	R_.DE_--;
	return 0;
}

auto CPU::opcode0x2B() -> u8 { // DEC HL
	R_.HL_--;
	return 0;
}

auto CPU::opcode0x3B() -> u8 { // DEC SP
	R_.SP_--;
	return 0;
}

auto CPU::opcode0x80() -> u8 { // ADD A B
	add_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0x81() -> u8 { // ADD A C
	add_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0x82() -> u8 { // ADD A D
	add_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0x83() -> u8 { // ADD A E
	add_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0x84() -> u8 { // ADD A H
	add_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0x85() -> u8 { // ADD A L
	add_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0x86() -> u8 { // ADD A HL
	u8 data = read(R_.HL_);
	add_a_r8(&data);
	return 0;
}

auto CPU::opcode0x87() -> u8 { // ADD A A
	add_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0x09() -> u8 { // ADD HL BC
	add_r16_r16(&R_.HL_, &R_.BC_);
	return 0;
}

auto CPU::opcode0x19() -> u8 { // ADD HL DE
	add_r16_r16(&R_.HL_, &R_.DE_);
	return 0;
}

auto CPU::opcode0x29() -> u8 { // ADD HL HL
	add_r16_r16(&R_.HL_, &R_.HL_);
	return 0;
}

auto CPU::opcode0x39() -> u8 { // ADD HL SP
	add_r16_r16(&R_.HL_, &R_.SP_);
	return 0;
}

auto CPU::opcode0xC6() -> u8 { // ADD A d8
	u8 data = read_u8_at_pc();
	add_a_r8(&data);
	return 0;
}

auto CPU::opcode0xE8() -> u8 { // ADD SP r8
	int data = static_cast<int>(read_i8_at_pc());
	int result = R_.SP_ + data;
	reset_all_flags();
	// set_flag_if(Flag::c, (((R.SP ^ data ^ (result & 0xFFFFu)) & 0x100u) == 0x100u));
	// set_flag_if(Flag::h, (((R.SP ^ data ^ (result & 0xFFFFu)) & 0x10u) == 0x10u));
	set_cpu_flag_if(Flag::c, ((result & 0xFF) < (R_.SP_ & 0xFF)));
	set_cpu_flag_if(Flag::h, ((result & 0xF) < (R_.SP_ & 0xF)));
	R_.SP_ = static_cast<u16>(result);
	return 0;
}

auto CPU::opcode0x90() -> u8 { // SUB B
	sub_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0x91() -> u8 { // SUB C
	sub_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0x92() -> u8 { // SUB D
	sub_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0x93() -> u8 { // SUB E
	sub_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0x94() -> u8 { // SUB H
	sub_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0x95() -> u8 { // SUB L
	sub_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0x96() -> u8 { // SUB HL
	u8 data = read(R_.HL_);
	sub_a_r8(&data);
	return 0;
}

auto CPU::opcode0x97() -> u8 { // SUB A
	sub_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0xD6() -> u8 { // SUB d8
	u8 data = read_u8_at_pc();
	sub_a_r8(&data);
	return 0;
}

auto CPU::opcode0xE6() -> u8 { // AND d8
	u8 data = read_u8_at_pc();
	and_a_r8(&data);
	return 0;
}

auto CPU::opcode0xA0() -> u8 { // AND B
	and_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0xA1() -> u8 { // AND C
	and_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0xA2() -> u8 { // AND D
	and_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0xA3() -> u8 { // AND E
	and_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0xA4() -> u8 { // AND H
	and_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0xA5() -> u8 { // AND L
	and_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0xA6() -> u8 { // AND HL
	u8 data = read(R_.HL_);
	and_a_r8(&data);
	return 0;
}

auto CPU::opcode0xA7() -> u8 { // AND A
	and_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0xB0() -> u8 { // OR B
	or_r8_r8(R_.B_);
	return 0;
}

auto CPU::opcode0xB1() -> u8 { // OR C
	or_r8_r8(R_.C_);
	return 0;
}

auto CPU::opcode0xB2() -> u8 { // OR D
	or_r8_r8(R_.D_);
	return 0;
}

auto CPU::opcode0xB3() -> u8 { // OR E
	or_r8_r8(R_.E_);
	return 0;
}

auto CPU::opcode0xB4() -> u8 { // OR H
	or_r8_r8(R_.H_);
	return 0;
}

auto CPU::opcode0xB5() -> u8 { // OR L
	or_r8_r8(R_.L_);
	return 0;
}

auto CPU::opcode0xB6() -> u8 { // OR HL
	or_r8_r8(read(R_.HL_));
	return 0;
}

auto CPU::opcode0xB7() -> u8 { // OR A
	or_r8_r8(R_.A_);
	return 0;
}

auto CPU::opcode0xF6() -> u8 { // OR d8
	or_r8_r8(read_u8_at_pc());
	return 0;
}

auto CPU::opcode0x98() -> u8 { // SBC A B
	sbc_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0x99() -> u8 { // SBC A C
	sbc_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0x9A() -> u8 { // SBC A D
	sbc_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0x9B() -> u8 { // SBC A E
	sbc_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0x9C() -> u8 { // SBC A H
	sbc_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0x9D() -> u8 { // SBC A L
	sbc_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0x9E() -> u8 { // SBC A HL
	u8 data = read(R_.HL_);
	sbc_a_r8(&data);
	return 0;
}

auto CPU::opcode0x9F() -> u8 { // SBC A A
	sbc_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0xDE() -> u8 { // SBC A d8
	u8 data = read_u8_at_pc();
	sbc_a_r8(&data);
	return 0;
}

auto CPU::opcode0xEE() -> u8 { // XOR d8
	u8 data = read_u8_at_pc();
	xor_a_r8(&data);
	return 0;
}

auto CPU::opcode0xA8() -> u8 { // XOR B
	xor_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0xA9() -> u8 { // XOR C
	xor_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0xAA() -> u8 { // XOR D
	xor_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0xAB() -> u8 { // XOR E
	xor_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0xAC() -> u8 { // XOR H
	xor_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0xAD() -> u8 { // XOR L
	xor_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0xAE() -> u8 { // XOR HL
	u8 data = read(R_.HL_);
	xor_a_r8(&data);
	return 0;
}

auto CPU::opcode0xAF() -> u8 { // XOR A
	xor_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0xB8() -> u8 { // CP B
	cp_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0xB9() -> u8 { // CP C
	cp_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0xBA() -> u8 { // CP D
	cp_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0xBB() -> u8 { // CP E
	cp_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0xBC() -> u8 { // CP H
	cp_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0xBD() -> u8 { // CP L
	cp_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0xBE() -> u8 { // CP HL
	u8 data = read(R_.HL_);
	cp_a_r8(&data);
	return 0;
}

auto CPU::opcode0xBF() -> u8 { // CP A
	cp_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0xFE() -> u8 { // CP d8
	auto data = read_u8_at_pc();
	cp_a_r8(&data);
	return 0;
}

auto CPU::opcode0x88() -> u8 { // ADC A B
	adc_a_r8(&R_.B_);
	return 0;
}

auto CPU::opcode0x89() -> u8 { // ADC A C
	adc_a_r8(&R_.C_);
	return 0;
}

auto CPU::opcode0x8A() -> u8 { // ADC A D
	adc_a_r8(&R_.D_);
	return 0;
}

auto CPU::opcode0x8B() -> u8 { // ADC A E
	adc_a_r8(&R_.E_);
	return 0;
}

auto CPU::opcode0x8C() -> u8 { // ADC A H
	adc_a_r8(&R_.H_);
	return 0;
}

auto CPU::opcode0x8D() -> u8 { // ADC A L
	adc_a_r8(&R_.L_);
	return 0;
}

auto CPU::opcode0x8E() -> u8 { // ADC A HL
	u8 data = read(R_.HL_);
	adc_a_r8(&data);
	return 0;
}

auto CPU::opcode0x8F() -> u8 { // ADC A A
	adc_a_r8(&R_.A_);
	return 0;
}

auto CPU::opcode0xCE() -> u8 { // ADC A d8
	u8 data = read_u8_at_pc();
	adc_a_r8(&data);
	return 0;
}
