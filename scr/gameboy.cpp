#include "gameboy.h"


gameboy::gameboy() : mem(&cpu, &card, &ppu, &joypad), cpu(&mem), ppu(&mem), joypad(&mem, &ppu),
                     debug(&cpu, &ppu, &mem, &card) {}

gameboy::~gameboy() {

}

void gameboy::start_emu() {
    cycles = 0;
    last_cycle = 0;
    mem.reset();

    card.load_rom("./../../testroms/tetris.gb");
    //card.load_rom("./../../testroms/sml.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/01-special.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/03-op sp,hl.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/11-op a,(hl).gb");
    //card.load_rom("./../../testroms/cpu_instrs/cpu_instrs.gb");
    //card.load_rom("./../../testroms/opus5.gb");
    //card.load_rom("./../../testroms/drmario.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\call_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\instr_timing\\instr_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\reg_f.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");

    debug.disassemble();
    debug.update();
    bool quit = false;
    // TODO:: Reset einbauen
    while (!quit) {

        while (!ppu.is_ready_to_render()) {
            cpu.fetch_op_code();
            debug.handle_breakpoints();
            last_cycle = cpu.update();
            cycles += last_cycle;
            ppu.update(last_cycle * 4);
        }
        ppu.render_frame();
        debug.update();
        cycles = 0;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                case SDL_KEYDOWN:
                    joypad.update_joypad();
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            exit(0);
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    joypad.update_joypad();
                    break;
                default:
                    break;
            }
        }


    }


}

