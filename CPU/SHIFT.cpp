#include "../CPU.h"

void CPU::test_n_bit_set_z(const u8 bit, const u8* reg) {
	// wenn bit gesetzt setze Zero Flag
	set_Flag_if(Flag::z, checkBit(*reg, bit));
	setFlag(Flag::h);
	clearFlag(Flag::n);
}

void CPU::rot_right_c_r8(u8* reg) {
	// Rotate register r8 right.
	// [0] ->[7 -> 0]->C
	u8 b0 = *reg & 1;
	*reg >>= 1;


	set_Flag_if(Flag::c, b0 == 1);
	set_Bit_if(*reg, 7, b0 == 1);
	set_Flag_if(Flag::z, *reg == 0);

	setFlags(Flag::n, Flag::h);
}

void CPU::rot_left_c_r8(u8* reg) {
	// Rotate register r8 left.
	// C < -[7 < -0] < -[7]
	u8 b7 = *reg & 0x80;
	*reg <<= 1;

	set_Flag_if(Flag::c, b7 == 1);
	set_Bit_if(*reg, 0, b7 == 0x80);
	set_Flag_if(Flag::z, *reg == 0);

	setFlags(Flag::n, Flag::h);
}

void CPU::rot_left_r8(u8* reg) {
	// Rotate bits in register r8 left through carry.
	// C <-[7 < -0] <-C
	u8 b7 = *reg & 0x80;
	*reg <<= 1;

	set_Bit_if(*reg, 0, checkFlag(Flag::c));
	set_Flag_if(Flag::c, b7 == 0x80);
	set_Flag_if(Flag::z, *reg == 0);
	clearFlags(Flag::n, Flag::h);
}
void CPU::rot_right_r8(u8* reg) {
	// Rotate register r8 right through carry.
	// C ->[7 -> 0]->C
	u8 b7 = *reg & 1;
	*reg >>= 1;
	set_Bit_if(*reg, 7, checkFlag(Flag::c));

	set_Flag_if(Flag::c, b7 == 1);
	set_Flag_if(Flag::z, *reg == 0);

	clearFlags(Flag::n, Flag::h);
}

void CPU::shift_left_a_r8(u8* reg) {
	// Shift Left Arithmetic register r8.
	// C < -[7 < -0] < -0
	u8 b7 = *reg & 0x80;
	*reg >>= 1;
	set_Flag_if(Flag::c, b7 == 0x80);
	set_Flag_if(Flag::z, *reg == 0);

	clearFlags(Flag::n, Flag::h);
}

void CPU::shift_right_a_r8(u8* reg) {
	// hift Right Arithmetic register r8.
	// [7] ->[7 -> 0]->C
	u8 b7 = *reg & 0x80;
	u8 b0 = *reg & 1;
	*reg >>= 1;

	set_Bit_if(*reg, 7, b7 == 0x80);
	set_Flag_if(Flag::c, b0 == 1);
	set_Flag_if(Flag::z, *reg == 0);

	clearFlags(Flag::n, Flag::h);
}

void CPU::shift_right_l_r8(u8* reg) {
	// Shift Right Logic register r8.
	// 0 ->[7 -> 0]->C
	u8 b0 = *reg & 1;
	*reg >>= 1;

	set_Flag_if(Flag::c, b0 == 1);
	set_Flag_if(Flag::z, *reg == 0);

	clearFlags(Flag::n, Flag::h);

}

void CPU::swap_r8(u8* reg) {
	// Swap upper 4 bits in register r8 and the lower 4 ones.
	u8 l_bits = *reg & 0xF0;
	u8 h_bits = *reg & 0x0F;

	l_bits >>= 4;
	h_bits <<= 4;

	*reg = l_bits | h_bits;
}


// OPCODES

u8 CPU::opcode0x07() {	// RLCA
	rot_right_c_r8(&R.A);
	clearFlag(Flag::z);
	return 0;
}
u8 CPU::opcode0x17() {	// RLA
	(R.A << 1) | (R.A >> (sizeof(u8) * CHAR_BIT - 1));
	return 0;
}
u8 CPU::opcode0x0F() {	// RRCA
	rot_right_c_r8(&R.A);
	return 0;
}
u8 CPU::opcode0x1F() {	// RRA
	rot_right_r8(&R.A);
	clearFlag(Flag::z);
	return 0;
}

u8 CPU::prefixedopcode0x00() {	// RLC B
	rot_left_c_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x01() {	// RLC C
	rot_left_c_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x02() {	// RLC D
	rot_left_c_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x03() {	// RLC E
	rot_left_c_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x04() {	// RLC H
	rot_left_c_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x05() {	// RLC L
	rot_left_c_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x06() {	// RLC HL
	rot_left_c_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x07() {	// RLC A
	rot_left_c_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x08() {	// RRC B
	rot_right_c_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x09() {	// RRC C
	rot_right_c_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x0A() {	// RRC D
	rot_right_c_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x0B() {	// RRC E
	rot_right_c_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x0C() {	// RRC H
	rot_right_c_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x0D() {	// RRC L
	rot_right_c_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x0E() {	// RRC HL
	rot_right_c_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x0F() {	// RRC A
	rot_right_c_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x10() {	// RL B
	rot_left_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x11() {	// RL C
	rot_left_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x12() {	// RL D
	rot_left_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x13() {	// RL E
	rot_left_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x14() {	// RL H
	rot_left_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x15() {	// RL L
	rot_left_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x16() {	// RL HL
	rot_left_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x17() {	// RL A
	rot_left_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x18() {	// RR B
	rot_right_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x19() {	// RR C
	rot_right_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x1A() {	// RR D
	rot_right_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x1B() {	// RR E
	rot_right_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x1C() {	// RR H
	rot_right_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x1D() {	// RR L
	rot_right_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x1E() {	// RR HL
	rot_right_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x1F() {	// RR A
	rot_right_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x20() {	// SLA B
	shift_left_a_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x21() {	// SLA C
	shift_left_a_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x22() {	// SLA D
	shift_left_a_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x23() {	// SLA E
	shift_left_a_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x24() {	// SLA H
	shift_left_a_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x25() {	// SLA L
	shift_left_a_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x26() {	// SLA HL
	shift_left_a_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x27() {	// SLA A
	shift_left_a_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x28() {	// SRA B
	shift_right_a_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x29() {	// SRA C
	shift_right_a_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x2A() {	// SRA D
	shift_right_a_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x2B() {	// SRA E
	shift_right_a_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x2C() {	// SRA H
	shift_right_a_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x2D() {	// SRA L
	shift_right_a_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x2E() {	// SRA HL
	shift_right_a_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x2F() {	// SRA A
	shift_right_a_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x30() {	// SWAP B
	swap_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x31() {	// SWAP C
	swap_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x32() {	// SWAP D
	swap_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x33() {	// SWAP E
	swap_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x34() {	// SWAP H
	swap_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x35() {	// SWAP L
	swap_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x36() {	// SWAP HL
	swap_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x37() {	// SWAP A
	swap_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x38() {	// SRL B
	shift_right_l_r8(&R.B);
	return 0;
}
u8 CPU::prefixedopcode0x39() {	// SRL C
	shift_right_l_r8(&R.C);
	return 0;
}
u8 CPU::prefixedopcode0x3A() {	// SRL D
	shift_right_l_r8(&R.D);
	return 0;
}
u8 CPU::prefixedopcode0x3B() {	// SRL E
	shift_right_l_r8(&R.E);
	return 0;
}
u8 CPU::prefixedopcode0x3C() {	// SRL H
	shift_right_l_r8(&R.H);
	return 0;
}
u8 CPU::prefixedopcode0x3D() {	// SRL L
	shift_right_l_r8(&R.L);
	return 0;
}
u8 CPU::prefixedopcode0x3E() {	// SRL HL
	shift_right_l_r8(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x3F() {	// SRL A
	shift_right_l_r8(&R.A);
	return 0;
}
u8 CPU::prefixedopcode0x40() {	// BIT 0 B
	test_n_bit_set_z(0, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x41() {	// BIT 0 C
	test_n_bit_set_z(0, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x42() {	// BIT 0 D
	test_n_bit_set_z(0, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x43() {	// BIT 0 E
	test_n_bit_set_z(0, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x44() {	// BIT 0 H
	test_n_bit_set_z(0, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x45() {	// BIT 0 L
	test_n_bit_set_z(0, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x46() {	// BIT 0 (HL)
	test_n_bit_set_z(0, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x47() {	// BIT 0 A
	test_n_bit_set_z(0, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x48() {	// BIT 1 B
	test_n_bit_set_z(1, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x49() {	// BIT 1 C
	test_n_bit_set_z(1, &R.C);;
	return 0;
}
u8 CPU::prefixedopcode0x4A() {	// BIT 1 D
	test_n_bit_set_z(1, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x4B() {	// BIT 1 E
	test_n_bit_set_z(1, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x4C() {	// BIT 1 H
	test_n_bit_set_z(1, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x4D() {	// BIT 1 L
	test_n_bit_set_z(1, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x4E() {	// BIT 1 HL
	test_n_bit_set_z(1, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x4F() {	// BIT 1 A
	test_n_bit_set_z(1, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x50() {	// BIT 2 B
	test_n_bit_set_z(2, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x51() {	// BIT 2 C
	test_n_bit_set_z(2, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x52() {	// BIT 2 D
	test_n_bit_set_z(2, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x53() {	// BIT 2 E
	test_n_bit_set_z(2, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x54() {	// BIT 2 H
	test_n_bit_set_z(2, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x55() {	// BIT 2 L
	test_n_bit_set_z(2, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x56() {	// BIT 2 HL
	test_n_bit_set_z(2, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x57() {	// BIT 2 A
	test_n_bit_set_z(2, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x58() {	// BIT 3 B
	test_n_bit_set_z(3, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x59() {	// BIT 3 C
	test_n_bit_set_z(3, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x5A() {	// BIT 3 D
	test_n_bit_set_z(3, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x5B() {	// BIT 3 E
	test_n_bit_set_z(3, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x5C() {	// BIT 3 H
	test_n_bit_set_z(3, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x5D() {	// BIT 3 L
	test_n_bit_set_z(3, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x5E() {	// BIT 3 HL
	test_n_bit_set_z(3, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x5F() {	// BIT 3 A
	test_n_bit_set_z(3, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x60() {	// BIT 4 B
	test_n_bit_set_z(4, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x61() {	// BIT 4 C
	test_n_bit_set_z(4, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x62() {	// BIT 4 D
	test_n_bit_set_z(4, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x63() {	// BIT 4 E
	test_n_bit_set_z(4, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x64() {	// BIT 4 H
	test_n_bit_set_z(4, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x65() {	// BIT 4 L
	test_n_bit_set_z(4, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x66() {	// BIT 4 HL
	test_n_bit_set_z(4, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x67() {	// BIT 4 A
	test_n_bit_set_z(4, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x68() {	// BIT 5 B
	test_n_bit_set_z(5, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x69() {	// BIT 5 C
	test_n_bit_set_z(5, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x6A() {	// BIT 5 D
	test_n_bit_set_z(5, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x6B() {	// BIT 5 E
	test_n_bit_set_z(5, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x6C() {	// BIT 5 H
	test_n_bit_set_z(5, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x6D() {	// BIT 5 L
	test_n_bit_set_z(5, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x6E() {	// BIT 5 HL
	test_n_bit_set_z(5, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x6F() {	// BIT 5 A
	test_n_bit_set_z(5, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x70() {	// BIT 6 B
	test_n_bit_set_z(6, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x71() {	// BIT 6 C
	test_n_bit_set_z(6, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x72() {	// BIT 6 D
	test_n_bit_set_z(6, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x73() {	// BIT 6 E
	test_n_bit_set_z(6, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x74() {	// BIT 6 H
	test_n_bit_set_z(6, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x75() {	// BIT 6 L
	test_n_bit_set_z(6, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x76() {	// BIT 6 HL
	test_n_bit_set_z(6, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x77() {	// BIT 6 A
	test_n_bit_set_z(6, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x78() {	// BIT 7 B
	test_n_bit_set_z(7, &R.B);
	return 0;
}
u8 CPU::prefixedopcode0x79() {	// BIT 7 C
	test_n_bit_set_z(7, &R.C);
	return 0;
}
u8 CPU::prefixedopcode0x7A() {	// BIT 7 D
	test_n_bit_set_z(7, &R.D);
	return 0;
}
u8 CPU::prefixedopcode0x7B() {	// BIT 7 E
	test_n_bit_set_z(7, &R.E);
	return 0;
}
u8 CPU::prefixedopcode0x7C() {	// BIT 7 H
	test_n_bit_set_z(7, &R.H);
	return 0;
}
u8 CPU::prefixedopcode0x7D() {	// BIT 7 L
	test_n_bit_set_z(7, &R.L);
	return 0;
}
u8 CPU::prefixedopcode0x7E() {	// BIT 7 HL
	test_n_bit_set_z(7, mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::prefixedopcode0x7F() {	// BIT 7 A
	test_n_bit_set_z(7, &R.A);
	return 0;
}
u8 CPU::prefixedopcode0x80() {	// RES 0 B
	clearBit(R.B, 0);
	return 0;
}
u8 CPU::prefixedopcode0x81() {	// RES 0 C
	clearBit(R.C, 0);
	return 0;
}
u8 CPU::prefixedopcode0x82() {	// RES 0 D
	clearBit(R.D, 0);
	return 0;
}
u8 CPU::prefixedopcode0x83() {	// RES 0 E
	clearBit(R.E, 0);
	return 0;
}
u8 CPU::prefixedopcode0x84() {	// RES 0 H
	clearBit(R.H, 0);
	return 0;
}
u8 CPU::prefixedopcode0x85() {	// RES 0 L
	clearBit(R.L, 0);
	return 0;
}
u8 CPU::prefixedopcode0x86() {	// RES 0 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 0);
	return 0;
}
u8 CPU::prefixedopcode0x87() {	// RES 0 A
	clearBit(R.A, 0);
	return 0;
}
u8 CPU::prefixedopcode0x88() {	// RES 1 B
	clearBit(R.B, 1);
	return 0;
}
u8 CPU::prefixedopcode0x89() {	// RES 1 C
	clearBit(R.C, 1);
	return 0;
}
u8 CPU::prefixedopcode0x8A() {	// RES 1 D
	clearBit(R.D, 1);
	return 0;
}
u8 CPU::prefixedopcode0x8B() {	// RES 1 E
	clearBit(R.E, 1);
	return 0;
}
u8 CPU::prefixedopcode0x8C() {	// RES 1 H
	clearBit(R.H, 1);
	return 0;
}
u8 CPU::prefixedopcode0x8D() {	// RES 1 L
	clearBit(R.L, 1);
	return 0;
}
u8 CPU::prefixedopcode0x8E() {	// RES 1 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 1);
	return 0;
}
u8 CPU::prefixedopcode0x8F() {	// RES 1 A
	clearBit(R.A, 1);
	return 0;
}
u8 CPU::prefixedopcode0x90() {	// RES 2 B
	clearBit(R.B, 2);
	return 0;
}
u8 CPU::prefixedopcode0x91() {	// RES 2 C
	clearBit(R.C, 2);
	return 0;
}
u8 CPU::prefixedopcode0x92() {	// RES 2 D
	clearBit(R.D, 2);
	return 0;
}
u8 CPU::prefixedopcode0x93() {	// RES 2 E
	clearBit(R.E, 2);
	return 0;
}
u8 CPU::prefixedopcode0x94() {	// RES 2 H
	clearBit(R.H, 2);
	return 0;
}
u8 CPU::prefixedopcode0x95() {	// RES 2 L
	clearBit(R.L, 2);
	return 0;
}
u8 CPU::prefixedopcode0x96() {	// RES 2 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 2);
	return 0;
}
u8 CPU::prefixedopcode0x97() {	// RES 2 A
	clearBit(R.A, 2);
	return 0;
}
u8 CPU::prefixedopcode0x98() {	// RES 3 B
	clearBit(R.B, 3);
	return 0;
}
u8 CPU::prefixedopcode0x99() {	// RES 3 C
	clearBit(R.C, 3);
	return 0;
}
u8 CPU::prefixedopcode0x9A() {	// RES 3 D
	clearBit(R.D, 3);
	return 0;
}
u8 CPU::prefixedopcode0x9B() {	// RES 3 E
	clearBit(R.E, 3);
	return 0;
}
u8 CPU::prefixedopcode0x9C() {	// RES 3 H
	clearBit(R.H, 3);
	return 0;
}
u8 CPU::prefixedopcode0x9D() {	// RES 3 L
	clearBit(R.L, 3);
	return 0;
}
u8 CPU::prefixedopcode0x9E() {	// RES 3 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 3);
	return 0;
}
u8 CPU::prefixedopcode0x9F() {	// RES 3 A
	clearBit(R.A, 3);
	return 0;
}
u8 CPU::prefixedopcode0xA0() {	// RES 4 B
	clearBit(R.B, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA1() {	// RES 4 C
	clearBit(R.C, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA2() {	// RES 4 D
	clearBit(R.D, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA3() {	// RES 4 E
	clearBit(R.E, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA4() {	// RES 4 H
	clearBit(R.H, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA5() {	// RES 4 L
	clearBit(R.L, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA6() {	// RES 4 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA7() {	// RES 4 A
	clearBit(R.A, 4);
	return 0;
}
u8 CPU::prefixedopcode0xA8() {	// RES 5 B
	clearBit(R.B, 5);
	return 0;
}
u8 CPU::prefixedopcode0xA9() {	// RES 5 C
	clearBit(R.C, 5);
	return 0;
}
u8 CPU::prefixedopcode0xAA() {	// RES 5 D
	clearBit(R.D, 5);
	return 0;
}
u8 CPU::prefixedopcode0xAB() {	// RES 5 E
	clearBit(R.E, 5);
	return 0;
}
u8 CPU::prefixedopcode0xAC() {	// RES 5 H
	clearBit(R.H, 5);
	return 0;
}
u8 CPU::prefixedopcode0xAD() {	// RES 5 L
	clearBit(R.L, 5);
	return 0;
}
u8 CPU::prefixedopcode0xAE() {	// RES 5 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 5);
	return 0;
}
u8 CPU::prefixedopcode0xAF() {	// RES 5 A
	clearBit(R.A, 5);
	return 0;
}
u8 CPU::prefixedopcode0xB0() {	// RES 6 B
	clearBit(R.B, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB1() {	// RES 6 C
	clearBit(R.C, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB2() {	// RES 6 D
	clearBit(R.D, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB3() {	// RES 6 E
	clearBit(R.E, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB4() {	// RES 6 H
	clearBit(R.H, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB5() {	// RES 6 L
	clearBit(R.L, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB6() {	// RES 6 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB7() {	// RES 6 A
	clearBit(R.A, 6);
	return 0;
}
u8 CPU::prefixedopcode0xB8() {	// RES 7 B
	clearBit(R.B, 7);
	return 0;
}
u8 CPU::prefixedopcode0xB9() {	// RES 7 C
	clearBit(R.C, 7);
	return 0;
}
u8 CPU::prefixedopcode0xBA() {	// RES 7 D
	clearBit(R.D, 7);
	return 0;
}
u8 CPU::prefixedopcode0xBB() {	// RES 7 E
	clearBit(R.E, 7);
	return 0;
}
u8 CPU::prefixedopcode0xBC() {	// RES 7 H
	clearBit(R.H, 7);
	return 0;
}
u8 CPU::prefixedopcode0xBD() {	// RES 7 L
	clearBit(R.L, 7);
	return 0;
}
u8 CPU::prefixedopcode0xBE() {	// RES 7 HL
	u8 data = mem->read(R.HL);
	clearBit(data, 7);
	return 0;
}
u8 CPU::prefixedopcode0xBF() {	// RES 7 A
	clearBit(R.A, 7);
	return 0;
}
u8 CPU::prefixedopcode0xC0() {	// SET 0 B
	setBit(R.B, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC1() {	// SET 0 C
	setBit(R.C, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC2() {	// SET 0 D
	setBit(R.D, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC3() {	// SET 0 E
	setBit(R.E, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC4() {	// SET 0 H
	setBit(R.H, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC5() {	// SET 0 L
	setBit(R.L, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC6() {	// SET 0 HL
	u8 data = mem->read(R.HL);
	setBit(data, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC7() {	// SET 0 A
	setBit(R.A, 0);
	return 0;
}
u8 CPU::prefixedopcode0xC8() {	// SET 1 B
	setBit(R.B, 1);
	return 0;
}
u8 CPU::prefixedopcode0xC9() {	// SET 1 C
	setBit(R.C, 1);
	return 0;
}
u8 CPU::prefixedopcode0xCA() {	// SET 1 D
	setBit(R.D, 1);
	return 0;
}
u8 CPU::prefixedopcode0xCB() {	// SET 1 E
	setBit(R.E, 1);
	return 0;
}
u8 CPU::prefixedopcode0xCC() {	// SET 1 H
	setBit(R.H, 1);
	return 0;
}
u8 CPU::prefixedopcode0xCD() {	// SET 1 L
	setBit(R.L, 1);
	return 0;
}
u8 CPU::prefixedopcode0xCE() {	// SET 1 HL
	u8 data = mem->read(R.HL);
	setBit(data, 1);
	return 0;
}
u8 CPU::prefixedopcode0xCF() {	// SET 1 A
	setBit(R.A, 1);
	return 0;
}
u8 CPU::prefixedopcode0xD0() {	// SET 2 B
	setBit(R.B, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD1() {	// SET 2 C
	setBit(R.C, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD2() {	// SET 2 D
	setBit(R.D, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD3() {	// SET 2 E
	setBit(R.E, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD4() {	// SET 2 H
	setBit(R.H, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD5() {	// SET 2 L
	setBit(R.L, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD6() {	// SET 2 HL
	u8 data = mem->read(R.HL);
	setBit(data, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD7() {	// SET 2 A
	setBit(R.A, 2);
	return 0;
}
u8 CPU::prefixedopcode0xD8() {	// SET 3 B
	setBit(R.B, 3);
	return 0;
}
u8 CPU::prefixedopcode0xD9() {	// SET 3 C
	setBit(R.C, 3);
	return 0;
}
u8 CPU::prefixedopcode0xDA() {	// SET 3 D
	setBit(R.D, 3);
	return 0;
}
u8 CPU::prefixedopcode0xDB() {	// SET 3 E
	setBit(R.E, 3);
	return 0;
}
u8 CPU::prefixedopcode0xDC() {	// SET 3 H
	setBit(R.H, 3);
	return 0;
}
u8 CPU::prefixedopcode0xDD() {	// SET 3 L
	setBit(R.L, 3);

	return 0;
}
u8 CPU::prefixedopcode0xDE() {	// SET 3 HL
	u8 data = mem->read(R.HL);
	setBit(data, 3);
	return 0;
}
u8 CPU::prefixedopcode0xDF() {	// SET 3 A
	setBit(R.A, 3);
	return 0;
}
u8 CPU::prefixedopcode0xE0() {	// SET 4 B
	setBit(R.B, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE1() {	// SET 4 C
	setBit(R.C, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE2() {	// SET 4 D
	setBit(R.D, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE3() {	// SET 4 E
	setBit(R.E, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE4() {	// SET 4 H
	setBit(R.H, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE5() {	// SET 4 L
	setBit(R.L, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE6() {	// SET 4 HL
	u8 data = mem->read(R.HL);
	setBit(data, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE7() {	// SET 4 A
	setBit(R.A, 4);
	return 0;
}
u8 CPU::prefixedopcode0xE8() {	// SET 5 B
	setBit(R.B, 5);
	return 0;
}
u8 CPU::prefixedopcode0xE9() {	// SET 5 C
	setBit(R.C, 5);
	return 0;
}
u8 CPU::prefixedopcode0xEA() {	// SET 5 D
	setBit(R.D, 5);
	return 0;
}
u8 CPU::prefixedopcode0xEB() {	// SET 5 E
	setBit(R.E, 5);
	return 0;
}
u8 CPU::prefixedopcode0xEC() {	// SET 5 H
	setBit(R.H, 5);
	return 0;
}
u8 CPU::prefixedopcode0xED() {	// SET 5 L
	setBit(R.L, 5);
	return 0;
}
u8 CPU::prefixedopcode0xEE() {	// SET 5 HL
	u8 data = mem->read(R.HL);
	setBit(data, 5);
	return 0;
}
u8 CPU::prefixedopcode0xEF() {	// SET 5 A
	setBit(R.A, 5);
	return 0;
}
u8 CPU::prefixedopcode0xF0() {	// SET 6 B
	setBit(R.B, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF1() {	// SET 6 C
	setBit(R.C, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF2() {	// SET 6 D
	setBit(R.D, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF3() {	// SET 6 E
	setBit(R.E, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF4() {	// SET 6 H
	setBit(R.H, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF5() {	// SET 6 L
	setBit(R.L, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF6() {	// SET 6 HL
	u8 data = mem->read(R.HL);
	setBit(data, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF7() {	// SET 6 A
	setBit(R.A, 6);
	return 0;
}
u8 CPU::prefixedopcode0xF8() {	// SET 7 B
	setBit(R.B, 7);
	return 0;
}
u8 CPU::prefixedopcode0xF9() {	// SET 7 C
	setBit(R.C, 7);
	return 0;
}
u8 CPU::prefixedopcode0xFA() {	// SET 7 D
	setBit(R.D, 7);
	return 0;
}
u8 CPU::prefixedopcode0xFB() {	// SET 7 E
	setBit(R.E, 7);
	return 0;
}
u8 CPU::prefixedopcode0xFC() {	// SET 7 H
	setBit(R.H, 7);
	return 0;
}
u8 CPU::prefixedopcode0xFD() {	// SET 7 L
	setBit(R.L, 7);
	return 0;
}
u8 CPU::prefixedopcode0xFE() {	// SET 7 HL
	u8 data = mem->read(R.HL);
	setBit(data, 7);
	return 0;
}
u8 CPU::prefixedopcode0xFF() {	// SET 7 A
	setBit(R.A, 7);
	return 0;
}
