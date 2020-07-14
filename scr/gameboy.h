#pragma once
#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD

#include "CARDRIDGE.h"
#include "CPU.h"
#include "MEMORY.h"
#include "PPU.h"
#include "CONTROLS.h"
#include "SDL.h"
#include "Debugger/Debugger.h"

class gameboy {
public:
    gameboy();

    ~gameboy();

    CARDRIDGE card;
    MEMORY mem;
    CPU cpu;
    PPU ppu;
    CONTROLS joypad;
    Debugger debug;

    [[noreturn]] void start_emu();

private:
    int last_cycle{};
    int cycles{};
    SDL_Event event{};
};

#endif  // !GAMEBOY_H_GUARD
