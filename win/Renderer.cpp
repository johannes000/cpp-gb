//
// Created by knaub on 15.07.2020.
//

#include "Renderer.h"

Renderer::Renderer(CONTROLS *j) : joypad(j) {
	init_window();

	glfwSetWindowUserPointer(window, (void *)joypad);
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	tile_buffer = {TILE_MAP_HEIGHT * TILE_MAP_HEIGHT,
				   {0xFF, 0xFF, 0xFF, 0x00}};

	frame_texture.data_ptr = nullptr;
	frame_texture.height = GB_LCD_SIZE_HEIGHT;
	frame_texture.width = GB_LCD_SIZE_WIDTH;

	tiles_texture.data_ptr = &tile_buffer[0];
	tiles_texture.height = TILE_MAP_HEIGHT;
	tiles_texture.width = TILE_MAP_WIDTH;

	load_texture(frame_texture);
	load_texture(tiles_texture);

	for (int sprite_index = 0; sprite_index < 40; sprite_index++) {
		Texture sprite;
		sprite.width = TILE_WIDTH;
		sprite.height = TILE_HEIGHT;
		load_texture(sprite);
		sprite_textures.push_back(sprite);
	}

	roms = {
		{"1MB", "C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_1Mb.gb"},
		{"2MB", "C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_2Mb.gb"},
		{"4MB", "C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_4Mb.gb"},
		{"8MB", "C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_8Mb.gb"},
		{"16MB", "C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_16Mb.gb"},
		{"1MB", "C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_1Mb.gb"},
		{"Tennis", "C:/Users/knaub/source/repos/testroms/tennis.gb"},
		{"Tetris", "C:/Users/knaub/source/repos/testroms/tetris.gb"},
		{"Mario", "C:/Users/knaub/source/repos/testroms/sml.gb"},
		{"OPUS 5", "C:/Users/knaub/source/repos/testroms/opus5.gb"},
		{"pocket", "C:/Users/knaub/source/repos/testroms/pocket.gb"},
		{"drmario", "C:/Users/knaub/source/repos/testroms/drmario.gb"},
		{"moon_boot_hwio", "C:/Users/knaub/source/repos/testroms/mooneye/acceptance/boot_hwio-S.gb"},
		{"moon_ei_sequence", "C:/Users/knaub/source/repos/testroms/mooneye/acceptance/ei_sequence.gb"},
		{"winpos", "C:/Users/knaub/source/repos/testroms/scribbltests/winpos/winpos.gb"},
		{"statcount", "C:/Users/knaub/source/repos/testroms/scribbltests/statcount/statcount-auto.gb"},
		{"scxly", "C:/Users/knaub/source/repos/testroms/scribbltests/scxly/scxly.gb"},
		{"palettely", "C:/Users/knaub/source/repos/testroms/scribbltests/palettely/palettely.gb"},
		{"lycscy", "C:/Users/knaub/source/repos/testroms/scribbltests/lycscy/lycscy.gb"},
		{"lycscx", "C:/Users/knaub/source/repos/testroms/scribbltests/lycscx/lycscx.gb"},
		{"fairylake", "C:/Users/knaub/source/repos/testroms/scribbltests/fairylake/fairylake.gb"},
	};
}

Renderer::~Renderer() { terminate_window(); }

void Renderer::init_window() {
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		exit(1);

	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
	// only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only

	// Create window with graphics context
	window =
		glfwCreateWindow(window_width, window_height, "Debug", nullptr, nullptr);
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
	glbinding::initialize([](const char *name) {
		return (glbinding::ProcAddress)glfwGetProcAddress(name);
	});
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader
					  // is likely to requires some form of initialization.
#endif
	if (err) {
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		exit(1);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
	// Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
	// Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can
	// also load multiple fonts and use ImGui::PushFont()/PopFont() to select
	// them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you
	// need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please
	// handle those errors in your application (e.g. use an assertion, or display
	// an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored
	// into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
	// ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string
	// literal you need to write a double backslash \\ !
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	// ImFont* font =
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
	// NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

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
void Renderer::start_new_imgui_frame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::update_views(PPU *ppu, Debugger *debug) {
	ppu->ready_to_render = false; // TODO: Richtig implementieren

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// imgui_demo();
	menu();
	main_view(ppu);
	if (show_debug_views) {
		tile_buffer_view(ppu);
		debug->update_views();
	}
}

void Renderer::update_tile_buffer(PPU *ppu) {
#if 1
	u16 tile_data_address = 0x8000;

	for (int row = 0; row < 24; row++) {
		for (int column = 0; column < 16; column++) {
			u16 start = tile_data_address + (16 * column);
			for (int line = (row * 8); line < 8 + (row * 8); line++, start += 2) {
				u8 p_line_1 = ppu->read(start);
				u8 p_line_2 = ppu->read(start + 1);

				for (int tileColumn = (column * 8), position = 7;
					 tileColumn < 8 + (column * 8); tileColumn++, position--) {
					int index = tileColumn + line * TILE_MAP_WIDTH;
					const u8 color =
						static_cast<u8>(check_bit(p_line_1, (position)) << 1u) |
						static_cast<u8>(check_bit(p_line_2, (position)));

					tile_buffer[index] = ppu->bg_palette[color];
				}
			}
		}
		tile_data_address += 0x100;
	}
#endif
}

bool Renderer::load_texture(GLuint *textureHandler, int width, int height,
							const void *data) {
	glGenTextures(1, textureHandler);
	glBindTexture(GL_TEXTURE_2D, *textureHandler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, data);
	//glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
bool Renderer::load_texture(Texture texture) {
	return load_texture(texture.texture_ptr, texture.width, texture.height, nullptr);
}

void Renderer::update_texture(GLuint *textureHandler, int width, int height, const void *data) {
	glBindTexture(GL_TEXTURE_2D, *textureHandler);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
void Renderer::update_texture(Texture texture, const void *data) {
	return update_texture(texture.texture_ptr, texture.width, texture.height, data);
}

void Renderer::main_view(PPU *ppu) {
	float ratio = (float)GB_LCD_SIZE_WIDTH / (float)GB_LCD_SIZE_HEIGHT;

	// int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y / 2;

	ImGui::Begin("Main Window");

	update_texture(frame_texture, ppu->get_lcd_buffer());
	ImGui::Image((ImTextureID)(intptr_t)frame_texture.texture_handler, ImVec2(h * ratio, h));

	show_fps();

	ImGui::End();
}

void Renderer::tile_buffer_view(PPU *ppu) {
	static int counter = 0;
	if (!(counter++ % 60)) {
		update_tile_buffer(ppu);
		update_texture(tiles_texture, &tile_buffer[0]);
	}
	ImGui::Begin("Tile Buffer");

	ImGui::Image((ImTextureID)(intptr_t)tiles_texture.texture_handler,
				 ImVec2(TILE_MAP_WIDTH * 2, TILE_MAP_HEIGHT * 2));

	if (ImGui::TreeNode("Sprites")) {
		for (auto sprite : ppu->sprites) {
			if (ImGui::TreeNode(std::to_string(sprite.id).c_str())) {
				ImGui::Text("Tile No:%i", sprite.tile_no);
				ImGui::Text("X:      %i", sprite.x_pos);
				ImGui::Text("Y:      %i", sprite.y_pos);
#if 0
				Texture spr = sprite_textures[sprite.id];
				std::vector<Pixel_a> translated_sprite;
				for (auto &pixel : ppu->tile_map_data[sprite.tile_no]) {
					translated_sprite.push_back(ppu->Palettes[ppu->current_palette][pixel]);
				}
				spr.translated_texture = translated_sprite;

				update_texture(spr, spr.translated_Texture_ptr);
				ImGui::Image((ImTextureID)(intptr_t)spr.texture_handler, ImVec2(spr.height * 4, spr.width * 4));
#endif
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void Renderer::menu() {
	ImGui::Begin("Menü");
	if (ImGui::Button("Show Debug")) {
		show_debug_views = !show_debug_views;
	}

	ImGui::End();
}

void Renderer::show_fps() const {
	ImGui::SetCursorPos(ImVec2(20.0f, 30.0f));
	ImGui::TextColored(magenta, "Frame Rate: %.2f FPS\nFrame Time: %.2f ms",
					   ImGui::GetIO().Framerate,
					   1000.0f / ImGui::GetIO().Framerate);
}

const char *Renderer::rom_select_view() {
	ImGui::Begin("ROMS");
	memset(filename, 0, sizeof(filename));
	if (ImGui::Button("Open File")) {

		OPENFILENAMEA ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL; // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Game Boy Rom\0*.gb\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select a Rom";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn)) {
			std::cout << "You chose the file \"" << filename << "\"\n";
		}
	}
	ImGui::Separator();
#if 1
	for (auto &rom : roms) {
		if (ImGui::Selectable("", true, ImGuiSelectableFlags_AllowDoubleClick))
			strcpy_s(filename, sizeof(filename), rom.second);
		ImGui::SameLine();
		ImGui::Text("%s", rom.first);
	}
#endif
	ImGui::End();
	return &filename[0];
}

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode,
				  int action, [[maybe_unused]] int mods) {
	auto joypad = (CONTROLS *)glfwGetWindowUserPointer(window);
	joypad->handle_glwf_inputs(key, action);
}
