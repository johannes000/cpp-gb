//
// Created by knaub on 25.06.2020.
//

#ifndef CPP_GB_CONTROLS_H
#define CPP_GB_CONTROLS_H

#include "SDL.h"
#include "type.h"

class MEMORY;

class PPU;

class CONTROLS {
public:
    CONTROLS(MEMORY *m, PPU *ppu);

private:
    MEMORY *mem;
    PPU *ppu;

    u8 joy_p1_state = 0x00;
    u8 joy_both_states = 0x00;


public:
    void update_joypad();

    [[nodiscard]] u8 read(const u16 &addr) const;
};


#endif //CPP_GB_CONTROLS_H
