//
// Created by knaub on 26.06.2020.
//
#include "Debugger.h"
#include <vector>

Debugger::Debugger(CPU *c, PPU *p) : cpu(c), ppu(p), debug_state(Debug_State::STEP), next_step(false) {
    init_debug_window();

    frame_buffer = ppu->get_lcd_buffer();
    tiles_buffer = ppu->get_tiles_buffer();

    load_texture(&frame_texture, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, frame_buffer);
    load_texture(&tiles_texture, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, tiles_buffer);

}

Debugger::~Debugger() {
    terminate_window();
}

void Debugger::update() {
    update_views();
}


void Debugger::step_mode() {
    if (debug_state != Debug_State::STEP)
        return;

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
    view_one();
    CPU_view();
    video_view();
    control_view();

    render();

}

void Debugger::control_view() {
    ImGui::Begin("Control");
    bool run = true;

    next_step = ImGui::Button("Step");
    run |= ImGui::Button("Run");

    switch (debug_state) {
        case Debug_State::STEP:
            if (run) {
                debug_state = Debug_State::RUN;
                next_step = true;
            }
            break;
        case Debug_State::RUN:
            if (next_step)
                debug_state = Debug_State::STEP;
            break;
    }


    ImGui::End();

}

void Debugger::view_one() {
    ImGui::Begin("Speicher");

    ImGui::Text("JOJOJOJ");

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

int line = 1100;

void Debugger::CPU_view() {

    ImGui::Begin("CPU");

    u16 pc = cpu->get_pc();
    ImGui::Text("%04X", pc);

    ImGui::Separator();
    bool goto_line = ImGui::Button("Goto");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    goto_line |= ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::Separator();
    ImGui::BeginChild("##scrollingregion");

    ImGui::Columns(3, "mycolumns"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text("Address");
    ImGui::NextColumn();
    ImGui::Text("Mnemonic");
    ImGui::NextColumn();
    ImGui::Text("Intermediate");
    ImGui::NextColumn();
    ImGui::Separator();

    ImGuiListClipper clipper(0x7FFE, ImGui::GetTextLineHeightWithSpacing());

    //if (debug_state == Debug_State::STEP)
    //ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + (pc * ImGui::GetTextLineHeightWithSpacing()), 0.5f);
    for (int i = clipper.DisplayStart; i <= clipper.DisplayEnd;) {
        debug_instruction inst = cpu->get_instruction_at_addr(i);

        if (goto_line && line == i)
            ImGui::SetScrollHereY();


        if (inst.addr == pc)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));

        char address[32];
        sprintf(address, "%04X", inst.addr);
        ImGui::Text("%s", address);

        ImGui::NextColumn();
        ImGui::Text("%s", inst.mnemonic);

        ImGui::NextColumn();
        char intermediate[32];
        sprintf(intermediate, "%04X", inst.intermediate);
        ImGui::Text("%s", intermediate);
        ImGui::NextColumn();

        if (inst.addr == pc)
            ImGui::PopStyleColor(1);

        i += i + inst.bytes;
    }
    clipper.End();
    ImGui::EndChild();
    ImGui::End();


}

void Debugger::reset() {

}

void Debugger::dissassamble() {
    debug_instruction debug_inst;
    for (int addr = 0; addr < 0x7FFF;) {
        debug_inst = cpu->get_instruction_at_addr(addr);
        dis_vec.push_back(debug_inst);
        addr += debug_inst.bytes;
    }
}

void Debugger::video_view() {

    ImGui::Begin("Frame");
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 ppu->get_lcd_buffer());

    glBindTexture(GL_TEXTURE_2D, tiles_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 ppu->get_tiles_buffer());

    ImGui::Image((ImTextureID) (intptr_t) frame_texture, ImVec2(GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT));

    ImGui::Separator();

    ImGui::Image((ImTextureID) (intptr_t) tiles_texture, ImVec2(TILE_BUFFER_WIDTH * 4, TILE_BUFFER_HEIGHT * 4));

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
