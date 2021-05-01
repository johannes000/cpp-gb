#include "MEMORY.h"
#include <iostream>
#include <fstream>
#include <windows.h>

MEMORY::MEMORY(CPU *c, CARDRIDGE *card, PPU *p, CONTROLS *j)
	: cardridge(card), ppu(p), cpu(c), joypad(j) {
	load_boot_rom();
}

void MEMORY::reset() {
}

u8 MEMORY::read(const u16 &addr) {
	if (addr <= CARD_ROM_END) {
		if (cpu->is_in_boot() && addr <= BOOT_ROM_END) {
			return boot.at(addr);
		}
		return cardridge->read(addr);
	} else if (addr <= GRAPHICS_RAM_END) {
		return ppu->read(addr);
	} else if (addr <= CARD_EXTERNAL_RAM_END) {
		return cardridge->read(addr);
	} else if (addr <= WORKING_RAM_END) {
		return memory.at(addr);
	} else if (addr <= WORKING_RAM_SHADOW_END) {
		return memory.at(addr - WORKING_RAM_SHADOW_OFFSET);
	} else if (addr <= SPRITE_INFORMATION_END) {
		return ppu->read(addr);
	} else if (addr >= MEMORY_MAPPED_IO && addr <= MEMORY_MAPPED_IO_END) {
		return handle_mapped_io_read(addr);
	} else if (addr <= ZERO_PAGE_RAM_END) {
		return memory.at(addr);
	} else if (addr == INTERRUPT_ENABLE_REG) {
		return cpu->read(addr);
	}
	return 0x00;
}

u8 MEMORY::handle_mapped_io_read(const u16 &addr) const {
	switch (addr) {
		case JOY_P1_REGISTER:
			return joypad->read(addr);
		case LCD_CONTROL:
		case LCD_STATUS:
		case LCD_SCY:
		case LCD_SCX:
		case LCD_LY:
		case LCD_LYC:
		case LCD_WY:
		case LCD_WX:
		case BG_PALETTE_DATA:
		case SPRITE_PALETTE_DATA_0:
		case SPRITE_PALETTE_DATA_1:
			return ppu->read(addr);
		case INTERRUPT_FLAGS_REG:
		case INTERRUPT_ENABLE_REG:
		case DIVIDER_REGISTER:
		case TIMER_CONTROL:
		case TIMER_COUNTER:
		case TIMER_MODULO:
			return cpu->read(addr);
		default:
			return memory.at(addr);
	}
}
void MEMORY::write(const u16 &addr, const u8 &data) {
	if (addr == 0xFF02 && data == 0x81) {
		char c = static_cast<char>(read(0xff01));
		TestOutput.push_back(c);
	}

	if (addr <= CARD_ROM_END) {
		cardridge->write(addr, data);
	} else if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
		ppu->write(addr, data);
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END) {
		cardridge->write(addr, data);
	} else if (addr >= WORKING_RAM && addr <= WORKING_RAM_END) {
		memory.at(addr) = data;
	} else if (addr >= WORKING_RAM_SHADOW && addr <= WORKING_RAM_SHADOW_END) {
		memory.at(addr - WORKING_RAM_SHADOW_OFFSET) = data;
	} else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
		ppu->write(addr, data);
	} else if (addr >= MEMORY_MAPPED_IO && addr <= MEMORY_MAPPED_IO_END) {
		handle_mapped_io_write(addr, data);
	} else if (addr >= ZERO_PAGE_RAM && addr <= ZERO_PAGE_RAM_END) {
		memory.at(addr) = data;
	} else if (addr == INTERRUPT_ENABLE_REG) {
		cpu->write(addr, data);
	}
}

void MEMORY::handle_mapped_io_write(const u16 &addr, const u8 &data) {
	switch (addr) {
		case LCD_CONTROL:
		case LCD_STATUS:
		case LCD_SCY:
		case LCD_SCX:
		case LCD_LY:
		case LCD_LYC:
		case LCD_WY:
		case LCD_WX:
		case BG_PALETTE_DATA:
		case SPRITE_PALETTE_DATA_0:
		case SPRITE_PALETTE_DATA_1:
			ppu->write(addr, data);
			break;
		case DMA_TRANSFER:
			cpu->write(addr, data);
			break;
		case INTERRUPT_FLAGS_REG:
		case IN_BOOT_ADDRESS:
		case DIVIDER_REGISTER:
		case TIMER_CONTROL:
		case TIMER_COUNTER:
		case TIMER_MODULO:
			cpu->write(addr, data);
			break;
		case JOY_P1_REGISTER:
			joypad->write(addr, data);
			break;
		default:
			memory.at(addr) = data;
			break;
	}
}

void MEMORY::write_u16(const u16 &addr, const u16 &data) {
	const u8 data_lb = data >> 8u;
	const u8 data_hb = data & 0xFFu;
	write(addr, data_hb);
	write(addr + 1, data_lb);
}

void MEMORY::reset(bool skip_boot_rom) {

	memory = std::vector<u8>(0x10000, 0);
	if (skip_boot_rom) {

		// Zero the memory from $8000-$9FFF (VRAM)
		for (u16 addr = 0x8000; addr < 0xA000; addr++) {
			write(addr, 0x00);
		}
		write(0xFF02, 0x72);
		write(0xFF04, 0x00); // DIV
		write(0xFF05, 0x00); // TIMA
		write(0xFF06, 0x00); // TMA
		write(0xFF07, 0x00); // TAC
		write(0xFF10, 0x80); // NR10
		write(0xFF11, 0xBF); // NR11
		write(0xFF12, 0xF3); // NR12
		write(0xFF14, 0xBF); // NR14
		write(0xFF16, 0x3F); // NR21
		write(0xFF17, 0x00); // NR22
		write(0xFF19, 0xBF); // NR24
		write(0xFF1A, 0x7F); // NR30
		write(0xFF1B, 0xFF); // NR31
		write(0xFF1C, 0x9F); // NR32
		write(0xFF1E, 0xBF); // NR33
		write(0xFF20, 0xFF); // NR41
		write(0xFF21, 0x00); // NR42
		write(0xFF22, 0x00); // NR43
		write(0xFF23, 0xBF); // NR30
		write(0xFF24, 0x77); // NR50
		write(0xFF25, 0xF3); // NR51
		write(0xFF26, 0xF1); // - GB, $F0 - SGB; NR52
		write(0xFF40, 0x91); // LCDC
		write(0xFF42, 0x00); // SCY
		write(0xFF43, 0x00); // SCX
		write(0xFF45, 0x00); // LYC
		write(0xFF47, 0xFC); // BGP
		write(0xFF48, 0xFF); // OBP0
		write(0xFF49, 0xFF); // OBP1
		write(0xFF4A, 0x00); // WY
		write(0xFF4B, 0x00); // WX
		write(0xFFFF, 0x00); // IE
	} else {
		boot.erase(boot.begin(), boot.end());
		load_boot_rom();
	}
}

bool MEMORY::load_boot_rom() {
	const char *boot_rom_path = "C:/Users/knaub/source/repos/testroms/DMG_ROM.bin";

	std::ifstream is(boot_rom_path, std::ios::binary);
	if (!is) {
		std::cerr << "Kann die Boot Rom nicht öffnen!" << std::endl;
		std::cerr << boot_rom_path << std::endl;
		std::abort();
	}
	const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
	this->boot = romf;
	return true;
}

u8 MEMORY::read_card_rom(const u16 &addr) {
	return cardridge->read_card_rom(addr);
}

void MEMORY::request_interrupt(Interrupt inter) {
	cpu->request_interrupt(inter);
}

void MEMORY::write_direct(const u16 &addr, const u8 &data) {
	memory.at(addr) = data;
}
