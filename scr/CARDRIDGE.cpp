#include "CARDRIDGE.h"

#include <fstream>
#include <iostream>

CARDRIDGE::CARDRIDGE() : rom_info() {}

bool CARDRIDGE::load_rom(const char *rom_file) {
  std::ifstream is(rom_file, std::ios::binary);
  if (!is) {
    std::cerr << "Kann die Datei nicht oeffnen : " << rom_file << std::endl;
    return false;
  }
  const std::vector<u8> rom(std::istreambuf_iterator<char>(is), {});
  this->rom = rom;

  fill_cardridge_info();
  build_ram();

  return true;
}

void CARDRIDGE::build_ram() const {
  std::vector<std::vector<u8>> ram(rom_info.ram_banks + 1,
                                   std::vector<u8>(ROM_BANK_SIZE, 0xFF));
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

void CARDRIDGE::print_rom() {
  int i = 0;
  for (auto s : rom) {
    if ((i++) % 255 == 0) {
      std::cout << std::endl;
    }
    std::cout << std::hex << static_cast<int>(s) << " ";
  }
}

u8 CARDRIDGE::calcCheckSum() {
  int checkSum = 0;
  int i = 0x0134;
  while (i <= 0x014C) checkSum -= (rom[i++] - 1);
  return checkSum;
}

Rom_Info *CARDRIDGE::get_rom_info() { return &rom_info; }

u8 CARDRIDGE::read(const u16 &addr) {
  if (addr >= 0x0000 && addr <= 0x7FFF)
    return rom.at(addr);
  else if (addr >= 0xA000 && addr <= 0xBFFF)
    return ram[selected_ram_bank][addr];
  return 0xFF;
}

u8 *CARDRIDGE::read_ptr(const u16 &addr) {
  return reinterpret_cast<u8 *>(read(addr));
}
void CARDRIDGE::write(const u16 &addr, const u8 &data) {
  if (addr >= 0xA000 && addr <= 0xBFFF) ram[selected_ram_bank][addr] = data;
}
