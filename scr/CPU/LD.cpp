
#include "../CPU.hpp"

auto CPU::opcode0x3E() -> u8 { // LD A d8
	R_.A_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x06() -> u8 { // LD B d8
	R_.B_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x0E() -> u8 { // LD C d8
	R_.C_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x16() -> u8 { // LD D d8
	R_.D_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x1E() -> u8 { // LD E d8
	R_.E_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x26() -> u8 { // LD H d8
	R_.H_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x2E() -> u8 { // LD L d8
	R_.L_ = read_u8_at_pc();
	return 0;
}

auto CPU::opcode0x01() -> u8 { // LD BC d16
	R_.BC_ = read_u16_at_pc();
	return 0;
}

auto CPU::opcode0x11() -> u8 { // LD DE d16
	R_.DE_ = read_u16_at_pc();
	return 0;
}

auto CPU::opcode0x21() -> u8 { // LD HL d16
	R_.HL_ = read_u16_at_pc();
	return 0;
}

auto CPU::opcode0x31() -> u8 { // LD SP d16
	R_.SP_ = read_u16_at_pc();
	return 0;
}

auto CPU::opcode0x02() -> u8 { // LD [BC] A
	mem_->write(R_.BC_, R_.A_);
	return 0;
}

auto CPU::opcode0x08() -> u8 { // LD (a16) SP
	mem_->write_u16(read_u16_at_pc(), R_.SP_);
	return 0;
}

auto CPU::opcode0xFA() -> u8 { // LD A (a16)
	R_.A_ = read(read_u16_at_pc());
	return 0;
}

auto CPU::opcode0xEA() -> u8 { // LD (a16) A
	mem_->write(read_u16_at_pc(), R_.A_);
	return 0;
}

auto CPU::opcode0x0A() -> u8 { // LD A (BC)
	R_.A_ = read(R_.BC_);
	return 0;
}

auto CPU::opcode0x78() -> u8 { // LD A B
	R_.A_ = R_.B_;
	return 0;
}

auto CPU::opcode0x79() -> u8 { // LD A C
	R_.A_ = R_.C_;
	return 0;
}

auto CPU::opcode0xF2() -> u8 { // LD A (C)
	R_.A_ = read(0xFF00 + R_.C_);
	return 0;
}

auto CPU::opcode0x7A() -> u8 { // LD A D
	R_.A_ = R_.D_;
	return 0;
}

auto CPU::opcode0x7B() -> u8 { // LD A E
	R_.A_ = R_.E_;
	return 0;
}

auto CPU::opcode0x7C() -> u8 { // LD A H
	R_.A_ = R_.H_;
	return 0;
}

auto CPU::opcode0x7D() -> u8 { // LD A L
	R_.A_ = R_.L_;
	return 0;
}

auto CPU::opcode0x7F() -> u8 { // LD A A
	return 0;
}

auto CPU::opcode0x7E() -> u8 { // LD A (HL)
	R_.A_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x1A() -> u8 { // LD A (DE)
	R_.A_ = read(R_.DE_);
	return 0;
}

auto CPU::opcode0x2A() -> u8 { // LD A (HL+)
	R_.A_ = read(R_.HL_++);
	return 0;
}

auto CPU::opcode0x3A() -> u8 { // LD A (HL-)
	R_.A_ = read(R_.HL_--);
	return 0;
}

auto CPU::opcode0x22() -> u8 { // LD (HL+) A
	write(R_.HL_++, R_.A_);
	return 0;
}

auto CPU::opcode0x32() -> u8 { // LD (HL-) A
	write(R_.HL_--, R_.A_);
	return 0;
}

auto CPU::opcode0x77() -> u8 { // LD (HL) A
	write(R_.HL_, R_.A_);
	return 0;
}

auto CPU::opcode0x12() -> u8 { // LD (DE) A
	write(R_.DE_, R_.A_);
	return 0;
}

auto CPU::opcode0x47() -> u8 { // LD B A
	R_.B_ = R_.A_;
	return 0;
}

auto CPU::opcode0x4F() -> u8 { // LD C A
	R_.C_ = R_.A_;
	return 0;
}

auto CPU::opcode0x57() -> u8 { // LD D A
	R_.D_ = R_.A_;
	return 0;
}

auto CPU::opcode0x67() -> u8 { // LD H A
	R_.H_ = R_.A_;
	return 0;
}

auto CPU::opcode0x6F() -> u8 { // LD L A
	R_.L_ = R_.A_;
	return 0;
}

auto CPU::opcode0xE2() -> u8 { // LD (C) A
	mem_->write(0xFF00 + R_.C_, R_.A_);
	return 0;
}

auto CPU::opcode0x40() -> u8 { // LD B B
	return 0;
}

auto CPU::opcode0x41() -> u8 { // LD B C
	R_.B_ = R_.C_;
	return 0;
}

auto CPU::opcode0x42() -> u8 { // LD B D
	R_.B_ = R_.D_;
	return 0;
}

auto CPU::opcode0x43() -> u8 { // LD B E
	R_.B_ = R_.E_;
	return 0;
}

auto CPU::opcode0x44() -> u8 { // LD B H
	R_.B_ = R_.H_;
	return 0;
}

auto CPU::opcode0x45() -> u8 { // LD B L
	R_.B_ = R_.L_;
	return 0;
}

auto CPU::opcode0x46() -> u8 { // LD B (HL)
	R_.B_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x48() -> u8 { // LD C B
	R_.C_ = R_.B_;
	return 0;
}

auto CPU::opcode0x49() -> u8 { // LD C C
	return 0;
}

auto CPU::opcode0x4A() -> u8 { // LD C D
	R_.C_ = R_.D_;
	return 0;
}

auto CPU::opcode0x4B() -> u8 { // LD C E
	R_.C_ = R_.E_;
	return 0;
}

auto CPU::opcode0x4C() -> u8 { // LD C H
	R_.C_ = R_.H_;
	return 0;
}

auto CPU::opcode0x4D() -> u8 { // LD C L
	R_.C_ = R_.L_;
	return 0;
}

auto CPU::opcode0x4E() -> u8 { // LD C HL
	R_.C_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x50() -> u8 { // LD D B
	R_.D_ = R_.B_;
	return 0;
}

auto CPU::opcode0x51() -> u8 { // LD D C
	R_.D_ = R_.C_;
	return 0;
}

auto CPU::opcode0x52() -> u8 { // LD D D
	return 0;
}

auto CPU::opcode0x53() -> u8 { // LD D E
	R_.D_ = R_.E_;
	return 0;
}

auto CPU::opcode0x54() -> u8 { // LD D H
	R_.D_ = R_.H_;
	return 0;
}

auto CPU::opcode0x55() -> u8 { // LD D L
	R_.D_ = R_.L_;
	return 0;
}

auto CPU::opcode0x56() -> u8 { // LD D HL
	R_.D_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x5F() -> u8 { // LD E A
	R_.E_ = R_.A_;
	return 0;
}

auto CPU::opcode0x58() -> u8 { // LD E B
	R_.E_ = R_.B_;
	return 0;
}

auto CPU::opcode0x59() -> u8 { // LD E C
	R_.E_ = R_.C_;
	return 0;
}

auto CPU::opcode0x5A() -> u8 { // LD E D
	R_.E_ = R_.D_;
	return 0;
}

auto CPU::opcode0x5B() -> u8 { // LD E E
	return 0;
}

auto CPU::opcode0x5C() -> u8 { // LD E H
	R_.E_ = R_.H_;
	return 0;
}

auto CPU::opcode0x5D() -> u8 { // LD E L
	R_.E_ = R_.L_;
	return 0;
}

auto CPU::opcode0x5E() -> u8 { // LD E HL
	R_.E_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x60() -> u8 { // LD H B
	R_.H_ = R_.B_;
	return 0;
}

auto CPU::opcode0x61() -> u8 { // LD H C
	R_.H_ = R_.C_;
	return 0;
}

auto CPU::opcode0x62() -> u8 { // LD H D
	R_.H_ = R_.D_;
	return 0;
}

auto CPU::opcode0x63() -> u8 { // LD H E
	R_.H_ = R_.E_;
	return 0;
}

auto CPU::opcode0x64() -> u8 { // LD H H
	return 0;
}

auto CPU::opcode0x65() -> u8 { // LD H L
	R_.H_ = R_.L_;
	return 0;
}

auto CPU::opcode0x66() -> u8 { // LD H HL
	R_.H_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x68() -> u8 { // LD L B
	R_.L_ = R_.B_;
	return 0;
}

auto CPU::opcode0x69() -> u8 { // LD L C
	R_.L_ = R_.C_;
	return 0;
}

auto CPU::opcode0x6A() -> u8 { // LD L D
	R_.L_ = R_.D_;
	return 0;
}

auto CPU::opcode0x6B() -> u8 { // LD L E
	R_.L_ = R_.E_;
	return 0;
}

auto CPU::opcode0x6C() -> u8 { // LD L H
	R_.L_ = R_.H_;
	return 0;
}

auto CPU::opcode0x6D() -> u8 { // LD L L
	return 0;
}

auto CPU::opcode0x6E() -> u8 { // LD L HL
	R_.L_ = read(R_.HL_);
	return 0;
}

auto CPU::opcode0x70() -> u8 { // LD HL B
	mem_->write(R_.HL_, R_.B_);
	return 0;
}

auto CPU::opcode0x71() -> u8 { // LD HL C
	mem_->write(R_.HL_, R_.C_);
	return 0;
}

auto CPU::opcode0x72() -> u8 { // LD HL D
	mem_->write(R_.HL_, R_.D_);
	return 0;
}

auto CPU::opcode0x73() -> u8 { // LD HL E
	mem_->write(R_.HL_, R_.E_);
	return 0;
}

auto CPU::opcode0x74() -> u8 { // LD HL H
	mem_->write(R_.HL_, R_.H_);
	return 0;
}

auto CPU::opcode0x75() -> u8 { // LD HL L
	mem_->write(R_.HL_, R_.L_);
	return 0;
}

auto CPU::opcode0x36() -> u8 { // LD (HL) d8
	mem_->write(R_.HL_, read_u8_at_pc());
	return 0;
}

auto CPU::opcode0xF8() -> u8 { // LD HL SP + r8
	const s8 r8 = read_i8_at_pc();
	const u16 result = R_.SP_ + r8;
	reset_all_flags();
	const u16 check = R_.SP_ ^ r8 ^ result;
	set_cpu_flag_if(Flag::c, (check & 0x100u) == 0x100u);
	set_cpu_flag_if(Flag::h, (check & 0x10u) == 0x10u);
	R_.HL_ = result;
	return 0;
}

auto CPU::opcode0xF9() -> u8 { // LD SP HL
	R_.SP_ = R_.HL_;
	return 0;
}

auto CPU::opcode0xE0() -> u8 { // LDH a8 A
	mem_->write(0xFF00 + read_u8_at_pc(), R_.A_);
	return 0;
}

auto CPU::opcode0xF0() -> u8 { // LDH A a8
	// Load value in register A from byte at address n16, provided it is between $FF00 and $FFFF.
	auto data = read_u8_at_pc();
	R_.A_ = read(0xFF00 + data);
	return 0;
}
