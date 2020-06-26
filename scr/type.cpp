#include "type.h"

void set_bit_if(u8 &value, const u8 &bit, bool condition) {
    condition ? set_bit(value, bit) : reset_bit(value, bit);
}

void set_bit_if(u16 &value, const u16 &bit, bool condition) {
    condition ? set_bit(value, bit) : reset_bit(value, bit);
}

void set_bit(u8 &value, const u8 &bit) { value |= (1u << bit); }

void set_bit(u16 &value, const u16 &bit) { value |= (1u << bit); }

void reset_bit(u8 &value, const u8 &bit) { value &= ~(1u << bit); }

void reset_bit(u16 &value, const u16 &bit) { value &= ~(1u << bit); }

void toggle_bit(u8 &value, const u8 &bit) { value ^= (1u << bit); }

void toggle_bit(u16 &value, const u16 &bit) { value ^= (1u << bit); }

bool check_bit(const u8 &value, const u8 &bit) { return static_cast<u8>(value >> bit) & 1u; }

bool check_bit(const u16 &value, const u16 &bit) { return static_cast<u16>(value >> bit) & 1u; }
