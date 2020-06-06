#include "CPU.h"

CPU::CPU() {
	std::cout << "CPU const" << std::endl;
}



void CPU::setFlag(const flag F) {
	AF.low |= (1 << F);
}
void CPU::clearFlag(const flag F) {
	AF.low &= ~(1 << F);
}
bool CPU::checkFlag(const flag F) {
	return (AF.low >> F) & 1;
}
void CPU::toggleFlag(const flag F) {
	AF.low ^= (1 << F);
}
void CPU::OPCODEONE() {
	setFlag(z_flag);
	checkFlag(z_flag);
	std::cout << "OP CODE ONE " << std::endl;

}

void CPU::parseOPCode(u8& opcode) {
	instruction const* instructioninfo = &instructions[opcode];

	switch (opcode) {
	case 0x00:
		std::cout << instructioninfo->mnemonic << " " << static_cast<int>(instructioninfo->bytes) << std::endl;
		break;

	default:
		break;
	}
}

