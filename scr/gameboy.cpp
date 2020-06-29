#include "gameboy.h"


gameboy::gameboy() : mem(&cpu, &card, &ppu, &joypad), cpu(&mem), ppu(&mem), joypad(&mem, &ppu), debug(&cpu, &ppu) {}

gameboy::~gameboy() {

}

void gameboy::start_emu() {
    cycles = 0;
    last_cycle = 0;
    mem.reset();

    card.load_rom("./../../testroms/tetris.gb");
    //card.load_rom("./../../testroms/sml.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    //card.load_rom("./../../testroms/cpu_instrs/cpu_instrs.gb");
    //card.load_rom("./../../testroms/opus5.gb");
    //card.load_rom("./../../testroms/drmario.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\utils\\bootrom_dumper.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\utils\\bootrom_dumper.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\call_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\reg_f.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");

    debug.dissassamble();
    debug.update();
    bool quit = false;

    while (!quit) {

        while (!ppu.is_ready_to_render()) {
            last_cycle = cpu.update();
            cycles += last_cycle;
            ppu.update(last_cycle * 4);
            debug.step_mode();
        }
        ppu.render_frame();
        debug.update();
        cycles = 0;

        while (SDL_PollEvent(&event)) {
            joypad.update_joypad();
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            exit(0);
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }


    }


}

