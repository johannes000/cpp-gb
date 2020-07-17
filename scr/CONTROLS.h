//
// Created by knaub on 25.06.2020.
//

#ifndef CPP_GB_CONTROLS_H
#define CPP_GB_CONTROLS_H

#include "SDL.h"
#include "type.h"
#include "MEMORY.h"
#include "../win/Renderer.h"

class MEMORY;

class PPU;

class CONTROLS {
public:
    CONTROLS(MEMORY *m, PPU *ppu);

    void write(const u16 &addr, const u8 &data);

    void handle_glwf_inputs(int key, int action);

private:
    MEMORY *mem;
    PPU *ppu;

    u8 joy_button_direction_selector = 0xFFu;
    u8 joy_both_states = 0xFFu;
    u8 joy_both_states_old = 0xFFu;


public:

    [[nodiscard]] u8 read(const u16 &addr);
};


#endif //CPP_GB_CONTROLS_H
