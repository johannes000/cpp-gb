#include "CPU.h"

CPU::CPU(MEMORY* memory) : mem(memory) {
	initRegister();
}

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



void CPU::print_last_opcode() {
	std::cout << "0x" << std::hex << std::uppercase << (int)(oldPC) << " "
		<< instruction_info.mnemonic << "\t" << (int)(OPCode);
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
		std::cout << "0x" << std::hex << std::uppercase << (int)(mypc) << "\t"
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

void CPU::handle_interrupt(u8 val) {
	write(INTERRUPT_ENABLE_REG, val);
	ie_flags.update(this);
	if_flags.update(this);
}


bool CPU::interrupts() {
	if (!IME) {
		return false;
	}
	return 0;
}
void CPU::IE_Flags::update(CPU* cpu) {
	u8 data = cpu->read(INTERRUPT_ENABLE_REG);
	JIR = check_bit(data, 4);
	SIR = check_bit(data, 3);
	TIR = check_bit(data, 2);
	LIR = check_bit(data, 1);
	VIR = check_bit(data, 0);
}

void CPU::IF_Flags::update(CPU* cpu) {
	u8 data = cpu->read(INTERRUPT_FLAGS_REG);
	JIE = check_bit(data, 4);
	SIE = check_bit(data, 3);
	TIE = check_bit(data, 2);
	LIE = check_bit(data, 1);
	VIE = check_bit(data, 0);
}
