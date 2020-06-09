#include "../CPU.h"



u16 CPU::merge_u8(const u8& data_lb, const u8& data_hb) {
	return (data_lb << 8) | (data_hb & 0xff);
}

void CPU::ld_8_8(u8& destination, u8& data) {
}

void CPU::ld_16_16(u16& destination, const u8& datalb, const u8& datahb) {

}

void CPU::ld_16_8(u16& destination, const u8& data) {
	destination = static_cast<u16>(data);
}



u8 CPU::xor_8(u8& data1, const u8& data2) {
	u8 xordata = data1 ^ data2;
	if (xordata == 0)
		setFlag(Flag::z);
	setFlags(Flag::n, Flag::h, Flag::c);
	return xordata;
}

