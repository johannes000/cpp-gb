#include "PPU.h"

PPU::PPU(MEMORY *m, CPU *c) : ready_to_render(false), cpu(c), mem(m) {

    v_ram = std::vector<u8>(VRAM_SIZE, 0xFF);

    sprite_attribute_table = std::vector<u8>(OAM_SIZE, 0xFF);
    // for (auto &v : sprite_attribute_table) v = rand() % 0x04;

    lcd_buffer = std::vector<Pixel>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT,
                                    {0xFF, 0xFF, 0xFF});
    tile_buffer = std::vector<std::vector<Pixel>>(0xFFF0, {(8 * 8), {0xFF, 0xFF, 0xFF}});
    lcd_control.update(this);


    set_lcd_mode(LCD_Mode::OAM);
    load_lcd_mode();
}


void PPU::update(const int cycles) {
    lcd_control.update(this);
    load_lcd_mode();
    cycles_in_mode += cycles;
    switch (lcd_mode) {
        // MODE 2 : Searching OAM Guckt vor jeder line ins OAM um die, für die
        // aktuelle line, benötigten sprites zu suchen
        case LCD_Mode::OAM:
            if (cycles_in_mode >= CLOCKS_SCANLINE_OAM) {
                cycles_in_mode -= CLOCKS_SCANLINE_OAM;
                set_lcd_mode(LCD_Mode::VRAM);
            }
            break;

            // MODE 3 : VRAM Zugriff Pixel Transfer
        case LCD_Mode::VRAM:
            if (cycles_in_mode >= CLOCKS_SCANLINE_VRAM) {
                cycles_in_mode -= CLOCKS_SCANLINE_VRAM;
                set_lcd_mode(LCD_Mode::H_Blank);
            }
            break;

            // MODE 0: H - Blank (204 cycles) nach jeder line
        case LCD_Mode::H_Blank:
            if (cycles_in_mode >= CLOCKS_HORIZONTAL_BLANK) {  // H-Blank vorbei
                cycles_in_mode -= CLOCKS_HORIZONTAL_BLANK;
                buffer_line(lcd_register.LCD_LY);

                // H-Blank sachen
                if (++lcd_register.LCD_LY == GB_LCD_SIZE_HEIGHT) {
                    set_lcd_mode(LCD_Mode::V_Blank);
                    cpu->request_interrupt(Interrupt::V_Blank);
                } else {
                    set_lcd_mode(LCD_Mode::OAM);
                }
            }
            break;

            // MODE 1 : V - Blank nachdem jede Zeile gezeichnet wurde
        case LCD_Mode::V_Blank:
            if (cycles_in_mode >= CLOCKS_SCANLINE) {
                cycles_in_mode -= CLOCKS_SCANLINE;
                if (++lcd_register.LCD_LY == VERTICAL_BLANK_LINES) {
                    lcd_register.LCD_LY = 0;
                    ready_to_render = true;
                    set_lcd_mode(LCD_Mode::OAM);
                }
            }
            break;
    }
}

void PPU::write(const u16 &addr, const u8 &data) {
    if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
        v_ram.at(addr - GRAPHICS_RAM) = data;
        update_tile(addr - GRAPHICS_RAM);
    } else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
        sprite_attribute_table.at(addr - SPRITE_INFORMATION) = data;
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
}


u8 PPU::read(const u16 &addr) const {
    if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
        return v_ram.at(addr - GRAPHICS_RAM);
    } else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
        return sprite_attribute_table.at(addr - SPRITE_INFORMATION);
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


void PPU::set_lcd_mode(LCD_Mode m) {
    // Schreibt den gewünschten mode ins LCD Status register
    const auto mode = static_cast<u8>(m);
    if (mode > 3) return;
    lcd_mode = m;
    auto *const data = &lcd_register.LCD_STATUS;
    mode & 1u ? set_bit(*data, 0) : reset_bit(*data, 0);
    mode & 2u ? set_bit(*data, 1) : reset_bit(*data, 1);
}

void PPU::load_lcd_mode() {
    // Liest den LCD Mode aus dem LCD Status register
    // und aktualisiert die lokale variable
    auto *const data = &lcd_register.LCD_STATUS;
    lcd_mode = static_cast<LCD_Mode>(*data & 3u);
}

void PPU::set_lcd_state(const LCD_State s) {
    // setzt den gewünschten status im LCD Status Register
    set_bit(lcd_register.LCD_STATUS, static_cast<u8>(s));
}

void PPU::reset_lcd_state(const LCD_State s) {
    // setzt den gewünschten status im LCD Status Register zurück
    reset_bit(lcd_register.LCD_STATUS, static_cast<u8>(s));
}

bool PPU::check_lcd_state(const LCD_State s) const {
    return check_bit(lcd_register.LCD_STATUS, static_cast<u8>(s));
}

/*void PPU::buffer_line(const u8 line) {
    lcd_control.update(this);
    if (lcd_control.bg_enabled && lcd_control.lcd_power) {
        // BG Tile Map Display Select
        const u16 bg_tileset = lcd_control.bg_window_tileset ? 0x8000 : 0x8800;//unsigned : signed 07cb
        const u16 bg_tilemap = lcd_control.bg_tile_map ? 0x9C00 : 0x9800;

        // Festlegen wo im Vector die Pixel gesetzt werden
        const auto y = line;

        for (auto x = 0; x < GB_LCD_SIZE_WIDTH;) {
            const auto sc_x = (x + lcd_register.LCD_SCX);
            const auto sc_y = (y + lcd_register.LCD_SCY);

            // Welches tile in der bg_tile-map
            const auto bg_map_x = sc_x % BG_MAP_SIZE;
            const auto bg_map_y = sc_y % BG_MAP_SIZE;

            const auto tile_x = (bg_map_x / TILE_WIDTH);
            const auto tile_y = (bg_map_y / TILE_HEIGHT);

            // Welcher Pixel im tile
            auto tile_pixel_x = (bg_map_x % TILE_WIDTH);
            const auto tile_pixel_y = bg_map_y % TILE_HEIGHT;

            // Adresse des Tiles im vram
            const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);

            const u8 tile_id = read(tile_addr);

            const u16 tile_addr_in_tileset = lcd_control.bg_window_tileset ? tile_id * TILE_WIDTH * 2 // unsigned
                                                                           : (static_cast<i8>(tile_id) + 128) *
                                                                             TILE_WIDTH * 2; // signed
            u16 tile_addr_start = (tile_pixel_y * 2) + bg_tileset + tile_addr_in_tileset;

            // Schreibe das gefundene Tile in den Buffer
            for (tile_pixel_x++; tile_pixel_x <= TILE_WIDTH && x <= GB_LCD_SIZE_WIDTH - 1;
                 x++, tile_pixel_x++) {
                const u8 p_line_1 = read(tile_addr_start);
                const u8 p_line_2 = read(tile_addr_start + 1);

                const u8 color = static_cast<u8>(check_bit(p_line_1, 8 - tile_pixel_x) << 1u) |
                                 check_bit(p_line_2, 8 - tile_pixel_x);

                const auto index = ((GB_LCD_SIZE_WIDTH * y) + x);
                lcd_buffer[index].R = Palettes[current_palette][color].R;
                lcd_buffer[index].G = Palettes[current_palette][color].G;
                lcd_buffer[index].B = Palettes[current_palette][color].B;
            }
        }
    }
}*/

void PPU::buffer_line(const u8 line) {
    lcd_control.update(this);
    if (lcd_control.bg_enabled && lcd_control.lcd_power) {
        const u16 bg_tilemap = lcd_control.bg_tile_map ? 0x9C00 : 0x9800;

        const auto y = line;

        for (auto x = 0; x < GB_LCD_SIZE_WIDTH;) {
            const auto sc_x = (x + lcd_register.LCD_SCX);
            const auto sc_y = (y + lcd_register.LCD_SCY);

            // Welches tile in der bg_tile-map
            const auto bg_map_x = sc_x % BG_MAP_SIZE;
            const auto bg_map_y = sc_y % BG_MAP_SIZE;

            const auto tile_x = (bg_map_x / TILE_WIDTH);
            const auto tile_y = (bg_map_y / TILE_HEIGHT);

            auto tile_pixel_x = (bg_map_x % TILE_WIDTH);
            const auto tile_pixel_y = bg_map_y % TILE_HEIGHT;

            // Adresse des Tiles im vram
            const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);


            int tile_id = read(tile_addr);

            if (!lcd_control.bg_window_tileset && tile_id < 0x80)
                tile_id += 0x100u;

            for (tile_pixel_x++; tile_pixel_x <= TILE_WIDTH && x <= GB_LCD_SIZE_WIDTH - 1;
                 x++, tile_pixel_x++) {
                const auto index = ((GB_LCD_SIZE_WIDTH * y) + x);
                lcd_buffer[index] = tile_buffer[tile_id][(tile_pixel_y * 8) + tile_pixel_x - 1];

            }
        }
    }
}


[[nodiscard]] const Pixel *PPU::get_lcd_buffer() const {
    return &lcd_buffer[0];
}

void PPU::update_tile(const u16 &a) {
    auto addr = a & 0x1FFEu;
    auto tile_id = static_cast<u16>(addr >> 4u) & 0x1FFu;
    auto y = (static_cast<u16>(addr >> 1u) & 7u);

    if (tile_id > 0xFF) {

    }
    const u8 p_line_1 = v_ram.at(addr);
    const u8 p_line_2 = v_ram.at(addr + 1);

    for (int pixel = 0; pixel <= 8; ++pixel) {
        const u8 color = static_cast<u8>(check_bit(p_line_1, (7 - pixel)) << 1u) |
                         check_bit(p_line_2, (7 - pixel));
        auto p = ((y * 8) + pixel);
        tile_buffer[tile_id][p] = Palettes[current_palette][color];
    }
}

void PPU::LCD_Control::update(PPU *ppu) {
    const u8 *data = &ppu->lcd_register.LCD_CONTROL;
    *this = {
            check_bit(*data, 7),
            check_bit(*data, 6),
            check_bit(*data, 5),
            check_bit(*data, 4),
            check_bit(*data, 3),
            check_bit(*data, 2),
            check_bit(*data, 1),
            check_bit(*data, 0),
    };
}


