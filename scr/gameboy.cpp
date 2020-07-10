#include "gameboy.h"


gameboy::gameboy() : mem(&cpu, &card, &ppu, &joypad), cpu(&mem), ppu(&mem, &cpu), joypad(&mem, &ppu),
                     debug(&cpu, &ppu, &mem, &card) {}

gameboy::~gameboy() {

}

void gameboy::start_emu() {
    cycles = 0;
    last_cycle = 0;
    mem.reset();

    card.load_rom("./../../testroms/tetris.gb");
    //card.load_rom("./../../testroms/tennis.gb");
    //card.load_rom("./../../testroms/sml.gb");
    //card.load_rom("./../../testroms/opus5.gb");
    //card.load_rom("./../../testroms/drmario.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/01-special.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/02-interrupts.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/03-op sp,hl.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/04-op r,imm.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/05-op rp.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/06-ld r,r.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/08-misc instrs.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/09-op r,r.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/10-bit ops.gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/individual/11-op a,(hl).gb"); passed
    //card.load_rom("./../../testroms/cpu_instrs/cpu_instrs.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\call_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\instr_timing\\instr_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\reg_f.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\emulator-only\\mbc1\\bits_bank1.gb");

    debug.disassemble();
    debug.update();
    bool quit = false;
    // TODO:: Reset einbauen
    while (!quit) {
        while (!ppu.is_ready_to_render()) {
            debug.handle_breakpoints();
            last_cycle = cpu.update();
            cycles += last_cycle;
            ppu.update(last_cycle * 4);

        }
        ppu.render_frame();
        joypad.process_sdl_events(&event);
        debug.update();
        cycles = 0;
    }


}

