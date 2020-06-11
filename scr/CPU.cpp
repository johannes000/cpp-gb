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
	R.PC = 0x0000;
}

u8 CPU::parseOPCode(u8& opcode) {
	u8 function;
	if (prefix) {
		prefix = false;
		instruction_info = prefixedinstructions[OPCode];
		instruction_info.bytes -= 1;
		function = (this->*prefixedinstructions[opcode].function)();
	}
	else {
		instruction_info = instructions[OPCode];
		function = (this->*instructions[opcode].function)();
	}
	return function;
}

void CPU::update() {
	if (interrupts())
		std::cout << "INTERRUPT" << std::endl;
	if (IME_scheduled)
		IME = true;

	OPCode = getNextOPCode();
	u8 jp = parseOPCode(OPCode);

	print_last_opcode();
	if (jp == 0) incPC();
}

void CPU::incPC() {
	R.PC += instruction_info.bytes;
}

u8 CPU::getNextOPCode() {
	return mem->read(R.PC);
}

void CPU::handle_interrupt() {
}


bool CPU::interrupts() {
	if (!IME)
		return false;
	return (JIR_state() & JIE_state()) || (SIR_state() & SIE_state()) || (TIR_state() & TIE_state()) || (LIR_state() & LIE_state()) || (VIR_state() & VIE_state());
}

void CPU::print_last_opcode() {
	std::cout << "0x" << std::hex << std::uppercase << (int)(OPCode) << " " << instruction_info.mnemonic;
	if (instruction_info.bytes == 3)
		std::cout << " " << (int)(read_u16_at_pc());
	if (instruction_info.bytes == 2)
		std::cout << " 0x" << (int)(read_u8_at_pc());
	print_flags();
	std::cout << std::endl;
}


void CPU::print_flags() {
	std::cout << " Z: " << check_flag(Flag::z);
}