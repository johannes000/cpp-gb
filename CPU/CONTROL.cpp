#include "../CPU.h"

u8 CPU::opcode0x00() {	// NOP
	return 0;
}
u8 CPU::opcode0x10() {	// STOP
	std::cout << "0x10 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x27() {	// DAA
	std::cout << "0x27 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x3F() {	// CCF
	toggleFlag(Flag::c);
	clearFlags(Flag::n, Flag::h);
	return 0;
}
u8 CPU::opcode0x37() {	// SCF
	setFlag(Flag::c);
	clearFlags(Flag::n, Flag::h);
	return 0;
}
u8 CPU::opcode0x2F() {	// CPL
	R.A = ~R.A;
	setFlags(Flag::n, Flag::h);
	return 0;
}
u8 CPU::opcode0xF3() {	// DI
	std::cout << "0xF3 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xFB() {	// EI
	std::cout << "0xFB nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x76() {	// HALT
	std::cout << "0x76 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xCB() {	// PREFIX
	prefix = true;
	return 0;
}


u8 CPU::opcode0xD3() {	// ILLEGAL_D3
	std::cout << "0xD3 ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xDB() {	// ILLEGAL_DB
	std::cout << "0xDB ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xDD() {	// ILLEGAL_DD
	std::cout << "0xDD ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xE3() {	// ILLEGAL_E3
	std::cout << "0xE3 ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xE4() {	// ILLEGAL_E4
	std::cout << "0xE4 ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xEB() {	// ILLEGAL_EB
	std::cout << "0xEB ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xEC() {	// ILLEGAL_EC
	std::cout << "0xEC ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xED() {	// ILLEGAL_ED
	std::cout << "0xED ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xF4() {	// ILLEGAL_F4
	std::cout << "0xF4 ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xFC() {	// ILLEGAL_FC
	std::cout << "0xFC ILLEGAL" << std::endl;
	return 0;
}
u8 CPU::opcode0xFD() {	// ILLEGAL_FD
	std::cout << "0xFD ILLEGAL" << std::endl;
	return 0;
}
