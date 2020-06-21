#include "gameboy.h"

gameboy::gameboy() : mem(&cpu, &card, &ppu), cpu(&mem), ppu(&mem) {}

gameboy::~gameboy() {
  delete &mem;
  delete &card;
  delete &ppu;
  delete &cpu;
}

void gameboy::start_emu() {
  auto cycles = 0;
  auto last_cycle = 0;
  mem.reset();
  card.load_rom("./testroms/tetris.gb");
  // cpu.print_all_ops();

  while (true) {
    last_cycle = cpu.update();
    cycles += last_cycle;
    ppu.update(last_cycle * 4);
    // SDL_Delay(20);
  }
}
