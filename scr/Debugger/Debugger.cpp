//
// Created by knaub on 26.06.2020.
//
#include "Debugger.h"
#include <vector>

Debugger::Debugger(CPU *c, PPU *p) : cpu(c), ppu(p), debug_state(Debug_State::RUN), next_step(false) {
    init_debug_window();

    frame_buffer = ppu->get_lcd_buffer();
    tiles_buffer = ppu->get_tiles_buffer(0);

    load_texture(&frame_texture, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, frame_buffer);
    load_texture(&tiles_texture, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, tiles_buffer);


    breakpoints.push_back(0x100);
    breakpoints.push_back(0x29A6);
}

Debugger::~Debugger() {
    terminate_window();
}

void Debugger::update() {
    switch (debug_state) {
        case Debug_State::STEP:
            step_mode();
            break;
        case Debug_State::RUN:
            update_views();
            break;
    }


}


void Debugger::step_mode() {
    if (debug_state != Debug_State::STEP) {
        if (!breakpoints.empty()) {
            auto iter = std::find(breakpoints.begin(), breakpoints.end(), cpu->get_pc());
            if (iter != breakpoints.end())
                debug_state = Debug_State::STEP;
        }
        return;
    }

    // TODO: Framelimitter einbauen
    next_step = false;
    while (!next_step) {
        update_views();

        while (SDL_PollEvent(&event)) {
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

void Debugger::render() const {// Rendering
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

    ImGui::Separator();
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
    debug_state == Debug_State::STEP ? ImGui::Text("STEP") : ImGui::Text("RUN");
    ImGui::End();

}

void Debugger::card_view() {
    ImGui::Begin("Rominfo");

    ImGui::Text("asd");

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

    // DECOMPILER
    ImGui::Separator();
    ImGui::BeginChild("##scrollingregion", ImVec2(0, 200));
    ImGui::Columns(4, "mycolumns"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text("Address");
    ImGui::NextColumn();
    ImGui::Text("Mnemonic");
    ImGui::NextColumn();
    ImGui::Text("Opcode");
    ImGui::NextColumn();
    ImGui::Text("Intermediate");
    ImGui::NextColumn();
    ImGui::Separator();

    auto pc = cpu->get_pc();
    ImGuiListClipper clipper(0x7FFF, ImGui::GetTextLineHeightWithSpacing());
    if (goto_pc)
        ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + (pc * ImGui::GetTextLineHeightWithSpacing()), 0.5f);

    auto iter = std::upper_bound(valid_addresses.begin(), valid_addresses.end(), clipper.DisplayStart - 1);
    clipper.DisplayStart = *iter;

    for (int i = clipper.DisplayStart; i <= clipper.DisplayEnd; i++) {
        auto inst = dis_map[*iter];
        auto addr = *iter;

        if (addr == pc)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));

        char address[32];
        sprintf(address, "%04X", addr);
        if (ImGui::Selectable(address, true, ImGuiSelectableFlags_SpanAllColumns))
            breakpoints.push_back(addr);
        ImGui::NextColumn();
        ImGui::Text("%s", inst.mnemonic);
        ImGui::NextColumn();
        ImGui::Text("%02X", inst.opcode);
        ImGui::NextColumn();
        char intermediate[32];
        sprintf(intermediate, "%04X", inst.intermediate);
        ImGui::Text("%s", intermediate);
        ImGui::NextColumn();
        if (addr == pc)
            ImGui::PopStyleColor(1);
        iter++;
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
    for (u16 addr = 0; addr < 0x7FFE;) {
        // Nur Adressen aus dem Card Rom nicht aus dem Bootrom
        // Deswegen kriegt man beim Booten nicht die richtigen Adressen im CPU View
        debug_inst = cpu->get_instruction_at_addr(addr);
        if (debug_inst.prefixed)
            addr++;

        dis_map.insert({addr, debug_inst});
        valid_addresses.push_back(addr);

        addr += debug_inst.bytes;
    }
}

void Debugger::video_view() {

    ImGui::Begin("Frame");
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 ppu->get_lcd_buffer());

    glBindTexture(GL_TEXTURE_2D, tiles_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    const int tileSize = 16;
    int tileCounter = 0;
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 16; x++) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, x * 8, y * 8, 8, 8, GL_RGB, GL_UNSIGNED_BYTE,
                            ppu->get_tiles_buffer(tileCounter++));
        }
    }

    ImGui::Image((ImTextureID) (intptr_t) frame_texture, ImVec2(GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT));

    ImGui::Separator();

    ImGui::Image((ImTextureID) (intptr_t) tiles_texture, ImVec2(TILE_BUFFER_WIDTH * 2, TILE_BUFFER_HEIGHT * 2));

    ImGui::End();


}

bool Debugger::load_texture(GLuint *textureHandler, int width, int height, const void *data) {
    glGenTextures(1, textureHandler);
    glBindTexture(GL_TEXTURE_2D, *textureHandler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
