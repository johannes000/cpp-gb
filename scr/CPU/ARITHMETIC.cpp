#include "../CPU.h"

void CPU::xor_r8_r8(u8 *data1, const u8 *data2) {
  // XOR data 1 und data 2 und speicher das ergebnis in data 1
  *data1 ^= *data2;
  set_flag_if(Flag::z, *data1 == 0);
  set_flags(Flag::n, Flag::h, Flag::c);
}

void CPU::and_r8_r8(u8 *reg1, const u8 *reg2) {
  // AND reg 1 und reg 2 und speicher das ergebnis in data 1
  *reg1 &= *reg2;
  set_flag_if(Flag::z, *reg1 == 0);
  reset_flags(Flag::n, Flag::c);
  set_flag(Flag::h);
}

void CPU::or_r8_r8(u8 *reg1, const u8 *reg2) {
  // OR reg 1 und reg 2 und speicher das ergebnis in data 1
  reset_all_flags();
  *reg1 |= *reg2;
  set_flag_if(Flag::z, *reg1 == 0);
}

void CPU::cp_r8_r8(const u8 *data1, const u8 *data2) {
  const u8 result = *data1 - *data2;

  set_flag(Flag::n);
  set_flag_if(Flag::h, ((*data1 & 0xf) - (*data2 & 0xf)) < 0);
  set_flag_if(Flag::c, *data1 < *data2);
  set_flag_if(Flag::z, result == 0);
}

void CPU::inc_u8_reg(u8 *reg) {
  //  Increment register.

  *reg = *reg + 1;

  reset_flag(Flag::n);

  set_flag_if(Flag::h, !(*reg & 0xFF));
  set_flag_if(Flag::z, *reg == 0);
}

void CPU::dec_u8_reg(u8 *reg) {
  // Decrement register.

  *reg = *reg - 1;

  set_flag(Flag::n);

  set_flag_if(Flag::h, (*reg & 0x0F) == 0x0F);
  set_flag_if(Flag::z, *reg == 0);
}

void CPU::add_r8_r8(u8 *reg, const u8 *value) {
  const u8 result = *reg + *value;
  reset_flag(Flag::n);
  set_flag_if(Flag::h, (*reg & 0xf) + (*value & 0xf) > 0xf);
  set_flag_if(Flag::c, (result & 0x100) != 0);
  *reg = result;
  set_flag_if(Flag::z, *reg == 0);
}

void CPU::adc_r8_r8(u8 *reg, u8 *value) {
  // Add n + Carry flag to A
  u8 c;
  check_flag(Flag::c) ? c = 1 : c = 0;
  *value += c;
  add_r8_r8(reg, value);
}

void CPU::add_r16_r16(u16 *reg, const u16 *value) {
  const u16 result = *reg + *value;
  reset_flag(Flag::n);
  set_flag_if(Flag::h, (*reg & 0xfff) + (*value & 0xfff) > 0xfff);
  set_flag_if(Flag::c, (result & 0x10000) != 0);
  *reg = result;
}

void CPU::sub_r8_r8(u8 *reg, const u8 *value) {
  const u8 result = *reg - *value;

  set_flag(Flag::n);
  set_flag_if(Flag::h, ((*reg & 0xf) - (*value & 0xf)) < 0);
  set_flag_if(Flag::c, *reg < *value);

  *reg = result;
  set_flag_if(Flag::z, *reg == 0);
}

void CPU::sbc_r8_r8(u8 *reg, u8 *value) {
  u8 c;
  check_flag(Flag::c) ? c = 1 : c = 0;
  *value += c;

  sub_r8_r8(reg, value);
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
  dec_u8_reg(&R.C);
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
  add_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0x81() {  // ADD A C
  add_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0x82() {  // ADD A D
  add_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0x83() {  // ADD A E
  add_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0x84() {  // ADD A H
  add_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0x85() {  // ADD A L
  add_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0x86() {  // ADD A HL
  u8 data = read(R.HL);
  add_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0x87() {  // ADD A A
  add_r8_r8(&R.A, &R.A);
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
  add_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xE8() {  // ADD SP r8
  u16 data = read_i8_at_pc() + 0xFF00;
  add_r16_r16(&R.SP, &data);
  return 0;
}
u8 CPU::opcode0x90() {  // SUB B
  sub_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0x91() {  // SUB C
  sub_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0x92() {  // SUB D
  sub_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0x93() {  // SUB E
  sub_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0x94() {  // SUB H
  sub_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0x95() {  // SUB L
  sub_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0x96() {  // SUB HL
  u8 data = read(R.HL);
  sub_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0x97() {  // SUB A
  sub_r8_r8(&R.A, &R.A);
  return 0;
}
u8 CPU::opcode0xD6() {  // SUB d8
  u8 data = read_u8_at_pc();
  sub_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xE6() {  // AND d8
  u8 data = read_u8_at_pc();
  and_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xA0() {  // AND B
  and_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0xA1() {  // AND C
  and_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0xA2() {  // AND D
  and_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0xA3() {  // AND E
  and_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0xA4() {  // AND H
  and_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0xA5() {  // AND L
  and_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0xA6() {  // AND HL
  u8 data = read(R.HL);
  and_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xA7() {  // AND A
  and_r8_r8(&R.A, &R.A);
  return 0;
}
u8 CPU::opcode0xB0() {  // OR B
  or_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0xB1() {  // OR C
  or_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0xB2() {  // OR D
  or_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0xB3() {  // OR E
  or_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0xB4() {  // OR H
  or_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0xB5() {  // OR L
  or_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0xB6() {  // OR HL
  u8 data = read(R.HL);
  or_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xB7() {  // OR A
  u8 data = read(R.HL);
  or_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xF6() {  // OR d8
  u8 data = read_u8_at_pc();
  or_r8_r8(&R.A, &data);
  return 0;
}

u8 CPU::opcode0x98() {  // SBC A B
  sbc_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0x99() {  // SBC A C
  sbc_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0x9A() {  // SBC A D
  sbc_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0x9B() {  // SBC A E
  sbc_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0x9C() {  // SBC A H
  sbc_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0x9D() {  // SBC A L
  sbc_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0x9E() {  // SBC A HL
  u8 data = read(R.HL);
  sbc_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0x9F() {  // SBC A A
  sbc_r8_r8(&R.A, &R.A);
  return 0;
}
u8 CPU::opcode0xDE() {  // SBC A d8
  u8 data = read_u8_at_pc();
  sbc_r8_r8(&R.A, &data);
  return 0;
}

u8 CPU::opcode0xEE() {  // XOR d8
  u8 data = read_u8_at_pc();
  xor_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xA8() {  // XOR B
  xor_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0xA9() {  // XOR C
  xor_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0xAA() {  // XOR D
  xor_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0xAB() {  // XOR E
  xor_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0xAC() {  // XOR H
  xor_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0xAD() {  // XOR L
  xor_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0xAE() {  // XOR HL
  u8 data = read(R.HL);
  xor_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xAF() {  // XOR A
  xor_r8_r8(&R.A, &R.A);
  return 0;
}
u8 CPU::opcode0xB8() {  // CP B
  cp_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0xB9() {  // CP C
  cp_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0xBA() {  // CP D
  cp_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0xBB() {  // CP E
  cp_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0xBC() {  // CP H
  cp_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0xBD() {  // CP L
  cp_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0xBE() {  // CP HL
  u8 data = read(R.HL);
  cp_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0xBF() {  // CP A
  cp_r8_r8(&R.A, &R.A);
  return 0;
}
u8 CPU::opcode0xFE() {  // CP d8
  u8 data = read_u8_at_pc();
  cp_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0x88() {  // ADC A B
  adc_r8_r8(&R.A, &R.B);
  return 0;
}
u8 CPU::opcode0x89() {  // ADC A C
  adc_r8_r8(&R.A, &R.C);
  return 0;
}
u8 CPU::opcode0x8A() {  // ADC A D
  adc_r8_r8(&R.A, &R.D);
  return 0;
}
u8 CPU::opcode0x8B() {  // ADC A E
  adc_r8_r8(&R.A, &R.E);
  return 0;
}
u8 CPU::opcode0x8C() {  // ADC A H
  adc_r8_r8(&R.A, &R.H);
  return 0;
}
u8 CPU::opcode0x8D() {  // ADC A L
  adc_r8_r8(&R.A, &R.L);
  return 0;
}
u8 CPU::opcode0x8E() {  // ADC A HL
  u8 data = read(R.HL);
  adc_r8_r8(&R.A, &data);
  return 0;
}
u8 CPU::opcode0x8F() {  // ADC A A
  adc_r8_r8(&R.A, &R.A);
  return 0;
}
u8 CPU::opcode0xCE() {  // ADC A d8
  u8 data = read_u8_at_pc();
  adc_r8_r8(&R.A, &data);
  return 0;
}
