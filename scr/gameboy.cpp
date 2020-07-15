#include "gameboy.h"


gameboy::gameboy() : mem(&cpu, &card, &ppu, &joypad), cpu(&mem), ppu(&mem, &cpu), joypad(&mem, &ppu),
                     debug(&cpu, &ppu, &mem, &card) {}




