#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include <vector>

#include "type.h"

constexpr u16 CARD_ROM_BANK_SIZE = 0x2000;
constexpr u16 CARD_RAM_BANK_SIZE = 0x4000;

class CARDRIDGE {
public:
    CARDRIDGE();

    bool load_rom(const char *rom_file);

    u8 read(const u16 &addr);

    u8 read_card_rom(const u16 &addr);

    void write(const u16 &addr, const u8 &data);

    const std::vector<u8> *get_cardridge_rom();

    Rom_Info rom_info;

private:
    void fill_cardridge_info();

    std::vector<u8> rom;
public:
    [[nodiscard]] const std::vector<u8> &get_rom() const;

private:
    std::vector<std::vector<u8>> ram;

    int selected_ram_bank = 0;

public:
    Rom_Info *get_rom_info();
};

#endif
