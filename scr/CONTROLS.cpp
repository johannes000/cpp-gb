//
// Created by knaub on 25.06.2020.
//

#include "CONTROLS.h"
#include <SDL.h>
#include <iostream>

CONTROLS::CONTROLS(MEMORY *m, PPU *p) : mem(m), ppu(p), joy_p1_state(0xFF) {}

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

void CONTROLS::process_sdl_events(SDL_Event *event) {
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_KEYUP:
                switch (event->key.keysym.sym) {
                    case SDLK_UP:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::Up));
                        break;
                    case SDLK_DOWN:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::Down));
                        break;
                    case SDLK_LEFT:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::Left));
                        break;
                    case SDLK_RIGHT:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::Right));
                        break;
                    case SDLK_a:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::A));
                        break;
                    case SDLK_s:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::B));
                        break;
                    case SDLK_RETURN:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::Start));
                        break;
                    case SDLK_BACKSPACE:
                        set_bit(joy_both_states, static_cast<u8>(GB_Key::Select));
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_UP:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::Up));
                        break;
                    case SDLK_DOWN:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::Down));
                        break;
                    case SDLK_LEFT:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::Left));
                        break;
                    case SDLK_RIGHT:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::Right));
                        break;
                    case SDLK_a:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::A));
                        break;
                    case SDLK_s:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::B));
                        break;
                    case SDLK_RETURN:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::Start));
                        break;
                    case SDLK_BACKSPACE:
                        reset_bit(joy_both_states, static_cast<u8>(GB_Key::Select));
                        break;
                    case SDLK_ESCAPE:
                        exit(0);
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                exit(0);
            default:
                break;
        }
    }
    update_joypad();
}

void CONTROLS::update_joypad() {
    u8 buttons = (joy_both_states & 0xF0u) >> 4u;
    u8 directions = joy_both_states & 0xFu;
    joy_p1_state = buttons & directions;

    // Buttons
    set_bit_if(joy_p1_state, 5, (joy_both_states & 0xF0u) == 0xF0u);
    // Directions
    set_bit_if(joy_p1_state, 4, (joy_both_states & 0x0Fu) == 0x0Fu);
    set_bit(joy_p1_state, 6);
    set_bit(joy_p1_state, 7);
    if (joy_p1_state != joy_p1_state_old) {
        joy_p1_state_old = joy_p1_state;
        mem->request_interrupt(Interrupt::Joypad);
    }
}



