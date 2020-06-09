#include "MEMORY.h"

MEMORY::MEMORY() {
	memory = std::vector<u8>(0xFFFF);

	// bootrom in memory
	std::copy(boot, (boot + 0xFF), memory.begin());
}

MEMORY::~MEMORY() {
}

u8 MEMORY::read(const u16& addr) {
	if (addr >= 0x00 && addr <= 0xFFFF)
		return memory[addr];
	return 0x00;
}
u8* MEMORY::read_ptr(const u16& addr) {
	if (addr >= 0x00 && addr <= 0xFFFF)
		return &memory[addr];
	return 0x00;
}
void MEMORY::write(const u16& addr, const u8& data) {
	if (addr >= 0x00 && addr <= 0xFFFF)
		memory[addr] = data;
}

void MEMORY::write_u16(const u16& addr, const u16& data) {
	if (addr >= 0x00 && (addr + 1) < 0xFFFF) {
		u8 data_lb = data >> 8;
		u8 data_hb = data & 0xFF;
		memory[addr] = data_hb;
		memory[addr + 1] = data_lb;
	}
	else {
		std::cout << "Addresse: " << addr << " out of Range(0x0000,0xFFFF)";
	}
}



