#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include <vector>

#include "type.h"

constexpr u16 CARD_ROM_BANK_SIZE = 0x4000;
constexpr u16 CARD_RAM_BANK_SIZE = 0x4000;

class CARDRIDGE {
public:
    CARDRIDGE();

    bool load_rom(const char *rom_file);

    Rom_Info rom_info;

    u8 read(const u16 &addr);

    u8 read_card_rom(const u16 &addr);

    void write(const u16 &addr, const u8 &data);


    [[nodiscard]] unsigned int get_selected_rom_bank() const { return selected_rom_bank; }

private:
    void fill_cardridge_info();

    std::vector<u8> rom;

    std::vector<std::vector<u8>> ram;

    int selected_ram_bank = 0;
    unsigned int selected_rom_bank = 0;


    bool ram_enabled;


};

#endif
