#include "CARDRIDGE.h"

#include <fstream>
#include <iostream>
#include <cstring>


CARDRIDGE::CARDRIDGE() : rom_info(), ram_enabled(false) {}

bool CARDRIDGE::load_rom(const char *rom_file) {
    std::ifstream is(rom_file, std::ios::binary);
    if (!is) {
        std::cerr << "Kann die Datei nicht oeffnen : " << rom_file << std::endl;
        return false;
    }
    const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
    this->rom = romf;

    std::cout << std::hex << (int) read(0x7FF3) << std::endl;

    fill_cardridge_info();

    for (int i = 0; i < rom_info.ram_banks + 1; ++i) {
        ram.push_back(std::vector<u8>(CARD_RAM_BANK_SIZE, 0xFF));
    }

    return true;
}

void CARDRIDGE::fill_cardridge_info() {
    if (rom.empty()) return;

    memcpy(rom_info.title, &rom[0x134], 15);
    rom_info.cgb = static_cast<CGBFlag>(rom[0x143]);
    rom_info.ctype = static_cast<CType>(rom[0x147]);

    rom_info.rom_size = static_cast<ROMSize>(rom[0x148]);
    switch (rom_info.rom_size) {
        case ROMSize::KB32:
            rom_info.rom_banks = 0;
            break;
        case ROMSize::KB64:
            rom_info.rom_banks = 4;
            break;
        case ROMSize::KB128:
            rom_info.rom_banks = 8;
            break;
        case ROMSize::KB256:
        case ROMSize::KB512:
            rom_info.rom_banks = 16;
            break;
        case ROMSize::MB1:
            switch (rom_info.ctype) {
                case CType::MBC1:
                    rom_info.rom_banks = 63;
                    break;
                default:
                    rom_info.rom_banks = 64;
                    break;
            }
            break;
        case ROMSize::MB2:
            switch (rom_info.ctype) {
                case CType::MBC1:
                    rom_info.rom_banks = 125;
                    break;
                default:
                    rom_info.rom_banks = 128;
                    break;
            }
            break;
        case ROMSize::MB4:
            rom_info.rom_banks = 256;
            break;
        case ROMSize::MB8:
            rom_info.rom_banks = 512;
            break;
        case ROMSize::MB1_1:
            rom_info.rom_banks = 72;
            break;
        case ROMSize::MB1_2:
            rom_info.rom_banks = 80;
            break;
        case ROMSize::MB1_5:
            rom_info.rom_banks = 96;
            break;
        default:
            break;
    }

    rom_info.ram_size = static_cast<RAMSize>(rom[0x149]);
    switch (rom_info.ram_size) {
        case RAMSize::None:
        case RAMSize::KB2:
        case RAMSize::KB8:
            rom_info.ram_banks = 0;
            break;
        case RAMSize::KB32:
            rom_info.ram_banks = 4;
            break;
        case RAMSize::KB128:
            rom_info.ram_banks = 16;
            break;
        case RAMSize::KB64:
            rom_info.ram_banks = 8;
            break;
        default:
            break;
    }
}

u8 CARDRIDGE::read(const u16 &addr) {
    if (addr <= CARD_BANKS_END)
        return rom.at(addr + selected_rom_bank * CARD_ROM_BANK_SIZE);
    else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END
             && rom_info.ram_size != RAMSize::None
             && ram_enabled) {
        return ram[selected_ram_bank][addr - CARD_EXTERNAL_RAM];
    }

    return 0xFF;
}

void CARDRIDGE::write(const u16 &addr, const u8 &data) {
    if (addr >= 0x2000 && addr <= 0x3FFF && rom_info.rom_banks > 0) {
        selected_rom_bank = data & 0x1Fu;
    }

    if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END
        && rom_info.ram_size != RAMSize::None
        && ram_enabled) {
        ram[selected_ram_bank].at(addr - CARD_EXTERNAL_RAM) = data;
    } else if (addr <= 0x1FFF) {
        if (data == 0x00)
            ram_enabled = false;
        if (data == 0xA)
            ram_enabled = true;
    }

}


u8 CARDRIDGE::read_card_rom(const u16 &addr) {
    return rom.at(addr);
}
