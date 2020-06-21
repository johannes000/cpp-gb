#include "PPU.h"

#include <cassert>
#include <cstdio>

PPU::PPU(MEMORY *m) : mem(m) {
  createWindow();

  v_ram = std::vector<u8>(VRAM_SIZE, 0x00);
  sprite_attribute_table = std::vector<u8>(OAM_SIZE, 0x00);

  lcd_buffer = std::vector<Pixel>(GB_LCD_SIZE_WIDTH * GB_LCD_SIZE_HEIGHT,
                                  {0x00, 0x00, 0x00});
  lcd_control.update(this);

  set_lcd_mode(LCD_Mode::OAM);
  load_lcd_mode();
  render_frame();
}

PPU::~PPU() { closeWindow(); }

void PPU::update(int cycles) {
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
          waitforinput();
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

void PPU::waitforinput() {
  bool quit = false;

  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
        case SDLK_p:
          std::cout << "P" << std::endl;
          current_palette = (current_palette + 1) % Palettes.size();
          break;
        case SDLK_ESCAPE:
          exit(0);
          break;
        default:
          quit = true;
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

void PPU::closeWindow() {
  // Destroy window
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  // Quit SDL subsystems
  SDL_Quit();
}

void PPU::set_lcd_mode(LCD_Mode m) {
  // Schreibt den gewünschten mode ins LCD Status register
  u8 mode = static_cast<u8>(m);
  if (mode > 3) return;
  lcd_mode = m;
  u8 *data = &lcd_register.LCD_STATUS;
  mode & 1 ? set_bit(*data, 0) : reset_bit(*data, 0);
  mode & 2 ? set_bit(*data, 1) : reset_bit(*data, 1);
}

void PPU::load_lcd_mode() {
  // Liest den LCD Mode aus dem LCD Status register
  // und aktualisiert die lokale variable
  u8 *data = &lcd_register.LCD_STATUS;
  lcd_mode = static_cast<LCD_Mode>(*data & 3);
}

void PPU::set_lcd_state(const LCD_State &s) {
  // setzt den gewünschten status im LCD Status Register
  u8 *data = &lcd_register.LCD_STATUS;
  set_bit(*data, static_cast<u8>(s));
}

void PPU::reset_lcd_state(const LCD_State &s) {
  // setzt den gewünschten status im LCD Status Register zurück
  u8 *data = &lcd_register.LCD_STATUS;
  reset_bit(*data, static_cast<u8>(s));
}

bool PPU::get_lcd_state(const LCD_State &s) const {
  return check_bit(lcd_register.LCD_STATUS, static_cast<u8>(s));
}

void PPU::buffer_line(u8 line) {
  lcd_control.update(this);
  if (lcd_control.bg_enabled) {
    // BG Tile Map Display Select
    //                                                   unsigned : signed
    const u16 bg_tileset = lcd_control.bg_window_tileset ? 0x8000 : 0x8800;
    const u16 wdw_tilemap = lcd_control.window_tile_map ? 0x9C00 : 0x9800;
    const u16 bg_tilemap = lcd_control.bg_tile_map ? 0x9C00 : 0x9800;
    const u8 bg_pallete = lcd_register.BG_PALETTE_DATA;

    // Festlegen wo im Vector die Pixel gesetzt werden
    int y = line;
    int sc_y = (y + lcd_register.LCD_SCY) % GB_LCD_SIZE_HEIGHT;

    int pal = rand() % 4;
    for (int x = 0; x <= GB_LCD_SIZE_WIDTH - 1; x++) {
      int sc_x = (x + lcd_register.LCD_SCX) % GB_LCD_SIZE_WIDTH;

      // Welches tile in der bg_tilemap
      int tile_x = sc_x / TILE_WIDTH;
      int tile_y = sc_y / TILE_WIDTH;

      // Welcher Pixel im tile
      int tile_pixel_x = sc_x % TILE_WIDTH;
      int tile_pixel_y = sc_y % TILE_HEIGHT;

      // Adresse des tiles im vram
      int addr = bg_tilemap + tile_y * TILE_PER_LINE + tile_x;

      // Ob die bg_tilemap adressen signed sind oder nicht
      int tile;
      if (lcd_control.bg_window_tileset) {
        tile = (i8)read(addr);
      } else {
        tile = read(addr);
      }

      size_t index = ((GB_LCD_SIZE_WIDTH * y) + x);
      if (index >= lcd_buffer.size()) return;

      lcd_buffer[index].R = Palettes[current_palette][pal].R;
      lcd_buffer[index].G = Palettes[current_palette][pal].G;
      lcd_buffer[index].B = Palettes[current_palette][pal].B;
    }
  }
}

void PPU::render_frame() {
  // FPS rechner
  fps_frames++;
  if (fps_lasttime < SDL_GetTicks() - FPS_INTERVAL * 1000) {
    fps_lasttime = SDL_GetTicks();
    fps_current = fps_frames;
    fps_frames = 0;
    sprintf_s(fps_text, "%d", fps_current);
    SDL_SetWindowTitle(window, fps_text);
  }
  SDL_RenderClear(renderer);
  SDL_UpdateTexture(texture, NULL, &lcd_buffer[0], GB_LCD_SIZE_WIDTH * 3);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

LCD_Mode PPU::mode() { return lcd_mode; }

bool PPU::createWindow() {
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
  Register *data = &ppu->lcd_register.LCD_CONTROL;
  *this = {(bool)((*data >> 7) & 1), (bool)((*data >> 6) & 1),
           (bool)((*data >> 5) & 1), (bool)((*data >> 4) & 1),
           (bool)((*data >> 3) & 1), (bool)((*data >> 2) & 1),
           (bool)((*data >> 1) & 1), (bool)((*data >> 0) & 1)};
}
