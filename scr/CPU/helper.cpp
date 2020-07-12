#include "../CPU.h"

u16 CPU::u8_to_u16(const u8 &data_lb, const u8 &data_hb) {
    return (data_lb << 8u) | (data_hb & 0xffu);
}

// Merged die Werte auf die PC + 1 und PC + 2 zeigt in ein u16
u16 CPU::read_u16_at_pc() { return u8_to_u16(read(R.PC + 2), read(R.PC + 1)); }

u16 CPU::read_u16_sp() { return u8_to_u16(read(R.SP + 1), read(R.SP)); }

i8 CPU::read_i8_at_pc() { return static_cast<i8>(read_u8_at_pc()); }

u8 CPU::read_u8_at_pc() { return static_cast<u8> (mem->read(R.PC + 1)); }

void CPU::set_flag_if(const Flag &F, bool condition) {
    condition ? set_flag(F) : reset_flag(F);
}

void CPU::set_flag(const Flag &F) { set_bit(R.F, static_cast<u8>(F)); }

void CPU::set_flags(const Flag &F1, const Flag &F2) {
    set_flag(F1);
    set_flag(F2);
}

void CPU::set_flags(const Flag &F1, const Flag &F2, const Flag &F3) {
    set_flag(F1);
    set_flag(F2);
    set_flag(F3);
}

void CPU::reset_flag(const Flag &F) { reset_bit(R.F, static_cast<u8>(F)); }

void CPU::reset_flags(const Flag &F1, const Flag &F2) {
    reset_flag(F1);
    reset_flag(F2);
}

void CPU::reset_flags(const Flag &F1, const Flag &F2, const Flag &F3) {
    reset_flag(F1);
    reset_flag(F2);
    reset_flag(F3);
}

void CPU::reset_all_flags() { R.F = 0x00; }

bool CPU::check_flag(const Flag &F) const {
    return check_bit(R.F, static_cast<u8>(F));
}

void CPU::toggle_flag(const Flag &F) { toggle_bit(R.F, static_cast<u8>(F)); }

