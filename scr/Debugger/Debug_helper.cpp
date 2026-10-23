#include "Debugger.hpp"

auto Debugger::what_register_is_this(const u16 &reg) -> const char * {
	if ((reg & 0xFF00u) == 0xFF00u)
		switch (reg & 0x00FFu) {
			case 0x00:
				return "JOYPAD";
			case 0x04:
				return "DIV";
			case 0x05:
				return "TIMA";
			case 0x06:
				return "TMA";
			case 0x07:
				return "TAC";
			case 0x0f:
				return "IF Interrupt Flags";
			case 0x10:
				return "NR10";
			case 0x11:
				return "NR11";
			case 0x12:
				return "NR12";
			case 0x14:
				return "NR14";
			case 0x16:
				return "NR21";
			case 0x17:
				return "NR22";
			case 0x19:
				return "NR24";
			case 0x1A:
				return "NR30";
			case 0x1B:
				return "NR31";
			case 0x1C:
				return "NR32";
			case 0x1E:
				return "NR33";
			case 0x20:
				return "NR41";
			case 0x21:
				return "NR42";
			case 0x22:
				return "NR43";
			case 0x23:
				return "NR30";
			case 0x24:
				return "NR50";
			case 0x25:
				return "NR51";
			case 0x26:
				return "NR52";
			case 0x40:
				return "LCD Controll";
			case 0x44:
				return "LY Current Scalline";
			case 0x45:
				return "LYC Compare";
			case 0x41:
				return "STAT";
			case 0x42:
				return "SCY";
			case 0x43:
				return "SCX";
			case 0x4A:
				return "WY";
			case 0x4B:
				return "WX";
			case 0x46:
				return "DMA";
			case 0x47:
				return "BG PAL";
			case 0x48:
				return "OBJ PAL 1";
			case 0x49:
				return "OBJ PAL 2";
			case 0xFF:
				return "IE Interrupt Enable";
			default:
				return " ";
		}
	return "";
}

auto Debugger::what_register_is_this(const u8 &reg) -> const char * {
	return what_register_is_this(static_cast<u16>(0xFF00u | reg));
}