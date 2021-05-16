/**
 * @file CPU.cpp
 * @author Johannes
 * @brief 
 * @date 22-06-2020
 * 
 * @copyright Copyright (c) 2021
 */

#include "CPU.h"

#include <iostream>

CPU::CPU(MEMORY *m) : mem(m) {}

void CPU::init_register(bool skip_boot) {
	in_boot = true;
	R.PC = 0x0000;
	IME = false;
	is_halted = false;

	interrupt_enable_register = 0x00;
	interrupt_request_register = 0x00;

	timer_cycle_sum = 0;
	div_cycle_sum = 0;

	op_code = 0x00;
	cycles = 0;

	if (skip_boot) {
		in_boot = false;

		R.AF = 0x01B0;
		R.BC = 0x0013;
		R.DE = 0x00D8;
		R.HL = 0x014D;
		R.SP = 0xFFFE;
		R.PC = 0x0100;
	}
}

u8 CPU::parse_op_code(const u8 &opcode) {
	u8 function;
	if (opcode == 0xCB) {
		prefix = false;
		cycles++;
		u8 p_opcode = read(R.PC + 1);
		current_instruction_info = prefixedinstructions[p_opcode];
		//current_instruction_info.bytes += 1;
		function = (this->*prefixedinstructions[p_opcode].function)();
	} else {
		current_instruction_info = instructions[op_code];
		function = (this->*instructions[opcode].function)();
	}
	return function;
}

int CPU::update() {
	if (is_halted || ((interrupt_request_register & interrupt_enable_register & 0x1Fu) && IME)) {
		return 1;
	}
	op_code = get_next_op_code();
	const auto jp = parse_op_code(op_code);
	if (jp == 0)
		inc_pc(); // Wenn 1 returned wird wurde der PC schon vom OPcode verändert.
	cycles += current_instruction_info.cycles;
	return current_instruction_info.cycles;
}

debug_instruction CPU::get_instruction_at_addr(u16 addr) const {
	debug_instruction debug_inst;

	cpu_instruction cpu_inst;
	u16 intermediate = 0xFFFF;

	u8 inst_code = mem->read(addr);
	cpu_inst = instructions[inst_code];
	if (inst_code == 0xCB) {
		inst_code = mem->read(++addr);
		cpu_inst = prefixedinstructions[inst_code];
		cpu_inst.bytes = 1;
		debug_inst.prefixed = true;
	}
	if (cpu_inst.bytes > 1)
		intermediate =
			cpu_inst.bytes == 2 ? static_cast<u16>(mem->read_card_rom(addr + 1)) : u8_to_u16(mem->read_card_rom((addr + 2)), mem->read_card_rom(addr + 1));

	return debug_inst = {
			   cpu_inst.mnemonic,
			   cpu_inst.bytes,
			   cpu_inst.cycles,
			   inst_code,
			   intermediate,
			   addr,
		   };
}

u8 CPU::read(const u16 &addr) {
	switch (addr) {
		case DIVIDER_REGISTER:
			return divider_register;
		case TIMER_CONTROL:
			return timer_control;
		case TIMER_COUNTER:
			return timer_counter;
		case TIMER_MODULO:
			return timer_modulo;
		case INTERRUPT_ENABLE_REG:
			return interrupt_enable_register;
		case INTERRUPT_FLAGS_REG:
			return interrupt_request_register;
		case IN_BOOT_ADDRESS:
			return in_boot;
		default:
			return mem->read(addr);
	}
}

void CPU::write(const u16 &addr, const u8 &data) {
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
			timer_counter = data;
			break;
		case TIMER_MODULO:
			timer_modulo = data;
			break;
		case INTERRUPT_ENABLE_REG:
			interrupt_enable_register = data & 0x1Fu;
			break;
		case INTERRUPT_FLAGS_REG:
			interrupt_request_register = data & 0x1Fu;
			break;
		case IN_BOOT_ADDRESS:
			in_boot = false;
			break;
		default:
			mem->write(addr, data);
			break;
	}
}
/**
 * @brief 
 * 
 * @param data 
 */
void CPU::dma_transfer(const u8 &data) {
	mem->write_direct(DMA_TRANSFER, data);
	// current_instruction_info.cycles = 160;
	current_instruction_info.cycles = 4;

	const u16 addr = data << 8u;
	if (addr >= 0x8000 && addr < 0xE000) {
		for (u8 i = 0; i < 0xA0; i++) {
			u16 temp = 0xFE00 + i;
			mem->write(temp, read(addr + i));
		}
	}
}

void CPU::handle_time_control_write(const u8 &data) {
	if ((timer_control & 3u) != (data & 3u))
		timer_cycle_sum = 0;
	timer_control = data;
}

void CPU::handle_div_register_write() { divider_register = 0; }

void CPU::write_u16(const u16 &addr, const u16 &data) const {
	mem->write_u16(addr, data);
}

void CPU::handle_interrupts() {
	u8 interrupt = interrupt_request_register & interrupt_enable_register & 0x1Fu;
	if (!IME) {
		if (is_halted && interrupt != 0)
			is_halted = false;
		return;
	}
	if (interrupt) {
		is_halted = false;
		IME = false;
		push_r16_reg(&R.PC);
		if (check_bit(interrupt, static_cast<u8>(Interrupt::V_Blank))) {
			reset_bit(interrupt_request_register, static_cast<u8>(Interrupt::V_Blank));
			R.PC = 0x40;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::LCD_Stat))) {
			reset_bit(interrupt_request_register, static_cast<u8>(Interrupt::LCD_Stat));
			R.PC = 0x48;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::Timer_overflow))) {
			reset_bit(interrupt_request_register, static_cast<u8>(Interrupt::Timer_overflow));
			R.PC = 0x50;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::Serial_link))) {
			reset_bit(interrupt_request_register, static_cast<u8>(Interrupt::Serial_link));
			R.PC = 0x58;
		} else if (check_bit(interrupt, static_cast<u8>(Interrupt::Joypad))) {
			reset_bit(interrupt_request_register, static_cast<u8>(Interrupt::Joypad));
			R.PC = 0x60;
		}
	}
}

void CPU::handle_timer(int t_cycles) {
	div_cycle_sum += t_cycles;
	if (div_cycle_sum >= 0xFF) {
		div_cycle_sum -= 0xFF;
		divider_register++;
	}

	// if Timer Enable
	if (check_bit(timer_control, 2)) {
		timer_cycle_sum += t_cycles * 4;

		int freq = 1024;
		switch (timer_control & 3u) {
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

		while (timer_cycle_sum >= freq) {
			timer_counter++;
			if (timer_counter == 0x100) {
				request_interrupt(Interrupt::Timer_overflow);
				timer_counter = static_cast<u16>(timer_modulo);
			}
			timer_cycle_sum -= freq;
		}
	}
}

void CPU::request_interrupt(Interrupt inter) {
	set_bit(interrupt_request_register, static_cast<u8>(inter));
}

bool CPU::is_in_boot() const { return in_boot; }

void CPU::inc_pc() { R.PC += current_instruction_info.bytes; }

inline u8 CPU::get_next_op_code() { return read(R.PC); }
