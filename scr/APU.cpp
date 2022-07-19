#include "APU.h"

APU::APU(MEMORY *m) : mem(m) {
}

u8 APU::read(const u16 &addr) {
	return mem->read(addr);
}

void APU::write(const u16 &addr, const u8 &data) const {
	mem->write_direct(addr, data);
}

void APU::write_u16(const u16 &addr, const u16 &data) const {
	mem->write_u16(addr, data);
}

void APU::apu_view() {
	ImGui::Text("APU");
}
