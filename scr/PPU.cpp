#include "PPU.hpp"

#include <algorithm>

PPU::PPU(MEMORY *m, CPU *c)
	: cpu_(c),
	  mem_(m),
	  ready_to_render_(false) {
	reset();
}
auto compare_sprite_x(Sprite *sp1, Sprite *sp2) -> bool {
	return (sp1->x_pos_ < sp2->x_pos_);
}
auto compare_sprite_x_reverse(Sprite *sp1, Sprite *sp2) -> bool {
	return (sp1->x_pos_ > sp2->x_pos_);
}
auto PPU::update(const int cycles, bool skip_render) -> bool {
	if (skip_render) {
		lcd_register_.LCD_LY_ = 0x90;
		return ready_to_render_;
	}
	update_lcd_mode();
	cycles_in_mode_ += cycles;
	switch (lcd_mode_) {
		case LCD_Mode::OAM:
			// MODE 2 : Searching OAM
			if (cycles_in_mode_ >= CLOCKS_SCANLINE_OAM) {
				cycles_in_mode_ -= CLOCKS_SCANLINE_OAM;
				set_lcd_mode(LCD_Mode::VRAM);
			}
			break;

		case LCD_Mode::VRAM:
			// MODE 3 : VRAM Zugriff Pixel Transfer
			if (cycles_in_mode_ >= CLOCKS_SCANLINE_VRAM) {
				cycles_in_mode_ -= CLOCKS_SCANLINE_VRAM;
				// compare_ly_lyc();
				set_lcd_mode(LCD_Mode::H_Blank);
				if (lcd_state_.h_blank_interrupt_)
					cpu_->request_interrupt(Interrupt::LCD_Stat);
			}
			break;

		case LCD_Mode::H_Blank:
			// MODE 0: H - Blank (204 cycles) nach jeder line
			if (cycles_in_mode_ >= CLOCKS_HORIZONTAL_BLANK) { // H-Blank vorbei
				cycles_in_mode_ -= CLOCKS_HORIZONTAL_BLANK;
				int line = lcd_register_.LCD_LY_++;
				buffer_line(line);
				compare_ly_lyc();
				if (line == GB_LCD_SIZE_HEIGHT - 1) {
					set_lcd_mode(LCD_Mode::V_Blank);
					cpu_->request_interrupt(Interrupt::V_Blank);
					if (lcd_state_.v_blank_interrupt_)
						cpu_->request_interrupt(Interrupt::LCD_Stat);
				} else {
					set_lcd_mode(LCD_Mode::OAM);
					if (lcd_state_.oam_interrupt_)
						cpu_->request_interrupt(Interrupt::LCD_Stat);
				}
			}
			break;

		case LCD_Mode::V_Blank:
			// MODE 1 : V - Blank nachdem jede Zeile gezeichnet wurde
			if (cycles_in_mode_ >= CLOCKS_SCANLINE) {
				cycles_in_mode_ -= CLOCKS_SCANLINE;
				if (++lcd_register_.LCD_LY_ > VERTICAL_BLANK_LINES) {
					lcd_register_.LCD_LY_ = 0;
					ready_to_render_ = true;
					set_lcd_mode(LCD_Mode::OAM);
					if (lcd_state_.oam_interrupt_)
						cpu_->request_interrupt(Interrupt::LCD_Stat);
				}
				compare_ly_lyc();
			}
			break;
	}
	return ready_to_render_;
}

auto PPU::write(const u16 &addr, const u8 &data) -> void {
	if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
		v_ram_.at(addr - GRAPHICS_RAM) = data;
		update_tile(addr - GRAPHICS_RAM);
	} else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
		sprite_attribute_table_.at(addr - SPRITE_INFORMATION) = data;
		build_sprite_data(addr, data);
	} else {
		switch (addr) {
			case LCD_CONTROL:
				lcd_control_.value_ = data;
				break;
			case LCD_STATUS:
				lcd_state_.value_ |= (data & 0xF8);
				break;
			case LCD_SCY:
				lcd_register_.LCD_SCY_ = data;
				break;
			case LCD_SCX:
				lcd_register_.LCD_SCX_ = data;
				break;
			case LCD_LY:
				// Writing will reset the counter.
				lcd_register_.LCD_LY_ = 0;
				break;
			case LCD_LYC:
				lcd_register_.LCD_LYC_ = data;
				break;
			case LCD_WY:
				lcd_register_.LCD_WY_ = data;
				break;
			case LCD_WX:
				lcd_register_.LCD_WX_ = data;
				break;
			case BG_PALETTE_DATA:
			case SPRITE_PALETTE_DATA_0:
			case SPRITE_PALETTE_DATA_1:
				build_palette(addr, data);
				break;
			default:
				mem_->write(addr, data);
				break;
		}
	}
}

auto PPU::read(const u16 &addr) const -> u8 {
	if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
		return v_ram_.at(addr - GRAPHICS_RAM);
	} else if (addr >= SPRITE_INFORMATION && addr <= SPRITE_INFORMATION_END) {
		return sprite_attribute_table_.at(addr - SPRITE_INFORMATION);
	} else {
		switch (addr) {
			case LCD_CONTROL:
				return lcd_control_.value_;
			case LCD_STATUS:
				return lcd_state_.value_;
			case LCD_SCY:
				return lcd_register_.LCD_SCY_;
			case LCD_SCX:
				return lcd_register_.LCD_SCX_;
			case LCD_LY:
				// return 0x90;
				return lcd_register_.LCD_LY_;
			case LCD_LYC:
				// return 0x90;
				return lcd_register_.LCD_LYC_;
			case LCD_WY:
				return lcd_register_.LCD_WY_;
			case LCD_WX:
				return lcd_register_.LCD_WX_;
			case BG_PALETTE_DATA:
				return lcd_register_.BG_PALETTE_DATA_;
			case SPRITE_PALETTE_DATA_0:
				return lcd_register_.SPRITE_PALETTE_DATA_0_;
			case SPRITE_PALETTE_DATA_1:
				return lcd_register_.SPRITE_PALETTE_DATA_1_;
			default:
				return mem_->read(addr);
		}
	}
}

auto PPU::reset() -> void {
	bg_palette_ = Palettes_[current_palette_];
	sprite_palette_ = std::vector<std::vector<Pixel_a>>(2, bg_palette_);

	set_lcd_mode(LCD_Mode::OAM);
	lcd_register_ = {};

	v_ram_ = std::vector<u8>(VRAM_SIZE, 0xFF);

	sprite_attribute_table_ = std::vector<u8>(OAM_SIZE, 0xFF);

	bg_buffer_ = std::vector<Pixel_a>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT, Palettes_[current_palette_][0]);
	sprite_buffer_ = std::vector<Pixel_a>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT, Palettes_[current_palette_][0]);
	tile_map_data_ = std::vector<std::vector<u8>>(0x0FFF, std::vector<u8>((8 * 8) + 1, 0));
	sprites_ = std::vector<Sprite>(40, {0, 0, 0, 0, false, false, false, false, std::vector<u8>((8 * 8) + 1, 0)});

	lcd_control_ = {};
	lcd_state_ = {};
	lcd_state_.stub_ = 1;
	cycles_in_mode_ = 0;
	cycles_in_vblank_ = 0;
	vblank_line_ = 0;
}
/**
 * @brief Schreibt den gewünschten mode ins LCD Status register
 *
 * @param m
 */
auto PPU::set_lcd_mode(LCD_Mode m) -> void {
	const u8 mode = static_cast<u8>(m);
	if (mode > 3)
		return;
	lcd_mode_ = m;
	lcd_state_.mode_flag_0_ = mode & 1;
	lcd_state_.mode_flag_1_ = mode & 2;
}

/**
 * @brief  Liest den LCD Mode aus dem LCD Status register und aktualisiert die lokale variable
 *
 */
auto PPU::update_lcd_mode() -> void {
	lcd_mode_ = static_cast<LCD_Mode>(lcd_state_.value_ & 3u);
}

/**
 * @brief  setzt den gewünschten status im LCD Status Register
 *
 * @param s
 */
auto PPU::buffer_line(const u8 line) -> void {
	auto bg_line = buffer_bg_line(line);
	auto wn_line = buffer_window_line(line);

	for (u16 index = 0; index < GB_LCD_SIZE_WIDTH; index++) {
		const u16 buffer_index = (line * GB_LCD_SIZE_WIDTH) + index;
		const u8 wn_pixel = wn_line[index];
		const u8 bg_pixel = bg_line[index];

		u8 pixel;

		if (wn_pixel != 5)
			pixel = wn_pixel;
		else
			pixel = bg_pixel;
		bg_buffer_[buffer_index] = Palettes_[current_palette_][pixel];
	}
	buffer_sprite_line(line, &bg_line);
}
auto PPU::buffer_bg_line(const u8 line) -> std::vector<u8> {
	std::vector<u8> background_line(GB_LCD_SIZE_WIDTH, 0);

	if (line >= GB_LCD_SIZE_HEIGHT)
		return background_line;
	if (lcd_control_.bg_enabled_ && lcd_control_.lcd_power_) {
		const u16 bg_tilemap = lcd_control_.bg_tile_map_ == 1 ? 0x9C00 : 0x9800;

		const auto y = line;

		for (auto x = 0; x < GB_LCD_SIZE_WIDTH;) {
			const auto sc_x = (x + lcd_register_.LCD_SCX_);
			const auto sc_y = (y + lcd_register_.LCD_SCY_);

			const auto bg_map_x = sc_x % BG_MAP_SIZE;
			const auto bg_map_y = sc_y % BG_MAP_SIZE;

			const auto tile_x = (bg_map_x / TILE_WIDTH);
			const auto tile_y = (bg_map_y / TILE_HEIGHT);

			auto tile_pixel_x = (bg_map_x % TILE_WIDTH);
			const auto tile_pixel_y = bg_map_y % TILE_HEIGHT;

			const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);

			int tile_id = read(tile_addr);

			if (!lcd_control_.bg_window_tileset_ && tile_id < 0x80)
				tile_id += 0x100u;

			for (tile_pixel_x++;
				 tile_pixel_x <= TILE_WIDTH && x <= GB_LCD_SIZE_WIDTH - 1;
				 x++, tile_pixel_x++) {
				const auto color = tile_map_data_[tile_id][((tile_pixel_y * 8) + tile_pixel_x - 1) % (8 * 8)];
				background_line[x] = color;
			}
		}
	}
	return background_line;
}

auto PPU::buffer_sprite_line(const u8 line, std::vector<u8> *bg_line) -> void {
	if (line >= GB_LCD_SIZE_HEIGHT + 8 || !lcd_control_.sprites_enabled_ || !lcd_control_.lcd_power_)
		return;
	int sprite_size = lcd_control_.sprite_size_ == 1 ? 16 : 8;
	const int y = static_cast<int>(line);

	std::vector<Sprite *> SpritesToDraw = {};
	// for (Sprite &spr : sprites) {
	for (auto &spr : sprites_) {
		const int real_ypos = lcd_control_.sprite_size_ == 1 ? spr.y_pos_ : spr.y_pos_ - 9;
		if (real_ypos != 0 && real_ypos < 160 && spr.x_pos_ != 0 && spr.x_pos_ < 168) {
			if (y > real_ypos - sprite_size && y <= real_ypos) {
				// Sprite wird gebufferd
				SpritesToDraw.push_back(&spr);
				// Y-Flip
			}
		}
	}
	std::sort(SpritesToDraw.begin(), SpritesToDraw.end(), compare_sprite_x_reverse);
	if (SpritesToDraw.size() > 1) {
		for (int SpriteIndex = 1; SpriteIndex < SpritesToDraw.size(); SpriteIndex++) {
			// Iteratoren um die Sprites leichter zu swapen.
			auto spr = SpritesToDraw.begin() + SpriteIndex;
			auto last_spr = SpritesToDraw.begin() + SpriteIndex - 1;
			if (((*spr)->x_pos_ == (*last_spr)->x_pos_) && ((*spr)->id_ > (*last_spr)->id_)) {
				// When X coordinates are the same, sprites located first in OAM have a higher priority.
				std::iter_swap(spr, last_spr);
			}
		}
	}
	while (SpritesToDraw.size() > 10)
		SpritesToDraw.erase(SpritesToDraw.begin());

	for (auto spr : SpritesToDraw) {
		int tile_no = spr->tile_no_;
		if (sprite_size == 16)
			tile_no &= 0xFE;
		const int real_ypos = lcd_control_.sprite_size_ == 1 ? spr->y_pos_ : spr->y_pos_ - 9;
		int pixel_y_pos = spr->y_flip_ ? real_ypos - y : y - real_ypos + sprite_size - 1;

		if (pixel_y_pos >= 0) {
			for (int pixel_x_pos = 0; pixel_x_pos <= 7 && (spr->x_pos_ + pixel_x_pos) != 0 && spr->x_pos_ + pixel_x_pos < 168; pixel_x_pos++) {
				// Pixel im Buffer

				const s32 x_pos_in_line = (std::max)(0, spr->x_pos_ + pixel_x_pos - 8);
				const int buffer_index = ((GB_LCD_SIZE_WIDTH * y) + x_pos_in_line);
				if (pixel_y_pos >= 8) {
					pixel_y_pos -= 8;
					tile_no |= 0x01;
				}
				const auto palette_color_index = tile_map_data_[tile_no][spr->x_flip_ ? (pixel_y_pos * 8) - pixel_x_pos + 7 : (pixel_y_pos * 8) + pixel_x_pos];
				if (palette_color_index) {
					const auto color = sprite_palette_[spr->palette_][palette_color_index];
					if (!spr->bg_priority_) {
						bg_buffer_[buffer_index] = color;
					} else if (bg_line->at(x_pos_in_line) == 0) {
						bg_buffer_[buffer_index] = color;
					}
				}
			}
		}
	}
	return;
}

auto PPU::buffer_window_line(const u8 line) -> std::vector<u8> {
	// TODO:	Winpos < 7 wird nicht richtig angezeigt
	std::vector<u8> window_line = std::vector<u8>(GB_LCD_SIZE_WIDTH, 5);

	if (line >= GB_LCD_SIZE_HEIGHT)
		return window_line;
	u16 window_y = lcd_register_.LCD_WY_;
	u16 window_x = lcd_register_.LCD_WX_;
	if (window_x < 8)
		window_x = 0;
	else
		window_x -= 7;
	const u16 bg_tilemap = lcd_control_.window_tile_map_ ? 0x9C00 : 0x9800;

	if (line - window_y < GB_LCD_SIZE_HEIGHT && line >= window_y && lcd_control_.window_enable_ && lcd_control_.lcd_power_) {
		for (int line_x_pos = window_x; line_x_pos < GB_LCD_SIZE_WIDTH; line_x_pos++) {

			const auto tile_x = (line_x_pos - window_x) / TILE_WIDTH;
			const auto tile_y = (line - window_y) / TILE_HEIGHT;

			const auto tile_pixel_x = (line_x_pos - window_x) % TILE_WIDTH;
			const auto tile_pixel_y = (line - window_y) % TILE_HEIGHT;

			const u16 tile_addr = bg_tilemap + (tile_y * TILE_PER_LINE + tile_x);

			int tile_id = read(tile_addr);

			if (!lcd_control_.bg_window_tileset_ && tile_id < 0x80)
				tile_id += 0x100u;

			const auto color = tile_map_data_[tile_id][((tile_pixel_y * 8) + tile_pixel_x) % (8 * 8)];
			window_line[line_x_pos] = color;
		}
	}
	return window_line;
}

auto PPU::build_palette(const u16 addr, u8 data) -> void {
	switch (addr) {
		case BG_PALETTE_DATA:
			lcd_register_.BG_PALETTE_DATA_ = data;
			bg_palette_[0] = Palettes_[current_palette_][(data & 3u)];
			bg_palette_[1] = Palettes_[current_palette_][((data >> 2) & 3u)];
			bg_palette_[2] = Palettes_[current_palette_][((data >> 4) & 3u)];
			bg_palette_[3] = Palettes_[current_palette_][((data >> 6) & 3u)];
			break;
		case SPRITE_PALETTE_DATA_0:
			lcd_register_.SPRITE_PALETTE_DATA_0_ = data;
			for (u8 i = 0; i < 4; i++) {
				sprite_palette_[0][i] =
					Palettes_[current_palette_][(data >> (i * 2)) & 3u];
			}
			break;
		case SPRITE_PALETTE_DATA_1:
			lcd_register_.SPRITE_PALETTE_DATA_1_ = data;
			for (u8 i = 0; i < 4; i++) {
				sprite_palette_[1][i] =
					Palettes_[current_palette_][(data >> (i * 2)) & 3u];
			}
			break;
		default:
			return;
	}
}

auto PPU::build_sprite_data(const u16 &addr, const u8 &data) -> void {
	u8 sprite_id = (addr - SPRITE_INFORMATION) >> 2;
	if (sprite_id < 40) {
		switch (addr & 3u) {
			case 0:
				sprites_[sprite_id].y_pos_ = data;
				break;
			case 1:
				sprites_[sprite_id].x_pos_ = data;
				break;
			case 2:
				sprites_[sprite_id].tile_no_ = data;
				sprites_[sprite_id].id_ = sprite_id;
				sprites_[sprite_id].tile_data_ = tile_map_data_[data];
				break;
			case 3:
				sprites_[sprite_id].palette_ = check_bit(data, 4);
				sprites_[sprite_id].x_flip_ = check_bit(data, 5);
				sprites_[sprite_id].y_flip_ = check_bit(data, 6);
				sprites_[sprite_id].bg_priority_ = check_bit(data, 7);
				break;
		}
	}
}

[[nodiscard]] auto PPU::get_lcd_buffer() const -> const Pixel_a * {
	return &bg_buffer_[0];
}

auto PPU::update_tile(const u16 &a) -> void {
	auto addr = a & 0x1FFEu;
	auto tile_id = static_cast<u16>(addr >> 4u) & 0x1FFu;
	auto y = (static_cast<u16>(addr >> 1u) & 7u);

	const u8 p_line_1 = v_ram_.at(addr);
	const u8 p_line_2 = v_ram_.at(addr + 1);

	for (int pixel = 0; pixel <= 8; ++pixel) {
		const u8 color = static_cast<u8>(check_bit(p_line_1, (7u - pixel)) << 1u) | check_bit(p_line_2, (7u - pixel));
		auto p = ((y * 8) + pixel);
		tile_map_data_[tile_id][p] = color;
	}
}

auto PPU::is_ready_to_render() -> bool {
	if (ready_to_render_) {
		ready_to_render_ = false;
		return true;
	} else {
		return false;
	}
}

auto PPU::compare_ly_lyc() -> void {
#if 1
	lcd_state_.coincidence_flag_ = lcd_register_.LCD_LY_ == lcd_register_.LCD_LYC_;

	if (lcd_state_.coincidence_flag_ && lcd_state_.lyc_interrupt_) {
		cpu_->request_interrupt(Interrupt::LCD_Stat);
	}
#else
	if (lcd_register.LCD_LY == lcd_register.LCD_LYC) {
		if (lcd_state.lyc_interrupt)
			cpu->request_interrupt(Interrupt::LCD_Stat);
		lcd_state.coincidence_flag = true;
	} else {
		lcd_state.coincidence_flag = false;
	}
#endif
}
