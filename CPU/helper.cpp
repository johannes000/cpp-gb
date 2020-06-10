#include "../CPU.h"



u16 CPU::u8_to_u16(const u8& data_lb, const u8& data_hb) {
	return (data_lb << 8) | (data_hb & 0xff);
}

// Merged die Werte auf die PC + 1 und PC + 2 zeigt in ein u16
u16 CPU::read_u16_at_pc() {
	return u8_to_u16(mem->read(R.PC + 2), mem->read(R.PC + 1));
}
u16 CPU::read_u16_sp() {
	return u8_to_u16(mem->read(R.SP + 1), mem->read(R.SP));
}
i8 CPU::read_i8_at_pc() {
	return (i8)(read_u8_at_pc());
}
u8 CPU::read_u8_at_pc() {
	return mem->read(R.PC + 1);
}


void CPU::set_Flag_if(const Flag& F, bool condition) {
	condition ? setFlag(F) : clearFlag(F);
}

void CPU::setFlag(const Flag& F) {
	setBit(R.F, static_cast<u8>(F));

}
void CPU::setFlags(const Flag& F1, const Flag& F2) {
	setFlag(F1);
	setFlag(F2);
}
void CPU::setFlags(const Flag& F1, const Flag& F2, const Flag& F3) {
	setFlag(F1);
	setFlag(F2);
	setFlag(F3);
}
void CPU::setAllFlags() {
	R.F = 0xFF;
}

void CPU::clearFlag(const Flag& F) {
	clearBit(R.F, static_cast<u8>(F));
}
void CPU::clearFlags(const Flag& F1, const Flag& F2) {
	clearFlag(F1);
	clearFlag(F2);
}
void CPU::clearFlags(const Flag& F1, const Flag& F2, const Flag& F3) {
	clearFlag(F1);
	clearFlag(F2);
	clearFlag(F3);
}
void CPU::clearAllFlags() {
	R.F = 0x00;
}

bool CPU::checkFlag(const Flag& F) {
	return checkBit(R.F, static_cast<u8>(F));
}

void CPU::toggleFlag(const Flag& F) {
	toggleBit(R.F, static_cast<u8>(F));
}

void CPU::toggleFlags(const Flag& F1, const Flag& F2) {
	toggleFlag(F1);
	toggleFlag(F2);
}
void CPU::toggleFlags(const Flag& F1, const Flag& F2, const Flag& F3) {
	toggleFlag(F1);
	toggleFlag(F2);
	toggleFlag(F3);
}
void CPU::toggleAllFlags() {
	toggleFlag(Flag::c);
	toggleFlag(Flag::z);
	toggleFlag(Flag::n);
	toggleFlag(Flag::h);
}


void CPU::set_Bit_if(u8& value, const u8 bit, bool condition) {
	condition ? setBit(value, 0) : clearBit(value, 0);
}

void CPU::set_Bit_if(u16& value, const u16 bit, bool condition) {
	condition ? setBit(value, 0) : clearBit(value, 0);
}

void CPU::setBit(u8& value, const u8 bit) {
	value |= (1 << bit);
}
void CPU::setBit(u16& value, const u16 bit) {
	value |= (1 << bit);
}

void CPU::clearBit(u8& value, const u8 bit) {
	value &= ~(1 << bit);
}
void CPU::clearBit(u16& value, const u16 bit) {
	value &= ~(1 << bit);
}

void CPU::toggleBit(u8& value, const u8 bit) {
	value ^= (1 << bit);
}
void CPU::toggleBit(u16& value, const u16 bit) {
	value ^= (1 << bit);
}

bool CPU::checkBit(const u8& value, const u8 bit) {
	return (value >> bit) & 1;
}
bool CPU::checkBit(const u16& value, const u16 bit) {
	return (value >> bit) & 1;
}