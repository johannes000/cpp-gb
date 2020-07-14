//
// Created by knaub on 25.06.2020.
//

#include "CONTROLS.h"
#include <SDL.h>
#include <iostream>

CONTROLS::CONTROLS(MEMORY *m, PPU *p) : mem(m), ppu(p), joy_button_direction_selector(0xFF) {}

u8 CONTROLS::read(const u16 &addr) {
    u8 joy_p1 = 0xFF;
    if (addr == JOY_P1_REGISTER) {
        switch (joy_button_direction_selector & 0xF0u) {
            case 0x10:
                joy_p1 = joy_both_states & 0x0Fu;
                reset_bit(joy_p1, 5);
                set_bit(joy_p1, 4);
                break;
            case 0x20:
                joy_p1 = (joy_both_states & 0xF0u) >> 4u;
                set_bit(joy_p1, 5);
                reset_bit(joy_p1, 4);
                break;
            default:
                return 0xFF;
        }
        return joy_p1;
    } else {
        return 0xFF;
    }
}

void CONTROLS::write(const u16 &addr, const u8 &data) {
    switch (addr) {
        case JOY_P1_REGISTER:
            joy_button_direction_selector = data & 0x30u;
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
    if (joy_both_states != joy_both_states_old) {
        joy_both_states_old = joy_both_states;
        mem->request_interrupt(Interrupt::Joypad);
    }
}



