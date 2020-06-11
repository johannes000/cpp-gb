#include "MEMORY.h"

MEMORY::MEMORY() {
	memory.fill(0);
	// bootrom in memory
	std::copy(boot.begin(), (boot.begin() + 0xFF), memory.begin());

	memory[0xFF05] = 0x00; // TIMA
	memory[0xFF06] = 0x00; // TMA
	memory[0xFF07] = 0x00; // TAC
	memory[0xFF10] = 0x80; // NR10
	memory[0xFF11] = 0xBF; // NR11
	memory[0xFF12] = 0xF3; // NR12
	memory[0xFF14] = 0xBF; // NR14
	memory[0xFF16] = 0x3F; // NR21
	memory[0xFF17] = 0x00; // NR22
	memory[0xFF19] = 0xBF; // NR24
	memory[0xFF1A] = 0x7F; // NR30
	memory[0xFF1B] = 0xFF; // NR31
	memory[0xFF1C] = 0x9F; // NR32
	memory[0xFF1E] = 0xBF; // NR33
	memory[0xFF20] = 0xFF; // NR41
	memory[0xFF21] = 0x00; // NR42
	memory[0xFF22] = 0x00; // NR43
	memory[0xFF23] = 0xBF; // NR30
	memory[0xFF24] = 0x77; // NR50
	memory[0xFF25] = 0xF3; // NR51
	memory[0xFF26] = 0xF1; // - GB, $F0 - SGB; NR52
	memory[0xFF40] = 0x91; // LCDC
	memory[0xFF42] = 0x00; // SCY
	memory[0xFF43] = 0x00; // SCX
	memory[0xFF45] = 0x00; // LYC
	memory[0xFF47] = 0xFC; // BGP
	memory[0xFF48] = 0xFF; // OBP0
	memory[0xFF49] = 0xFF; // OBP1
	memory[0xFF4A] = 0x00; // WY
	memory[0xFF4B] = 0x00; // WX
	memory[0xFFFF] = 0x00; // IE
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



