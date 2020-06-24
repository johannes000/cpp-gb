#pragma once
#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD

#include "CARDRIDGE.h"
#include "CPU.h"
#include "MEMORY.h"
#include "PPU.h"
#include "SDL.h"

class gameboy {
public:
    gameboy();

    ~gameboy();

    CARDRIDGE card;
    MEMORY mem;
    CPU cpu;
    PPU ppu;

    void start_emu();

private:
    bool step_mode{};
    bool show_opcode{};
    SDL_Event event{};
};

#endif  // !GAMEBOY_H_GUARD
