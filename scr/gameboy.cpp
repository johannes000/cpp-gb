#include "gameboy.h"

gameboy::gameboy() : mem(&cpu, &card, &ppu), cpu(&mem), ppu(&mem) {}

gameboy::~gameboy() {

}

void gameboy::start_emu() {
    auto cycles = 0;
    auto last_cycle = 0;
    mem.reset();
    card.load_rom("./../testroms/tetris.gb");
    //card.load_rom("./../testroms/sml.gb");
    //card.load_rom("./../testroms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    //card.load_rom("./../testroms/cpu_instrs/cpu_instrs.gb");
    //card.load_rom("C:\\Users\\knaub\\source\\repos\\cpp-gb\\testroms\\mooneye\\utils\\bootrom_dumper.gb");
    // cpu.print_all_ops();
    bool quit = false;
    step_mode = false;
    while (!quit) {

        last_cycle = cpu.update();
        cycles += last_cycle;
        ppu.update(last_cycle * 4);

        if (cycles >= VERTICAL_REFRESH_CYCLES) {
            cycles = 0;
            SDL_PollEvent(&event);
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
        if (step_mode)
            ppu.next_step();
        if (show_opcode)
            cpu.print_last_opcode();


    }
}
