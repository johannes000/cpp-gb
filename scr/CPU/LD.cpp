
#include "../CPU.h"

u8 CPU::opcode0x3E() {  // LD A d8
    R.A = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x06() {  // LD B d8
    R.B = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x0E() {  // LD C d8
    R.C = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x16() {  // LD D d8
    R.D = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x1E() {  // LD E d8
    R.E = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x26() {  // LD H d8
    R.H = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x2E() {  // LD L d8
    R.L = read_u8_at_pc();
    return 0;
}

u8 CPU::opcode0x01() {  // LD BC d16
    R.BC = read_u16_at_pc();
    return 0;
}

u8 CPU::opcode0x11() {  // LD DE d16
    R.DE = read_u16_at_pc();
    return 0;
}

u8 CPU::opcode0x21() {  // LD HL d16
    R.HL = read_u16_at_pc();
    return 0;
}

u8 CPU::opcode0x31() {  // LD SP d16
    R.SP = read_u16_at_pc();
    return 0;
}

u8 CPU::opcode0x02() {  // LD [BC] A
    mem->write(R.BC, R.A);
    return 0;
}

u8 CPU::opcode0x08() {  // LD (a16) SP
    mem->write_u16(read_u16_at_pc(), R.SP);
    return 0;
}

u8 CPU::opcode0xFA() {  // LD A (a16)
    R.A = read(read_u16_at_pc());
    return 0;
}

u8 CPU::opcode0xEA() {  // LD (a16) A
    mem->write(read_u16_at_pc(), R.A);
    return 0;
}

u8 CPU::opcode0x0A() {  // LD A (BC)
    R.A = read(R.BC);
    return 0;
}

u8 CPU::opcode0x78() {  // LD A B
    R.A = R.B;
    return 0;
}

u8 CPU::opcode0x79() {  // LD A C
    R.A = R.C;
    return 0;
}

u8 CPU::opcode0xF2() {  // LD A (C)
    R.A = read(0xFF00 + R.C);
    return 0;
}

u8 CPU::opcode0x7A() {  // LD A D
    R.A = R.D;
    return 0;
}

u8 CPU::opcode0x7B() {  // LD A E
    R.A = R.E;
    return 0;
}

u8 CPU::opcode0x7C() {  // LD A H
    R.A = R.H;
    return 0;
}

u8 CPU::opcode0x7D() {  // LD A L
    R.A = R.L;
    return 0;
}

u8 CPU::opcode0x7F() {  // LD A A
    return 0;
}

u8 CPU::opcode0x7E() {  // LD A (HL)
    R.A = read(R.HL);
    return 0;
}

u8 CPU::opcode0x1A() {  // LD A (DE)
    R.A = read(R.DE);
    return 0;
}

u8 CPU::opcode0x2A() {  // LD A (HL+)
    R.A = read(R.HL++);
    return 0;
}

u8 CPU::opcode0x3A() {  // LD A (HL-)
    R.A = read(R.HL--);
    return 0;
}

u8 CPU::opcode0x22() {  // LD (HL+) A
    write(R.HL++, R.A);
    return 0;
}

u8 CPU::opcode0x32() {  // LD (HL-) A
    write(R.HL--, R.A);
    return 0;
}

u8 CPU::opcode0x77() {  // LD (HL) A
    write(R.HL, R.A);
    return 0;
}

u8 CPU::opcode0x12() {  // LD (DE) A
    write(R.DE, R.A);
    return 0;
}

u8 CPU::opcode0x47() {  // LD B A
    R.B = R.A;
    return 0;
}

u8 CPU::opcode0x4F() {  // LD C A
    R.C = R.A;
    return 0;
}

u8 CPU::opcode0x57() {  // LD D A
    R.D = R.A;
    return 0;
}

u8 CPU::opcode0x67() {  // LD H A
    R.H = R.A;
    return 0;
}

u8 CPU::opcode0x6F() {  // LD L A
    R.L = R.A;
    return 0;
}

u8 CPU::opcode0xE2() {  // LD (C) A
    mem->write(0xFF00 + R.C, R.A);
    return 0;
}

u8 CPU::opcode0x40() {  // LD B B
    return 0;
}

u8 CPU::opcode0x41() {  // LD B C
    R.B = R.C;
    return 0;
}

u8 CPU::opcode0x42() {  // LD B D
    R.B = R.D;
    return 0;
}

u8 CPU::opcode0x43() {  // LD B E
    R.B = R.E;
    return 0;
}

u8 CPU::opcode0x44() {  // LD B H
    R.B = R.H;
    return 0;
}

u8 CPU::opcode0x45() {  // LD B L
    R.B = R.L;
    return 0;
}

u8 CPU::opcode0x46() {  // LD B (HL)
    R.B = read(R.HL);
    return 0;
}

u8 CPU::opcode0x48() {  // LD C B
    R.C = R.B;
    return 0;
}

u8 CPU::opcode0x49() {  // LD C C
    return 0;
}

u8 CPU::opcode0x4A() {  // LD C D
    R.C = R.D;
    return 0;
}

u8 CPU::opcode0x4B() {  // LD C E
    R.C = R.E;
    return 0;
}

u8 CPU::opcode0x4C() {  // LD C H
    R.C = R.H;
    return 0;
}

u8 CPU::opcode0x4D() {  // LD C L
    R.C = R.L;
    return 0;
}

u8 CPU::opcode0x4E() {  // LD C HL
    R.C = read(R.HL);
    return 0;
}

u8 CPU::opcode0x50() {  // LD D B
    R.D = R.B;
    return 0;
}

u8 CPU::opcode0x51() {  // LD D C
    R.D = R.C;
    return 0;
}

u8 CPU::opcode0x52() {  // LD D D
    return 0;
}

u8 CPU::opcode0x53() {  // LD D E
    R.D = R.E;
    return 0;
}

u8 CPU::opcode0x54() {  // LD D H
    R.D = R.H;
    return 0;
}

u8 CPU::opcode0x55() {  // LD D L
    R.D = R.L;
    return 0;
}

u8 CPU::opcode0x56() {  // LD D HL
    R.D = read(R.HL);
    return 0;
}

u8 CPU::opcode0x5F() {  // LD E A
    R.E = R.A;
    return 0;
}

u8 CPU::opcode0x58() {  // LD E B
    R.E = R.B;
    return 0;
}

u8 CPU::opcode0x59() {  // LD E C
    R.E = R.C;
    return 0;
}

u8 CPU::opcode0x5A() {  // LD E D
    R.E = R.D;
    return 0;
}

u8 CPU::opcode0x5B() {  // LD E E
    return 0;
}

u8 CPU::opcode0x5C() {  // LD E H
    R.E = R.H;
    return 0;
}

u8 CPU::opcode0x5D() {  // LD E L
    R.E = R.L;
    return 0;
}

u8 CPU::opcode0x5E() {  // LD E HL
    R.E = read(R.HL);
    return 0;
}

u8 CPU::opcode0x60() {  // LD H B
    R.H = R.B;
    return 0;
}

u8 CPU::opcode0x61() {  // LD H C
    R.H = R.C;
    return 0;
}

u8 CPU::opcode0x62() {  // LD H D
    R.H = R.D;
    return 0;
}

u8 CPU::opcode0x63() {  // LD H E
    R.H = R.E;
    return 0;
}

u8 CPU::opcode0x64() {  // LD H H
    return 0;
}

u8 CPU::opcode0x65() {  // LD H L
    R.H = R.L;
    return 0;
}

u8 CPU::opcode0x66() {  // LD H HL
    R.H = read(R.HL);
    return 0;
}

u8 CPU::opcode0x68() {  // LD L B
    R.L = R.B;
    return 0;
}

u8 CPU::opcode0x69() {  // LD L C
    R.L = R.C;
    return 0;
}

u8 CPU::opcode0x6A() {  // LD L D
    R.L = R.D;
    return 0;
}

u8 CPU::opcode0x6B() {  // LD L E
    R.L = R.E;
    return 0;
}

u8 CPU::opcode0x6C() {  // LD L H
    R.L = R.H;
    return 0;
}

u8 CPU::opcode0x6D() {  // LD L L
    return 0;
}

u8 CPU::opcode0x6E() {  // LD L HL
    R.L = read(R.HL);
    return 0;
}

u8 CPU::opcode0x70() {  // LD HL B
    mem->write(R.HL, R.B);
    return 0;
}

u8 CPU::opcode0x71() {  // LD HL C
    mem->write(R.HL, R.C);
    return 0;
}

u8 CPU::opcode0x72() {  // LD HL D
    mem->write(R.HL, R.D);
    return 0;
}

u8 CPU::opcode0x73() {  // LD HL E
    mem->write(R.HL, R.E);
    return 0;
}

u8 CPU::opcode0x74() {  // LD HL H
    mem->write(R.HL, R.H);
    return 0;
}

u8 CPU::opcode0x75() {  // LD HL L
    mem->write(R.HL, R.L);
    return 0;
}

u8 CPU::opcode0x36() {  // LD (HL) d8
    mem->write(R.HL, read_u8_at_pc());
    return 0;
}

u8 CPU::opcode0xF8() {  // LD HL SP + r8
    const i8 r8 = read_i8_at_pc();
    const u16 result = R.SP + r8;
    const u16 check = (R.SP ^ r8 ^ ((result) & 0xFFFFu));
    set_flag_if(Flag::c, check_bit(check, 7));
    set_flag_if(Flag::h, check_bit(check, 3));

    R.HL = result;
    reset_flags(Flag::z, Flag::n);
    return 0;
}

u8 CPU::opcode0xF9() {  // LD SP HL
    R.SP = R.HL;
    return 0;
}

u8 CPU::opcode0xE0() {  // LDH a8 A
    mem->write(0xFF00 + read_u8_at_pc(), R.A);
    return 0;
}

u8 CPU::opcode0xF0() {  // LDH A a8
    // Load value in register A from byte at address n16, provided it is between $FF00 and $FFFF.
    auto data = read_u8_at_pc();
    R.A = read(0xFF00 + data);
    return 0;
}
