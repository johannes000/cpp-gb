#include "CPU.h"


CPU::CPU() {}

CPU::CPU(MEMORY* memory) {
	initRegister();
	mem = memory;

	IR = mem->read_ptr(A_interrupt_enable);
	IE = mem->read_ptr(A_interrupt_flags);
}

void CPU::initRegister() {
	R.AF = 0x01B0;
	R.BC = 0x0013;
	R.DE = 0x00D8;
	R.HL = 0x014D;
	R.SP = 0xFFFE;
	R.PC = 0x0100;
	OPCode = 0x00;
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
	oldPC = R.PC;
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
	if (!IME) {
		if ((*IE & *IR & 0x1F) == 0)
			return false;
		else
			return true;
	}
	return (JIR_state() & JIE_state()) || (SIR_state() & SIE_state()) || (TIR_state() & TIE_state()) || (LIR_state() & LIE_state()) || (VIR_state() & VIE_state());
}

void CPU::print_last_opcode() {
	std::cout << "0x" << std::hex << std::uppercase << (int)(oldPC) << " " << instruction_info.mnemonic;
	if (instruction_info.bytes == 3)
		std::cout << " " << (int)(read_u16_at_pc());
	if (instruction_info.bytes == 2)
		std::cout << " 0x" << (int)(read_u8_at_pc());
	std::cout << std::endl;
}

void CPU::printallops() {
	u16 mypc = 0x00;
	instruction myinstruction_info;
	while (mypc < 0x4000) {
		u8 myopcode = mem->read(mypc);
		if (myopcode == 0xCB) {
			myopcode = mem->read(++mypc);
			myinstruction_info = prefixedinstructions[myopcode];
		}
		else {
			myinstruction_info = instructions[myopcode];
		}
		std::cout << "0x" << std::hex << std::uppercase << (int)(mypc) << " " << myinstruction_info.mnemonic;
		if (myinstruction_info.bytes == 3)
			std::cout << " " << (int)(read_u16_at_pc());
		if (myinstruction_info.bytes == 2)
			std::cout << " 0x" << (int)(read_u8_at_pc());
		std::cout << std::endl;
		mypc += myinstruction_info.bytes;
	}
}
// INTERRUPT FLAGS
bool CPU::JIR_state() { return check_bit(JIR, mem->read(A_interrupt_flags)); };
bool CPU::SIR_state() { return check_bit(SIR, mem->read(A_interrupt_flags)); };
bool CPU::TIR_state() { return check_bit(TIR, mem->read(A_interrupt_flags)); };
bool CPU::LIR_state() { return check_bit(LIR, mem->read(A_interrupt_flags)); };
bool CPU::VIR_state() { return check_bit(VIR, mem->read(A_interrupt_flags)); };

bool CPU::JIE_state() { return check_bit(JIE, mem->read(A_interrupt_enable)); };
bool CPU::SIE_state() { return check_bit(SIE, mem->read(A_interrupt_enable)); };
bool CPU::TIE_state() { return check_bit(TIE, mem->read(A_interrupt_enable)); };
bool CPU::LIE_state() { return check_bit(LIE, mem->read(A_interrupt_enable)); };
bool CPU::VIE_state() { return check_bit(VIE, mem->read(A_interrupt_enable)); };