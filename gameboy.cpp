#include "gameboy.h"


gameboy::gameboy() {

}

void gameboy::startemu() {
	ppu.createWindow();
	ppu.renderScreen();
	u8 a = 0;
	cpu.parseOPCode(a);
}

