#include "CPU.h"

CPU::CPU(MEMORY* memory) : mem(memory) { initRegister(); }

CPU::~CPU() {}

void CPU::initRegister() {
	R.AF = 0x01B0;
	R.BC = 0x0013;
	R.DE = 0x00D8;
	R.HL = 0x014D;
	R.SP = 0xFFFE;
	R.PC = 0x0000;
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
	if (interrupts()) std::cout << "INTERRUPT" << std::endl;
	if (IME_scheduled) IME = true;

	OPCode = getNextOPCode();
	u8 jp = parseOPCode(OPCode);

	print_last_opcode();
	if (jp == 0) incPC();
}

void CPU::incPC() { R.PC += instruction_info.bytes; }

u8 CPU::getNextOPCode() { return read(R.PC); }

void CPU::handle_interrupt() {}

bool CPU::interrupts() {
	if (!IME) {
		return false;
	}
	return (JIR_state() & JIE_state()) || (SIR_state() & SIE_state()) ||
		(TIR_state() & TIE_state()) || (LIR_state() & LIE_state()) ||
		(VIR_state() & VIE_state());
}

void CPU::print_last_opcode() {
	std::cout << "0x" << std::hex << std::uppercase << (int)(oldPC) << " "
		<< instruction_info.mnemonic;
	if (instruction_info.bytes == 3) std::cout << " " << (int)(read_u16_at_pc());
	if (instruction_info.bytes == 2) std::cout << " 0x" << (int)(read_u8_at_pc());
	std::cout << std::endl;
}

void CPU::printallops() {
	u16 mypc = 0x00;
	instruction myinstruction_info;
	while (mypc < 0x4000) {
		u8 myopcode = read(mypc);
		if (myopcode == 0xCB) {
			myopcode = read(++mypc);
			myinstruction_info = prefixedinstructions[myopcode];
		}
		else {
			myinstruction_info = instructions[myopcode];
		}
		std::cout << "0x" << std::hex << std::uppercase << (int)(mypc) << " "
			<< myinstruction_info.mnemonic;
		if (myinstruction_info.bytes == 3)
			std::cout << " " << (int)(read_u16_at_pc());
		if (myinstruction_info.bytes == 2)
			std::cout << " 0x" << (int)(read_u8_at_pc());
		std::cout << std::endl;
		mypc += myinstruction_info.bytes;
	}
}

bool CPU::in_boot_r() { return in_boot; }

u8 CPU::read(const u16& addr) { return mem->read(addr); }
void CPU::write(const u16& addr, const u8& data) {
	mem->write(addr, data);
}
void CPU::write_u16(const u16& addr, const u16& data) {
	mem->write_u16(addr, data);
}

// INTERRUPT FLAGS
bool CPU::JIR_state() { return check_bit(JIR, read(INTERRUPT_FLAGS)); };
bool CPU::SIR_state() { return check_bit(SIR, read(INTERRUPT_FLAGS)); };
bool CPU::TIR_state() { return check_bit(TIR, read(INTERRUPT_FLAGS)); };
bool CPU::LIR_state() { return check_bit(LIR, read(INTERRUPT_FLAGS)); };
bool CPU::VIR_state() { return check_bit(VIR, read(INTERRUPT_FLAGS)); };

bool CPU::JIE_state() { return check_bit(JIE, read(INTERRUPT_ENABLE)); };
bool CPU::SIE_state() { return check_bit(SIE, read(INTERRUPT_ENABLE)); };
bool CPU::TIE_state() { return check_bit(TIE, read(INTERRUPT_ENABLE)); };
bool CPU::LIE_state() { return check_bit(LIE, read(INTERRUPT_ENABLE)); };
bool CPU::VIE_state() { return check_bit(VIE, read(INTERRUPT_ENABLE)); };
