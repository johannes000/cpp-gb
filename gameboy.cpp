#include "gameboy.h"


gameboy::gameboy() {

}

void gameboy::startemu() {
	ppu.createWindow();
	ppu.renderScreen();
	cpu.OPCODEONE();
}

