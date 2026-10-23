#include "CPU.hpp"
#include <iostream>

CPU::CPU(MEMORY *m) : mem_(m) {}

auto CPU::init_register(bool skip_boot) -> void {
	in_boot_ = true;
	R_.PC_ = 0x0000;
	IME_ = false;
	is_halted_ = false;

	interrupt_enable_register_ = 0x00;
	interrupt_request_register_ = 0x00;

	timer_cycle_sum_ = 0;
	div_cycle_sum_ = 0;

	op_code_ = 0x00;
	cycles_ = 0;

	if (skip_boot) {
		in_boot_ = false;

		R_.AF_ = 0x01B0;
		R_.BC_ = 0x0013;
		R_.DE_ = 0x00D8;
		R_.HL_ = 0x014D;
		R_.SP_ = 0xFFFE;
		R_.PC_ = 0x0100;
	}
}

auto CPU::parse_op_code(const u8 &opcode) -> u8 {
	u8 function;
	if (opcode == 0xCB) {
		prefix_ = false;
		cycles_++;
		u8 p_opcode = read(R_.PC_ + 1);
		current_instruction_info_ = prefixedinstructions_[p_opcode];
		// current_instruction_info.bytes += 1;
		function = (this->*prefixedinstructions_[p_opcode].function_)();
	} else {
		current_instruction_info_ = instructions_[op_code_];
		function = (this->*instructions_[opcode].function_)();
	}
	return function;
}

auto CPU::update() -> int {
	if (is_halted_ || ((interrupt_request_register_ & interrupt_enable_register_ & 0x1Fu) && IME_)) {
		return 1;
	}
	op_code_ = get_next_op_code();
	const auto jp = parse_op_code(op_code_);
	if (jp == 0)
		inc_pc(); // Wenn 1 returned wird wurde der PC schon vom OPcode verändert.
	cycles_ += current_instruction_info_.cycles_;
	return current_instruction_info_.cycles_;
}

auto CPU::get_instruction_at_addr(u16 addr) const -> debug_instruction {
	debug_instruction debug_inst;

	cpu_instruction cpu_inst;
	u16 intermediate = 0xFFFF;

	u8 inst_code = mem_->read(addr);
	cpu_inst = instructions_[inst_code];
	if (inst_code == 0xCB) {
		inst_code = mem_->read(++addr);
		cpu_inst = prefixedinstructions_[inst_code];
		cpu_inst.bytes_ = 1;
		debug_inst.prefixed_ = true;
	}
	if (cpu_inst.bytes_ > 1)
		intermediate =
			cpu_inst.bytes_ == 2 ? static_cast<u16>(mem_->read_card_rom(addr + 1)) : u8_to_u16(mem_->read_card_rom((addr + 2)), mem_->read_card_rom(addr + 1));

	return debug_inst = {
			   cpu_inst.mnemonic_,
			   cpu_inst.bytes_,
			   cpu_inst.cycles_,
			   inst_code,
			   intermediate,
			   addr,
		   };
}

auto CPU::read(const u16 &addr) -> u8 {
	switch (addr) {
		case DIVIDER_REGISTER:
			return divider_register_;
		case TIMER_CONTROL:
			return timer_control_;
		case TIMER_COUNTER:
			return timer_counter_;
		case TIMER_MODULO:
			return timer_modulo_;
		case INTERRUPT_ENABLE_REG:
			return interrupt_enable_register_;
		case INTERRUPT_FLAGS_REG:
			return interrupt_request_register_;
		case IN_BOOT_ADDRESS:
			return in_boot_;
		default:
			return mem_->read(addr);
	}
}

auto CPU::write(const u16 &addr, const u8 &data) -> void {
	switch (addr) {
		case DMA_TRANSFER: // This register starts the OAM DMA transfer
			dma_transfer(data);
			break;
		case DIVIDER_REGISTER:
			handle_div_register_write();
			break;
		case TIMER_CONTROL:
			handle_time_control_write(data);
			break;
		case TIMER_COUNTER:
			timer_counter_ = data;
			break;
		case TIMER_MODULO:
			timer_modulo_ = data;
			break;
		case INTERRUPT_ENABLE_REG:
			interrupt_enable_register_ = data & 0x1Fu;
			break;
		case INTERRUPT_FLAGS_REG:
			interrupt_request_register_ = data & 0x1Fu;
			break;
		case IN_BOOT_ADDRESS:
			in_boot_ = false;
			break;
		default:
			mem_->write(addr, data);
			break;
	}
}
/**
 * @brief
 *
 * @param data
 */
auto CPU::dma_transfer(const u8 &data) -> void {
	mem_->write_direct(DMA_TRANSFER, data);
	current_instruction_info_.cycles_ = 160;
	// current_instruction_info.cycles = 4;

	const u16 addr = data << 8u;
	if (addr >= 0x8000 && addr < 0xE000) {
		for (u8 i = 0; i < 0xA0; i++) {
			u16 temp = 0xFE00 + i;
			mem_->write(temp, read(addr + i));
		}
	}
}

auto CPU::handle_time_control_write(const u8 &data) -> void {
	if ((timer_control_ & 3u) != (data & 3u))
		timer_cycle_sum_ = 0;
	timer_control_ = data;
}

auto CPU::handle_div_register_write() -> void { divider_register_ = 0; }

auto CPU::write_u16(const u16 &addr, const u16 &data) const -> void {
	mem_->write_u16(addr, data);
}

auto CPU::handle_interrupts() -> void {
	u8 interrupt = interrupt_request_register_ & interrupt_enable_register_ & 0x1Fu;
	if (!IME_) {
		if (is_halted_ && interrupt != 0)
			is_halted_ = false;
		return;
	}
	if (interrupt) {
		is_halted_ = false;
		IME_ = false;
		push_r16_reg(&R_.PC_);
		if (check_bit(interrupt, static_cast<u8>(Interrupt::V_Blank))) {
			reset_bit(interrupt_request_register_, static_cast<u8>(Interrupt::V_Blank));
			R_.PC_ = 0x40;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::LCD_Stat))) {
			reset_bit(interrupt_request_register_, static_cast<u8>(Interrupt::LCD_Stat));
			R_.PC_ = 0x48;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::Timer_overflow))) {
			reset_bit(interrupt_request_register_, static_cast<u8>(Interrupt::Timer_overflow));
			R_.PC_ = 0x50;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::Serial_link))) {
			reset_bit(interrupt_request_register_, static_cast<u8>(Interrupt::Serial_link));
			R_.PC_ = 0x58;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::Joypad))) {
			reset_bit(interrupt_request_register_, static_cast<u8>(Interrupt::Joypad));
			R_.PC_ = 0x60;
		}
	}
}

auto CPU::handle_timer(int t_cycles) -> void {
	div_cycle_sum_ += t_cycles;
	if (div_cycle_sum_ >= 0xFF) {
		div_cycle_sum_ -= 0xFF;
		divider_register_++;
	}

	// if Timer Enable
	if (check_bit(timer_control_, 2)) {
		timer_cycle_sum_ += t_cycles * 4;

		int freq = 1024;
		switch (timer_control_ & 3u) {
			case 1:
				freq = 16;
				break;
			case 2:
				freq = 64;
				break;
			case 3:
				freq = 256;
				break;
		}

		while (timer_cycle_sum_ >= freq) {
			timer_counter_++;
			if (timer_counter_ == 0x100) {
				request_interrupt(Interrupt::Timer_overflow);
				timer_counter_ = static_cast<u16>(timer_modulo_);
			}
			timer_cycle_sum_ -= freq;
		}
	}
}

auto CPU::request_interrupt(Interrupt inter) -> void {
	set_bit(interrupt_request_register_, static_cast<u8>(inter));
}

auto CPU::is_in_boot() const -> bool { return in_boot_; }

auto CPU::inc_pc() -> void { R_.PC_ += current_instruction_info_.bytes_; }

inline auto CPU::get_next_op_code() -> u8 { return read(R_.PC_); }
