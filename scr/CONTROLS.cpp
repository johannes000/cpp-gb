//
// Created by knaub on 25.06.2020.
//

#include "CONTROLS.h"
#include <SDL.h>

CONTROLS::CONTROLS(MEMORY *m, PPU *p) : mem(m), ppu(p) {}

void CONTROLS::update_joypad(SDL_Event &event) {

    switch (event.type) {
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    key_up(GB_Key::Up);
                    break;
                case SDLK_DOWN:
                    key_up(GB_Key::Down);
                    break;
                case SDLK_LEFT:
                    key_up(GB_Key::Left);
                    break;
                case SDLK_RIGHT:
                    key_up(GB_Key::Right);
                    break;
                case SDLK_a:
                    key_up(GB_Key::A);
                    break;
                case SDLK_b:
                    key_up(GB_Key::B);
                    break;
                case SDLK_KP_ENTER:
                    key_up(GB_Key::Start);
                    break;
                case SDLK_BACKSPACE:
                    key_up(GB_Key::Select);
                    break;
            }
            break;
        case SDLK_DOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    key_down(GB_Key::Up);
                    break;
                case SDLK_DOWN:
                    key_down(GB_Key::Down);
                    break;
                case SDLK_LEFT:
                    key_down(GB_Key::Left);
                    break;
                case SDLK_RIGHT:
                    key_down(GB_Key::Right);
                    break;
                case SDLK_a:
                    key_down(GB_Key::A);
                    break;
                case SDLK_b:
                    key_down(GB_Key::B);
                    break;
                case SDLK_KP_ENTER:
                    key_down(GB_Key::Start);
                    break;
                case SDLK_BACKSPACE:
                    key_down(GB_Key::Select);
                    break;
            }
        default:
            break;
    }
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

}

void CONTROLS::key_down(GB_Key key) {
    switch (key) {
        case GB_Key::A:
            reset_bit(joy_p1_state, 5);
            reset_bit(joy_p1_state, 0);
            break;
        case GB_Key::B:
            reset_bit(joy_p1_state, 5);
            reset_bit(joy_p1_state, 1);
            break;
        case GB_Key::Select:
            reset_bit(joy_p1_state, 5);
            reset_bit(joy_p1_state, 2);
            break;
        case GB_Key::Start:
            reset_bit(joy_p1_state, 5);
            reset_bit(joy_p1_state, 3);
            break;
        case GB_Key::Right:
            reset_bit(joy_p1_state, 4);
            reset_bit(joy_p1_state, 0);
            break;
        case GB_Key::Left:
            reset_bit(joy_p1_state, 4);
            reset_bit(joy_p1_state, 1);
            break;
        case GB_Key::Up:
            reset_bit(joy_p1_state, 4);
            reset_bit(joy_p1_state, 2);
            break;
        case GB_Key::Down:
            reset_bit(joy_p1_state, 4);
            reset_bit(joy_p1_state, 3);
            break;
    }

}

void CONTROLS::key_up(GB_Key key) {
    switch (key) {
        case GB_Key::A:
            set_bit(joy_p1_state, 5);
            set_bit(joy_p1_state, 0);
            break;
        case GB_Key::B:
            set_bit(joy_p1_state, 5);
            set_bit(joy_p1_state, 1);
            break;
        case GB_Key::Select:
            set_bit(joy_p1_state, 5);
            set_bit(joy_p1_state, 2);
            break;
        case GB_Key::Start:
            set_bit(joy_p1_state, 5);
            set_bit(joy_p1_state, 3);
            break;
        case GB_Key::Right:
            set_bit(joy_p1_state, 4);
            set_bit(joy_p1_state, 0);
            break;
        case GB_Key::Left:
            set_bit(joy_p1_state, 4);
            set_bit(joy_p1_state, 1);
            break;
        case GB_Key::Up:
            set_bit(joy_p1_state, 4);
            set_bit(joy_p1_state, 2);
            break;
        case GB_Key::Down:
            set_bit(joy_p1_state, 4);
            set_bit(joy_p1_state, 3);
            break;
    }
}

