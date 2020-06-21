#pragma once
#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include <vector>

#include "type.h"

constexpr u16 ROM_BANK_SIZE = 0x2000;
constexpr u16 RAM_BANK_SIZE = 0x4000;

class CARDRIDGE {
 public:
  CARDRIDGE();

  bool load_rom(const char *rom_file);

  u8 read(const u16 &addr);
  u8 *read_ptr(const u16 &addr);
  void write(const u16 &addr, const u8 &data);
  Rom_Info rom_info;

 private:
  void fill_cardridge_info();
  void print_rom();
  void build_ram() const;

  std::vector<u8> rom;
  std::vector<std::vector<u8>> ram;

  int selected_ram_bank = 0;
  int selected_rom_bank = 0;

  u8 checkSum = 0;
  u8 calcCheckSum();

 public:
  Rom_Info *get_rom_info();
};

#endif
