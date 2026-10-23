#ifndef CPP_GB_DEBUGGER_H
#define CPP_GB_DEBUGGER_H

#include <cstdio>
#include <cstdlib>

#include <map>

#include "imgui.h"

#include "CPU.hpp"
#include "PPU.hpp"
#include "MEMORY.hpp"
#include "CARTRIDGE.hpp"
#include "CONTROLS.hpp"
#include "APU.hpp"

#include <utl/type.hpp>

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

	auto reset() -> void;

	auto disassemble() -> void;

	auto handle_breakpoints() -> void;

	bool hit_breakpoint_;

private:
	CPU *cpu_;
	PPU *ppu_;
	MEMORY *mem_;
	CARTRIDGE *card_;
	CONTROLS *joypad_;
	APU *apu_;

	Debug_State debug_state_;

	bool next_step_;

public:
	auto setDebugState(Debug_State debugState) -> void { debug_state_ = debugState; }
	auto get_debug_state() const -> Debug_State { return debug_state_; }

	auto get_next_step() const -> bool { return next_step_; }

	auto update_views() -> void;

	auto card_view() -> void;

	auto cpu_view() -> void;

	auto memory_view() -> void;

	auto control_view() -> void;

private:
	auto what_register_is_this(const u8 &reg) -> const char *;

	auto what_register_is_this(const u16 &reg) -> const char *;

	std::vector<debug_instruction> dis_vec_;
	std::vector<u16> dis_addr_vec_;

	std::vector<u16> breakpoints_;

	const ImVec4 bg_color_ = {0.8f, 0.8f, 0.8f, 1.0f};
	ImVec4 cyan_ = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 magenta_ = ImVec4(1.0f, 0.502f, 0.957f, 1.0f);
	ImVec4 yellow_ = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 red_ = ImVec4(1.0f, 0.149f, 0.447f, 1.0f);
	ImVec4 green_ = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 white_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 gray_ = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 dark_gray_ = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
};

#endif // CPP_GB_DEBUGGER_H
