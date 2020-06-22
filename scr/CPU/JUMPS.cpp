#include "../CPU.h"

void CPU::push_r16_reg(u16 *reg) {
  // Push register pair nn onto stack.
  // Decrement Stack Pointer(SP) twice
  R.SP -= 2;
  mem->write_u16(R.SP, *reg);
}

void CPU::pop_r16_reg(u16 *reg) {
  // Pop two bytes off stack into register pair nn.
  // Increment Stack Pointer(SP) twice
  *reg = read_u16_sp();
  R.SP += 2;
}
void CPU::ret() {
  //  Pop two bytes from stack & jump to that address.
  R.PC = read_u16_sp();
  R.SP += 2;
}

void CPU::call() {
  u16 next_inst = R.PC + 3;
  push_r16_reg(&next_inst);
  R.PC = read_u16_at_pc();
}

u8 CPU::opcode0x18() {  // JR r8
  R.PC += read_i8_at_pc();
  return 1;
}
u8 CPU::opcode0x20() {  // JR NZ r8
  if (!check_flag(Flag::z)) {
    R.PC += read_i8_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0x28() {  // JR Z r8
  if (check_flag(Flag::z)) {
    R.PC += read_i8_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0x30() {  // JR NC r8
  if (!check_flag(Flag::c)) {
    R.PC += read_i8_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0x38() {  // JR C r8
  if (check_flag(Flag::c)) {
    R.PC += read_i8_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xC0() {  // RET NZ
  if (!check_flag(Flag::z)) {
    ret();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xC8() {  // RET Z
  if (check_flag(Flag::z)) {
    ret();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xD0() {  // RET NC
  if (!check_flag(Flag::c)) {
    ret();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xD8() {  // RET C
  if (check_flag(Flag::c)) {
    ret();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xC2() {  // JP NZ a16
  if (!check_flag(Flag::z)) {
    R.PC = read_u16_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xC3() {  // JP a16
  R.PC = read_u16_at_pc();
  return 1;
}
u8 CPU::opcode0xCA() {  // JP Z a16
  if (check_flag(Flag::z)) {
    R.PC = read_u16_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xD2() {  // JP NC a16
  if (!check_flag(Flag::c)) {
    R.PC = read_u16_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xDA() {  // JP C a16
  if (check_flag(Flag::c)) {
    R.PC = read_u16_at_pc();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xE9() {  // JP HL
  R.PC = R.HL;
  return 1;
}
u8 CPU::opcode0xC4() {  // CALL NZ a16
  if (!check_flag(Flag::z)) {
    call();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xCC() {  // CALL Z a16
  if (check_flag(Flag::z)) {
    call();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xCD() {  // CALL a16
  call();
  return 1;
}
u8 CPU::opcode0xD4() {  // CALL NC a16
  if (!check_flag(Flag::c)) {
    call();
    return 1;
  }
  return 0;
}
u8 CPU::opcode0xDC() {  // CALL C a16
  if (check_flag(Flag::c)) call();
  return 1;
}
u8 CPU::opcode0xDF() {  // RST 18H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0018;
  return 1;
}
u8 CPU::opcode0xE7() {  // RST 20H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0020;
  return 1;
}
u8 CPU::opcode0xEF() {  // RST 28H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0028;
  return 1;
}
u8 CPU::opcode0xF7() {  // RST 30H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0030;
  return 1;
}
u8 CPU::opcode0xFF() {  // RST 38H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0038;
  return 1;
}
u8 CPU::opcode0xC9() {  // RET
  ret();
  return 1;
}
u8 CPU::opcode0xD9() {  // RETI
  ret();
  IME_scheduled = true;
  return 1;
}
u8 CPU::opcode0xC7() {  // RST 00H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0000;
  return 1;
}
u8 CPU::opcode0xCF() {  // RST 08H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0008;
  return 1;
}
u8 CPU::opcode0xD7() {  // RST 10H
  push_r16_reg(&R.PC);
  R.PC = 0x0000 + 0x0010;
  return 1;
}
u8 CPU::opcode0xC1() {  // POP BC
  pop_r16_reg(&R.BC);
  return 0;
}
u8 CPU::opcode0xD1() {  // POP DE
  pop_r16_reg(&R.DE);
  return 0;
}
u8 CPU::opcode0xE1() {  // POP HL
  pop_r16_reg(&R.HL);
  return 0;
}
u8 CPU::opcode0xF1() {  // POP AF
  pop_r16_reg(&R.AF);
  return 0;
}
u8 CPU::opcode0xC5() {  // PUSH BC
  push_r16_reg(&R.BC);
  return 0;
}
u8 CPU::opcode0xD5() {  // PUSH DE
  push_r16_reg(&R.DE);
  return 0;
}
u8 CPU::opcode0xE5() {  // PUSH HL
  push_r16_reg(&R.HL);
  return 0;
}
u8 CPU::opcode0xF5() {  // PUSH AF
  push_r16_reg(&R.AF);
  return 0;
}
