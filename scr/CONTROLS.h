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
    PPU *ppu;
    MEMORY *mem;


    void key_up(GB_Key key);

    void key_down(GB_Key key);

    u8 joy_p1_state{};

public:
    void update_joypad(SDL_Event &event);

    [[nodiscard]] u8 read(const u16 &addr) const;

    void write(const u16 &addr, const u8 &data);
};


#endif //CPP_GB_CONTROLS_H
