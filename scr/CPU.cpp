#include "CPU.h"

CPU::CPU(MEMORY *m) : mem(m) { initRegister(); }

CPU::~CPU() {}

void CPU::initRegister() {
  R.AF = 0x01B0;
  R.BC = 0x0013;
  R.DE = 0x00D8;
  R.HL = 0x014D;
  R.SP = 0xFFFE;
  R.PC = 0x0000;
  oldPC = 0x0000;

  OPCode = 0x00;
  cycles = 0;

  in_boot = true;
  IME = false;
  IME_scheduled = false;
  DI_scheduled = false;
}

u8 CPU::parseOPCode(u8 &opcode) {
  u8 function;
  if (prefix) {
    prefix = false;
    current_instruction_info = prefixedinstructions[OPCode];
    current_instruction_info.bytes -= 1;
    function = (this->*prefixedinstructions[opcode].function)();
  } else {
    current_instruction_info = instructions[OPCode];
    function = (this->*instructions[opcode].function)();
  }
  return function;
}

int CPU::update() {
  oldPC = R.PC;
  if (interrupts()) std::cout << "INTERRUPT" << std::endl;
  if (IME_scheduled) IME = true;

  OPCode = getNextOPCode();
  u8 jp = parseOPCode(OPCode);
  // print_last_opcode();
  if (jp == 0) incPC();
  cycles += current_instruction_info.cycles;
  return current_instruction_info.cycles;
}

void CPU::incPC() { R.PC += current_instruction_info.bytes; }

u8 CPU::getNextOPCode() { return read(R.PC); }

void CPU::print_last_opcode() {
  std::cout << "0x" << std::hex << std::uppercase << (int)(oldPC) << " "
            << current_instruction_info.mnemonic << "\t" << (int)(OPCode);
  if (current_instruction_info.bytes == 3)
    std::cout << " " << (int)(read_u16_at_pc());
  if (current_instruction_info.bytes == 2)
    std::cout << " 0x" << (int)(read_u8_at_pc());
  std::cout << std::endl;
}

void CPU::printallops() {
  u16 mypc = 0x00;
  instruction myinstruction_info;
  while (mypc < 0x4000) {
    u8 myopcode = read(mypc);
    if (myopcode == 0xCB) {
      myopcode = read(++mypc);
      myinstruction_info = prefixedinstructions[myopcode];
    } else {
      myinstruction_info = instructions[myopcode];
    }
    std::cout << "0x" << std::hex << std::uppercase << (int)(mypc) << "\t"
              << myinstruction_info.mnemonic;
    if (myinstruction_info.bytes == 3)
      std::cout << " " << (int)(read_u16_at_pc());
    if (myinstruction_info.bytes == 2)
      std::cout << " 0x" << (int)(read_u8_at_pc());
    std::cout << std::endl;
    mypc += myinstruction_info.bytes;
  }
}

bool CPU::in_boot_r() { return in_boot; }

u8 CPU::read(const u16 &addr) {
  switch (addr) {
    case INTERRUPT_ENABLE_REG:
      return IE_Register;
    case INTERRUPT_FLAGS_REG:
      return IF_Register;
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
    default:
      if (in_boot && R.PC >= 0x100) {
        in_boot = false;
        std::cout << "Boot verlassen nach: " << std::dec << cycles << " Cycles"
                  << std::endl;
      }
      mem->write(addr, data);
      break;
  }
}
void CPU::write_u16(const u16 &addr, const u16 &data) {
  mem->write_u16(addr, data);
}

// INTERRUPT FLAGS

void CPU::handle_interrupt(u8 val) {
  write(INTERRUPT_ENABLE_REG, val);
  ie_flags.update(this);
  if_flags.update(this);
}

bool CPU::interrupts() {
  if (!IME) {
    return false;
  }
  return 0;
}
void CPU::IE_Flags::update(CPU *cpu) {
  u8 data = cpu->read(INTERRUPT_ENABLE_REG);
  JIR = check_bit(data, 4);
  SIR = check_bit(data, 3);
  TIR = check_bit(data, 2);
  LIR = check_bit(data, 1);
  VIR = check_bit(data, 0);
}

void CPU::IF_Flags::update(CPU *cpu) {
  u8 data = cpu->read(INTERRUPT_FLAGS_REG);
  JIE = check_bit(data, 4);
  SIE = check_bit(data, 3);
  TIE = check_bit(data, 2);
  LIE = check_bit(data, 1);
  VIE = check_bit(data, 0);
}
