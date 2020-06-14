#include "type.h"

void set_bit_if(u8& value, const u8& bit, bool condition) {
	condition ? set_bit(value, 0) : reset_bit(value, 0);
}

void set_bit_if(u16& value, const u16& bit, bool condition) {
	condition ? set_bit(value, 0) : reset_bit(value, 0);
}

void set_bit(u8& value, const u8& bit) {
	value |= (1 << bit);
}
void set_bit(u16& value, const u16& bit) {
	value |= (1 << bit);
}

void reset_bit(u8& value, const u8& bit) {
	value &= ~(1 << bit);
}
void reset_bit(u16& value, const u16& bit) {
	value &= ~(1 << bit);
}

void toggle_bit(u8& value, const u8& bit) {
	value ^= (1 << bit);
}
void toggle_bit(u16& value, const u16& bit) {
	value ^= (1 << bit);
}

bool check_bit(const u8& value, const u8& bit) {
	return (value >> bit) & 1;
}
bool check_bit(const u16& value, const u16& bit) {
	return (value >> bit) & 1;
}
