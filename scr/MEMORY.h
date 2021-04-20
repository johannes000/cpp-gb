#ifndef MEMORY_H_GUARD
#define MEMORY_H_GUARD

#include <iostream>
#include <vector>
#include <string>

#include "CARDRIDGE.h"
#include "CPU.h"
#include "PPU.h"
#include "type.h"
#include "CONTROLS.h"

class CPU;

class PPU;

class CARDRIDGE;

class CONTROLS;

class MEMORY {
public:
	MEMORY(CPU *c, CARDRIDGE *card, PPU *p, CONTROLS *j);

	u8 read(const u16 &addr);

	u8 read_card_rom(const u16 &addr);

	void write(const u16 &addr, const u8 &data);

	void write_direct(const u16 &addr, const u8 &data);

	void write_u16(const u16 &addr, const u16 &data);

	void request_interrupt(Interrupt inter);

	std::string TestOutput;

private:
	CARDRIDGE *cardridge;
	PPU *ppu;
	CPU *cpu;
	CONTROLS *joypad;

	std::vector<u8> memory;
	std::vector<u8> boot;
	bool skip_boot = true;

public:
	void reset(bool);

	bool load_boot_rom();

	void handle_mapped_io_write(const u16 &addr, const u8 &data);

	u8 handle_mapped_io_read(const u16 &addr) const;
};

#endif // !MEMORY_H_GUARD
