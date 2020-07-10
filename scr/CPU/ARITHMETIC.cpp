#include "../CPU.h"

void CPU::xor_a_r8(const u8 *reg) {
    // XOR data 1 und data 2 und speicher das ergebnis in data 1
    R.A ^= *reg;
    set_flag_if(Flag::z, R.A == 0);
    reset_flags(Flag::n, Flag::h, Flag::c);
}

void CPU::and_a_r8(const u8 *reg) {
    // AND A und reg 2 und speicher das ergebnis in A
    R.A &= *reg;
    set_flag_if(Flag::z, R.A == 0);
    reset_flags(Flag::n, Flag::c);
    set_flag(Flag::h);
}

void CPU::or_r8_r8(const u8 &reg) {
    // OR reg 1 und reg 2 und Speicher das Ergebnis in data 1
    reset_all_flags();
    R.A |= reg;
    set_flag_if(Flag::z, R.A == 0);
}

void CPU::cp_a_r8(const u8 *reg) {
    //Subtract the value in data2 from data1 and set flags accordingly, but don't store the result. This is useful for ComParing values.
    set_flag(Flag::n);
    set_flag_if(Flag::h, ((R.A & 0xfu) < (*reg & 0xfu)));
    set_flag_if(Flag::c, R.A < *reg);
    set_flag_if(Flag::z, R.A == *reg);
}

void CPU::inc_u8_reg(u8 *reg) {
    //  Increment register.
    u8 result = *reg + 1;

    reset_flag(Flag::n);

    set_flag_if(Flag::h, ((*reg & 0xFu) + 0x1 > 0xF));
    set_flag_if(Flag::z, result == 0);
    *reg = result;
}

void CPU::dec_u8_reg(u8 *reg) {
    // Decrement register
    u8 result = *reg - 1;

    set_flag(Flag::n);
    set_flag_if(Flag::h, (static_cast<i8>((*reg & 0xFu) - 0x1) < 0));
    set_flag_if(Flag::z, result == 0);
    *reg = result;
}

void CPU::add_a_r8(const u8 *value) {
    const unsigned int result = R.A + *value;
    reset_flag(Flag::n);
    set_flag_if(Flag::c, ((R.A ^ *value ^ result) & 0x100u) != 0);
    set_flag_if(Flag::h, ((R.A ^ *value ^ result) & 0x10u) != 0);
    R.A = static_cast<u8>(result);
    set_flag_if(Flag::z, R.A == 0);
}

void CPU::adc_a_r8(const u8 *value) {
    // Add n + Carry flag to A
    u8 c;
    check_flag(Flag::c) ? c = 1 : c = 0;
    int result = R.A + *value + c;
    reset_flag(Flag::n);
    set_flag_if(Flag::c, result > 0xFF);
    set_flag_if(Flag::h, ((R.A & 0x0Fu) + (*value & 0x0Fu) + c) > 0x0F);
    R.A = static_cast<u8>(result);
    set_flag_if(Flag::z, R.A == 0);
}

void CPU::add_r16_r16(u16 *reg, const u16 *value) {
    const unsigned int result = *reg + *value;
    reset_flag(Flag::n);
    set_flag_if(Flag::h, (*reg & 0xfffu) + (*value & 0xfffu) > 0xfffu);
    set_flag_if(Flag::c, (result & 0x10000u) != 0);
    *reg = result;
}

void CPU::sub_a_r8(const u8 *value) {

    set_flag(Flag::n);
    set_flag_if(Flag::h, (((R.A & 0x0Fu) < (*value & 0x0Fu))));
    set_flag_if(Flag::c, (R.A & 0xFFu) < (*value & 0xFFu));

    R.A -= *value;
    set_flag_if(Flag::z, R.A == 0);
}

void CPU::sbc_a_r8(const u8 *value) {
    u8 c;
    check_flag(Flag::c) ? c = 1 : c = 0;
    int result = R.A - *value - c;
    set_flag(Flag::n);
    set_flag_if(Flag::c, result < 0);
    set_flag_if(Flag::h, ((R.A & 0x0F) - (*value & 0x0F) - c) < 0);
    R.A = static_cast<u8>(result);
    set_flag_if(Flag::z, R.A == 0);
}

u8 CPU::opcode0x3C() {  // INC A
    inc_u8_reg(&R.A);
    return 0;
}

u8 CPU::opcode0x04() {  // INC B
    inc_u8_reg(&R.B);
    return 0;
}

u8 CPU::opcode0x0C() {  // INC C
    inc_u8_reg(&R.C);
    return 0;
}

u8 CPU::opcode0x14() {  // INC D
    inc_u8_reg(&R.D);
    return 0;
}

u8 CPU::opcode0x1C() {  // INC E
    inc_u8_reg(&R.E);
    return 0;
}

u8 CPU::opcode0x24() {  // INC H
    inc_u8_reg(&R.H);
    return 0;
}

u8 CPU::opcode0x2C() {  // INC L
    inc_u8_reg(&R.L);
    return 0;
}

u8 CPU::opcode0x34() {  // INC (HL)
    u8 data = read(R.HL);
    inc_u8_reg(&data);
    write(R.HL, data);
    return 0;
}

u8 CPU::opcode0x13() {  // INC DE
    R.DE++;
    return 0;
}

u8 CPU::opcode0x03() {  // INC BC
    R.BC++;
    return 0;
}

u8 CPU::opcode0x23() {  // INC HL
    R.HL++;
    return 0;
}

u8 CPU::opcode0x33() {  // INC SP
    R.SP++;
    return 0;
}

u8 CPU::opcode0x05() {  // DEC B
    dec_u8_reg(&R.B);
    return 0;
}

u8 CPU::opcode0x0D() {  // DEC C
    dec_u8_reg(&R.C);
    return 0;
}

u8 CPU::opcode0x15() {  // DEC D
    dec_u8_reg(&R.D);
    return 0;
}

u8 CPU::opcode0x1D() {  // DEC E
    dec_u8_reg(&R.E);
    return 0;
}

u8 CPU::opcode0x25() {  // DEC H
    dec_u8_reg(&R.H);
    return 0;
}

u8 CPU::opcode0x3D() {  // DEC A
    dec_u8_reg(&R.A);
    return 0;
}

u8 CPU::opcode0x2D() {  // DEC L
    dec_u8_reg(&R.L);
    return 0;
}

u8 CPU::opcode0x35() {  // DEC (HL)
    u8 data = read(R.HL);
    dec_u8_reg(&data);
    write(R.HL, data);
    return 0;
}

u8 CPU::opcode0x0B() {  // DEC BC
    R.BC--;
    return 0;
}

u8 CPU::opcode0x1B() {  // DEC DE
    R.DE--;
    return 0;
}

u8 CPU::opcode0x2B() {  // DEC HL
    R.HL--;
    return 0;
}

u8 CPU::opcode0x3B() {  // DEC SP
    R.SP--;
    return 0;
}

u8 CPU::opcode0x80() {  // ADD A B
    add_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0x81() {  // ADD A C
    add_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0x82() {  // ADD A D
    add_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0x83() {  // ADD A E
    add_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0x84() {  // ADD A H
    add_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0x85() {  // ADD A L
    add_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0x86() {  // ADD A HL
    u8 data = read(R.HL);
    add_a_r8(&data);
    return 0;
}

u8 CPU::opcode0x87() {  // ADD A A
    add_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0x09() {  // ADD HL BC
    add_r16_r16(&R.HL, &R.BC);
    return 0;
}

u8 CPU::opcode0x19() {  // ADD HL DE
    add_r16_r16(&R.HL, &R.DE);
    return 0;
}

u8 CPU::opcode0x29() {  // ADD HL HL
    add_r16_r16(&R.HL, &R.HL);
    return 0;
}

u8 CPU::opcode0x39() {  // ADD HL SP
    add_r16_r16(&R.HL, &R.SP);
    return 0;
}

u8 CPU::opcode0xC6() {  // ADD A d8
    u8 data = read_u8_at_pc();
    add_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xE8() {  // ADD SP r8
    u8 data = read_u8_at_pc();
    int result = R.SP + data;
    set_flag_if(Flag::c, (((R.SP ^ data ^ (result & 0xFFFF)) & 0x100) == 0x100));
    set_flag_if(Flag::h, (((R.SP ^ data ^ (result & 0xFFFF)) & 0x10) == 0x10));
    R.SP = static_cast<u16>(result);
    return 0;
}

u8 CPU::opcode0x90() {  // SUB B
    sub_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0x91() {  // SUB C
    sub_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0x92() {  // SUB D
    sub_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0x93() {  // SUB E
    sub_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0x94() {  // SUB H
    sub_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0x95() {  // SUB L
    sub_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0x96() {  // SUB HL
    u8 data = read(R.HL);
    sub_a_r8(&data);
    return 0;
}

u8 CPU::opcode0x97() {  // SUB A
    sub_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0xD6() {  // SUB d8
    u8 data = read_u8_at_pc();
    sub_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xE6() {  // AND d8
    u8 data = read_u8_at_pc();
    and_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xA0() {  // AND B
    and_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0xA1() {  // AND C
    and_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0xA2() {  // AND D
    and_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0xA3() {  // AND E
    and_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0xA4() {  // AND H
    and_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0xA5() {  // AND L
    and_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0xA6() {  // AND HL
    u8 data = read(R.HL);
    and_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xA7() {  // AND A
    and_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0xB0() {  // OR B
    or_r8_r8(R.B);
    return 0;
}

u8 CPU::opcode0xB1() {  // OR C
    or_r8_r8(R.C);
    return 0;
}

u8 CPU::opcode0xB2() {  // OR D
    or_r8_r8(R.D);
    return 0;
}

u8 CPU::opcode0xB3() {  // OR E
    or_r8_r8(R.E);
    return 0;
}

u8 CPU::opcode0xB4() {  // OR H
    or_r8_r8(R.H);
    return 0;
}

u8 CPU::opcode0xB5() {  // OR L
    or_r8_r8(R.L);
    return 0;
}

u8 CPU::opcode0xB6() {  // OR HL
    or_r8_r8(read(R.HL));
    return 0;
}

u8 CPU::opcode0xB7() {  // OR A
    or_r8_r8(R.A);
    return 0;
}

u8 CPU::opcode0xF6() {  // OR d8
    or_r8_r8(read_u8_at_pc());
    return 0;
}

u8 CPU::opcode0x98() {  // SBC A B
    sbc_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0x99() {  // SBC A C
    sbc_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0x9A() {  // SBC A D
    sbc_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0x9B() {  // SBC A E
    sbc_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0x9C() {  // SBC A H
    sbc_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0x9D() {  // SBC A L
    sbc_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0x9E() {  // SBC A HL
    u8 data = read(R.HL);
    sbc_a_r8(&data);
    return 0;
}

u8 CPU::opcode0x9F() {  // SBC A A
    sbc_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0xDE() {  // SBC A d8
    u8 data = read_u8_at_pc();
    sbc_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xEE() {  // XOR d8
    u8 data = read_u8_at_pc();
    xor_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xA8() {  // XOR B
    xor_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0xA9() {  // XOR C
    xor_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0xAA() {  // XOR D
    xor_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0xAB() {  // XOR E
    xor_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0xAC() {  // XOR H
    xor_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0xAD() {  // XOR L
    xor_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0xAE() {  // XOR HL
    u8 data = read(R.HL);
    xor_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xAF() {  // XOR A
    xor_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0xB8() {  // CP B
    cp_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0xB9() {  // CP C
    cp_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0xBA() {  // CP D
    cp_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0xBB() {  // CP E
    cp_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0xBC() {  // CP H
    cp_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0xBD() {  // CP L
    cp_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0xBE() {  // CP HL
    u8 data = read(R.HL);
    cp_a_r8(&data);
    return 0;
}

u8 CPU::opcode0xBF() {  // CP A
    cp_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0xFE() {  // CP d8
    auto data = read_u8_at_pc();
    cp_a_r8(&data);
    return 0;
}

u8 CPU::opcode0x88() {  // ADC A B
    adc_a_r8(&R.B);
    return 0;
}

u8 CPU::opcode0x89() {  // ADC A C
    adc_a_r8(&R.C);
    return 0;
}

u8 CPU::opcode0x8A() {  // ADC A D
    adc_a_r8(&R.D);
    return 0;
}

u8 CPU::opcode0x8B() {  // ADC A E
    adc_a_r8(&R.E);
    return 0;
}

u8 CPU::opcode0x8C() {  // ADC A H
    adc_a_r8(&R.H);
    return 0;
}

u8 CPU::opcode0x8D() {  // ADC A L
    adc_a_r8(&R.L);
    return 0;
}

u8 CPU::opcode0x8E() {  // ADC A HL
    u8 data = read(R.HL);
    adc_a_r8(&data);
    return 0;
}

u8 CPU::opcode0x8F() {  // ADC A A
    adc_a_r8(&R.A);
    return 0;
}

u8 CPU::opcode0xCE() {  // ADC A d8
    u8 data = read_u8_at_pc();
    adc_a_r8(&data);
    return 0;
}






