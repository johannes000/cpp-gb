#include "CPU.h"


CPU::CPU() {}

CPU::CPU(MEMORY* memory) {
	initRegister();
	mem = memory;

}

void CPU::initRegister() {
	R.AF = 0x01B0;
	R.BC = 0x0013;
	R.DE = 0x00D8;
	R.HL = 0x014D;
	R.SP = 0xFFFE;
	R.PC = 0x0100;
}

u8 CPU::parseOPCode(u8& opcode) {
	u8 function;
	if (prefix) {
		prefix = false;
		instruction_info = prefixedinstructions[OPCode];
		function = (this->*prefixedinstructions[opcode].function)();
	}
	else {
		instruction_info = instructions[OPCode];
		function = (this->*instructions[opcode].function)();
	}
	std::cout << "0x" << std::hex << std::uppercase << (int)(R.PC) << " " << instruction_info.mnemonic;
	if (instruction_info.bytes == 3)
		std::cout << " " << (int)(read_u16_at_pc());
	if (instruction_info.bytes == 2)
		std::cout << " 0x" << (int)(read_u8_at_pc());
	std::cout << std::endl;
	return function;
}

void CPU::update() {
	OPCode = getNextOPCode();
	u8 jp = parseOPCode(OPCode);
	if (jp == 1) {
		R.PC++;
	}
	else {
		incPC();
	}
}

void CPU::incPC() {
	R.PC += instruction_info.bytes;
}

u8 CPU::getNextOPCode() {
	return mem->read(R.PC);
}


