#ifndef MEMORY_H_GUARD
#define MEMORY_H_GUARD

#include "CPU.h"
#include "PPU.h"
#include "CARTRIDGE.h"
#include "CONTROLS.h"
#include "APU.h"

class CPU;
class PPU;
class CARTRIDGE;
class CONTROLS;
class APU;

class MEMORY {
public:
	MEMORY(CPU *c, CARTRIDGE *card, PPU *p, CONTROLS *j, APU *a);

	u8 read(const u16 &addr);

	u8 read_card_rom(const u16 &addr);

	void write(const u16 &addr, const u8 &data);

	void write_direct(const u16 &addr, const u8 &data);

	void write_u16(const u16 &addr, const u16 &data);

	void request_interrupt(Interrupt inter);

	void reset();

	std::string TestOutput;

private:
	CARTRIDGE *card;
	PPU *ppu;
	CPU *cpu;
	CONTROLS *joypad;
	APU *apu;

	std::vector<u8> memory;
	bool skip_boot = false;

public:
	void reset(bool);

	void handle_mapped_io_write(const u16 &addr, const u8 &data);

	u8 handle_mapped_io_read(const u16 &addr) const;
};

#endif // !MEMORY_H_GUARD
