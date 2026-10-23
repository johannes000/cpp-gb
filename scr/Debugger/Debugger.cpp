#include "Debugger.hpp"
#include "imgui.h"

#include <algorithm>
#include <vector>

Debugger::Debugger(CPU *c, PPU *p, MEMORY *m, CARTRIDGE *card, CONTROLS *joy, APU *apu)
	: hit_breakpoint_(false),
	  cpu_(c),
	  ppu_(p),
	  mem_(m),
	  card_(card),
	  joypad_(joy),
	  apu_(apu),
	  debug_state_(Debug_State::RUN),
	  next_step_(false) {
	breakpoints_.push_back(0x20A);
}

auto Debugger::handle_breakpoints() -> void {
	next_step_ = false;
	if (debug_state_ != Debug_State::STEP) {
		if (!breakpoints_.empty()) {
			u16 pc = cpu_->get_pc();
			auto iter = std::find(breakpoints_.begin(), breakpoints_.end(), pc);
			if (iter != breakpoints_.end()) {
				debug_state_ = Debug_State::STEP;
			}
		}
		return;
	}
}

auto Debugger::update_views() -> void {
	// card_view();
	// cpu_view();
	// memory_view();
	// control_view();
}

auto Debugger::card_view() -> void {

	ImGui::Text("%s", card_->rom_info_.title_);
	ImGui::Separator();

	ImGui::Text("CType: %s", card_->rom_info_.CType_string());
	ImGui::SameLine();
	ImGui::Text("Rom Bank: %d", card_->get_selected_rom_bank());
	ImGui::Text("Ram Size: %s", card_->rom_info_.ram_size_string());
	ImGui::SameLine();
	ImGui::Text("| Rom Size: %s", card_->rom_info_.rom_size_string());
}

auto Debugger::cpu_view() -> void {
	return;
	bool goto_pc = ImGui::Button("Goto PC");
	ImGui::SameLine();
	if (ImGui::Button("Step")) {
		debug_state_ = Debug_State::STEP;
		next_step_ = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Run")) {
		debug_state_ = Debug_State::RUN;
		next_step_ = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Breakpoint")) {
		debug_state_ = Debug_State::BREAKPOINT;
		next_step_ = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("ReDisassemble")) {
		disassemble();
	}
	ImGui::SameLine();

	switch (debug_state_) {
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
	ImGui::BeginChild("##scrollingregion", ImVec2(0, 350));

	// DISASSAMBLER
	auto pc = cpu_->get_pc();
	ImGuiListClipper clipper;
	clipper.Begin(static_cast<s32>(dis_vec_.size()), ImGui::GetTextLineHeightWithSpacing());

	auto itr = std::find(dis_addr_vec_.begin(), dis_addr_vec_.end(), pc);
	auto pc_offset = std::distance(dis_addr_vec_.begin(), itr);

	if (goto_pc) {
		ImGui::SetScrollFromPosY(
			ImGui::GetCursorStartPos().y +
				(pc_offset * ImGui::GetTextLineHeightWithSpacing()),
			0.5f);
	}
	while (clipper.Step()) {
		for (int i = clipper.DisplayStart; i <= clipper.DisplayEnd; i++) {
			auto inst = dis_vec_[i];

			if (ImGui::Selectable("", true, ImGuiSelectableFlags_AllowDoubleClick))
				breakpoints_.push_back(inst.address_);
			ImGui::SameLine();
			if (inst.address_ == pc) {
				ImGui::SetItemDefaultFocus();
				ImGui::TextColored(red_, "%04X", inst.address_);
				ImGui::SameLine();
				switch (inst.bytes_) {
					case 1:
						ImGui::TextColored(gray_, "%02X\t\t\t", inst.opcode_);
						break;
					case 2:
						ImGui::TextColored(gray_, "%02X %02X\t\t ", inst.opcode_,
										   inst.intermediate_ & 0xFFu);
						break;
					case 3:
						ImGui::TextColored(gray_, "%02X %02X %02X\t  ", inst.opcode_,
										   inst.intermediate_ & 0xFF00u,
										   inst.intermediate_ & 0x00FFu);
						break;
				}
				ImGui::SameLine();
				ImGui::TextColored(red_, "->%s", inst.mnemonic_);
				if (inst.bytes_ > 2) {
					ImGui::SameLine();
					ImGui::TextColored(red_, "%s",
									   what_register_is_this(inst.intermediate_));
				} else if (inst.bytes_ > 1) {
					ImGui::SameLine();
					ImGui::TextColored(red_, "%s",
									   what_register_is_this(static_cast<u16>(
										   inst.intermediate_ | 0xFF00u)));
				}
			} else {
				ImGui::TextColored(cyan_, "%04X", inst.address_);
				ImGui::SameLine();
				switch (inst.bytes_) {
					case 1:
						ImGui::TextColored(gray_, "%02X\t\t\t", inst.opcode_);
						break;
					case 2:
						ImGui::TextColored(gray_, "%02X %02X\t\t ", inst.opcode_,
										   inst.intermediate_ & 0xFFu);
						break;
					case 3:
						ImGui::TextColored(gray_, "%02X %02X %02X\t  ", inst.opcode_,
										   (inst.intermediate_ & 0xFFu) >> 4u,
										   inst.intermediate_ & 0x00FFu);
						break;
				}
				ImGui::SameLine();
				ImGui::TextColored(cyan_, "%s", inst.mnemonic_);
				if (inst.bytes_ > 2) {
					ImGui::SameLine();
					ImGui::TextColored(gray_, "%s",
									   what_register_is_this(inst.intermediate_));
				} else if (inst.bytes_ > 1) {
					ImGui::SameLine();
					ImGui::TextColored(gray_, "%s",
									   what_register_is_this(static_cast<u16>(
										   inst.intermediate_ | 0xFF00u)));
				}
			}
			// i += inst.bytes;
		}
	}
	clipper.End();
	ImGui::EndChild();

	// Breakpoints
	ImGui::BeginChild("##ScrollingRegion");

	std::sort(breakpoints_.begin(), breakpoints_.end());
	ImGui::Text("Breakpoints");

	auto b = breakpoints_.begin();
	auto e = breakpoints_.end();

	while (b != e) {
		if (*b == pc)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));
		char address[32];
		sprintf(address, "%X", *b);
		if (ImGui::Selectable(address, true, ImGuiSelectableFlags_SpanAllColumns)) {
			breakpoints_.erase(b);
		}
		if (*b == pc)
			ImGui::PopStyleColor(1);
		b++;
	}

	ImGui::EndChild();
}

auto Debugger::reset() -> void {}

auto Debugger::disassemble() -> void {
	debug_instruction debug_inst;
	dis_vec_.erase(dis_vec_.begin(), dis_vec_.end());
	dis_addr_vec_.erase(dis_addr_vec_.begin(), dis_addr_vec_.end());
	for (u16 addr = 0; addr < 0xFFFF;) {
		// Nur Adressen aus dem Card Rom nicht aus dem Bootrom
		// Deswegen kriegt man beim Booten nicht die richtigen Adressen im CPU View
		debug_inst = cpu_->get_instruction_at_addr(addr);
		if (debug_inst.prefixed_)
			addr++;
		dis_vec_.push_back(debug_inst);
		dis_addr_vec_.push_back(addr);
		addr += debug_inst.bytes_;
	}
}

auto Debugger::control_view() -> void {
	CPU_Register cpu_regs = cpu_->get_register();

	ImGui::Text("AF = %04X", cpu_regs.AF_);
	ImGui::SameLine();
	ImGui::Text("DE = %04X", cpu_regs.DE_);
	ImGui::SameLine();
	ImGui::Text("PC = %04X", cpu_regs.PC_);
	ImGui::Text("BC = %04X", cpu_regs.BC_);
	ImGui::SameLine();
	ImGui::Text("HL = %04X", cpu_regs.HL_);
	ImGui::SameLine();
	ImGui::Text("SP = %04X", cpu_regs.SP_);

	ImGui::Separator();

	ImGui::Text("Z %1d", cpu_->check_flag(Flag::z));
	ImGui::SameLine();
	ImGui::Text("N %1d", cpu_->check_flag(Flag::n));
	ImGui::SameLine();
	ImGui::Text("H %1d", cpu_->check_flag(Flag::h));
	ImGui::SameLine();
	ImGui::Text("C %1d", cpu_->check_flag(Flag::c));
	ImGui::SameLine();
	ImGui::Text("(HL) %02X", cpu_->read(cpu_regs.HL_));

	LCD_Register lcd_regs = ppu_->get_lcd_register();
	ImGui::Separator();

	ImGui::Text("IME %X", cpu_->is_IME());

	ImGui::Text("Interrupt Request");
	for (int i = 0; i < 5; ++i) {
		ImGui::SameLine();
		ImGui::Text("%X", check_bit(mem_->read(INTERRUPT_FLAGS_REG), i));
	}
	ImGui::Text("Interrupt Enable ");
	for (int i = 0; i < 5; ++i) {
		ImGui::SameLine();
		ImGui::Text("%X", check_bit(mem_->read(INTERRUPT_ENABLE_REG), i));
	}

	ImGui::Separator();
	ImGui::Text(" %d", lcd_regs.LCD_LY_);
	ImGui::SameLine();
	ImGui::Text("JOY ");
	u8 joy = joypad_->get_joypad_state();
	for (int i = 0; i < 8; ++i) {
		ImGui::SameLine();
		ImGui::Text("%1d", check_bit(joy, i));
	}
	ImGui::Separator();
	ImGui::Text(" TIMER ");
	ImGui::Text("DIV  %X", mem_->read(DIVIDER_REGISTER));
	ImGui::SameLine();
	ImGui::Text("TIMA %X", mem_->read(TIMER_COUNTER));
	ImGui::Text("TMA  %X", mem_->read(TIMER_MODULO));
	ImGui::SameLine();
	ImGui::Text("TAC  %X", mem_->read(TIMER_CONTROL));
}

auto Debugger::memory_view() -> void {
	ImGuiListClipper clipper;
	clipper.Begin(0xFFFF / 16, ImGui::GetTextLineHeightWithSpacing());
	while (clipper.Step()) {
		for (int mem_line = clipper.DisplayStart; mem_line <= clipper.DisplayEnd; mem_line++) {
			int mem_line_addr = mem_line * 0x10;
			ImGui::Text("0x%04X ", mem_line_addr);
			for (int i = 0; i < 16; ++i) {
				ImGui::SameLine();
				u8 data = mem_->read(mem_line_addr + i);
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
	}
	clipper.End();
}
