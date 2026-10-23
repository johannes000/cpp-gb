#include "../CPU.hpp"

auto CPU::opcode0x00() -> u8 { // NOP
	return 0;
}

auto CPU::opcode0x10() -> u8 { // STOP
	divider_register_ = 0x00;
	return 0;
}

auto CPU::opcode0x27() -> u8 { // DAA
	int a = R_.A_;

	if (!check_flag(Flag::n)) {
		if (check_flag(Flag::c) || (a > 0x99)) {
			a += 0x60;
			set_flag(Flag::c);
		}
		if (check_flag(Flag::h) || ((a & 0x0F) > 0x09))
			a += 0x06;
	} else {
		if (check_flag(Flag::c))
			a -= 0x60;
		if (check_flag(Flag::h))
			a = a - 0x6;
	}
	reset_flag(Flag::h);

	R_.A_ = static_cast<u8>(a);
	set_cpu_flag_if(Flag::z, R_.A_ == 0);
	return 0;
}

auto CPU::opcode0x3F() -> u8 { // CCF
	toggle_flag(Flag::c);
	reset_flags(Flag::n, Flag::h);
	return 0;
}

auto CPU::opcode0x37() -> u8 { // SCF
	set_flag(Flag::c);
	reset_flags(Flag::n, Flag::h);
	return 0;
}

auto CPU::opcode0x2F() -> u8 { // CPL
	R_.A_ = ~R_.A_;
	set_flags(Flag::n, Flag::h);
	return 0;
}

auto CPU::opcode0xF3() -> u8 { // DI
	IME_ = false;
	return 0;
}

auto CPU::opcode0xFB() -> u8 { // EI
	IME_ = true;
	return 0;
}

auto CPU::opcode0x76() -> u8 { // HALT
	is_halted_ = true;
	return 0;
}

auto CPU::opcode0xCB() -> u8 { // PREFIX
	prefix_ = true;
	return 0;
}

auto CPU::opcode0xD3() -> u8 { // ILLEGAL_D3
	std::cout << "0xD3 ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xDB() -> u8 { // ILLEGAL_DB
	std::cout << "0xDB ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xDD() -> u8 { // ILLEGAL_DD
	std::cout << "0xDD ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xE3() -> u8 { // ILLEGAL_E3
	std::cout << "0xE3 ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xE4() -> u8 { // ILLEGAL_E4
	std::cout << "0xE4 ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xEB() -> u8 { // ILLEGAL_EB
	std::cout << "0xEB ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xEC() -> u8 { // ILLEGAL_EC
	std::cout << "0xEC ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xED() -> u8 { // ILLEGAL_ED
	std::cout << "0xED ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xF4() -> u8 { // ILLEGAL_F4
	std::cout << "0xF4 ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xFC() -> u8 { // ILLEGAL_FC
	std::cout << "0xFC ILLEGAL" << std::endl;
	return 0;
}

auto CPU::opcode0xFD() -> u8 { // ILLEGAL_FD
	std::cout << "0xFD ILLEGAL" << std::endl;
	return 0;
}
