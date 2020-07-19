#include "gameboy.h"


gameboy::gameboy() :
        mem(&cpu, &card, &ppu, &joypad),
        cpu(&mem), ppu(&mem, &cpu),
        joypad(&mem),
        debug(&cpu, &ppu, &mem, &card, &joypad) {}




