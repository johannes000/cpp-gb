#include "../CPU.hpp"
#include <climits>

auto CPU::test_n_bit_set_z(const u8 bit, const u8 *reg) -> void {
	// wenn bit gesetzt setze Zero Flag
	set_cpu_flag_if(Flag::z, !check_bit(*reg, bit));
	set_flag(Flag::h);
	reset_flag(Flag::n);
}

auto CPU::rot_right_c_r8(u8 *reg) -> void {
	// Rotate register r8 right.
	// [0] ->[7 -> 0]->C
	u8 result = *reg;
	reset_all_flags();
	if ((result & 0x01) != 0) {
		set_flag(Flag::c);
		result >>= 1;
		result |= 0x80;
	} else {
		result >>= 1;
	}
	set_cpu_flag_if(Flag::z, result == 0);
	*reg = result;
}

auto CPU::rot_left_c_r8(u8 *reg) -> void {
	// Rotate register r8 left.
	// C < -[7 < -0] < -[7]
	u8 result = *reg;
	reset_all_flags();
	if ((result & 0x80) != 0) {
		set_flag(Flag::c);
		result <<= 1;
		result |= 0x1;
	} else {
		result <<= 1;
	}
	set_cpu_flag_if(Flag::z, result == 0);
	*reg = result;
}

auto CPU::rot_left_r8(u8 *reg) -> void {
	// Rotate bits in register r8 left through carry.
	// C <-[7 < -0] <-C
	const u8 carry = static_cast<u8>(check_flag(Flag::c));
	set_cpu_flag_if(Flag::c, *reg & 0x80u);

	*reg <<= 1u;
	*reg |= carry;

	set_cpu_flag_if(Flag::z, *reg == 0);
	reset_flags(Flag::n, Flag::h);
}

auto CPU::rot_right_r8(u8 *reg) -> void {
	// Rotate register r8 right through carry.
	// C ->[7 -> 0]->C
	const u8 b7 = *reg & 1u;
	*reg >>= 1u;
	set_bit_if(*reg, 7, check_flag(Flag::c));

	set_cpu_flag_if(Flag::c, b7 == 1);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

auto CPU::shift_left_a_r8(u8 *reg) -> void {
	// Shift Left Arithmetic register r8.
	// C < -[7 < -0] < -0
	const u8 b7 = *reg & 0x80u;
	*reg <<= 1u;
	set_cpu_flag_if(Flag::c, b7 != 0);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

auto CPU::shift_right_a_r8(u8 *reg) -> void {
	// hift Right Arithmetic register r8.
	// [7] ->[7 -> 0]->C
	const u8 b7 = *reg & 0x80u;
	const u8 b0 = *reg & 1u;
	*reg >>= 1u;

	set_bit_if(*reg, 7, b7 == 0x80);
	set_cpu_flag_if(Flag::c, b0 == 1);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

auto CPU::shift_right_l_r8(u8 *reg) -> void {
	// Shift Right Logic register r8.
	// 0 ->[7 -> 0]->C
	const u8 b0 = *reg & 1u;
	*reg >>= 1u;

	set_cpu_flag_if(Flag::c, b0 == 1);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

auto CPU::swap_r8(u8 *reg) -> void {
	// Swap upper 4 bits in register r8 and the lower 4 ones.
	reset_all_flags();
	u8 l_bits = *reg & 0xF0u;
	u8 h_bits = *reg & 0x0Fu;

	*reg = (l_bits >> 4u) | (h_bits << 4u);
	set_cpu_flag_if(Flag::z, *reg == 0);
}

// OPCODES

auto CPU::opcode0x07() -> u8 { // RLCA
	rot_left_c_r8(&R_.A_);
	reset_flag(Flag::z);
	return 0;
}

auto CPU::opcode0x17() -> u8 { // RLA
	rot_left_r8(&R_.A_);
	reset_flag((Flag::z));
	return 0;
}

auto CPU::opcode0x0F() -> u8 { // RRCA
	rot_right_c_r8(&R_.A_);
	reset_flag((Flag::z));
	return 0;
}

auto CPU::opcode0x1F() -> u8 { // RRA
	rot_right_r8(&R_.A_);
	reset_flag(Flag::z);
	return 0;
}

auto CPU::prefixedopcode0x00() -> u8 { // RLC B
	rot_left_c_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x01() -> u8 { // RLC C
	rot_left_c_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x02() -> u8 { // RLC D
	rot_left_c_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x03() -> u8 { // RLC E
	rot_left_c_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x04() -> u8 { // RLC H
	rot_left_c_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x05() -> u8 { // RLC L
	rot_left_c_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x06() -> u8 { // RLC HL
	u8 data = read(R_.HL_);
	rot_left_c_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x07() -> u8 { // RLC A
	rot_left_c_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x08() -> u8 { // RRC B
	rot_right_c_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x09() -> u8 { // RRC C
	rot_right_c_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x0A() -> u8 { // RRC D
	rot_right_c_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x0B() -> u8 { // RRC E
	rot_right_c_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x0C() -> u8 { // RRC H
	rot_right_c_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x0D() -> u8 { // RRC L
	rot_right_c_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x0E() -> u8 { // RRC HL
	u8 data = read(R_.HL_);
	rot_right_c_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x0F() -> u8 { // RRC A
	rot_right_c_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x10() -> u8 { // RL B
	rot_left_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x11() -> u8 { // RL C
	rot_left_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x12() -> u8 { // RL D
	rot_left_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x13() -> u8 { // RL E
	rot_left_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x14() -> u8 { // RL H
	rot_left_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x15() -> u8 { // RL L
	rot_left_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x16() -> u8 { // RL HL
	u8 data = read(R_.HL_);
	rot_left_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x17() -> u8 { // RL A
	rot_left_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x18() -> u8 { // RR B
	rot_right_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x19() -> u8 { // RR C
	rot_right_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x1A() -> u8 { // RR D
	rot_right_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x1B() -> u8 { // RR E
	rot_right_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x1C() -> u8 { // RR H
	rot_right_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x1D() -> u8 { // RR L
	rot_right_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x1E() -> u8 { // RR HL
	u8 data = read(R_.HL_);
	rot_right_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x1F() -> u8 { // RR A
	rot_right_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x20() -> u8 { // SLA B
	shift_left_a_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x21() -> u8 { // SLA C
	shift_left_a_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x22() -> u8 { // SLA D
	shift_left_a_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x23() -> u8 { // SLA E
	shift_left_a_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x24() -> u8 { // SLA H
	shift_left_a_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x25() -> u8 { // SLA L
	shift_left_a_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x26() -> u8 { // SLA HL
	u8 data = read(R_.HL_);
	shift_left_a_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x27() -> u8 { // SLA A
	shift_left_a_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x28() -> u8 { // SRA B
	shift_right_a_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x29() -> u8 { // SRA C
	shift_right_a_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x2A() -> u8 { // SRA D
	shift_right_a_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x2B() -> u8 { // SRA E
	shift_right_a_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x2C() -> u8 { // SRA H
	shift_right_a_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x2D() -> u8 { // SRA L
	shift_right_a_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x2E() -> u8 { // SRA HL
	u8 data = read(R_.HL_);
	shift_right_a_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x2F() -> u8 { // SRA A
	shift_right_a_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x30() -> u8 { // SWAP B
	swap_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x31() -> u8 { // SWAP C
	swap_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x32() -> u8 { // SWAP D
	swap_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x33() -> u8 { // SWAP E
	swap_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x34() -> u8 { // SWAP H
	swap_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x35() -> u8 { // SWAP L
	swap_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x36() -> u8 { // SWAP HL
	u8 data = read(R_.HL_);
	swap_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x37() -> u8 { // SWAP A
	swap_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x38() -> u8 { // SRL B
	shift_right_l_r8(&R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x39() -> u8 { // SRL C
	shift_right_l_r8(&R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x3A() -> u8 { // SRL D
	shift_right_l_r8(&R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x3B() -> u8 { // SRL E
	shift_right_l_r8(&R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x3C() -> u8 { // SRL H
	shift_right_l_r8(&R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x3D() -> u8 { // SRL L
	shift_right_l_r8(&R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x3E() -> u8 { // SRL HL
	u8 data = read(R_.HL_);
	shift_right_l_r8(&data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x3F() -> u8 { // SRL A
	shift_right_l_r8(&R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x40() -> u8 { // BIT 0 B
	test_n_bit_set_z(0, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x41() -> u8 { // BIT 0 C
	test_n_bit_set_z(0, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x42() -> u8 { // BIT 0 D
	test_n_bit_set_z(0, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x43() -> u8 { // BIT 0 E
	test_n_bit_set_z(0, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x44() -> u8 { // BIT 0 H
	test_n_bit_set_z(0, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x45() -> u8 { // BIT 0 L
	test_n_bit_set_z(0, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x46() -> u8 { // BIT 0 (HL)
	u8 data = read(R_.HL_);
	test_n_bit_set_z(0, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x47() -> u8 { // BIT 0 A
	test_n_bit_set_z(0, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x48() -> u8 { // BIT 1 B
	test_n_bit_set_z(1, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x49() -> u8 { // BIT 1 C
	test_n_bit_set_z(1, &R_.C_);
	;
	return 0;
}

auto CPU::prefixedopcode0x4A() -> u8 { // BIT 1 D
	test_n_bit_set_z(1, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x4B() -> u8 { // BIT 1 E
	test_n_bit_set_z(1, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x4C() -> u8 { // BIT 1 H
	test_n_bit_set_z(1, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x4D() -> u8 { // BIT 1 L
	test_n_bit_set_z(1, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x4E() -> u8 { // BIT 1 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(1, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x4F() -> u8 { // BIT 1 A
	test_n_bit_set_z(1, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x50() -> u8 { // BIT 2 B
	test_n_bit_set_z(2, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x51() -> u8 { // BIT 2 C
	test_n_bit_set_z(2, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x52() -> u8 { // BIT 2 D
	test_n_bit_set_z(2, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x53() -> u8 { // BIT 2 E
	test_n_bit_set_z(2, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x54() -> u8 { // BIT 2 H
	test_n_bit_set_z(2, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x55() -> u8 { // BIT 2 L
	test_n_bit_set_z(2, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x56() -> u8 { // BIT 2 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(2, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x57() -> u8 { // BIT 2 A
	test_n_bit_set_z(2, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x58() -> u8 { // BIT 3 B
	test_n_bit_set_z(3, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x59() -> u8 { // BIT 3 C
	test_n_bit_set_z(3, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x5A() -> u8 { // BIT 3 D
	test_n_bit_set_z(3, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x5B() -> u8 { // BIT 3 E
	test_n_bit_set_z(3, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x5C() -> u8 { // BIT 3 H
	test_n_bit_set_z(3, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x5D() -> u8 { // BIT 3 L
	test_n_bit_set_z(3, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x5E() -> u8 { // BIT 3 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(3, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x5F() -> u8 { // BIT 3 A
	test_n_bit_set_z(3, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x60() -> u8 { // BIT 4 B
	test_n_bit_set_z(4, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x61() -> u8 { // BIT 4 C
	test_n_bit_set_z(4, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x62() -> u8 { // BIT 4 D
	test_n_bit_set_z(4, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x63() -> u8 { // BIT 4 E
	test_n_bit_set_z(4, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x64() -> u8 { // BIT 4 H
	test_n_bit_set_z(4, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x65() -> u8 { // BIT 4 L
	test_n_bit_set_z(4, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x66() -> u8 { // BIT 4 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(4, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x67() -> u8 { // BIT 4 A
	test_n_bit_set_z(4, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x68() -> u8 { // BIT 5 B
	test_n_bit_set_z(5, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x69() -> u8 { // BIT 5 C
	test_n_bit_set_z(5, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x6A() -> u8 { // BIT 5 D
	test_n_bit_set_z(5, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x6B() -> u8 { // BIT 5 E
	test_n_bit_set_z(5, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x6C() -> u8 { // BIT 5 H
	test_n_bit_set_z(5, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x6D() -> u8 { // BIT 5 L
	test_n_bit_set_z(5, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x6E() -> u8 { // BIT 5 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(5, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x6F() -> u8 { // BIT 5 A
	test_n_bit_set_z(5, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x70() -> u8 { // BIT 6 B
	test_n_bit_set_z(6, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x71() -> u8 { // BIT 6 C
	test_n_bit_set_z(6, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x72() -> u8 { // BIT 6 D
	test_n_bit_set_z(6, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x73() -> u8 { // BIT 6 E
	test_n_bit_set_z(6, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x74() -> u8 { // BIT 6 H
	test_n_bit_set_z(6, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x75() -> u8 { // BIT 6 L
	test_n_bit_set_z(6, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x76() -> u8 { // BIT 6 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(6, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x77() -> u8 { // BIT 6 A
	test_n_bit_set_z(6, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x78() -> u8 { // BIT 7 B
	test_n_bit_set_z(7, &R_.B_);
	return 0;
}

auto CPU::prefixedopcode0x79() -> u8 { // BIT 7 C
	test_n_bit_set_z(7, &R_.C_);
	return 0;
}

auto CPU::prefixedopcode0x7A() -> u8 { // BIT 7 D
	test_n_bit_set_z(7, &R_.D_);
	return 0;
}

auto CPU::prefixedopcode0x7B() -> u8 { // BIT 7 E
	test_n_bit_set_z(7, &R_.E_);
	return 0;
}

auto CPU::prefixedopcode0x7C() -> u8 { // BIT 7 H
	test_n_bit_set_z(7, &R_.H_);
	return 0;
}

auto CPU::prefixedopcode0x7D() -> u8 { // BIT 7 L
	test_n_bit_set_z(7, &R_.L_);
	return 0;
}

auto CPU::prefixedopcode0x7E() -> u8 { // BIT 7 HL
	u8 data = read(R_.HL_);
	test_n_bit_set_z(7, &data);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x7F() -> u8 { // BIT 7 A
	test_n_bit_set_z(7, &R_.A_);
	return 0;
}

auto CPU::prefixedopcode0x80() -> u8 { // RES 0 B
	reset_bit(R_.B_, 0);
	return 0;
}

auto CPU::prefixedopcode0x81() -> u8 { // RES 0 C
	reset_bit(R_.C_, 0);
	return 0;
}

auto CPU::prefixedopcode0x82() -> u8 { // RES 0 D
	reset_bit(R_.D_, 0);
	return 0;
}

auto CPU::prefixedopcode0x83() -> u8 { // RES 0 E
	reset_bit(R_.E_, 0);
	return 0;
}

auto CPU::prefixedopcode0x84() -> u8 { // RES 0 H
	reset_bit(R_.H_, 0);
	return 0;
}

auto CPU::prefixedopcode0x85() -> u8 { // RES 0 L
	reset_bit(R_.L_, 0);
	return 0;
}

auto CPU::prefixedopcode0x86() -> u8 { // RES 0 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 0);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x87() -> u8 { // RES 0 A
	reset_bit(R_.A_, 0);
	return 0;
}

auto CPU::prefixedopcode0x88() -> u8 { // RES 1 B
	reset_bit(R_.B_, 1);
	return 0;
}

auto CPU::prefixedopcode0x89() -> u8 { // RES 1 C
	reset_bit(R_.C_, 1);
	return 0;
}

auto CPU::prefixedopcode0x8A() -> u8 { // RES 1 D
	reset_bit(R_.D_, 1);
	return 0;
}

auto CPU::prefixedopcode0x8B() -> u8 { // RES 1 E
	reset_bit(R_.E_, 1);
	return 0;
}

auto CPU::prefixedopcode0x8C() -> u8 { // RES 1 H
	reset_bit(R_.H_, 1);
	return 0;
}

auto CPU::prefixedopcode0x8D() -> u8 { // RES 1 L
	reset_bit(R_.L_, 1);
	return 0;
}

auto CPU::prefixedopcode0x8E() -> u8 { // RES 1 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 1);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x8F() -> u8 { // RES 1 A
	reset_bit(R_.A_, 1);
	return 0;
}

auto CPU::prefixedopcode0x90() -> u8 { // RES 2 B
	reset_bit(R_.B_, 2);
	return 0;
}

auto CPU::prefixedopcode0x91() -> u8 { // RES 2 C
	reset_bit(R_.C_, 2);
	return 0;
}

auto CPU::prefixedopcode0x92() -> u8 { // RES 2 D
	reset_bit(R_.D_, 2);
	return 0;
}

auto CPU::prefixedopcode0x93() -> u8 { // RES 2 E
	reset_bit(R_.E_, 2);
	return 0;
}

auto CPU::prefixedopcode0x94() -> u8 { // RES 2 H
	reset_bit(R_.H_, 2);
	return 0;
}

auto CPU::prefixedopcode0x95() -> u8 { // RES 2 L
	reset_bit(R_.L_, 2);
	return 0;
}

auto CPU::prefixedopcode0x96() -> u8 { // RES 2 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 2);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x97() -> u8 { // RES 2 A
	reset_bit(R_.A_, 2);
	return 0;
}

auto CPU::prefixedopcode0x98() -> u8 { // RES 3 B
	reset_bit(R_.B_, 3);
	return 0;
}

auto CPU::prefixedopcode0x99() -> u8 { // RES 3 C
	reset_bit(R_.C_, 3);
	return 0;
}

auto CPU::prefixedopcode0x9A() -> u8 { // RES 3 D
	reset_bit(R_.D_, 3);
	return 0;
}

auto CPU::prefixedopcode0x9B() -> u8 { // RES 3 E
	reset_bit(R_.E_, 3);
	return 0;
}

auto CPU::prefixedopcode0x9C() -> u8 { // RES 3 H
	reset_bit(R_.H_, 3);
	return 0;
}

auto CPU::prefixedopcode0x9D() -> u8 { // RES 3 L
	reset_bit(R_.L_, 3);
	return 0;
}

auto CPU::prefixedopcode0x9E() -> u8 { // RES 3 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 3);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0x9F() -> u8 { // RES 3 A
	reset_bit(R_.A_, 3);
	return 0;
}

auto CPU::prefixedopcode0xA0() -> u8 { // RES 4 B
	reset_bit(R_.B_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA1() -> u8 { // RES 4 C
	reset_bit(R_.C_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA2() -> u8 { // RES 4 D
	reset_bit(R_.D_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA3() -> u8 { // RES 4 E
	reset_bit(R_.E_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA4() -> u8 { // RES 4 H
	reset_bit(R_.H_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA5() -> u8 { // RES 4 L
	reset_bit(R_.L_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA6() -> u8 { // RES 4 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 4);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xA7() -> u8 { // RES 4 A
	reset_bit(R_.A_, 4);
	return 0;
}

auto CPU::prefixedopcode0xA8() -> u8 { // RES 5 B
	reset_bit(R_.B_, 5);
	return 0;
}

auto CPU::prefixedopcode0xA9() -> u8 { // RES 5 C
	reset_bit(R_.C_, 5);
	return 0;
}

auto CPU::prefixedopcode0xAA() -> u8 { // RES 5 D
	reset_bit(R_.D_, 5);
	return 0;
}

auto CPU::prefixedopcode0xAB() -> u8 { // RES 5 E
	reset_bit(R_.E_, 5);
	return 0;
}

auto CPU::prefixedopcode0xAC() -> u8 { // RES 5 H
	reset_bit(R_.H_, 5);
	return 0;
}

auto CPU::prefixedopcode0xAD() -> u8 { // RES 5 L
	reset_bit(R_.L_, 5);
	return 0;
}

auto CPU::prefixedopcode0xAE() -> u8 { // RES 5 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 5);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xAF() -> u8 { // RES 5 A
	reset_bit(R_.A_, 5);
	return 0;
}

auto CPU::prefixedopcode0xB0() -> u8 { // RES 6 B
	reset_bit(R_.B_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB1() -> u8 { // RES 6 C
	reset_bit(R_.C_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB2() -> u8 { // RES 6 D
	reset_bit(R_.D_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB3() -> u8 { // RES 6 E
	reset_bit(R_.E_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB4() -> u8 { // RES 6 H
	reset_bit(R_.H_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB5() -> u8 { // RES 6 L
	reset_bit(R_.L_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB6() -> u8 { // RES 6 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 6);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xB7() -> u8 { // RES 6 A
	reset_bit(R_.A_, 6);
	return 0;
}

auto CPU::prefixedopcode0xB8() -> u8 { // RES 7 B
	reset_bit(R_.B_, 7);
	return 0;
}

auto CPU::prefixedopcode0xB9() -> u8 { // RES 7 C
	reset_bit(R_.C_, 7);
	return 0;
}

auto CPU::prefixedopcode0xBA() -> u8 { // RES 7 D
	reset_bit(R_.D_, 7);
	return 0;
}

auto CPU::prefixedopcode0xBB() -> u8 { // RES 7 E
	reset_bit(R_.E_, 7);
	return 0;
}

auto CPU::prefixedopcode0xBC() -> u8 { // RES 7 H
	reset_bit(R_.H_, 7);
	return 0;
}

auto CPU::prefixedopcode0xBD() -> u8 { // RES 7 L
	reset_bit(R_.L_, 7);
	return 0;
}

auto CPU::prefixedopcode0xBE() -> u8 { // RES 7 HL
	u8 data = read(R_.HL_);
	reset_bit(data, 7);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xBF() -> u8 { // RES 7 A
	reset_bit(R_.A_, 7);
	return 0;
}

auto CPU::prefixedopcode0xC0() -> u8 { // SET 0 B
	set_bit(R_.B_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC1() -> u8 { // SET 0 C
	set_bit(R_.C_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC2() -> u8 { // SET 0 D
	set_bit(R_.D_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC3() -> u8 { // SET 0 E
	set_bit(R_.E_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC4() -> u8 { // SET 0 H
	set_bit(R_.H_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC5() -> u8 { // SET 0 L
	set_bit(R_.L_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC6() -> u8 { // SET 0 HL
	u8 data = read(R_.HL_);
	set_bit(data, 0);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xC7() -> u8 { // SET 0 A
	set_bit(R_.A_, 0);
	return 0;
}

auto CPU::prefixedopcode0xC8() -> u8 { // SET 1 B
	set_bit(R_.B_, 1);
	return 0;
}

auto CPU::prefixedopcode0xC9() -> u8 { // SET 1 C
	set_bit(R_.C_, 1);
	return 0;
}

auto CPU::prefixedopcode0xCA() -> u8 { // SET 1 D
	set_bit(R_.D_, 1);
	return 0;
}

auto CPU::prefixedopcode0xCB() -> u8 { // SET 1 E
	set_bit(R_.E_, 1);
	return 0;
}

auto CPU::prefixedopcode0xCC() -> u8 { // SET 1 H
	set_bit(R_.H_, 1);
	return 0;
}

auto CPU::prefixedopcode0xCD() -> u8 { // SET 1 L
	set_bit(R_.L_, 1);
	return 0;
}

auto CPU::prefixedopcode0xCE() -> u8 { // SET 1 HL
	u8 data = read(R_.HL_);
	set_bit(data, 1);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xCF() -> u8 { // SET 1 A
	set_bit(R_.A_, 1);
	return 0;
}

auto CPU::prefixedopcode0xD0() -> u8 { // SET 2 B
	set_bit(R_.B_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD1() -> u8 { // SET 2 C
	set_bit(R_.C_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD2() -> u8 { // SET 2 D
	set_bit(R_.D_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD3() -> u8 { // SET 2 E
	set_bit(R_.E_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD4() -> u8 { // SET 2 H
	set_bit(R_.H_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD5() -> u8 { // SET 2 L
	set_bit(R_.L_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD6() -> u8 { // SET 2 HL
	u8 data = read(R_.HL_);
	set_bit(data, 2);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xD7() -> u8 { // SET 2 A
	set_bit(R_.A_, 2);
	return 0;
}

auto CPU::prefixedopcode0xD8() -> u8 { // SET 3 B
	set_bit(R_.B_, 3);
	return 0;
}

auto CPU::prefixedopcode0xD9() -> u8 { // SET 3 C
	set_bit(R_.C_, 3);
	return 0;
}

auto CPU::prefixedopcode0xDA() -> u8 { // SET 3 D
	set_bit(R_.D_, 3);
	return 0;
}

auto CPU::prefixedopcode0xDB() -> u8 { // SET 3 E
	set_bit(R_.E_, 3);
	return 0;
}

auto CPU::prefixedopcode0xDC() -> u8 { // SET 3 H
	set_bit(R_.H_, 3);
	return 0;
}

auto CPU::prefixedopcode0xDD() -> u8 { // SET 3 L
	set_bit(R_.L_, 3);

	return 0;
}

auto CPU::prefixedopcode0xDE() -> u8 { // SET 3 HL
	u8 data = read(R_.HL_);
	set_bit(data, 3);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xDF() -> u8 { // SET 3 A
	set_bit(R_.A_, 3);
	return 0;
}

auto CPU::prefixedopcode0xE0() -> u8 { // SET 4 B
	set_bit(R_.B_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE1() -> u8 { // SET 4 C
	set_bit(R_.C_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE2() -> u8 { // SET 4 D
	set_bit(R_.D_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE3() -> u8 { // SET 4 E
	set_bit(R_.E_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE4() -> u8 { // SET 4 H
	set_bit(R_.H_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE5() -> u8 { // SET 4 L
	set_bit(R_.L_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE6() -> u8 { // SET 4 HL
	u8 data = read(R_.HL_);
	set_bit(data, 4);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xE7() -> u8 { // SET 4 A
	set_bit(R_.A_, 4);
	return 0;
}

auto CPU::prefixedopcode0xE8() -> u8 { // SET 5 B
	set_bit(R_.B_, 5);
	return 0;
}

auto CPU::prefixedopcode0xE9() -> u8 { // SET 5 C
	set_bit(R_.C_, 5);
	return 0;
}

auto CPU::prefixedopcode0xEA() -> u8 { // SET 5 D
	set_bit(R_.D_, 5);
	return 0;
}

auto CPU::prefixedopcode0xEB() -> u8 { // SET 5 E
	set_bit(R_.E_, 5);
	return 0;
}

auto CPU::prefixedopcode0xEC() -> u8 { // SET 5 H
	set_bit(R_.H_, 5);
	return 0;
}

auto CPU::prefixedopcode0xED() -> u8 { // SET 5 L
	set_bit(R_.L_, 5);
	return 0;
}

auto CPU::prefixedopcode0xEE() -> u8 { // SET 5 HL
	u8 data = read(R_.HL_);
	set_bit(data, 5);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xEF() -> u8 { // SET 5 A
	set_bit(R_.A_, 5);
	return 0;
}

auto CPU::prefixedopcode0xF0() -> u8 { // SET 6 B
	set_bit(R_.B_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF1() -> u8 { // SET 6 C
	set_bit(R_.C_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF2() -> u8 { // SET 6 D
	set_bit(R_.D_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF3() -> u8 { // SET 6 E
	set_bit(R_.E_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF4() -> u8 { // SET 6 H
	set_bit(R_.H_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF5() -> u8 { // SET 6 L
	set_bit(R_.L_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF6() -> u8 { // SET 6 HL
	u8 data = read(R_.HL_);
	set_bit(data, 6);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xF7() -> u8 { // SET 6 A
	set_bit(R_.A_, 6);
	return 0;
}

auto CPU::prefixedopcode0xF8() -> u8 { // SET 7 B
	set_bit(R_.B_, 7);
	return 0;
}

auto CPU::prefixedopcode0xF9() -> u8 { // SET 7 C
	set_bit(R_.C_, 7);
	return 0;
}

auto CPU::prefixedopcode0xFA() -> u8 { // SET 7 D
	set_bit(R_.D_, 7);
	return 0;
}

auto CPU::prefixedopcode0xFB() -> u8 { // SET 7 E
	set_bit(R_.E_, 7);
	return 0;
}

auto CPU::prefixedopcode0xFC() -> u8 { // SET 7 H
	set_bit(R_.H_, 7);
	return 0;
}

auto CPU::prefixedopcode0xFD() -> u8 { // SET 7 L
	set_bit(R_.L_, 7);
	return 0;
}

auto CPU::prefixedopcode0xFE() -> u8 { // SET 7 HL
	u8 data = read(R_.HL_);
	set_bit(data, 7);
	write(R_.HL_, data);
	return 0;
}

auto CPU::prefixedopcode0xFF() -> u8 { // SET 7 A
	set_bit(R_.A_, 7);
	return 0;
}
