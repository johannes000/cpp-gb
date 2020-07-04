#include "../CPU.h"

u8 CPU::opcode0x00() {  // NOP
    return 0;
}

u8 CPU::opcode0x10() {  // STOP
    std::cout << "0x10 nicht implementiert" << std::endl;
    return 0;
}

u8 CPU::opcode0x27() {  // DAA
    auto a = R.A;

    if (check_flag(Flag::n)) {
        if (check_flag(Flag::h))
            a = (a - 6u) & 0xFFu;
        if (check_flag(Flag::c))
            a -= 0x60;
    } else {
        if (check_flag(Flag::h) || ((a & 0xF) > 9))
            a += 0x06;
        if (check_flag(Flag::c) || (a > 0x9F))
            a += 0x60;
    }
    reset_flag(Flag::h);
    set_flag_if(Flag::c, (a & 0x100u) == 0x100u);

    a &= 0xFFu;
    set_flag_if(Flag::z, a == 0);

    R.A = a;
    return 0;
}

u8 CPU::opcode0x3F() {  // CCF
    toggle_flag(Flag::c);
    reset_flags(Flag::n, Flag::h);
    return 0;
}

u8 CPU::opcode0x37() {  // SCF
    set_flag(Flag::c);
    reset_flags(Flag::n, Flag::h);
    return 0;
}

u8 CPU::opcode0x2F() {  // CPL
    R.A = ~R.A;
    set_flags(Flag::n, Flag::h);
    return 0;
}

u8 CPU::opcode0xF3() {  // DI
    DI_scheduled = true;
    return 0;
}

u8 CPU::opcode0xFB() {  // EI
    IME_scheduled = true;
    return 0;
}

u8 CPU::opcode0x76() {  // HALT
    //TODO: Für morgen
    //std::cout << "0x76 nicht implementiert" << std::endl;
    return 0;
}

u8 CPU::opcode0xCB() {  // PREFIX
    prefix = true;
    return 0;
}

u8 CPU::opcode0xD3() {  // ILLEGAL_D3
    std::cout << "0xD3 ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xDB() {  // ILLEGAL_DB
    std::cout << "0xDB ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xDD() {  // ILLEGAL_DD
    std::cout << "0xDD ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xE3() {  // ILLEGAL_E3
    std::cout << "0xE3 ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xE4() {  // ILLEGAL_E4
    std::cout << "0xE4 ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xEB() {  // ILLEGAL_EB
    std::cout << "0xEB ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xEC() {  // ILLEGAL_EC
    std::cout << "0xEC ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xED() {  // ILLEGAL_ED
    std::cout << "0xED ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xF4() {  // ILLEGAL_F4
    std::cout << "0xF4 ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xFC() {  // ILLEGAL_FC
    std::cout << "0xFC ILLEGAL" << std::endl;
    return 0;
}

u8 CPU::opcode0xFD() {  // ILLEGAL_FD
    std::cout << "0xFD ILLEGAL" << std::endl;
    return 0;
}
