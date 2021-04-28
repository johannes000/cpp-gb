#include "../CPU.h"
#include <climits>

void CPU::test_n_bit_set_z(const u8 bit, const u8 *reg) {
	// wenn bit gesetzt setze Zero Flag
	set_cpu_flag_if(Flag::z, !check_bit(*reg, bit));
	set_flag(Flag::h);
	reset_flag(Flag::n);
}

void CPU::rot_right_c_r8(u8 *reg) {
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

void CPU::rot_left_c_r8(u8 *reg) {
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

void CPU::rot_left_r8(u8 *reg) {
	// Rotate bits in register r8 left through carry.
	// C <-[7 < -0] <-C
	const u8 carry = static_cast<u8>(check_flag(Flag::c));
	set_cpu_flag_if(Flag::c, *reg & 0x80u);

	*reg <<= 1u;
	*reg |= carry;

	set_cpu_flag_if(Flag::z, *reg == 0);
	reset_flags(Flag::n, Flag::h);
}

void CPU::rot_right_r8(u8 *reg) {
	// Rotate register r8 right through carry.
	// C ->[7 -> 0]->C
	const u8 b7 = *reg & 1u;
	*reg >>= 1u;
	set_bit_if(*reg, 7, check_flag(Flag::c));

	set_cpu_flag_if(Flag::c, b7 == 1);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

void CPU::shift_left_a_r8(u8 *reg) {
	// Shift Left Arithmetic register r8.
	// C < -[7 < -0] < -0
	const u8 b7 = *reg & 0x80u;
	*reg <<= 1u;
	set_cpu_flag_if(Flag::c, b7 != 0);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

void CPU::shift_right_a_r8(u8 *reg) {
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

void CPU::shift_right_l_r8(u8 *reg) {
	// Shift Right Logic register r8.
	// 0 ->[7 -> 0]->C
	const u8 b0 = *reg & 1u;
	*reg >>= 1u;

	set_cpu_flag_if(Flag::c, b0 == 1);
	set_cpu_flag_if(Flag::z, *reg == 0);

	reset_flags(Flag::n, Flag::h);
}

void CPU::swap_r8(u8 *reg) {
	// Swap upper 4 bits in register r8 and the lower 4 ones.
	reset_all_flags();
	u8 l_bits = *reg & 0xF0u;
	u8 h_bits = *reg & 0x0Fu;

	*reg = (l_bits >> 4u) | (h_bits << 4u);
	set_cpu_flag_if(Flag::z, *reg == 0);
}

// OPCODES

u8 CPU::opcode0x07() { // RLCA
	rot_left_c_r8(&R.A);
	reset_flag(Flag::z);
	return 0;
}

u8 CPU::opcode0x17() { // RLA
	rot_left_r8(&R.A);
	reset_flag((Flag::z));
	return 0;
}

u8 CPU::opcode0x0F() { // RRCA
	rot_right_c_r8(&R.A);
	reset_flag((Flag::z));
	return 0;
}

u8 CPU::opcode0x1F() { // RRA
	rot_right_r8(&R.A);
	reset_flag(Flag::z);
	return 0;
}

u8 CPU::prefixedopcode0x00() { // RLC B
	rot_left_c_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x01() { // RLC C
	rot_left_c_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x02() { // RLC D
	rot_left_c_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x03() { // RLC E
	rot_left_c_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x04() { // RLC H
	rot_left_c_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x05() { // RLC L
	rot_left_c_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x06() { // RLC HL
	u8 data = read(R.HL);
	rot_left_c_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x07() { // RLC A
	rot_left_c_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x08() { // RRC B
	rot_right_c_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x09() { // RRC C
	rot_right_c_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x0A() { // RRC D
	rot_right_c_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x0B() { // RRC E
	rot_right_c_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x0C() { // RRC H
	rot_right_c_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x0D() { // RRC L
	rot_right_c_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x0E() { // RRC HL
	u8 data = read(R.HL);
	rot_right_c_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x0F() { // RRC A
	rot_right_c_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x10() { // RL B
	rot_left_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x11() { // RL C
	rot_left_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x12() { // RL D
	rot_left_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x13() { // RL E
	rot_left_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x14() { // RL H
	rot_left_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x15() { // RL L
	rot_left_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x16() { // RL HL
	u8 data = read(R.HL);
	rot_left_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x17() { // RL A
	rot_left_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x18() { // RR B
	rot_right_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x19() { // RR C
	rot_right_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x1A() { // RR D
	rot_right_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x1B() { // RR E
	rot_right_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x1C() { // RR H
	rot_right_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x1D() { // RR L
	rot_right_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x1E() { // RR HL
	u8 data = read(R.HL);
	rot_right_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x1F() { // RR A
	rot_right_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x20() { // SLA B
	shift_left_a_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x21() { // SLA C
	shift_left_a_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x22() { // SLA D
	shift_left_a_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x23() { // SLA E
	shift_left_a_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x24() { // SLA H
	shift_left_a_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x25() { // SLA L
	shift_left_a_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x26() { // SLA HL
	u8 data = read(R.HL);
	shift_left_a_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x27() { // SLA A
	shift_left_a_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x28() { // SRA B
	shift_right_a_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x29() { // SRA C
	shift_right_a_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x2A() { // SRA D
	shift_right_a_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x2B() { // SRA E
	shift_right_a_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x2C() { // SRA H
	shift_right_a_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x2D() { // SRA L
	shift_right_a_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x2E() { // SRA HL
	u8 data = read(R.HL);
	shift_right_a_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x2F() { // SRA A
	shift_right_a_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x30() { // SWAP B
	swap_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x31() { // SWAP C
	swap_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x32() { // SWAP D
	swap_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x33() { // SWAP E
	swap_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x34() { // SWAP H
	swap_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x35() { // SWAP L
	swap_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x36() { // SWAP HL
	u8 data = read(R.HL);
	swap_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x37() { // SWAP A
	swap_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x38() { // SRL B
	shift_right_l_r8(&R.B);
	return 0;
}

u8 CPU::prefixedopcode0x39() { // SRL C
	shift_right_l_r8(&R.C);
	return 0;
}

u8 CPU::prefixedopcode0x3A() { // SRL D
	shift_right_l_r8(&R.D);
	return 0;
}

u8 CPU::prefixedopcode0x3B() { // SRL E
	shift_right_l_r8(&R.E);
	return 0;
}

u8 CPU::prefixedopcode0x3C() { // SRL H
	shift_right_l_r8(&R.H);
	return 0;
}

u8 CPU::prefixedopcode0x3D() { // SRL L
	shift_right_l_r8(&R.L);
	return 0;
}

u8 CPU::prefixedopcode0x3E() { // SRL HL
	u8 data = read(R.HL);
	shift_right_l_r8(&data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x3F() { // SRL A
	shift_right_l_r8(&R.A);
	return 0;
}

u8 CPU::prefixedopcode0x40() { // BIT 0 B
	test_n_bit_set_z(0, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x41() { // BIT 0 C
	test_n_bit_set_z(0, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x42() { // BIT 0 D
	test_n_bit_set_z(0, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x43() { // BIT 0 E
	test_n_bit_set_z(0, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x44() { // BIT 0 H
	test_n_bit_set_z(0, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x45() { // BIT 0 L
	test_n_bit_set_z(0, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x46() { // BIT 0 (HL)
	u8 data = read(R.HL);
	test_n_bit_set_z(0, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x47() { // BIT 0 A
	test_n_bit_set_z(0, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x48() { // BIT 1 B
	test_n_bit_set_z(1, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x49() { // BIT 1 C
	test_n_bit_set_z(1, &R.C);
	;
	return 0;
}

u8 CPU::prefixedopcode0x4A() { // BIT 1 D
	test_n_bit_set_z(1, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x4B() { // BIT 1 E
	test_n_bit_set_z(1, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x4C() { // BIT 1 H
	test_n_bit_set_z(1, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x4D() { // BIT 1 L
	test_n_bit_set_z(1, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x4E() { // BIT 1 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(1, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x4F() { // BIT 1 A
	test_n_bit_set_z(1, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x50() { // BIT 2 B
	test_n_bit_set_z(2, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x51() { // BIT 2 C
	test_n_bit_set_z(2, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x52() { // BIT 2 D
	test_n_bit_set_z(2, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x53() { // BIT 2 E
	test_n_bit_set_z(2, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x54() { // BIT 2 H
	test_n_bit_set_z(2, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x55() { // BIT 2 L
	test_n_bit_set_z(2, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x56() { // BIT 2 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(2, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x57() { // BIT 2 A
	test_n_bit_set_z(2, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x58() { // BIT 3 B
	test_n_bit_set_z(3, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x59() { // BIT 3 C
	test_n_bit_set_z(3, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x5A() { // BIT 3 D
	test_n_bit_set_z(3, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x5B() { // BIT 3 E
	test_n_bit_set_z(3, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x5C() { // BIT 3 H
	test_n_bit_set_z(3, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x5D() { // BIT 3 L
	test_n_bit_set_z(3, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x5E() { // BIT 3 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(3, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x5F() { // BIT 3 A
	test_n_bit_set_z(3, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x60() { // BIT 4 B
	test_n_bit_set_z(4, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x61() { // BIT 4 C
	test_n_bit_set_z(4, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x62() { // BIT 4 D
	test_n_bit_set_z(4, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x63() { // BIT 4 E
	test_n_bit_set_z(4, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x64() { // BIT 4 H
	test_n_bit_set_z(4, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x65() { // BIT 4 L
	test_n_bit_set_z(4, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x66() { // BIT 4 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(4, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x67() { // BIT 4 A
	test_n_bit_set_z(4, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x68() { // BIT 5 B
	test_n_bit_set_z(5, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x69() { // BIT 5 C
	test_n_bit_set_z(5, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x6A() { // BIT 5 D
	test_n_bit_set_z(5, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x6B() { // BIT 5 E
	test_n_bit_set_z(5, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x6C() { // BIT 5 H
	test_n_bit_set_z(5, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x6D() { // BIT 5 L
	test_n_bit_set_z(5, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x6E() { // BIT 5 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(5, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x6F() { // BIT 5 A
	test_n_bit_set_z(5, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x70() { // BIT 6 B
	test_n_bit_set_z(6, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x71() { // BIT 6 C
	test_n_bit_set_z(6, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x72() { // BIT 6 D
	test_n_bit_set_z(6, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x73() { // BIT 6 E
	test_n_bit_set_z(6, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x74() { // BIT 6 H
	test_n_bit_set_z(6, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x75() { // BIT 6 L
	test_n_bit_set_z(6, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x76() { // BIT 6 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(6, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x77() { // BIT 6 A
	test_n_bit_set_z(6, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x78() { // BIT 7 B
	test_n_bit_set_z(7, &R.B);
	return 0;
}

u8 CPU::prefixedopcode0x79() { // BIT 7 C
	test_n_bit_set_z(7, &R.C);
	return 0;
}

u8 CPU::prefixedopcode0x7A() { // BIT 7 D
	test_n_bit_set_z(7, &R.D);
	return 0;
}

u8 CPU::prefixedopcode0x7B() { // BIT 7 E
	test_n_bit_set_z(7, &R.E);
	return 0;
}

u8 CPU::prefixedopcode0x7C() { // BIT 7 H
	test_n_bit_set_z(7, &R.H);
	return 0;
}

u8 CPU::prefixedopcode0x7D() { // BIT 7 L
	test_n_bit_set_z(7, &R.L);
	return 0;
}

u8 CPU::prefixedopcode0x7E() { // BIT 7 HL
	u8 data = read(R.HL);
	test_n_bit_set_z(7, &data);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x7F() { // BIT 7 A
	test_n_bit_set_z(7, &R.A);
	return 0;
}

u8 CPU::prefixedopcode0x80() { // RES 0 B
	reset_bit(R.B, 0);
	return 0;
}

u8 CPU::prefixedopcode0x81() { // RES 0 C
	reset_bit(R.C, 0);
	return 0;
}

u8 CPU::prefixedopcode0x82() { // RES 0 D
	reset_bit(R.D, 0);
	return 0;
}

u8 CPU::prefixedopcode0x83() { // RES 0 E
	reset_bit(R.E, 0);
	return 0;
}

u8 CPU::prefixedopcode0x84() { // RES 0 H
	reset_bit(R.H, 0);
	return 0;
}

u8 CPU::prefixedopcode0x85() { // RES 0 L
	reset_bit(R.L, 0);
	return 0;
}

u8 CPU::prefixedopcode0x86() { // RES 0 HL
	u8 data = read(R.HL);
	reset_bit(data, 0);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x87() { // RES 0 A
	reset_bit(R.A, 0);
	return 0;
}

u8 CPU::prefixedopcode0x88() { // RES 1 B
	reset_bit(R.B, 1);
	return 0;
}

u8 CPU::prefixedopcode0x89() { // RES 1 C
	reset_bit(R.C, 1);
	return 0;
}

u8 CPU::prefixedopcode0x8A() { // RES 1 D
	reset_bit(R.D, 1);
	return 0;
}

u8 CPU::prefixedopcode0x8B() { // RES 1 E
	reset_bit(R.E, 1);
	return 0;
}

u8 CPU::prefixedopcode0x8C() { // RES 1 H
	reset_bit(R.H, 1);
	return 0;
}

u8 CPU::prefixedopcode0x8D() { // RES 1 L
	reset_bit(R.L, 1);
	return 0;
}

u8 CPU::prefixedopcode0x8E() { // RES 1 HL
	u8 data = read(R.HL);
	reset_bit(data, 1);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x8F() { // RES 1 A
	reset_bit(R.A, 1);
	return 0;
}

u8 CPU::prefixedopcode0x90() { // RES 2 B
	reset_bit(R.B, 2);
	return 0;
}

u8 CPU::prefixedopcode0x91() { // RES 2 C
	reset_bit(R.C, 2);
	return 0;
}

u8 CPU::prefixedopcode0x92() { // RES 2 D
	reset_bit(R.D, 2);
	return 0;
}

u8 CPU::prefixedopcode0x93() { // RES 2 E
	reset_bit(R.E, 2);
	return 0;
}

u8 CPU::prefixedopcode0x94() { // RES 2 H
	reset_bit(R.H, 2);
	return 0;
}

u8 CPU::prefixedopcode0x95() { // RES 2 L
	reset_bit(R.L, 2);
	return 0;
}

u8 CPU::prefixedopcode0x96() { // RES 2 HL
	u8 data = read(R.HL);
	reset_bit(data, 2);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x97() { // RES 2 A
	reset_bit(R.A, 2);
	return 0;
}

u8 CPU::prefixedopcode0x98() { // RES 3 B
	reset_bit(R.B, 3);
	return 0;
}

u8 CPU::prefixedopcode0x99() { // RES 3 C
	reset_bit(R.C, 3);
	return 0;
}

u8 CPU::prefixedopcode0x9A() { // RES 3 D
	reset_bit(R.D, 3);
	return 0;
}

u8 CPU::prefixedopcode0x9B() { // RES 3 E
	reset_bit(R.E, 3);
	return 0;
}

u8 CPU::prefixedopcode0x9C() { // RES 3 H
	reset_bit(R.H, 3);
	return 0;
}

u8 CPU::prefixedopcode0x9D() { // RES 3 L
	reset_bit(R.L, 3);
	return 0;
}

u8 CPU::prefixedopcode0x9E() { // RES 3 HL
	u8 data = read(R.HL);
	reset_bit(data, 3);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0x9F() { // RES 3 A
	reset_bit(R.A, 3);
	return 0;
}

u8 CPU::prefixedopcode0xA0() { // RES 4 B
	reset_bit(R.B, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA1() { // RES 4 C
	reset_bit(R.C, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA2() { // RES 4 D
	reset_bit(R.D, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA3() { // RES 4 E
	reset_bit(R.E, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA4() { // RES 4 H
	reset_bit(R.H, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA5() { // RES 4 L
	reset_bit(R.L, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA6() { // RES 4 HL
	u8 data = read(R.HL);
	reset_bit(data, 4);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xA7() { // RES 4 A
	reset_bit(R.A, 4);
	return 0;
}

u8 CPU::prefixedopcode0xA8() { // RES 5 B
	reset_bit(R.B, 5);
	return 0;
}

u8 CPU::prefixedopcode0xA9() { // RES 5 C
	reset_bit(R.C, 5);
	return 0;
}

u8 CPU::prefixedopcode0xAA() { // RES 5 D
	reset_bit(R.D, 5);
	return 0;
}

u8 CPU::prefixedopcode0xAB() { // RES 5 E
	reset_bit(R.E, 5);
	return 0;
}

u8 CPU::prefixedopcode0xAC() { // RES 5 H
	reset_bit(R.H, 5);
	return 0;
}

u8 CPU::prefixedopcode0xAD() { // RES 5 L
	reset_bit(R.L, 5);
	return 0;
}

u8 CPU::prefixedopcode0xAE() { // RES 5 HL
	u8 data = read(R.HL);
	reset_bit(data, 5);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xAF() { // RES 5 A
	reset_bit(R.A, 5);
	return 0;
}

u8 CPU::prefixedopcode0xB0() { // RES 6 B
	reset_bit(R.B, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB1() { // RES 6 C
	reset_bit(R.C, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB2() { // RES 6 D
	reset_bit(R.D, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB3() { // RES 6 E
	reset_bit(R.E, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB4() { // RES 6 H
	reset_bit(R.H, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB5() { // RES 6 L
	reset_bit(R.L, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB6() { // RES 6 HL
	u8 data = read(R.HL);
	reset_bit(data, 6);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xB7() { // RES 6 A
	reset_bit(R.A, 6);
	return 0;
}

u8 CPU::prefixedopcode0xB8() { // RES 7 B
	reset_bit(R.B, 7);
	return 0;
}

u8 CPU::prefixedopcode0xB9() { // RES 7 C
	reset_bit(R.C, 7);
	return 0;
}

u8 CPU::prefixedopcode0xBA() { // RES 7 D
	reset_bit(R.D, 7);
	return 0;
}

u8 CPU::prefixedopcode0xBB() { // RES 7 E
	reset_bit(R.E, 7);
	return 0;
}

u8 CPU::prefixedopcode0xBC() { // RES 7 H
	reset_bit(R.H, 7);
	return 0;
}

u8 CPU::prefixedopcode0xBD() { // RES 7 L
	reset_bit(R.L, 7);
	return 0;
}

u8 CPU::prefixedopcode0xBE() { // RES 7 HL
	u8 data = read(R.HL);
	reset_bit(data, 7);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xBF() { // RES 7 A
	reset_bit(R.A, 7);
	return 0;
}

u8 CPU::prefixedopcode0xC0() { // SET 0 B
	set_bit(R.B, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC1() { // SET 0 C
	set_bit(R.C, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC2() { // SET 0 D
	set_bit(R.D, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC3() { // SET 0 E
	set_bit(R.E, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC4() { // SET 0 H
	set_bit(R.H, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC5() { // SET 0 L
	set_bit(R.L, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC6() { // SET 0 HL
	u8 data = read(R.HL);
	set_bit(data, 0);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xC7() { // SET 0 A
	set_bit(R.A, 0);
	return 0;
}

u8 CPU::prefixedopcode0xC8() { // SET 1 B
	set_bit(R.B, 1);
	return 0;
}

u8 CPU::prefixedopcode0xC9() { // SET 1 C
	set_bit(R.C, 1);
	return 0;
}

u8 CPU::prefixedopcode0xCA() { // SET 1 D
	set_bit(R.D, 1);
	return 0;
}

u8 CPU::prefixedopcode0xCB() { // SET 1 E
	set_bit(R.E, 1);
	return 0;
}

u8 CPU::prefixedopcode0xCC() { // SET 1 H
	set_bit(R.H, 1);
	return 0;
}

u8 CPU::prefixedopcode0xCD() { // SET 1 L
	set_bit(R.L, 1);
	return 0;
}

u8 CPU::prefixedopcode0xCE() { // SET 1 HL
	u8 data = read(R.HL);
	set_bit(data, 1);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xCF() { // SET 1 A
	set_bit(R.A, 1);
	return 0;
}

u8 CPU::prefixedopcode0xD0() { // SET 2 B
	set_bit(R.B, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD1() { // SET 2 C
	set_bit(R.C, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD2() { // SET 2 D
	set_bit(R.D, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD3() { // SET 2 E
	set_bit(R.E, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD4() { // SET 2 H
	set_bit(R.H, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD5() { // SET 2 L
	set_bit(R.L, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD6() { // SET 2 HL
	u8 data = read(R.HL);
	set_bit(data, 2);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xD7() { // SET 2 A
	set_bit(R.A, 2);
	return 0;
}

u8 CPU::prefixedopcode0xD8() { // SET 3 B
	set_bit(R.B, 3);
	return 0;
}

u8 CPU::prefixedopcode0xD9() { // SET 3 C
	set_bit(R.C, 3);
	return 0;
}

u8 CPU::prefixedopcode0xDA() { // SET 3 D
	set_bit(R.D, 3);
	return 0;
}

u8 CPU::prefixedopcode0xDB() { // SET 3 E
	set_bit(R.E, 3);
	return 0;
}

u8 CPU::prefixedopcode0xDC() { // SET 3 H
	set_bit(R.H, 3);
	return 0;
}

u8 CPU::prefixedopcode0xDD() { // SET 3 L
	set_bit(R.L, 3);

	return 0;
}

u8 CPU::prefixedopcode0xDE() { // SET 3 HL
	u8 data = read(R.HL);
	set_bit(data, 3);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xDF() { // SET 3 A
	set_bit(R.A, 3);
	return 0;
}

u8 CPU::prefixedopcode0xE0() { // SET 4 B
	set_bit(R.B, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE1() { // SET 4 C
	set_bit(R.C, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE2() { // SET 4 D
	set_bit(R.D, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE3() { // SET 4 E
	set_bit(R.E, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE4() { // SET 4 H
	set_bit(R.H, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE5() { // SET 4 L
	set_bit(R.L, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE6() { // SET 4 HL
	u8 data = read(R.HL);
	set_bit(data, 4);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xE7() { // SET 4 A
	set_bit(R.A, 4);
	return 0;
}

u8 CPU::prefixedopcode0xE8() { // SET 5 B
	set_bit(R.B, 5);
	return 0;
}

u8 CPU::prefixedopcode0xE9() { // SET 5 C
	set_bit(R.C, 5);
	return 0;
}

u8 CPU::prefixedopcode0xEA() { // SET 5 D
	set_bit(R.D, 5);
	return 0;
}

u8 CPU::prefixedopcode0xEB() { // SET 5 E
	set_bit(R.E, 5);
	return 0;
}

u8 CPU::prefixedopcode0xEC() { // SET 5 H
	set_bit(R.H, 5);
	return 0;
}

u8 CPU::prefixedopcode0xED() { // SET 5 L
	set_bit(R.L, 5);
	return 0;
}

u8 CPU::prefixedopcode0xEE() { // SET 5 HL
	u8 data = read(R.HL);
	set_bit(data, 5);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xEF() { // SET 5 A
	set_bit(R.A, 5);
	return 0;
}

u8 CPU::prefixedopcode0xF0() { // SET 6 B
	set_bit(R.B, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF1() { // SET 6 C
	set_bit(R.C, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF2() { // SET 6 D
	set_bit(R.D, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF3() { // SET 6 E
	set_bit(R.E, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF4() { // SET 6 H
	set_bit(R.H, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF5() { // SET 6 L
	set_bit(R.L, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF6() { // SET 6 HL
	u8 data = read(R.HL);
	set_bit(data, 6);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xF7() { // SET 6 A
	set_bit(R.A, 6);
	return 0;
}

u8 CPU::prefixedopcode0xF8() { // SET 7 B
	set_bit(R.B, 7);
	return 0;
}

u8 CPU::prefixedopcode0xF9() { // SET 7 C
	set_bit(R.C, 7);
	return 0;
}

u8 CPU::prefixedopcode0xFA() { // SET 7 D
	set_bit(R.D, 7);
	return 0;
}

u8 CPU::prefixedopcode0xFB() { // SET 7 E
	set_bit(R.E, 7);
	return 0;
}

u8 CPU::prefixedopcode0xFC() { // SET 7 H
	set_bit(R.H, 7);
	return 0;
}

u8 CPU::prefixedopcode0xFD() { // SET 7 L
	set_bit(R.L, 7);
	return 0;
}

u8 CPU::prefixedopcode0xFE() { // SET 7 HL
	u8 data = read(R.HL);
	set_bit(data, 7);
	write(R.HL, data);
	return 0;
}

u8 CPU::prefixedopcode0xFF() { // SET 7 A
	set_bit(R.A, 7);
	return 0;
}
