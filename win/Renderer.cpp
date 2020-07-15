//
// Created by knaub on 15.07.2020.
//

#include "Renderer.h"

Renderer::Renderer() {
    init_window();

    load_texture(&frame_texture, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, frame_buffer);
    load_texture(&tiles_texture, TILE_BUFFER_WIDTH, TILE_BUFFER_HEIGHT, &tile_buffer[0]);
}

Renderer::~Renderer() {
    terminate_window();
}

void Renderer::init_window() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(1);

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(window_width, window_height, "Debug", nullptr, nullptr);
    if (window == nullptr)
        exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Enable vsync

    glfwSetWindowPos(window, window_xpos, window_ypos);


    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        exit(1);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state

}

void Renderer::glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


void Renderer::terminate_window() const {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void Renderer::render() const {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Renderer::update_views(PPU *ppu, Debugger *debug) {
    ppu->ready_to_render = false; // TODO: Richtig implementieren

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    imgui_demo();
    main_view(ppu);
    debug->update_views();
    render();

}

void Renderer::imgui_demo() {
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


bool Renderer::load_texture(GLuint *textureHandler, int width, int height, const void *data) {
    glGenTextures(1, textureHandler);
    glBindTexture(GL_TEXTURE_2D, *textureHandler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Renderer::main_view(PPU *ppu) {

    float ratio = (float) GB_LCD_SIZE_WIDTH / (float) GB_LCD_SIZE_HEIGHT;

    //int w = ImGui::GetIO().DisplaySize.x;
    int h = ImGui::GetIO().DisplaySize.y / 2;


    ImGui::Begin("Frame");
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 ppu->get_lcd_buffer());

    ImGui::Image((ImTextureID) (intptr_t) frame_texture, ImVec2(h * ratio, h));

    show_fps();

    ImGui::End();
}

void Renderer::show_fps() {
    ImGui::SetCursorPos(ImVec2(20.0f, 20.0f));
    ImGui::TextColored(magenta, "Frame Rate: %.2f FPS\nFrame Time: %.2f ms", ImGui::GetIO().Framerate,
                       1000.0f / ImGui::GetIO().Framerate);
}


void Renderer::card_view(CARDRIDGE *card) {
    ImGui::Begin("Rominfo");


    ImGui::Text("%s", card->rom_info.title);
    ImGui::Separator();

    ImGui::Text("CType: %s", card->rom_info.CType_string());
    ImGui::SameLine();
    ImGui::Text("Rom Bank: %d", card->get_selected_rom_bank());
    ImGui::Text("Ram Size: %s", card->rom_info.ram_size_string());
    ImGui::SameLine();
    ImGui::Text("| Rom Size: %s", card->rom_info.rom_size_string());

    ImGui::End();
}


