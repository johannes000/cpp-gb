//
// Created by knaub on 26.06.2020.
//
#include "Debugger.h"
#include <vector>


Debugger::Debugger(CPU *c, PPU *p, MEMORY *m, CARDRIDGE *card) : cpu(c), ppu(p), mem(m), cardridge(card),
                                                                 debug_state(Debug_State::RUN),
                                                                 next_step(false) {
    init_debug_window();

    frame_buffer = (void *) ppu->get_lcd_buffer();
    tile_buffer = std::vector<Pixel>(TILE_BUFFER_WIDTH * TILE_BUFFER_HEIGHT, {0xFF, 0xFF, 0xFF});


    load_texture(&frame_texture, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, frame_buffer);
    load_texture(&tiles_texture, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, &tile_buffer[0]);


    breakpoints.push_back(0x100);
}

Debugger::~Debugger() {
    terminate_window();
}

void Debugger::update() {
    switch (debug_state) {
        case Debug_State::STEP:
            handle_breakpoints();
            break;
        case Debug_State::RUN:
            update_views();
            break;
    }


}


void Debugger::handle_breakpoints() {
    if (debug_state != Debug_State::STEP) {
        if (!breakpoints.empty()) {
            auto iter = std::find(breakpoints.begin(), breakpoints.end(), cpu->get_pc());
            if (iter != breakpoints.end())
                debug_state = Debug_State::STEP;
        }
        return;
    }
    next_step = false;
    while (!next_step) {
        update_views();

        while (SDL_PollEvent(&debug_event)) {
            switch (debug_event.type) {
                case SDL_QUIT:
                    exit(0);
                case SDL_KEYDOWN:
                    switch (debug_event.key.keysym.sym) {
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

void Debugger::render() const {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Debugger::update_views() {

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    imgui_demo();
    card_view();
    cpu_view();
    video_view();
    control_view();
    memory_view();
    render();

}

void Debugger::control_view() {
    ImGui::Begin("Control");
    CPU_Register cpu_regs = cpu->get_register();

    ImGui::Text("AF = %04X", cpu_regs.AF);
    ImGui::SameLine();
    ImGui::Text("DE = %04X", cpu_regs.DE);
    ImGui::SameLine();
    ImGui::Text("PC = %04X", cpu_regs.PC);
    ImGui::Text("BC = %04X", cpu_regs.BC);
    ImGui::SameLine();
    ImGui::Text("HL = %04X", cpu_regs.HL);
    ImGui::SameLine();
    ImGui::Text("SP = %04X", cpu_regs.SP);

    ImGui::Separator();

    ImGui::Text("Z %1d", cpu->check_flag(Flag::z));
    ImGui::SameLine();
    ImGui::Text("N %1d", cpu->check_flag(Flag::n));
    ImGui::SameLine();
    ImGui::Text("H %1d", cpu->check_flag(Flag::h));
    ImGui::SameLine();
    ImGui::Text("C %1d", cpu->check_flag(Flag::c));
    ImGui::SameLine();
    ImGui::Text("(HL) %02X", cpu->read(cpu_regs.HL));

    LCD_Register lcd_regs = ppu->get_lcd_register();
    ImGui::Separator();

    ImGui::Text("IME %X", cpu->is_IME());

    ImGui::Text("Interrupt Request");
    for (int i = 0; i < 5; ++i) {
        ImGui::SameLine();
        ImGui::Text("%X", check_bit(mem->read(INTERRUPT_FLAGS_REG), i));
    }
    ImGui::Text("Interrupt Enable ");
    for (int i = 0; i < 5; ++i) {
        ImGui::SameLine();
        ImGui::Text("%X", check_bit(mem->read(INTERRUPT_ENABLE_REG), i));
    }

    ImGui::Separator();
    ImGui::Text(" %d", lcd_regs.LCD_LY);
    ImGui::SameLine();
    ImGui::Text("JOY ");
    u8 joy = cpu->read(JOY_P1_REGISTER);
    for (int i = 0; i < 6; ++i) {
        ImGui::SameLine();
        ImGui::Text("%X", check_bit(joy, i));
    }
    ImGui::Separator();
    ImGui::Text(" TIMER ");
    ImGui::Text("DIV  %02X", mem->read(DIVIDER_REGISTER));
    ImGui::SameLine();
    ImGui::Text("TIMA %02X", mem->read(TIMER_COUNTER));
    ImGui::Text("TMA  %02X", mem->read(TIMER_MODULO));
    ImGui::SameLine();
    ImGui::Text("TAC  %02X", mem->read(TIMER_CONTROL));


    ImGui::End();

}

void Debugger::card_view() {
    ImGui::Begin("Rominfo");


    ImGui::Text("%s", cardridge->rom_info.title);
    ImGui::Separator();

    ImGui::Text("CType: %s", cardridge->rom_info.CType_string());
    ImGui::SameLine();
    ImGui::Text("Rom Bank: %d", cardridge->get_selected_rom_bank());
    ImGui::Text("Ram Size: %s", cardridge->rom_info.ram_size_string());
    ImGui::SameLine();
    ImGui::Text("| Rom Size: %s", cardridge->rom_info.rom_size_string());

    ImGui::End();
}

void Debugger::imgui_demo() {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");

    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float *) &bg_color);

    if (ImGui::Button("Button"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
}


void Debugger::cpu_view() {

    ImGui::Begin("CPU");

    bool goto_pc = ImGui::Button("Goto PC");
    ImGui::SameLine();
    if (ImGui::Button("Step")) {
        debug_state = Debug_State::STEP;
        next_step = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Run")) {
        debug_state = Debug_State::RUN;
        next_step = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("ReDisassemble")) {
        disassemble();
    }
    ImGui::SameLine();
    debug_state == Debug_State::STEP ? ImGui::Text("STEP") : ImGui::Text("RUN");
    // DECOMPILER
    ImGui::Separator();
    ImGui::BeginChild("##scrollingregion", ImVec2(0, 400));


    // DISASSAMBLER
    auto pc = cpu->get_pc();
    ImGuiListClipper clipper(dis_vec.size(), ImGui::GetTextLineHeightWithSpacing());

    auto itr = std::find(dis_addr_vec.begin(), dis_addr_vec.end(), pc);
    auto pc_offset = std::distance(dis_addr_vec.begin(), itr);

    if (goto_pc) {
        ImGui::SetScrollFromPosY(
                ImGui::GetCursorStartPos().y + (pc_offset * ImGui::GetTextLineHeightWithSpacing()), 0.5f);
    }
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i <= clipper.DisplayEnd; i++) {
            auto inst = dis_vec[i];

            if (ImGui::Selectable("", true, ImGuiSelectableFlags_AllowDoubleClick))
                breakpoints.push_back(inst.address);
            ImGui::SameLine();
            if (inst.address == pc) {
                ImGui::SetItemDefaultFocus();
                ImGui::TextColored(red, "%04X", inst.address);
                ImGui::SameLine();
                switch (inst.bytes) {
                    case 1:
                        ImGui::TextColored(gray, "%02X\t\t\t", inst.opcode);
                        break;
                    case 2:
                        ImGui::TextColored(gray, "%02X %02X\t\t ", inst.opcode, inst.intermediate & 0xFFu);
                        break;
                    case 3:
                        ImGui::TextColored(gray, "%02X %02X %02X\t  ", inst.opcode,
                                           inst.intermediate & 0xFF00u,
                                           inst.intermediate & 0x00FFu);
                        break;
                }
                ImGui::SameLine();
                ImGui::TextColored(red, "->%s", inst.mnemonic);
                if (inst.bytes > 2) {
                    ImGui::SameLine();
                    ImGui::TextColored(red, "%s", what_register_is_this(inst.intermediate));
                } else if (inst.bytes > 1) {
                    ImGui::SameLine();
                    ImGui::TextColored(red, "%s",
                                       what_register_is_this(static_cast<u16> (inst.intermediate | 0xFF00u)));
                }
            } else {
                ImGui::TextColored(cyan, "%04X", inst.address);
                ImGui::SameLine();
                switch (inst.bytes) {
                    case 1:
                        ImGui::TextColored(gray, "%02X\t\t\t", inst.opcode);
                        break;
                    case 2:
                        ImGui::TextColored(gray, "%02X %02X\t\t ", inst.opcode, inst.intermediate & 0xFFu);
                        break;
                    case 3:
                        ImGui::TextColored(gray, "%02X %02X %02X\t  ", inst.opcode,
                                           (inst.intermediate & 0xFFu) >> 4u,
                                           inst.intermediate & 0x00FFu);
                        break;
                }
                ImGui::SameLine();
                ImGui::TextColored(cyan, "%s", inst.mnemonic);
                if (inst.bytes > 2) {
                    ImGui::SameLine();
                    ImGui::TextColored(gray, "%s", what_register_is_this(inst.intermediate));
                } else if (inst.bytes > 1) {
                    ImGui::SameLine();
                    ImGui::TextColored(gray, "%s",
                                       what_register_is_this(static_cast<u16> (inst.intermediate | 0xFF00u)));
                }
            }
            //i += inst.bytes;

        }
    }
    clipper.End();
    ImGui::EndChild();

    // Breakpoints
    ImGui::BeginChild("##ScrollingRegion");

    std::sort(breakpoints.begin(), breakpoints.end());
    ImGui::Text("Breakpoints");

    auto b = breakpoints.begin();
    auto e = breakpoints.end();

    while (b != e) {
        if (*b == pc)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));
        char address[32];
        sprintf(address, "%04X", *b);
        if (ImGui::Selectable(address, true, ImGuiSelectableFlags_SpanAllColumns))
            breakpoints.erase(b);

        if (*b == pc)
            ImGui::PopStyleColor(1);
        b++;
    }

    ImGui::EndChild();
    ImGui::End();


}

void Debugger::reset() {

}

void Debugger::disassemble() {
    debug_instruction debug_inst;
    dis_vec.erase(dis_vec.begin(), dis_vec.end());
    dis_addr_vec.erase(dis_addr_vec.begin(), dis_addr_vec.end());
    for (u16 addr = 0; addr < 0xFFFF;) {
        // Nur Adressen aus dem Card Rom nicht aus dem Bootrom
        // Deswegen kriegt man beim Booten nicht die richtigen Adressen im CPU View
        debug_inst = cpu->get_instruction_at_addr(addr);
        if (debug_inst.prefixed)
            addr++;
        dis_vec.push_back(debug_inst);
        dis_addr_vec.push_back(addr);
        addr += debug_inst.bytes;
    }
}

void Debugger::video_view() {
    ImGui::Begin("Frame");
    update_tile_buffer();

    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 ppu->get_lcd_buffer());

    glBindTexture(GL_TEXTURE_2D, tiles_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 &tile_buffer[0]);

    ImGui::Image((ImTextureID) (intptr_t) frame_texture, ImVec2(GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT));

    ImGui::Separator();

    ImGui::Image((ImTextureID) (intptr_t) tiles_texture, ImVec2(TILE_BUFFER_WIDTH * 2, TILE_BUFFER_HEIGHT * 2));

    ImGui::End();


}

void Debugger::update_tile_buffer() {
    u16 tileData = 0x8000;
    for (int row = 0; row < 24; row++) {
        for (int column = 0; column < 16; column++) {
            u16 start = tileData + (16 * column);
            for (int line = (row * 8); line < 8 + (row * 8); line++, start += 2) {
                u8 p_line_1 = ppu->read(start);
                u8 p_line_2 = ppu->read(start + 1);

                for (int tileColumn = (column * 8), position = 7;
                     tileColumn < 8 + (column * 8); tileColumn++, position--) {
                    int index = tileColumn + line * TILE_BUFFER_WIDTH;
                    const u8 color = static_cast<u8>(check_bit(p_line_1, (position)) << 1u) |
                                     check_bit(p_line_2, (position));

                    tile_buffer[index] = ppu->Palettes[1][color];
                }
            }
        }
        tileData += 0x100;
    }


}

/*
void Debugger::update_tile_buffer() {
    u16 tileData = 0x8000;
    for (int row = 0; row < 24; row++) {
        for (int column = 0; column < 16; column++) {
            u16 start = tileData + (16 * column);
            for (int line = (row * 8); line < 8 + (row * 8); line++, start += 2) {
                u8 p_line_1 = ppu->read(start);
                u8 p_line_2 = ppu->read(start + 1);

                for (int tileColumn = (column * 8), position = 7;
                     tileColumn < 8 + (column * 8); tileColumn++, position--) {
                    int index = tileColumn + line * TILE_BUFFER_WIDTH;
                    const u8 color = static_cast<u8>(check_bit(p_line_1, (position)) << 1u) |
                                     check_bit(p_line_2, (position));

                    tile_buffer[index].R = ppu->Palettes[1][color].R;
                    tile_buffer[index].G = ppu->Palettes[1][color].G;
                    tile_buffer[index].B = ppu->Palettes[1][color].B;
                }
            }
        }
        tileData += 0x100;
    }


}*/



void Debugger::memory_view() {
    ImGui::Begin("Memory View");

    ImGui::BeginChild("##scrollingregion", ImVec2(0, 300));
    ImGuiListClipper clipper(0xFFFF / 0x10, ImGui::GetTextLineHeightWithSpacing());
    for (int mem_line = clipper.DisplayStart; mem_line <= clipper.DisplayEnd; mem_line++) {
        int mem_line_addr = mem_line * 0x10;
        ImGui::Text("0x%04X ", mem_line_addr);
        for (int i = 0; i < 16; ++i) {
            ImGui::SameLine();
            u8 data = mem->read(mem_line_addr + i);
            if (data == 0x00)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Text("%02X", data);
            if (i == 7) {
                ImGui::SameLine(120.0f);
            }
            if (data == 0x00)
                ImGui::PopStyleColor(1);
            if ((mem_line_addr + i) == 0xFEFF)
                ImGui::Separator();
        }

    }
    clipper.End();
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::Text("ASSD");
    ImGui::End();
}


bool Debugger::load_texture(GLuint *textureHandler, int width, int height, const void *data) {
    glGenTextures(1, textureHandler);
    glBindTexture(GL_TEXTURE_2D, *textureHandler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
