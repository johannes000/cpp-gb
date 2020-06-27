//
// Created by knaub on 26.06.2020.
//
#include "Debugger.h"
#include <vector>

Debugger::Debugger(CPU *c, PPU *p) : cpu(c), ppu(p) {
    init_debug_window();
    frame_buffer = ppu->getLcdBuffer();

    load_texture(&frame_texture, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, frame_buffer);

}

Debugger::~Debugger() {
    terminate_window();
}

void Debugger::update() {

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

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
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

void Debugger::CPU_view() {

    ImGui::Begin("CPU");
    ImGui::Columns(3, "mycolumns"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text("Address");
    ImGui::NextColumn();
    ImGui::Text("Mnemonic");
    ImGui::NextColumn();
    ImGui::Text("Intermediate");
    ImGui::NextColumn();
    ImGui::Separator();

    //TODO: Nur anzeigen wenn der Emulator im Step Modus ist
    //TODO: Rom Paging einbauen

    u16 pc = cpu->get_pc();
    auto it = dis_map.find(pc);
    auto beg = std::prev(it, 20);
    auto end = std::next(it, 20);
    while (beg != end) {
        if (beg->first == it->first)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.8f, 0.8f, 1.0f));

        debug_instruction debug_inst = beg->second;
        char address[32];
        sprintf(address, "%04X", beg->first);
        ImGui::Text("%s", address);

        ImGui::NextColumn();
        ImGui::Text("%s", debug_inst.mnemonic);

        ImGui::NextColumn();
        char intermediate[32];
        sprintf(intermediate, "%04X", debug_inst.intermediate);
        ImGui::Text("%s", intermediate);
        ImGui::NextColumn();

        if (beg->first == it->first)
            ImGui::PopStyleColor(1);

        beg++;
    }
    ImGui::End();


}

void Debugger::reset() {

}

void Debugger::dissassamble() {
    debug_instruction debug_inst;
    for (int addr = 0; addr < 0x7FFF;) {
        debug_inst = cpu->get_instruction_at_addr(addr);
        dis_map.insert({addr, debug_inst});
        addr += debug_inst.bytes;
    }
}

void Debugger::video_view() {

    ImGui::Begin("Frame");
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 ppu->getLcdBuffer());


    ImGui::Image((ImTextureID) (intptr_t) frame_texture, ImVec2(200, 200));

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