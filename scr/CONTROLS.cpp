//
// Created by knaub on 25.06.2020.
//

#include "CONTROLS.h"
#include <SDL.h>
#include <iostream>

CONTROLS::CONTROLS(MEMORY *m, PPU *p) : mem(m), ppu(p), joy_p1_state(0xFF) {}

void CONTROLS::update_joypad() {
    joy_p1_state = 0x11;
    return;
    // Bit 7 - Not used
    // Bit 6 - Not used
    // Bit 5 - P15 Select Button Keys      (0=Select)
    // Bit 4 - P14 Select Direction Keys   (0=Select)
    // Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
    // Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
    // Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
    // Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
    joy_both_states = 0x00;
    auto *keystate = SDL_GetKeyboardState(nullptr);

    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::Up), keystate[SDL_SCANCODE_UP]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::Down), keystate[SDL_SCANCODE_DOWN]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::Left), keystate[SDL_SCANCODE_LEFT]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::Right), keystate[SDL_SCANCODE_RIGHT]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::A), keystate[SDL_SCANCODE_A]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::B), keystate[SDL_SCANCODE_S]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::Select), keystate[SDL_SCANCODE_BACKSPACE]);
    set_bit_if(joy_both_states, static_cast<u8>(GB_Key::Start), keystate[SDL_SCANCODE_RETURN]);


    u8 buttons = (joy_both_states & 0xF0u) >> 4u;
    u8 directions = joy_both_states & 0xFu;
    joy_p1_state |= buttons | directions;

    // Buttons
    set_bit_if(joy_p1_state, 5, (joy_both_states & 0xF0u) != 0x00u);
    // Directions
    set_bit_if(joy_p1_state, 4, (joy_both_states & 0x0Fu) == 0x00u);
}

u8 CONTROLS::read(const u16 &addr) const {
    switch (addr) {
        case JOY_P1_REGISTER:
            return joy_p1_state;
        default:
            return 0xFF;
    }
}

void CONTROLS::write(const u16 &addr, const u8 &data) {
    switch (addr) {
        case JOY_P1_REGISTER:
            joy_p1_state = data;
            break;
        default:
            mem->write(addr, data);
            break;
    }
}



