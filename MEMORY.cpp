#include "MEMORY.h"

MEMORY::MEMORY() {
	memory = std::vector<u8>(0xFFFF);

	// bootrom in memory
	std::copy(boot, (boot + 0xFF), memory.begin());
}

MEMORY::~MEMORY() {
	delete[] boot;
}

u8 MEMORY::read(const u16 addr) {
	if (addr >= 0x00 && addr <= 0xFFFF)
		return memory[addr];
	return 0x00;
}

void MEMORY::write(const u16 addr, u8 data) {
	if (addr >= 0x00 && addr <= 0xFFFF)
		memory[addr] = data;
}
