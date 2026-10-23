#include "../CPU.hpp"

auto CPU::u8_to_u16(const u8 &data_lb, const u8 &data_hb) -> u16 {
	return (data_lb << 8u) | (data_hb & 0xffu);
}

// Merged die Werte auf die PC + 1 und PC + 2 zeigt in ein u16
auto CPU::read_u16_at_pc() -> u16 { return u8_to_u16(read(R_.PC_ + 2), read(R_.PC_ + 1)); }

auto CPU::read_u16_sp() -> u16 { return u8_to_u16(read(R_.SP_ + 1), read(R_.SP_)); }

auto CPU::read_i8_at_pc() -> s8 { return static_cast<s8>(read_u8_at_pc()); }

auto CPU::read_u8_at_pc() -> u8 { return static_cast<u8>(mem_->read(R_.PC_ + 1)); }

auto CPU::set_cpu_flag_if(const Flag &F, bool condition) -> void {
	condition ? set_flag(F) : reset_flag(F);
}

auto CPU::set_flag(const Flag &F) -> void { set_bit(R_.F_, static_cast<u8>(F)); }

auto CPU::set_flags(const Flag &F1, const Flag &F2) -> void {
	set_flag(F1);
	set_flag(F2);
}

auto CPU::set_flags(const Flag &F1, const Flag &F2, const Flag &F3) -> void {
	set_flag(F1);
	set_flag(F2);
	set_flag(F3);
}

auto CPU::reset_flag(const Flag &F) -> void { reset_bit(R_.F_, static_cast<u8>(F)); }

auto CPU::reset_flags(const Flag &F1, const Flag &F2) -> void {
	reset_flag(F1);
	reset_flag(F2);
}

auto CPU::reset_flags(const Flag &F1, const Flag &F2, const Flag &F3) -> void {
	reset_flag(F1);
	reset_flag(F2);
	reset_flag(F3);
}

auto CPU::reset_all_flags() -> void { R_.F_ = 0x00; }

auto CPU::check_flag(const Flag &F) const -> bool {
	return check_bit(R_.F_, static_cast<u8>(F));
}

auto CPU::toggle_flag(const Flag &F) -> void { toggle_bit(R_.F_, static_cast<u8>(F)); }
