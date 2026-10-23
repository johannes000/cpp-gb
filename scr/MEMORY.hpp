#ifndef MEMORY_H_GUARD
#define MEMORY_H_GUARD

#include "CPU.hpp"
#include "PPU.hpp"
#include "CARTRIDGE.hpp"
#include "CONTROLS.hpp"
#include "APU.hpp"

class CPU;
class PPU;
class CARTRIDGE;
class CONTROLS;
class APU;

class MEMORY {
public:
	MEMORY(CPU *c, CARTRIDGE *card, PPU *p, CONTROLS *j, APU *a);

	auto read(const u16 &addr) -> u8;

	auto read_card_rom(const u16 &addr) -> u8;

	auto write(const u16 &addr, const u8 &data) -> void;

	auto write_direct(const u16 &addr, const u8 &data) -> void;

	auto write_u16(const u16 &addr, const u16 &data) -> void;

	auto request_interrupt(Interrupt inter) -> void;

	auto reset() -> void;

	std::string TestOutput_;

private:
	CARTRIDGE *card_;
	PPU *ppu_;
	CPU *cpu_;
	CONTROLS *joypad_;
	APU *apu_;

	std::vector<u8> memory_;
	bool skip_boot_ = false;

public:
	auto reset(bool) -> void;

	auto handle_mapped_io_write(const u16 &addr, const u8 &data) -> void;

	auto handle_mapped_io_read(const u16 &addr) const -> u8;
};

#endif // !MEMORY_H_GUARD
