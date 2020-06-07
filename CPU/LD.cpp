#include "../CPU.h"

u8 CPU::opcode0x02() {	// LD BC A
	std::cout << "0x02 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x06() {	// LD B d8
	std::cout << "0x06 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x01() {	// LD BC d16
	std::cout << "0x01 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x08() {	// LD a16 SP
	std::cout << "0x08 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x0A() {	// LD A BC
	std::cout << "0x0A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x0E() {	// LD C d8
	std::cout << "0x0E nicht implementiert" << std::endl;
	return 0;
}

u8 CPU::opcode0x11() {	// LD DE d16
	std::cout << "0x11 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x12() {	// LD DE A
	std::cout << "0x12 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x16() {	// LD D d8
	std::cout << "0x16 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x1A() {	// LD A DE
	std::cout << "0x1A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x1E() {	// LD E d8
	std::cout << "0x1E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x21() {	// LD HL d16
	std::cout << "0x21 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x22() {	// LD HL A
	std::cout << "0x22 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x26() {	// LD H d8
	std::cout << "0x26 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x2A() {	// LD A HL
	std::cout << "0x2A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x2E() {	// LD L d8
	std::cout << "0x2E nicht implementiert" << std::endl;
	return 0;
}

u8 CPU::opcode0x31() {  // LD SP d16
	return u8();
}


u8 CPU::opcode0x32() {	// LD HL A
	std::cout << "0x32 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x36() {	// LD HL d8
	std::cout << "0x36 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x3A() {	// LD A HL
	std::cout << "0x3A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x3E() {	// LD A d8
	std::cout << "0x3E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x40() {	// LD B B
	std::cout << "0x40 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x41() {	// LD B C
	std::cout << "0x41 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x42() {	// LD B D
	std::cout << "0x42 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x43() {	// LD B E
	std::cout << "0x43 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x44() {	// LD B H
	std::cout << "0x44 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x45() {	// LD B L
	std::cout << "0x45 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x46() {	// LD B HL
	std::cout << "0x46 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x47() {	// LD B A
	std::cout << "0x47 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x48() {	// LD C B
	std::cout << "0x48 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x49() {	// LD C C
	std::cout << "0x49 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x4A() {	// LD C D
	std::cout << "0x4A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x4B() {	// LD C E
	std::cout << "0x4B nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x4C() {	// LD C H
	std::cout << "0x4C nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x4D() {	// LD C L
	std::cout << "0x4D nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x4E() {	// LD C HL
	std::cout << "0x4E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x4F() {	// LD C A
	std::cout << "0x4F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x50() {	// LD D B
	std::cout << "0x50 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x51() {	// LD D C
	std::cout << "0x51 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x52() {	// LD D D
	std::cout << "0x52 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x53() {	// LD D E
	std::cout << "0x53 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x54() {	// LD D H
	std::cout << "0x54 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x55() {	// LD D L
	std::cout << "0x55 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x56() {	// LD D HL
	std::cout << "0x56 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x57() {	// LD D A
	std::cout << "0x57 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x58() {	// LD E B
	std::cout << "0x58 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x59() {	// LD E C
	std::cout << "0x59 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x5A() {	// LD E D
	std::cout << "0x5A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x5B() {	// LD E E
	std::cout << "0x5B nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x5C() {	// LD E H
	std::cout << "0x5C nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x5D() {	// LD E L
	std::cout << "0x5D nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x5E() {	// LD E HL
	std::cout << "0x5E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x5F() {	// LD E A
	std::cout << "0x5F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x60() {	// LD H B
	std::cout << "0x60 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x61() {	// LD H C
	std::cout << "0x61 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x62() {	// LD H D
	std::cout << "0x62 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x63() {	// LD H E
	std::cout << "0x63 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x64() {	// LD H H
	std::cout << "0x64 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x65() {	// LD H L
	std::cout << "0x65 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x66() {	// LD H HL
	std::cout << "0x66 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x67() {	// LD H A
	std::cout << "0x67 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x68() {	// LD L B
	std::cout << "0x68 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x69() {	// LD L C
	std::cout << "0x69 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x6A() {	// LD L D
	std::cout << "0x6A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x6B() {	// LD L E
	std::cout << "0x6B nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x6C() {	// LD L H
	std::cout << "0x6C nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x6D() {	// LD L L
	std::cout << "0x6D nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x6E() {	// LD L HL
	std::cout << "0x6E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x6F() {	// LD L A
	std::cout << "0x6F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x70() {	// LD HL B
	std::cout << "0x70 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x71() {	// LD HL C
	std::cout << "0x71 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x72() {	// LD HL D
	std::cout << "0x72 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x73() {	// LD HL E
	std::cout << "0x73 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x74() {	// LD HL H
	std::cout << "0x74 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x75() {	// LD HL L
	std::cout << "0x75 nicht implementiert" << std::endl;
	return 0;
}

u8 CPU::opcode0x77() {	// LD HL A
	std::cout << "0x77 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x78() {	// LD A B
	std::cout << "0x78 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x79() {	// LD A C
	std::cout << "0x79 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x7A() {	// LD A D
	std::cout << "0x7A nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x7B() {	// LD A E
	std::cout << "0x7B nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x7C() {	// LD A H
	std::cout << "0x7C nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x7D() {	// LD A L
	std::cout << "0x7D nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x7E() {	// LD A HL
	std::cout << "0x7E nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x7F() {	// LD A A
	std::cout << "0x7F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xE2() {	// LD C A
	std::cout << "0xE2 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xEA() {	// LD a16 A
	std::cout << "0xEA nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xF2() {	// LD A C
	std::cout << "0xF2 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xF8() {	// LD HL SP
	std::cout << "0xF8 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xF9() {	// LD SP HL
	std::cout << "0xF9 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xFA() {	// LD A a16
	std::cout << "0xFA nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xF0() {	// LDH A a8
	std::cout << "0xF0 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xE0() {	// LDH a8 A
	std::cout << "0xE0 nicht implementiert" << std::endl;
	return 0;
}