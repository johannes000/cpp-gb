#ifndef MEMORY_H_GUARD
#define MEMORY_H_GUARD

#include <iostream>
#include <vector>

#include "CARDRIDGE.h"
#include "CPU.h"
#include "PPU.h"
#include "type.h"

class CPU;

class PPU;

class CARDRIDGE;

class MEMORY {
public:
    MEMORY(CPU *c, CARDRIDGE *card, PPU *p);

    ~MEMORY();

    u8 read(const u16 &addr);

    void write(const u16 &addr, const u8 &data);

    void write_u16(const u16 &addr, const u16 &data);

private:
    CARDRIDGE *cardridge;
    PPU *ppu;
    CPU *cpu;

    const char *boot_rom_path = "./../boot.gb";
    std::vector<u8> memory;
    std::vector<u8> boot;

    void dma_transfer(const u8 &data);

public:
    void reset();

    bool load_boot_rom();
};

#endif  // !MEMORY_H_GUARD
