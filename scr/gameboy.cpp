#include "gameboy.h"

gameboy::gameboy() : mem(&cpu, &card, &ppu, &joypad), cpu(&mem), ppu(&mem), joypad(&mem, &ppu) {}

gameboy::~gameboy() {

}

void gameboy::start_emu() {
    auto cycles = 0;
    auto last_cycle = 0;
    mem.reset();
    //card.load_rom("./../../testroms/tetris.gb");
    //card.load_rom("./../../testroms/sml.gb");
    //card.load_rom("./../../testroms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    //card.load_rom("./../../testroms/cpu_instrs/cpu_instrs.gb");
    //card.load_rom("./../../testroms/opus5.gb");
    //card.load_rom("./../../testroms/drmario.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\utils\\bootrom_dumper.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\utils\\bootrom_dumper.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\call_timing.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\reg_f.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\testroms\\mooneye\\acceptance\\bits\\mem_oam.gb");
    // cpu.print_all_ops();
    bool quit = false;
    while (!quit) {
        last_cycle = cpu.update();
        cycles += last_cycle;
        ppu.update(last_cycle * 4);

        if (cycles >= VERTICAL_REFRESH_CYCLES) {
            cycles = 0;
            while (SDL_PollEvent(&event)) {
                joypad.update_joypad(event);
                switch (event.type) {
                    case SDL_QUIT:
                        exit(0);
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_n:
                                step_mode = !step_mode;
                                break;
                            case SDLK_b:
                                show_opcode = !show_opcode;
                                break;
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
}
