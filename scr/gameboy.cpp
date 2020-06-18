#include "gameboy.h"

#include <time.h>

#include <chrono>
#include <thread>

gameboy::gameboy() : cpu(&mem), mem(&cpu, &card, &ppu), ppu(&mem) {}

gameboy::~gameboy() {}

void gameboy::startemu() {
  int cycles = 0;
  int last_cycle = 0;
  mem.reset();
  card.loadROM("./testroms/tetris.gb");
  // cpu.printallops();

  while (true) {
    last_cycle = cpu.update();
    cycles += last_cycle;

    ppu.waitforinput();
    ppu.update(last_cycle);
    // SDL_Delay(20);
  }
}
