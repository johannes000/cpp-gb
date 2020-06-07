#include "gameboy.h"


gameboy::gameboy() {

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

}

