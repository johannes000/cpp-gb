#include "Gui.hpp"
#include "gameboy.h"
#include "utl/type.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

namespace {
const ImVec4 bg_color = {0.8f, 0.8f, 0.8f, 1.0f};
ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
ImVec4 magenta = ImVec4(1.0f, 0.502f, 0.957f, 1.0f);
ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
ImVec4 red = ImVec4(1.0f, 0.149f, 0.447f, 1.0f);
ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 gray = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
ImVec4 dark_gray = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
}; // namespace

bool generate_texture(GLuint &textureHandler, int width, int height,
					  const void *data) {
	glGenTextures(1, &textureHandler);
	glBindTexture(GL_TEXTURE_2D, textureHandler);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, data);

	// glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

bool generate_texture(TextureData &texture) {
	return generate_texture(texture.textureID, texture.width, texture.height, nullptr);
}

void update_buffer(TextureData &textureData, const void *videoBuffer) {
	glBindTexture(GL_TEXTURE_2D, textureData.textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureData.width, textureData.height, GL_RGBA, GL_UNSIGNED_BYTE, videoBuffer);
}

Gui::Gui() {
	// Setup SDL
	CHECK_EQ_F(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER), 0, "Error: {}\n", SDL_GetError());

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char *glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char *glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	context.window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	context.gl_context = SDL_GL_CreateContext(context.window);
	SDL_GL_MakeCurrent(context.window, context.gl_context);
	SDL_GL_SetSwapInterval(0); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows
	// io.ConfigViewportsNoAutoMerge = true;
	// io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(context.window, context.gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	// ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	// IM_ASSERT(font != nullptr);

	tile_buffer = {TILE_MAP_HEIGHT * TILE_MAP_HEIGHT,
				   {0xFF, 0xFF, 0xFF, 0x00}};

	gb_framebuffer_texture.width = GB_LCD_SIZE_WIDTH;
	gb_framebuffer_texture.height = GB_LCD_SIZE_HEIGHT;

	generate_texture(gb_framebuffer_texture);

	tile_buffer_texture.height = TILE_MAP_HEIGHT;
	tile_buffer_texture.width = TILE_MAP_WIDTH;

	generate_texture(tile_buffer_texture);
}

void Gui::generate_widgets(gameboy &gb) {
	auto &cpu = gb.cpu;
	auto &ppu = gb.ppu;
	auto &mem = gb.mem;
	auto &joypad = gb.joypad;
	auto &card = gb.card;
	{
		ImGui::Begin("Gameboy Emulator");

		ImVec2 widget_size = ImGui::GetContentRegionAvail();
		ImVec2 original_image_size = {GB_LCD_SIZE_WIDTH, GB_LCD_SIZE_HEIGHT};

		f32 aspect_ratio = original_image_size.x / original_image_size.y;
		ImVec2 image_size;
		image_size.x = widget_size.x;
		image_size.y = widget_size.x / aspect_ratio;
		if (image_size.y > widget_size.y) {
			image_size.y = widget_size.y;
			image_size.x = widget_size.y * aspect_ratio;
		}

		update_buffer(gb_framebuffer_texture, gb.ppu.get_lcd_buffer());
		ImGui::Image((ImTextureID)(intptr_t)gb_framebuffer_texture.textureID, image_size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

		ImGui::End();
	}

	// Tile Buffer
	{

		static int counter = 0;
		if (!(counter++ % 60)) {
			u16 tile_data_address = 0x8000;

			for (int row = 0; row < 24; row++) {
				for (int column = 0; column < 16; column++) {
					u16 start = tile_data_address + (16 * column);
					for (int line = (row * 8); line < 8 + (row * 8); line++, start += 2) {
						u8 p_line_1 = gb.ppu.read(start);
						u8 p_line_2 = gb.ppu.read(start + 1);

						for (int tileColumn = (column * 8), position = 7;
							 tileColumn < 8 + (column * 8); tileColumn++, position--) {
							int index = tileColumn + line * TILE_MAP_WIDTH;
							const u8 color =
								static_cast<u8>(check_bit(p_line_1, (position)) << 1u) |
								static_cast<u8>(check_bit(p_line_2, (position)));

							tile_buffer[index] = gb.ppu.bg_palette[color];
						}
					}
				}
				tile_data_address += 0x100;
			}
			update_buffer(tile_buffer_texture, &tile_buffer[0]);
		}

		ImGui::Begin("Tile Buffer");
		ImGui::Image((ImTextureID)(intptr_t)tile_buffer_texture.textureID,
					 ImVec2(TILE_MAP_WIDTH * 2, TILE_MAP_HEIGHT * 2));

		if (ImGui::TreeNode("Sprites")) {
			for (auto sprite : gb.ppu.sprites) {
				if (ImGui::TreeNode(std::to_string(sprite.id).c_str())) {
					ImGui::Text("Tile No:%i", sprite.tile_no);
					ImGui::Text("X:      %i", sprite.x_pos);
					ImGui::Text("Y:      %i", sprite.y_pos);
#if 0
				Texture spr = sprite_textures[sprite.id];
				std::vector<Pixel_a> translated_sprite;
				for (auto &pixel : ppu.tile_map_data[sprite.tile_no]) {
					translated_sprite.push_back(ppu.Palettes[ppu.current_palette][pixel]);
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

	// Memory
	{
		ImGui::Begin("Memory");
		ImGuiListClipper clipper;
		clipper.Begin(0xFFFF / 16, ImGui::GetTextLineHeightWithSpacing());
		// ImGuiListClipper clipper(0xFFFF / 16, ImGui::GetTextLineHeightWithSpacing());
		while (clipper.Step()) {
			for (int mem_line = clipper.DisplayStart; mem_line <= clipper.DisplayEnd; mem_line++) {
				int mem_line_addr = mem_line * 0x10;
				ImGui::Text("0x%04X ", mem_line_addr);
				for (int i = 0; i < 16; ++i) {
					ImGui::SameLine();
					u8 data = gb.mem.read(mem_line_addr + i);
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
		}
		clipper.End();
		ImGui::End();
	}

	// Control
	{
		ImGui::Begin("Control");

		CPU_Register cpu_regs = cpu.get_register();

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

		ImGui::Text("Z %1d", cpu.check_flag(Flag::z));
		ImGui::SameLine();
		ImGui::Text("N %1d", cpu.check_flag(Flag::n));
		ImGui::SameLine();
		ImGui::Text("H %1d", cpu.check_flag(Flag::h));
		ImGui::SameLine();
		ImGui::Text("C %1d", cpu.check_flag(Flag::c));
		ImGui::SameLine();
		ImGui::Text("(HL) %02X", cpu.read(cpu_regs.HL));

		LCD_Register lcd_regs = ppu.get_lcd_register();
		ImGui::Separator();

		ImGui::Text("IME %X", cpu.is_IME());

		ImGui::Text("Interrupt Request");
		for (int i = 0; i < 5; ++i) {
			ImGui::SameLine();
			ImGui::Text("%X", check_bit(mem.read(INTERRUPT_FLAGS_REG), i));
		}
		ImGui::Text("Interrupt Enable ");
		for (int i = 0; i < 5; ++i) {
			ImGui::SameLine();
			ImGui::Text("%X", check_bit(mem.read(INTERRUPT_ENABLE_REG), i));
		}

		ImGui::Separator();
		ImGui::Text(" %d", lcd_regs.LCD_LY);
		ImGui::SameLine();
		ImGui::Text("JOY ");
		u8 joy = joypad.get_joypad_state();
		for (int i = 0; i < 8; ++i) {
			ImGui::SameLine();
			ImGui::Text("%1d", check_bit(joy, i));
		}
		ImGui::Separator();
		ImGui::Text(" TIMER ");
		ImGui::Text("DIV  %X", mem.read(DIVIDER_REGISTER));
		ImGui::SameLine();
		ImGui::Text("TIMA %X", mem.read(TIMER_COUNTER));
		ImGui::Text("TMA  %X", mem.read(TIMER_MODULO));
		ImGui::SameLine();
		ImGui::Text("TAC  %X", mem.read(TIMER_CONTROL));
		ImGui::End();
	}

	// Card
	{
		ImGui::Begin("Card");

		ImGui::Text("%s", card.rom_info.title);
		ImGui::Separator();

		ImGui::Text("CType: %s", card.rom_info.CType_string());
		ImGui::SameLine();
		ImGui::Text("Rom Bank: %d", card.get_selected_rom_bank());
		ImGui::Text("Ram Size: %s", card.rom_info.ram_size_string());
		ImGui::SameLine();
		ImGui::Text("| Rom Size: %s", card.rom_info.rom_size_string());
		ImGui::End();
	}
}

void Gui::render(gameboy &gb) {

	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	auto &io = ImGui::GetIO();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// Submit the DockSpace
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Emulator")) {
			if (ImGui::MenuItem("Skip Boot Rom", NULL, gb.skip_boot_rom)) {
			}
			if (ImGui::MenuItem("60 FPS lock", NULL, gb.fps_drossel)) {
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Palettes")) {
			char address[32];
			for (u32 index = 0; index < gb.ppu.Palettes.size(); index++) {
				sprintf_s(address, "%i", index, sizeof(address));
				if (ImGui::MenuItem(address)) {
					gb.ppu.current_palette = index;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window) {
		ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	generate_widgets(gb);

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	SDL_GL_SwapWindow(context.window);
}

void Gui::destroy() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context.gl_context);
	SDL_DestroyWindow(context.window);
	SDL_Quit();
}

void Gui::showFPS() const {
	ImGui::SetCursorPos(ImVec2(20.0f, 30.0f));
	ImGui::TextColored(magenta, "Frame Rate: %.2f FPS\nFrame Time: %.2f ms",
					   ImGui::GetIO().Framerate,
					   1000.0f / ImGui::GetIO().Framerate);
}
