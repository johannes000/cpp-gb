#include "CPU.h"

CPU::CPU() {
	sp = 0x110;
	pc = 0x00;
	std::cout << "CPU const" << std::endl;
}
void CPU::setFlag(flag F) {
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
	clearFlag(z_flag);
	checkFlag(z_flag);
	toggleFlag(n_flag);
	checkFlag(n_flag);

	std::cout << "OP CODE ONE " << std::endl;
}
