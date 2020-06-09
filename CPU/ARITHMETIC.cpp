#include "../CPU.h"

void CPU::inc_u8_reg(u8* reg) {
	*reg = *reg + 1;

	clearFlag(Flag::n);

	if (*reg & 0xFF)
		clearFlag(Flag::h);
	else
		setFlag(Flag::h);

	if (*reg == 0)
		setFlag(Flag::z);
	else
		clearFlag(Flag::z);
}

void CPU::dec_u8_reg(u8* reg) {
	*reg = *reg - 1;

	setFlag(Flag::n);

	if (*reg == 0)
		setFlag(Flag::z);
	else
		clearFlag(Flag::z);

	if ((*reg & 0x0F) == 0x0F)
		setFlag(Flag::h);
	else
		clearFlag(Flag::h);

}

void CPU::add_a_r8(u8* reg) {
}


u8 CPU::opcode0x3C() {	// INC A
	inc_u8_reg(&R.A);
	return 0;
}

u8 CPU::opcode0x04() {	// INC B
	inc_u8_reg(&R.B);
	return 0;
}
u8 CPU::opcode0x0C() {	// INC C
	inc_u8_reg(&R.C);
	return 0;
}
u8 CPU::opcode0x14() {	// INC D
	inc_u8_reg(&R.D);
	return 0;
}
u8 CPU::opcode0x1C() {	// INC E
	inc_u8_reg(&R.E);
	return 0;
}
u8 CPU::opcode0x24() {	// INC H
	inc_u8_reg(&R.H);
	return 0;
}
u8 CPU::opcode0x2C() {	// INC L
	inc_u8_reg(&R.L);
	return 0;
}
u8 CPU::opcode0x34() {	// INC (HL)
	inc_u8_reg(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::opcode0x13() {	// INC DE
	R.DE++;
	return 0;
}
u8 CPU::opcode0x03() {	// INC BC
	R.BC++;
	return 0;
}
u8 CPU::opcode0x23() {	// INC HL
	R.HL++;
	return 0;
}
u8 CPU::opcode0x33() {	// INC SP
	R.SP++;
	return 0;
}
u8 CPU::opcode0x05() {	// DEC B
	dec_u8_reg(&R.B);
	return 0;
}
u8 CPU::opcode0x0D() {	// DEC C
	dec_u8_reg(&R.C);
	return 0;
}
u8 CPU::opcode0x15() {	// DEC D
	dec_u8_reg(&R.C);
	return 0;
}
u8 CPU::opcode0x1D() {	// DEC E
	dec_u8_reg(&R.E);
	return 0;
}
u8 CPU::opcode0x25() {	// DEC H
	dec_u8_reg(&R.H);
	return 0;
}
u8 CPU::opcode0x3D() {	// DEC A
	dec_u8_reg(&R.A);
	return 0;
}
u8 CPU::opcode0x2D() {	// DEC L
	dec_u8_reg(&R.L);
	return 0;
}
u8 CPU::opcode0x35() {	// DEC (HL)
	dec_u8_reg(mem->read_ptr(R.HL));
	return 0;
}
u8 CPU::opcode0x0B() {	// DEC BC
	R.BC--;
	return 0;
}
u8 CPU::opcode0x1B() {	// DEC DE
	R.DE--;
	return 0;
}
u8 CPU::opcode0x2B() {	// DEC HL
	R.HL--;
	return 0;
}
u8 CPU::opcode0x3B() {	// DEC SP
	R.SP--;
	return 0;
}


u8 CPU::opcode0x80() {	// ADD A B
	std::cout << "0x80 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x81() {	// ADD A C
	std::cout << "0x81 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x82() {	// ADD A D
	std::cout << "0x82 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x83() {	// ADD A E
	std::cout << "0x83 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x84() {	// ADD A H
	std::cout << "0x84 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x85() {	// ADD A L
	std::cout << "0x85 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x86() {	// ADD A HL
	std::cout << "0x86 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x87() {	// ADD A A
	std::cout << "0x87 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x09() {	// ADD HL BC
	std::cout << "0x09 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x19() {	// ADD HL DE
	std::cout << "0x19 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x29() {	// ADD HL HL
	std::cout << "0x29 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x39() {	// ADD HL SP
	std::cout << "0x39 nicht implementiert" << std::endl;
	return 0;
}

u8 CPU::opcode0xC6() {	// ADD A d8
	std::cout << "0xC6 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xE8() {	// ADD SP r8
	std::cout << "0xE8 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x90() {	// SUB B
	std::cout << "0x90 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x91() {	// SUB C
	std::cout << "0x91 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x92() {	// SUB D
	std::cout << "0x92 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x93() {	// SUB E
	std::cout << "0x93 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x94() {	// SUB H
	std::cout << "0x94 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x95() {	// SUB L
	std::cout << "0x95 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x96() {	// SUB HL
	std::cout << "0x96 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x97() {	// SUB A
	std::cout << "0x97 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xD6() {	// SUB d8
	std::cout << "0xD6 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xE6() {	// AND d8
	std::cout << "0xE6 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA0() {	// AND B
	std::cout << "0xA0 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA1() {	// AND C
	std::cout << "0xA1 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA2() {	// AND D
	std::cout << "0xA2 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA3() {	// AND E
	std::cout << "0xA3 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA4() {	// AND H
	std::cout << "0xA4 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA5() {	// AND L
	std::cout << "0xA5 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA6() {	// AND HL
	std::cout << "0xA6 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xA7() {	// AND A
	std::cout << "0xA7 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB0() {	// OR B
	std::cout << "0xB0 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB1() {	// OR C
	std::cout << "0xB1 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB2() {	// OR D
	std::cout << "0xB2 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB3() {	// OR E
	std::cout << "0xB3 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB4() {	// OR H
	std::cout << "0xB4 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB5() {	// OR L
	std::cout << "0xB5 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB6() {	// OR HL
	std::cout << "0xB6 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB7() {	// OR A
	std::cout << "0xB7 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xF6() {	// OR d8
	std::cout << "0xF6 nicht implementiert" << std::endl;
	return 0;
}

u8 CPU::opcode0x98() {	// SBC A B
	std::cout << "0x98 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x99() {	// SBC A C
	std::cout << "0x99 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x9A() {	// SBC A D
	std::cout << "0x9A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x9B() {	// SBC A E
	std::cout << "0x9B nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x9C() {	// SBC A H
	std::cout << "0x9C nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x9D() {	// SBC A L
	std::cout << "0x9D nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x9E() {	// SBC A HL
	std::cout << "0x9E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x9F() {	// SBC A A
	std::cout << "0x9F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xDE() {	// SBC A d8
	std::cout << "0xDE nicht implementiert" << std::endl;
	return 0;
}

u8 CPU::opcode0xEE() {	// XOR d8
	R.A = xor_8(R.A, mem->read(R.PC + 1));
	return 0;
}
u8 CPU::opcode0xA8() {	// XOR B
	R.A = xor_8(R.A, R.B);
	return 0;
}
u8 CPU::opcode0xA9() {	// XOR C
	R.A = xor_8(R.A, R.B);
	return 0;
}
u8 CPU::opcode0xAA() {	// XOR D
	R.A = xor_8(R.A, R.D);
	return 0;
}
u8 CPU::opcode0xAB() {	// XOR E
	R.A = xor_8(R.A, R.E);
	return 0;
}
u8 CPU::opcode0xAC() {	// XOR H
	R.A = xor_8(R.A, R.H);
	return 0;
}
u8 CPU::opcode0xAD() {	// XOR L
	R.A = xor_8(R.A, R.L);
	return 0;
}
u8 CPU::opcode0xAE() {	// XOR HL
	std::cout << "0xAE nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xAF() {	// XOR A
	R.A = xor_8(R.A, R.A);
	return 0;
}
u8 CPU::opcode0xB8() {	// CP B
	std::cout << "0xB8 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xB9() {	// CP C
	std::cout << "0xB9 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xBA() {	// CP D
	std::cout << "0xBA nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xBB() {	// CP E
	std::cout << "0xBB nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xBC() {	// CP H
	std::cout << "0xBC nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xBD() {	// CP L
	std::cout << "0xBD nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xBE() {	// CP HL
	std::cout << "0xBE nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xBF() {	// CP A
	std::cout << "0xBF nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xFE() {	// CP d8
	std::cout << "0xFE nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x88() {	// ADC A B
	std::cout << "0x88 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x89() {	// ADC A C
	std::cout << "0x89 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x8A() {	// ADC A D
	std::cout << "0x8A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x8B() {	// ADC A E
	std::cout << "0x8B nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x8C() {	// ADC A H
	std::cout << "0x8C nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x8D() {	// ADC A L
	std::cout << "0x8D nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x8E() {	// ADC A HL
	std::cout << "0x8E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x8F() {	// ADC A A
	std::cout << "0x8F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xCE() {	// ADC A d8
	std::cout << "0xCE nicht implementiert" << std::endl;
	return 0;
}