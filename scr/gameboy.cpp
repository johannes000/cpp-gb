#include "gameboy.h"


gameboy::gameboy() : mem(&cpu, &card, &ppu, &joypad), cpu(&mem), ppu(&mem, &cpu), joypad(&mem, &ppu),
                     debug(&cpu, &ppu, &mem, &card) {}

gameboy::~gameboy() {

}

[[noreturn]] void gameboy::start_emu() {
    last_cycle = 0;
    mem.reset();

    //card.load_rom("./../../testroms/tetris.gb");
    //card.load_rom("./../../testroms/tennis.gb");
    //card.load_rom("./../../testroms/sml.gb");
    //card.load_rom("./../../testroms/opus5.gb");
    //card.load_rom("./../../testroms/pocket.gb");
    //card.load_rom("./../../testroms/dmg-acid2.gb");
    //card.load_rom("./../../testroms/drmario.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/01-special.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/02-interrupts.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/03-op sp,hl.gb");
    //card.load_rom("./../../testroms/cpu_instrs/cpu_instrs.gb");
    //card.load_rom("./../../testroms/instr_timing/instr_timing.gb");
    //card.load_rom("./../../testroms/mem_timing/individual/01-read_timing.gb");
    //card.load_rom("./../../testroms/mem_timing/individual/02-write_timing.gb");
    //card.load_rom("./../../testroms/interrupt_time/interrupt_time.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\call_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\instr\\daa.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\reg_f.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\emulator-only\\mbc1\\bits_bank1.gb");

    debug.disassemble();
    debug.update();
    // TODO:: Reset einbauen
    while (true) {
        while (!ppu.is_ready_to_render()) {
            debug.handle_breakpoints();
            last_cycle = cpu.update();
            cpu.handle_interrupts();
            cpu.handle_timer(last_cycle);
            ppu.update(last_cycle * 4);
        }
        ppu.render_frame();
        joypad.process_sdl_events(&event);
        debug.update();
    }


}

