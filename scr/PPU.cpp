#include "PPU.h"

PPU::PPU(MEMORY *m, CPU *c)
	: ready_to_render(false),
	  cpu(c),
	  mem(m) {
	reset();
}

void PPU::update(const int cycles, bool skip_render) {
	if (skip_render) {
		lcd_register.LCD_LY = 0x90;
		return;
	}
	update_lcd_mode();
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
				if (lcd_state.h_blank_interrupt)
					cpu->request_interrupt(Interrupt::LCD_Stat);
			}
			break;

			// MODE 0: H - Blank (204 cycles) nach jeder line
		case LCD_Mode::H_Blank:
			if (cycles_in_mode >= CLOCKS_HORIZONTAL_BLANK) { // H-Blank vorbei
				cycles_in_mode -= CLOCKS_HORIZONTAL_BLANK;
				int line = lcd_register.LCD_LY;

				buffer_bg_line(line);
				buffer_sprite_line(line);
				buffer_window_line(line);

				if (line == lcd_register.LCD_LYC) {
					if (lcd_state.lyc_interrupt)
						cpu->request_interrupt(Interrupt::LCD_Stat);
					lcd_state.coincidence_flag = true;
				} else {
					lcd_state.coincidence_flag = false;
				}

				if (++lcd_register.LCD_LY >= GB_LCD_SIZE_HEIGHT) {
					set_lcd_mode(LCD_Mode::V_Blank);
					cpu->request_interrupt(Interrupt::V_Blank);
					if (lcd_state.v_blank_interrupt)
						cpu->request_interrupt(Interrupt::LCD_Stat);
				} else {
					set_lcd_mode(LCD_Mode::OAM);
					if (lcd_state.oam_interrupt)
						cpu->request_interrupt(Interrupt::LCD_Stat);
				}
			}
			break;

			// MODE 1 : V - Blank nachdem jede Zeile gezeichnet wurde
		case LCD_Mode::V_Blank:
			if (cycles_in_mode >= CLOCKS_SCANLINE) {
				cycles_in_mode -= CLOCKS_SCANLINE;
				if (++lcd_register.LCD_LY > VERTICAL_BLANK_LINES) {
					lcd_register.LCD_LY = 0;
					ready_to_render = true;
					set_lcd_mode(LCD_Mode::OAM);
					if (lcd_state.oam_interrupt)
						cpu->request_interrupt(Interrupt::LCD_Stat);
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
		build_sprite_data(addr, data);
	} else {
		switch (addr) {
			case LCD_CONTROL:
				lcd_control.value = data;
				break;
			case LCD_STATUS:
				lcd_state.value = data & 0x78;
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
			case SPRITE_PALETTE_DATA_0:
			case SPRITE_PALETTE_DATA_1:
				build_palette(addr, data);
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
				return lcd_control.value;
			case LCD_STATUS:
				return lcd_state.value;
			case LCD_SCY:
				return lcd_register.LCD_SCY;
			case LCD_SCX:
				return lcd_register.LCD_SCX;
			case LCD_LY:
				return lcd_register.LCD_LY;
			case LCD_LYC:
				//return 0x90;
				return lcd_register.LCD_LYC;
			case LCD_WY:
				return lcd_register.LCD_WY;
			case LCD_WX:
				return lcd_register.LCD_WX;
			case BG_PALETTE_DATA:
				return lcd_register.BG_PALETTE_DATA;
			case SPRITE_PALETTE_DATA_0:
				return lcd_register.SPRITE_PALETTE_DATA_0;
			case SPRITE_PALETTE_DATA_1:
				return lcd_register.SPRITE_PALETTE_DATA_1;
			default:
				return mem->read(addr);
		}
	}
}

void PPU::reset() {
	bg_palette = Palettes[current_palette];
	sprite_obj_palette = std::vector<std::vector<Pixel_a>>(2, bg_palette);

	set_lcd_mode(LCD_Mode::OAM);
	lcd_register = {};

	v_ram = std::vector<u8>(VRAM_SIZE, 0xFF);

	sprite_attribute_table = std::vector<u8>(OAM_SIZE, 0xFF);

	bg_buffer = std::vector<Pixel_a>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT, Palettes[current_palette][0]);
	sprite_buffer = std::vector<Pixel_a>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT, Palettes[current_palette][0]);
	tile_map_data = std::vector<std::vector<u8>>(0x0FFF, std::vector<u8>((8 * 8) + 1, 0));
	sprites = std::vector<Sprite>(40, {0, 0, 0, 0, false, false, false, false, std::vector<u8>((8 * 8) + 1, 0)});

	lcd_control = {};
	lcd_state = {};
	cycles_in_mode = 0;
}
/**
 * @brief Schreibt den gewünschten mode ins LCD Status register
 *
 * @param m
 */
void PPU::set_lcd_mode(LCD_Mode m) {
	const u8 mode = static_cast<u8>(m);
	if (mode > 3)
		return;
	lcd_mode = m;
	lcd_state.value = mode & 0x3u;
}

/**
 * @brief  Liest den LCD Mode aus dem LCD Status register und aktualisiert die lokale variable
 *
 */
void PPU::update_lcd_mode() {
	lcd_mode = static_cast<LCD_Mode>(lcd_state.value & 3u);
}

/**
 * @brief  setzt den gewünschten status im LCD Status Register
 *
 * @param s
 */
void PPU::buffer_bg_line(const u8 line) {
	if (line >= GB_LCD_SIZE_HEIGHT)
		return;
	if (lcd_control.bg_enabled && lcd_control.lcd_power) {
		const u16 bg_tilemap = lcd_control.bg_tile_map ? 0x9C00 : 0x9800;

		const auto y = line;

		for (auto x = 0; x < GB_LCD_SIZE_WIDTH;) {
			const auto sc_x = (x + lcd_register.LCD_SCX);
			const auto sc_y = (y + lcd_register.LCD_SCY);

			const auto bg_map_x = sc_x % BG_MAP_SIZE;
			const auto bg_map_y = sc_y % BG_MAP_SIZE;

			const auto tile_x = (bg_map_x / TILE_WIDTH);
			const auto tile_y = (bg_map_y / TILE_HEIGHT);

			auto tile_pixel_x = (bg_map_x % TILE_WIDTH);
			const auto tile_pixel_y = bg_map_y % TILE_HEIGHT;

			const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);

			int tile_id = read(tile_addr);

			if (!lcd_control.bg_window_tileset && tile_id < 0x80)
				tile_id += 0x100u;

			for (tile_pixel_x++;
				 tile_pixel_x <= TILE_WIDTH && x <= GB_LCD_SIZE_WIDTH - 1;
				 x++, tile_pixel_x++) {
				const auto index = ((GB_LCD_SIZE_WIDTH * y) + x);
				const auto color = tile_map_data[tile_id][((tile_pixel_y * 8) + tile_pixel_x - 1) % (8 * 8)];
				bg_buffer[index] = bg_palette[color];
			}
		}
	}
}
/**
 * @brief Bufferd die Sprites für die jeweilige Bildzeile
 *
 * @param u8 line
 */
void PPU::buffer_sprite_line(const u8 line) {
	if (line >= GB_LCD_SIZE_HEIGHT + 8 || !lcd_control.sprites_enabled || !lcd_control.lcd_power)
		return;
	u8 sprites_count = 0;
	int sprite_size = lcd_control.sprite_size ? 16 : 8;
	const int y = static_cast<int>(line);

	for (Sprite &spr : sprites) {
		const int real_ypos = lcd_control.sprite_size ? spr.y_pos - 17 : spr.y_pos - 9;
		if (real_ypos != 0 && real_ypos < 160 && spr.x_pos != 0 && spr.x_pos < 168 && sprites_count <= 10) {
			if (y > real_ypos - sprite_size && y <= real_ypos) {
				// Sprite wird gebufferd
				sprites_count++;
				// Y-Flip
				const int pixel_y_pos = spr.y_flip ? real_ypos - y : y - real_ypos + sprite_size - 1;

				if (pixel_y_pos >= 0) {
					for (int pixel_x_pos = 0; pixel_x_pos <= 7 && (spr.x_pos + pixel_x_pos) != 0 && spr.x_pos + pixel_x_pos < 168; pixel_x_pos++) {
						// Pixel im Buffer
						const int buffer_index = ((GB_LCD_SIZE_WIDTH * y) + spr.x_pos + pixel_x_pos - 8);
						// Pixel im Tile
						const auto palette_color_index = spr.tile_data[spr.x_flip ? (pixel_y_pos * 8) - pixel_x_pos + 7 : (pixel_y_pos * 8) + pixel_x_pos];
						if (palette_color_index != 0) {
							const auto color = sprite_obj_palette[spr.palette][palette_color_index];
							bg_buffer[buffer_index] = color;
						}
					}
				}
			}
		}
	}
	return;
}

void PPU::buffer_window_line(const u8 line) {
	if (line >= GB_LCD_SIZE_HEIGHT)
		return;
	u16 window_y = read(LCD_WY);
	u16 window_x = read(LCD_WX);
	if (window_x <= 7)
		window_x = 0;
	else
		window_x -= 7;
	const u16 bg_tilemap = lcd_control.window_tile_map ? 0x9C00 : 0x9800;

	if (line - window_y < GB_LCD_SIZE_HEIGHT && line >= window_y && lcd_control.window_enable && lcd_control.lcd_power) {
		for (int line_x_pos = window_x; line_x_pos < GB_LCD_SIZE_WIDTH; line_x_pos++) {
			bg_buffer[(line * GB_LCD_SIZE_WIDTH) + line_x_pos] = {0xFF, 0xFF, 0xFF, 0xFF};

			const auto tile_x = (line_x_pos - window_x) / TILE_WIDTH;
			const auto tile_y = (line - window_y) / TILE_HEIGHT;

			const auto tile_pixel_x = (line_x_pos - window_x) % TILE_WIDTH;
			const auto tile_pixel_y = (line - window_y) % TILE_HEIGHT;

			const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);

			int tile_id = read(tile_addr);

			if (!lcd_control.bg_window_tileset && tile_id < 0x80)
				tile_id += 0x100u;

			const auto index = ((GB_LCD_SIZE_WIDTH * line) + line_x_pos);
			const auto color = tile_map_data[tile_id][((tile_pixel_y * 8) + tile_pixel_x) % (8 * 8)];
			bg_buffer[index] = bg_palette[color];
		}
	}
}

void PPU::build_palette(const u16 addr, u8 data) {
	switch (addr) {
		case BG_PALETTE_DATA:
			lcd_register.BG_PALETTE_DATA = data;
			bg_palette[0] = Palettes[current_palette][(data & 3u)];
			bg_palette[1] = Palettes[current_palette][((data >> 2) & 3u)];
			bg_palette[2] = Palettes[current_palette][((data >> 4) & 3u)];
			bg_palette[3] = Palettes[current_palette][((data >> 6) & 3u)];
			break;
		case SPRITE_PALETTE_DATA_0:
			lcd_register.SPRITE_PALETTE_DATA_0 = data;
			for (u8 i = 0; i < 4; i++) {
				sprite_obj_palette[0][i] =
					Palettes[current_palette][(data >> (i * 2)) & 3u];
			}
			break;
		case SPRITE_PALETTE_DATA_1:
			lcd_register.SPRITE_PALETTE_DATA_1 = data;
			for (u8 i = 0; i < 4; i++) {
				sprite_obj_palette[1][i] =
					Palettes[current_palette][(data >> (i * 2)) & 3u];
			}
			break;
		default:
			return;
	}
}

void PPU::build_sprite_data(const u16 &addr, const u8 &data) {
	u8 sprite_id = (addr - SPRITE_INFORMATION) >> 2;
	if (sprite_id < 40) {
		switch (addr & 3u) {
			case 0:
				sprites[sprite_id].y_pos = data;
				break;
			case 1:
				sprites[sprite_id].x_pos = data;
				break;
			case 2:
				sprites[sprite_id].tile_no = data;
				sprites[sprite_id].id = sprite_id;
				sprites[sprite_id].tile_data = tile_map_data[data];
				break;
			case 3:
				sprites[sprite_id].palette = check_bit(data, 4);
				sprites[sprite_id].x_flip = check_bit(data, 5);
				sprites[sprite_id].y_flip = check_bit(data, 6);
				sprites[sprite_id].bg_priority = check_bit(data, 7);
				break;
		}
	}
}

[[nodiscard]] const Pixel_a *PPU::get_lcd_buffer() const {
	// return &bg_buffer[0];
	return &bg_buffer[0];
}

void PPU::update_tile(const u16 &a) {
	auto addr = a & 0x1FFEu;
	auto tile_id = static_cast<u16>(addr >> 4u) & 0x1FFu;
	auto y = (static_cast<u16>(addr >> 1u) & 7u);

	const u8 p_line_1 = v_ram.at(addr);
	const u8 p_line_2 = v_ram.at(addr + 1);

	for (int pixel = 0; pixel <= 8; ++pixel) {
		const u8 color = static_cast<u8>(check_bit(p_line_1, (7u - pixel)) << 1u) | check_bit(p_line_2, (7u - pixel));
		auto p = ((y * 8) + pixel);
		tile_map_data[tile_id][p] = color;
	}
}
