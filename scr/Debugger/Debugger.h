#ifndef CPP_GB_DEBUGGER_H
#define CPP_GB_DEBUGGER_H

#include <cstdio>
#include <cstdlib>

#include <map>

#include "imgui.h"

#include "CPU.h"
#include "PPU.h"
#include "MEMORY.h"
#include "CARTRIDGE.h"
#include "CONTROLS.h"
#include "APU.h"

#include <utl/type.h>

enum class Debug_State {
	STEP,
	RUN,
	BREAKPOINT
};

class CPU;
class PPU;
class MEMORY;
class CARTRIDGE;
class CONTROLS;
class APU;

class Debugger {
public:
	explicit Debugger(CPU *c, PPU *p, MEMORY *m, CARTRIDGE *card, CONTROLS *joy, APU *a);

	void reset();

	void disassemble();

	void handle_breakpoints();

	bool hit_breakpoint;

private:
	CPU *cpu;
	PPU *ppu;
	MEMORY *mem;
	CARTRIDGE *card;
	CONTROLS *joypad;
	APU *apu;

	Debug_State debug_state;

	bool next_step;

public:
	void setDebugState(Debug_State debugState) { debug_state = debugState; }
	Debug_State get_debug_state() const { return debug_state; }

	bool get_next_step() const { return next_step; }

	void update_views();

	void card_view();

	void cpu_view();

	void memory_view();

	void control_view();

private:
	const char *what_register_is_this(const u8 &reg);

	const char *what_register_is_this(const u16 &reg);

	std::vector<debug_instruction> dis_vec;
	std::vector<u16> dis_addr_vec;

	std::vector<u16> breakpoints;

	const ImVec4 bg_color = {0.8f, 0.8f, 0.8f, 1.0f};
	ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 magenta = ImVec4(1.0f, 0.502f, 0.957f, 1.0f);
	ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 red = ImVec4(1.0f, 0.149f, 0.447f, 1.0f);
	ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 gray = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 dark_gray = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
};

#endif // CPP_GB_DEBUGGER_H
