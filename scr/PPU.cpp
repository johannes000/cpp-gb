#include "PPU.h"

PPU::PPU(MEMORY *m) : mem(m) {
    create_window();

    v_ram = std::vector<u8>(VRAM_SIZE, 0xFF);
    sprite_attribute_table = std::vector<u8>(OAM_SIZE, 0x1F);

    lcd_buffer = std::vector<Pixel>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT,
                                    {0x00, 0x00, 0x00});
    lcd_control.update(this);

    set_lcd_mode(LCD_Mode::OAM);
    load_lcd_mode();
    render_frame();
}

PPU::~PPU() { close_window(); }

int pp = 0;

void PPU::update(const int cycles) {
    lcd_control.update(this);
    load_lcd_mode();
    cycles_in_mode += cycles;
    switch (lcd_mode) {
        // MODE 2 : Searching OAM Guckt vor jeder line ins OAM um die, für die
        // aktuelle line, benötigten sprites zu suchen
        case LCD_Mode::OAM:
            if (cycles_in_mode >= CLOCKS_SCANLINE_OAM) {
                cycles_in_mode = 0;
                set_lcd_mode(LCD_Mode::VRAM);
            }
            break;

            // MODE 3 : VRAM Zugriff Pixel Transfer
        case LCD_Mode::VRAM:
            if (cycles_in_mode >= CLOCKS_SCANLINE_VRAM) {
                cycles_in_mode = 0;
                set_lcd_mode(LCD_Mode::H_Blank);
            }
            break;

            // MODE 0: H - Blank (204 cycles) nach jeder line
        case LCD_Mode::H_Blank:
            if (cycles_in_mode >= CLOCKS_HORIZONTAL_BLANK) {  // H-Blank vorbei
                cycles_in_mode = 0;
                buffer_line(lcd_register.LCD_LY);
                // H-Blank sachen
                if (++lcd_register.LCD_LY == GB_LCD_SIZE_HEIGHT) {
                    // alle lines für dieses Frame gebuffert,
                    // das frame kann jetzt gerendered werden
                    render_frame();
                    wait_for_input();
                    set_lcd_mode(LCD_Mode::V_Blank);
                } else {
                    set_lcd_mode(LCD_Mode::OAM);
                }
            }
            break;

            // MODE 1 : V - Blank nachdem jede linie gezeichnet wurde
        case LCD_Mode::V_Blank:
            if (cycles_in_mode >= CLOCKS_SCANLINE) {
                cycles_in_mode = 0;
                if (++lcd_register.LCD_LY == VERTICAL_BLANK_LINES) {
                    lcd_register.LCD_LY = 0;
                    set_lcd_mode(LCD_Mode::OAM);
                }
            }
            break;
    }
}

void PPU::wait_for_input() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            exit(0);
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_p:
                    std::cout << "P" << std::endl;
                    current_palette =
                            (current_palette + 1) % static_cast<int>(Palettes.size());
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

void PPU::write(const u16 &addr, const u8 &data) {
    if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
        v_ram[addr - GRAPHICS_RAM] = data;
    } else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
        sprite_attribute_table[addr - SPRITE_INFORMATION] = data;
    } else {
        switch (addr) {
            case LCD_CONTROL:
                lcd_register.LCD_CONTROL = data;
                break;
            case LCD_STATUS:
                lcd_register.LCD_STATUS = data;
                break;
            case LCD_SCY:
                lcd_register.LCD_SCY = data;
                break;
            case LCD_SCX:
                lcd_register.LCD_SCX = data;
                break;
            case LCD_LY:
                // Writing will reset the counter.
                lcd_register.LCD_LY = 0;
                break;
            case LCD_LYC:
                lcd_register.LCD_LYC = data;
                break;
            case LCD_WY:
                lcd_register.LCD_WY = data;
                break;
            case LCD_WX:
                lcd_register.LCD_WX = data;
                break;
            case BG_PALETTE_DATA:
                lcd_register.BG_PALETTE_DATA = data;
                break;
            case BG_PALETTE_DATA_0:
                lcd_register.BG_PALETTE_DATA_0 = data;
                break;
            case BG_PALETTE_DATA_1:
                lcd_register.BG_PALETTE_DATA_1 = data;
                break;
            default:
                mem->write(addr, data);
                break;
        }
    }
};

u8 PPU::read(const u16 &addr) const {
    if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
        return v_ram.at(addr - GRAPHICS_RAM);
    } else {
        switch (addr) {
            case LCD_CONTROL:
                return lcd_register.LCD_CONTROL;
            case LCD_STATUS:
                return lcd_register.LCD_STATUS;
            case LCD_SCY:
                return lcd_register.LCD_SCY;
            case LCD_SCX:
                return lcd_register.LCD_SCX;
            case LCD_LY:
                return lcd_register.LCD_LY;
            case LCD_LYC:
                return lcd_register.LCD_LYC;
            case LCD_WY:
                return lcd_register.LCD_WY;
            case LCD_WX:
                return lcd_register.LCD_WX;
            case BG_PALETTE_DATA:
                return lcd_register.BG_PALETTE_DATA;
            case BG_PALETTE_DATA_0:
                return lcd_register.BG_PALETTE_DATA_0;
            case BG_PALETTE_DATA_1:
                return lcd_register.BG_PALETTE_DATA_1;
            default:
                return mem->read(addr);
        }
    }
}

void PPU::close_window() const {
    // Destroy window
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    // Quit SDL subsystems
    SDL_Quit();
}

void PPU::set_lcd_mode(LCD_Mode m) {
    // Schreibt den gewünschten mode ins LCD Status register
    const auto mode = static_cast<u8>(m);
    if (mode > 3) return;
    lcd_mode = m;
    auto *const data = &lcd_register.LCD_STATUS;
    mode & 1 ? set_bit(*data, 0) : reset_bit(*data, 0);
    mode & 2 ? set_bit(*data, 1) : reset_bit(*data, 1);
}

void PPU::load_lcd_mode() {
    // Liest den LCD Mode aus dem LCD Status register
    // und aktualisiert die lokale variable
    auto *const data = &lcd_register.LCD_STATUS;
    lcd_mode = static_cast<LCD_Mode>(*data & 3);
}

void PPU::set_lcd_state(const LCD_State &s) {
    // setzt den gewünschten status im LCD Status Register
    auto *const data = &lcd_register.LCD_STATUS;
    set_bit(*data, static_cast<u8>(s));
}

void PPU::reset_lcd_state(const LCD_State &s) {
    // setzt den gewünschten status im LCD Status Register zurück
    auto *const data = &lcd_register.LCD_STATUS;
    reset_bit(*data, static_cast<u8>(s));
}

bool PPU::get_lcd_state(const LCD_State &s) const {
    return check_bit(lcd_register.LCD_STATUS, static_cast<u8>(s));
}

int pal = -1;

void PPU::buffer_line(const u8 line) {
    lcd_control.update(this);
    if (lcd_control.bg_enabled) {
        lcd_register.LCD_SCX++;
        // BG Tile Map Display Select
        const u16 bg_tileset = lcd_control.bg_window_tileset ? 0x8000 : 0x8800;//unsigned : signed
        const u16 wdw_tilemap = lcd_control.window_tile_map ? 0x9C00 : 0x9800;
        const u16 bg_tilemap = lcd_control.bg_tile_map ? 0x9C00 : 0x9800;
        const auto bg_pallete = lcd_register.BG_PALETTE_DATA;

        // Festlegen wo im Vector die Pixel gesetzt werden
        const int y = line;
        const auto sc_y = (y + lcd_register.LCD_SCY) % GB_LCD_SIZE_HEIGHT;

        for (auto x = 0; x <= GB_LCD_SIZE_WIDTH - 1;) {
            const auto sc_x = (x + lcd_register.LCD_SCX) % GB_LCD_SIZE_WIDTH;

            // Welches tile in der bg_tile-map
            const auto tile_x = sc_x / TILE_WIDTH;
            const auto tile_y = sc_y / TILE_WIDTH;

            // Welcher Pixel im tile
            auto tile_pixel_x = sc_x % TILE_WIDTH;
            const u8 tile_pixel_y = sc_y % TILE_HEIGHT;

            // Adresse des Tiles im vram
            const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);

            // Welche Tile ID im vram
            const u8 tile_id = read(tile_addr);

            const u16 tile_addr_in_tileset = lcd_control.bg_window_tileset ? tile_id * TILE_WIDTH * 2 // unsigned
                                                                           : (static_cast<i8>(tile_id) + 128) *
                                                                             TILE_WIDTH * 2; // signed

            const u16 tile_addr_start = bg_tileset + tile_pixel_y * 2 + tile_addr_in_tileset;


            // Schreibe das gefundene Tile in den Buffer
            for (; tile_pixel_x <= TILE_WIDTH && x <= GB_LCD_SIZE_WIDTH - 1;
                   x++, tile_pixel_x++) {
                u8 p_line_1 = read(tile_addr_start);
                u8 p_line_2 = read(tile_addr_start + 1);

                u8 color = (check_bit(p_line_1, 7 - tile_pixel_x - 1) | check_bit(p_line_2, 7 - tile_pixel_x - 1));

                const auto index = ((GB_LCD_SIZE_WIDTH * y) + x);
                lcd_buffer[index].R = Palettes[current_palette][color].R;
                lcd_buffer[index].G = Palettes[current_palette][color].G;
                lcd_buffer[index].B = Palettes[current_palette][color].B;
            }
        }
    }
}

void PPU::render_frame() {
    // FPS rechner
    fps_frames++;
    if (fps_last_time < SDL_GetTicks() - FPS_INTERVAL * 1000) {
        fps_last_time = SDL_GetTicks();
        fps_current = fps_frames;
        fps_frames = 0;
        sprintf(fps_text, "%d", fps_current);
        SDL_SetWindowTitle(window, fps_text);
    }
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, nullptr, &lcd_buffer[0], GB_LCD_SIZE_WIDTH * 3);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

LCD_Mode PPU::mode() const { return lcd_mode; }

bool PPU::create_window() {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("cpp-gb", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 4,
                              SCREEN_HEIGHT * 4, SDL_WINDOW_SHOWN);
    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);

    return true;
}

void PPU::LCD_Control::update(PPU *ppu) {
    auto *const data = &ppu->lcd_register.LCD_CONTROL;
    *this = {
            static_cast<bool>((*data >> 7) & 1), static_cast<bool>((*data >> 6) & 1),
            static_cast<bool>((*data >> 5) & 1), static_cast<bool>((*data >> 4) & 1),
            static_cast<bool>((*data >> 3) & 1), static_cast<bool>((*data >> 2) & 1),
            static_cast<bool>((*data >> 1) & 1), static_cast<bool>((*data >> 0) & 1)};
}
