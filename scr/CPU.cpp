#include "CPU.h"

#include <iostream>


CPU::CPU(MEMORY *m) : mem(m) { init_register(); }

void CPU::init_register() {
    R.AF = 0x01B0;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.SP = 0xFFFE;
    R.PC = 0x0000;

    interrupt_enable_register = 0x00;
    interrupt_request_register = 0x00;

    timer_cycle_sum = 0;
    div_cycle_sum = 0;

    op_code = 0x00;
    cycles = 0;

    in_boot = true;
    IME = false;
}

u8 CPU::parse_op_code(u8 &opcode) {
    u8 function;
    if (prefix) {
        prefix = false;
        current_instruction_info = prefixedinstructions[op_code];
        current_instruction_info.bytes -= 1;
        function = (this->*prefixedinstructions[opcode].function)();
    } else {
        current_instruction_info = instructions[op_code];
        function = (this->*instructions[opcode].function)();
    }
    return function;
}

int CPU::update() {
    if (is_halted) {
        return 1;
    }
    op_code = get_next_op_code();;
    const auto jp = parse_op_code(op_code);
    if (jp == 0) inc_pc();
    cycles += current_instruction_info.cycles;
    return current_instruction_info.cycles;
}

debug_instruction CPU::get_instruction_at_addr(u16 addr) const {
    debug_instruction debug_inst;

    cpu_instruction cpu_inst;
    u16 intermediate = -1;

    u8 inst_code = mem->read_card_rom(addr);
    cpu_inst = instructions[inst_code];
    if (inst_code == 0xCB) {
        inst_code = mem->read_card_rom(++addr);
        cpu_inst = prefixedinstructions[inst_code];
        cpu_inst.bytes = 1;
        debug_inst.prefixed = true;
    }
    if (cpu_inst.bytes > 1)
        intermediate =
                cpu_inst.bytes == 2 ? static_cast<u16>(mem->read_card_rom(addr + 1)) :
                u8_to_u16(mem->read_card_rom((addr + 2)), mem->read_card_rom(addr + 1));

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
            return interrupt_request_register | 0xE0u;
        case IN_BOOT_ADDRESS:
            return in_boot;
        default:
            return mem->read(addr);
    }
}


void CPU::write(const u16 &addr, const u8 &data) {
    switch (addr) {
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
            if (in_boot)
                std::cout << "Boot verlassen nach: " << std::dec << cycles << " Cycles" << std::endl;
            in_boot = false;
            break;
        default:
            mem->write(addr, data);
            break;
    }
}

void CPU::handle_time_control_write(const u8 &data) {
    if ((timer_control & 3u) != (data & 3u))
        timer_cycle_sum = 0;
    timer_cycle_sum = data;
}

void CPU::handle_div_register_write() { divider_register = 0; }

void CPU::write_u16(const u16 &addr, const u16 &data) const {
    mem->write_u16(addr, data);
}

void CPU::handle_interrupts() {
    if (!IME)
        return;
    u8 interrupt = interrupt_request_register & interrupt_enable_register;

    if (interrupt != 0x00) {
        pop_r16_reg(&R.PC);
        IME = false;
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
    if (div_cycle_sum >= 256) {
        div_cycle_sum -= 256;
        divider_register++;
    }

    if (check_bit(timer_control, 2)) {
        timer_cycle_sum += t_cycles * 4;


        int freq = 0;
        switch (timer_control & 3u) {
            case 1:
                freq = 262144;
                break;
            case 2:
                freq = 65536;
                break;
            case 3:
                freq = 16384;
                break;
            default:
                freq = 4096;
                break;
        }
        int osci = OSCILLATOR_CLOCK_FREQ / freq;
        while (timer_cycle_sum >= osci) {
            timer_counter++;
            if (timer_counter == 0) {
                request_interrupt(Interrupt::Timer_overflow);
                timer_counter = timer_modulo;
            }
            timer_cycle_sum -= osci;
        }

    }

}


void CPU::request_interrupt(Interrupt inter) {
    is_halted = false;
    set_bit(interrupt_request_register, static_cast<u8>(inter));
}

bool CPU::in_boot_r() const { return in_boot; }

void CPU::inc_pc() { R.PC += current_instruction_info.bytes; }

u8 CPU::get_next_op_code() { return read(R.PC); }

