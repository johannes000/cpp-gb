#include "MEMORY.h"
#include <iostream>
#include <fstream>


MEMORY::MEMORY(CPU *c, CARDRIDGE *card, PPU *p)
        : cardridge(card), ppu(p), cpu(c) {
    load_boot_rom();
}

MEMORY::~MEMORY() {}

u8 MEMORY::read(const u16 &addr) {
    if (addr <= CARD_ROM_END) {
        if (cpu->in_boot_r() && addr <= BOOT_ROM_END) {
            return boot.at(addr);
        }
        return cardridge->read(addr);
    } else if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
        return ppu->read(addr);
    } else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END) {
        return cardridge->read(addr);
    } else if (addr >= WORKING_RAM && addr <= WORKING_RAM_END) {
        return memory.at(addr);
    } else if (addr >= WORKING_RAM_SHADOW && addr <= WORKING_RAM_SHADOW_END) {
        return memory.at(addr - WORKING_RAM_SHADOW_OFFSET);
    } else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
        return ppu->read(addr);
    } else if (addr >= MEMORY_MAPPED_IO && addr <= MEMORY_MAPPED_IO_END) {
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
            case BG_PALETTE_DATA_0:
            case BG_PALETTE_DATA_1:
                return ppu->read(addr);
            case INTERRUPT_FLAGS_REG:
                return cpu->read(addr);
            default:
                return memory.at(addr);
        }
    } else if (addr >= ZERO_PAGE_RAM && addr <= ZERO_PAGE_RAM_END) {
        return memory.at(addr);
    } else if (addr == INTERRUPT_ENABLE_REG) {
        return cpu->read(addr);
    }
    return 0x00;
}

void MEMORY::write(const u16 &addr, const u8 &data) {
    if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
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
            case BG_PALETTE_DATA_0:
            case BG_PALETTE_DATA_1:
                ppu->write(addr, data);
                break;
            case DMA_TRANSFER:  // This register starts the OAM DMA transfer
                dma_transfer(data);
                break;
            case INTERRUPT_FLAGS_REG:
                cpu->write(addr, data);
                break;
            default:
                memory.at(addr) = data;
                break;
        }
    } else if (addr >= ZERO_PAGE_RAM && addr <= ZERO_PAGE_RAM_END) {
        memory.at(addr) = data;
    } else if (addr == INTERRUPT_ENABLE_REG) {
        cpu->write(addr, data);
    }
}

void MEMORY::write_u16(const u16 &addr, const u16 &data) {
    if (addr >= 0x00 && (addr + 1) < 0xFFFF) {
        const u8 data_lb = data >> 8u;
        const u8 data_hb = data & 0xFFu;
        write(addr, data_hb);
        write(addr + 1, data_lb);
    }
}

void MEMORY::reset() {
    memory = std::vector<u8>(0x10000, 0);

    load_boot_rom();

    write(0xFF05, 0x00);  // TIMA
    write(0xFF06, 0x00);  // TMA
    write(0xFF07, 0x00);  // TAC
    write(0xFF10, 0x80);  // NR10
    write(0xFF11, 0xBF);  // NR11
    write(0xFF12, 0xF3);  // NR12
    write(0xFF14, 0xBF);  // NR14
    write(0xFF16, 0x3F);  // NR21
    write(0xFF17, 0x00);  // NR22
    write(0xFF19, 0xBF);  // NR24
    write(0xFF1A, 0x7F);  // NR30
    write(0xFF1B, 0xFF);  // NR31
    write(0xFF1C, 0x9F);  // NR32
    write(0xFF1E, 0xBF);  // NR33
    write(0xFF20, 0xFF);  // NR41
    write(0xFF21, 0x00);  // NR42
    write(0xFF22, 0x00);  // NR43
    write(0xFF23, 0xBF);  // NR30
    write(0xFF24, 0x77);  // NR50
    write(0xFF25, 0xF3);  // NR51
    write(0xFF26, 0xF1);  // - GB, $F0 - SGB; NR52
    write(0xFF40, 0x91);  // LCDC
    write(0xFF42, 0x00);  // SCY
    write(0xFF43, 0x00);  // SCX
    write(0xFF45, 0x00);  // LYC
    write(0xFF47, 0xFC);  // BGP
    write(0xFF48, 0xFF);  // OBP0
    write(0xFF49, 0xFF);  // OBP1
    write(0xFF4A, 0x00);  // WY
    write(0xFF4B, 0x00);  // WX
    write(0xFFFF, 0x00);  // IE
}

void MEMORY::dma_transfer(const u8 &data) {
    memory.at(DMA_TRANSFER) = data;
    const u16 addr = data << 8;
    for (u8 i = 0; i < 0xA0; i++) {
        write(0xFE00 + i, read(addr + i));
    }
}

bool MEMORY::load_boot_rom() {
    std::ifstream is(boot_rom_path, std::ios::binary);
    if (!is) {
        std::cerr << "Kann die Boot Rom nicht öffnen!" << std::endl;
        std::abort();
        return false;
    }
    const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
    this->boot = romf;
    return true;
}
