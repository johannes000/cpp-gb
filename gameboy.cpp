#include "gameboy.h"


gameboy::gameboy() {
}

gameboy::~gameboy() {
}

void gameboy::startemu() {
	ppu.createWindow();
	ppu.renderScreen();
	u8 a = 0;
	u8 b = 0x10;
	u8 c = 0xFF;
	cpu.parseOPCode(a);
	cpu.parseOPCode(b);
	cpu.parseOPCode(c);
	cpu.setFlag(Flag::n);

	mem.write(0x1234, 0x33);
	std::cout << std::hex << "mem: " << mem.read(0x01) << std::endl;


	rom.loadROM("C:\/Users\/knaub\/source\/repos\/cpp-gb\/testroms\/cpu_instrs\/cpu_instrs.gb");
}

