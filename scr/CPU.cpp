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
    old_pc = 0x0000;

    op_code = 0x00;
    cycles = 0;

    in_boot = true;
    IME = false;
    IME_scheduled = false;
    DI_scheduled = false;
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
    old_pc = R.PC;
    if (interrupts()) std::cout << "INTERRUPT" << std::endl;
    if (IME_scheduled) IME = true;
    if (R.PC == 0x0034)
        old_pc = R.PC;
    op_code = get_next_op_code();
    const auto jp = parse_op_code(op_code);
#if 0
    if (!in_boot)
        print_last_opcode();
#endif
    if (jp == 0) inc_pc();
    cycles += current_instruction_info.cycles;
    return current_instruction_info.cycles;
}

void CPU::inc_pc() { R.PC += current_instruction_info.bytes; }

u8 CPU::get_next_op_code() { return read(R.PC); }

void CPU::print_last_opcode() {
    std::cout << "0x" << std::hex << std::uppercase << static_cast<int>(old_pc)
              << " " << current_instruction_info.mnemonic << "\t"
              << "OP: " << std::hex << static_cast<int>(op_code);
    if (current_instruction_info.bytes == 3)
        std::cout << "  " << static_cast<int>(read_u16_at_pc());
    else
        std::cout << "\t";
    if (current_instruction_info.bytes == 2)
        std::cout << "  0x" << static_cast<int>(read_u8_at_pc()) << "\t";
    else
        std::cout << "\t";
    std::cout << " Z: " << check_flag(Flag::z);
    std::cout << " N: " << check_flag(Flag::n);
    std::cout << " H: " << check_flag(Flag::h);
    std::cout << " C: " << check_flag(Flag::c);
    std::cout << std::endl;
}

void CPU::print_all_ops() {
    u16 mypc = 0x00;
    instruction myinstruction_info;
    while (mypc < 0x4000) {
        auto myopcode = read(mypc);
        if (myopcode == 0xCB) {
            myopcode = read(++mypc);
            myinstruction_info = prefixedinstructions[myopcode];
        } else {
            myinstruction_info = instructions[myopcode];
        }
        std::cout << "0x" << std::hex << std::uppercase << static_cast<int>(mypc)
                  << "\t" << myinstruction_info.mnemonic;
        if (myinstruction_info.bytes == 3)
            std::cout << " " << static_cast<int>(read_u16_at_pc());
        if (myinstruction_info.bytes == 2)
            std::cout << " 0x" << static_cast<int>(read_u8_at_pc());
        std::cout << std::endl;
        mypc += myinstruction_info.bytes;
    }
}

bool CPU::in_boot_r() const { return in_boot; }

u8 CPU::read(const u16 &addr) {
    switch (addr) {
        case INTERRUPT_ENABLE_REG:
            return IE_Register;
        case INTERRUPT_FLAGS_REG:
            return IF_Register;
        case IN_BOOT_ADDRESS:
            return in_boot;
        default:
            if (in_boot && R.PC > 0x100) {
                in_boot = false;
                std::cout << "Boot verlassen nach: " << std::dec << cycles << " Cycles"
                          << std::endl;
            }
            return mem->read(addr);
    }
}

void CPU::write(const u16 &addr, const u8 &data) {
    switch (addr) {
        case INTERRUPT_ENABLE_REG:
            IE_Register = data;
            break;
        case INTERRUPT_FLAGS_REG:
            IF_Register = data;
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

void CPU::write_u16(const u16 &addr, const u16 &data) const {
    mem->write_u16(addr, data);
}

// INTERRUPT FLAGS

void CPU::handle_interrupt(u8 val) {
    write(INTERRUPT_ENABLE_REG, val);
    ie_flags.update(this);
    if_flags.update(this);
}

bool CPU::interrupts() const {
    if (!IME) {
        return false;
    }
    return false;
}

void CPU::IE_Flags::update(CPU *cpu) {
    const auto data = cpu->read(INTERRUPT_ENABLE_REG);
    JIR = check_bit(data, 4);
    SIR = check_bit(data, 3);
    TIR = check_bit(data, 2);
    LIR = check_bit(data, 1);
    VIR = check_bit(data, 0);
}

void CPU::IF_Flags::update(CPU *cpu) {
    const auto data = cpu->read(INTERRUPT_FLAGS_REG);
    JIE = check_bit(data, 4);
    SIE = check_bit(data, 3);
    TIE = check_bit(data, 2);
    LIE = check_bit(data, 1);
    VIE = check_bit(data, 0);
}
