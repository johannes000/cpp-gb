//
// Created by knaub on 26.06.2020.
//
#include "Debugger.h"

#include <algorithm>
#include <vector>

Debugger::Debugger(CPU *c, PPU *p, MEMORY *m, CARDRIDGE *card, CONTROLS *joy)
	: hit_breakpoint(false),
	  cpu(c),
	  ppu(p),
	  mem(m),
	  cardridge(card),
	  joypad(joy),
	  debug_state(Debug_State::RUN),
	  next_step(false) {
	// breakpoints.push_back(0x100);
}

Debugger::~Debugger() {}

void Debugger::handle_breakpoints() {
	next_step = false;
	if (debug_state != Debug_State::STEP) {
		if (!breakpoints.empty()) {
			u16 pc = cpu->get_pc();
			auto iter = std::find(breakpoints.begin(), breakpoints.end(), pc);
			if (iter != breakpoints.end()) {
				debug_state = Debug_State::STEP;
			}
		}
		return;
	}
}

void Debugger::update_views() {
	card_view();
	cpu_view();
	memory_view();
	control_view();
}

void Debugger::card_view() {
	ImGui::Begin("Rominfo");

	ImGui::Text("%s", cardridge->rom_info.title);
	ImGui::Separator();

	ImGui::Text("CType: %s", cardridge->rom_info.CType_string());
	ImGui::SameLine();
	ImGui::Text("Rom Bank: %d", cardridge->get_selected_rom_bank());
	ImGui::Text("Ram Size: %s", cardridge->rom_info.ram_size_string());
	ImGui::SameLine();
	ImGui::Text("| Rom Size: %s", cardridge->rom_info.rom_size_string());

	ImGui::End();
}

void Debugger::cpu_view() {
	ImGui::Begin("CPU");

	bool goto_pc = ImGui::Button("Goto PC");
	ImGui::SameLine();
	if (ImGui::Button("Step")) {
		debug_state = Debug_State::STEP;
		next_step = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Run")) {
		debug_state = Debug_State::RUN;
		next_step = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Breakpoint")) {
		debug_state = Debug_State::BREAKPOINT;
		next_step = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("ReDisassemble")) {
		disassemble();
	}
	ImGui::SameLine();

	switch (debug_state) {
		case Debug_State::STEP:
			ImGui::Text("STEP");
			break;
		case Debug_State::RUN:
			ImGui::Text("RUN");
			break;
		case Debug_State::BREAKPOINT:
			ImGui::Text("Breakpoint");
			break;
	}

	// DECOMPILER
	ImGui::Separator();
	ImGui::BeginChild("##scrollingregion", ImVec2(0, 400));

	// DISASSAMBLER
	auto pc = cpu->get_pc();
	ImGuiListClipper clipper(dis_vec.size(),
							 ImGui::GetTextLineHeightWithSpacing());

	auto itr = std::find(dis_addr_vec.begin(), dis_addr_vec.end(), pc);
	auto pc_offset = std::distance(dis_addr_vec.begin(), itr);

	if (goto_pc) {
		ImGui::SetScrollFromPosY(
			ImGui::GetCursorStartPos().y +
				(pc_offset * ImGui::GetTextLineHeightWithSpacing()),
			0.5f);
	}
	while (clipper.Step()) {
		for (int i = clipper.DisplayStart; i <= clipper.DisplayEnd; i++) {
			auto inst = dis_vec[i];

			if (ImGui::Selectable("", true, ImGuiSelectableFlags_AllowDoubleClick))
				breakpoints.push_back(inst.address);
			ImGui::SameLine();
			if (inst.address == pc) {
				ImGui::SetItemDefaultFocus();
				ImGui::TextColored(red, "%04X", inst.address);
				ImGui::SameLine();
				switch (inst.bytes) {
					case 1:
						ImGui::TextColored(gray, "%02X\t\t\t", inst.opcode);
						break;
					case 2:
						ImGui::TextColored(gray, "%02X %02X\t\t ", inst.opcode,
										   inst.intermediate & 0xFFu);
						break;
					case 3:
						ImGui::TextColored(gray, "%02X %02X %02X\t  ", inst.opcode,
										   inst.intermediate & 0xFF00u,
										   inst.intermediate & 0x00FFu);
						break;
				}
				ImGui::SameLine();
				ImGui::TextColored(red, "->%s", inst.mnemonic);
				if (inst.bytes > 2) {
					ImGui::SameLine();
					ImGui::TextColored(red, "%s",
									   what_register_is_this(inst.intermediate));
				} else if (inst.bytes > 1) {
					ImGui::SameLine();
					ImGui::TextColored(red, "%s",
									   what_register_is_this(static_cast<u16>(
										   inst.intermediate | 0xFF00u)));
				}
			} else {
				ImGui::TextColored(cyan, "%04X", inst.address);
				ImGui::SameLine();
				switch (inst.bytes) {
					case 1:
						ImGui::TextColored(gray, "%02X\t\t\t", inst.opcode);
						break;
					case 2:
						ImGui::TextColored(gray, "%02X %02X\t\t ", inst.opcode,
										   inst.intermediate & 0xFFu);
						break;
					case 3:
						ImGui::TextColored(gray, "%02X %02X %02X\t  ", inst.opcode,
										   (inst.intermediate & 0xFFu) >> 4u,
										   inst.intermediate & 0x00FFu);
						break;
				}
				ImGui::SameLine();
				ImGui::TextColored(cyan, "%s", inst.mnemonic);
				if (inst.bytes > 2) {
					ImGui::SameLine();
					ImGui::TextColored(gray, "%s",
									   what_register_is_this(inst.intermediate));
				} else if (inst.bytes > 1) {
					ImGui::SameLine();
					ImGui::TextColored(gray, "%s",
									   what_register_is_this(static_cast<u16>(
										   inst.intermediate | 0xFF00u)));
				}
			}
			// i += inst.bytes;
		}
	}
	clipper.End();
	ImGui::EndChild();

	// Breakpoints
	ImGui::BeginChild("##ScrollingRegion");

	std::sort(breakpoints.begin(), breakpoints.end());
	ImGui::Text("Breakpoints");

	auto b = breakpoints.begin();
	auto e = breakpoints.end();

	while (b != e) {
		if (*b == pc)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));
		char address[32];
		sprintf(address, "%X", *b);
		if (ImGui::Selectable(address, true, ImGuiSelectableFlags_SpanAllColumns))
			breakpoints.erase(b);

		if (*b == pc)
			ImGui::PopStyleColor(1);
		b++;
	}

	ImGui::EndChild();
	ImGui::End();
}

void Debugger::reset() {}

void Debugger::disassemble() {
	debug_instruction debug_inst;
	dis_vec.erase(dis_vec.begin(), dis_vec.end());
	dis_addr_vec.erase(dis_addr_vec.begin(), dis_addr_vec.end());
	for (u16 addr = 0; addr < 0xFFFF;) {
		// Nur Adressen aus dem Card Rom nicht aus dem Bootrom
		// Deswegen kriegt man beim Booten nicht die richtigen Adressen im CPU View
		debug_inst = cpu->get_instruction_at_addr(addr);
		if (debug_inst.prefixed)
			addr++;
		dis_vec.push_back(debug_inst);
		dis_addr_vec.push_back(addr);
		addr += debug_inst.bytes;
	}
}

void Debugger::control_view() {
	ImGui::Begin("Control");
	CPU_Register cpu_regs = cpu->get_register();

	ImGui::Text("AF = %04X", cpu_regs.AF);
	ImGui::SameLine();
	ImGui::Text("DE = %04X", cpu_regs.DE);
	ImGui::SameLine();
	ImGui::Text("PC = %04X", cpu_regs.PC);
	ImGui::Text("BC = %04X", cpu_regs.BC);
	ImGui::SameLine();
	ImGui::Text("HL = %04X", cpu_regs.HL);
	ImGui::SameLine();
	ImGui::Text("SP = %04X", cpu_regs.SP);

	ImGui::Separator();

	ImGui::Text("Z %1d", cpu->check_flag(Flag::z));
	ImGui::SameLine();
	ImGui::Text("N %1d", cpu->check_flag(Flag::n));
	ImGui::SameLine();
	ImGui::Text("H %1d", cpu->check_flag(Flag::h));
	ImGui::SameLine();
	ImGui::Text("C %1d", cpu->check_flag(Flag::c));
	ImGui::SameLine();
	ImGui::Text("(HL) %02X", cpu->read(cpu_regs.HL));

	LCD_Register lcd_regs = ppu->get_lcd_register();
	ImGui::Separator();

	ImGui::Text("IME %X", cpu->is_IME());

	ImGui::Text("Interrupt Request");
	for (int i = 0; i < 5; ++i) {
		ImGui::SameLine();
		ImGui::Text("%X", check_bit(mem->read(INTERRUPT_FLAGS_REG), i));
	}
	ImGui::Text("Interrupt Enable ");
	for (int i = 0; i < 5; ++i) {
		ImGui::SameLine();
		ImGui::Text("%X", check_bit(mem->read(INTERRUPT_ENABLE_REG), i));
	}

	ImGui::Separator();
	ImGui::Text(" %d", lcd_regs.LCD_LY);
	ImGui::SameLine();
	ImGui::Text("JOY ");
	u8 joy = joypad->get_joypad_state();
	for (int i = 0; i < 8; ++i) {
		ImGui::SameLine();
		ImGui::Text("%1d", check_bit(joy, i));
	}
	ImGui::Separator();
	ImGui::Text(" TIMER ");
	ImGui::Text("DIV  %X", mem->read(DIVIDER_REGISTER));
	ImGui::SameLine();
	ImGui::Text("TIMA %X", mem->read(TIMER_COUNTER));
	ImGui::Text("TMA  %X", mem->read(TIMER_MODULO));
	ImGui::SameLine();
	ImGui::Text("TAC  %X", mem->read(TIMER_CONTROL));

	ImGui::End();
}

void Debugger::memory_view() {
	ImGui::Begin("Memory View");

	ImGui::BeginChild("##scrollingregion", ImVec2(0, 300));
	ImGuiListClipper clipper(0xFFFF / 0x10,
							 ImGui::GetTextLineHeightWithSpacing());
	for (int mem_line = clipper.DisplayStart; mem_line <= clipper.DisplayEnd;
		 mem_line++) {
		int mem_line_addr = mem_line * 0x10;
		ImGui::Text("0x%04X ", mem_line_addr);
		for (int i = 0; i < 16; ++i) {
			ImGui::SameLine();
			u8 data = mem->read(mem_line_addr + i);
			if (data == 0x00)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::Text("%02X", data);
			if (i == 7) {
				ImGui::SameLine(120.0f);
			}
			if (data == 0x00)
				ImGui::PopStyleColor(1);
			if ((mem_line_addr + i) == 0xFEFF)
				ImGui::Separator();
		}
	}
	clipper.End();
	ImGui::EndChild();
	ImGui::End();
}
