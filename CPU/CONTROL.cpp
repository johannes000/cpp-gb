#include "../CPU.h"

u8 CPU::opcode0x00() {	// NOP
	setBit(AF.low, static_cast<u8>(Flag::c));
	std::cout << "0x00 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x10() {	// STOP
	toggleFlag(Flag::z);
	std::cout << "Z Flag: " << checkFlag(Flag::z) << std::endl;
	toggleFlag(Flag::z);
	std::cout << "Z Flag: " << checkFlag(Flag::z) << std::endl;
	std::cout << "0x10 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x3F() {	// CCF
	std::cout << "0x3F nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0x2F() {	// CPL
	std::cout << "0x2F nicht implementiert" << std::endl;
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
u8 CPU::opcode0xC9() {	// RET
	std::cout << "0xC9 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xCB() {	// PREFIX
	std::cout << "0xCB nicht implementiert" << std::endl;
	return 0;
}


u8 CPU::opcode0xD3() {	// ILLEGAL_D3
	std::cout << "0xD3 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xDB() {	// ILLEGAL_DB
	std::cout << "0xDB nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xDD() {	// ILLEGAL_DD
	std::cout << "0xDD nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xE3() {	// ILLEGAL_E3
	std::cout << "0xE3 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xE4() {	// ILLEGAL_E4
	std::cout << "0xE4 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xEB() {	// ILLEGAL_EB
	std::cout << "0xEB nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xEC() {	// ILLEGAL_EC
	std::cout << "0xEC nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xED() {	// ILLEGAL_ED
	std::cout << "0xED nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xF4() {	// ILLEGAL_F4
	std::cout << "0xF4 nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xFC() {	// ILLEGAL_FC
	std::cout << "0xFC nicht implementiert" << std::endl;
	return 0;
}
u8 CPU::opcode0xFD() {	// ILLEGAL_FD
	std::cout << "0xFD nicht implementiert" << std::endl;
	return 0;
}
