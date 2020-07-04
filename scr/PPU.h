
#ifndef PPU_H_GUARD
#define PPU_H_GUARD


#include <SDL.h>

#include <vector>

#include "MEMORY.h"
#include "type.h"

constexpr auto FPS_INTERVAL = 1.0;  // seconds.;

constexpr u16 VRAM_SIZE = 0x2000;
constexpr u16 OAM_SIZE = 0x0100;

// Period                       GPU mode number     Time spent(clocks)
// Scan line(accessing OAM)         2                   80
// Scan line(accessing VRAM)        3                   172
// Horizontal blank                 0                   204
// One line(scan and blank)                             456
// Vertical blank                   1                   4560 (10 lines)
// Full frame(scans and vblank)                         70224

constexpr int CLOCKS_SCANLINE_OAM = 80;
constexpr int CLOCKS_SCANLINE_VRAM = 172;
constexpr int CLOCKS_HORIZONTAL_BLANK = 204;
constexpr int CLOCKS_SCANLINE =
        CLOCKS_SCANLINE_OAM + CLOCKS_SCANLINE_VRAM + CLOCKS_HORIZONTAL_BLANK;
constexpr int CLOCKS_VERTICAL_BLANK = CLOCKS_SCANLINE * 10;
constexpr int CLOCKS_FULL_FRAME = 70224;

constexpr int VERTICAL_BLANK_LINES = 154;


class MEMORY;

// ReSharper disable once CppInconsistentNaming
class PPU {
public:
    explicit PPU(MEMORY *m);

    ~PPU();

    void next_step();

    void update(const int cycles);

    void write(const u16 &addr, const u8 &data);

    u8 read(const u16 &addr) const;

private:
    MEMORY *mem;

    int cycles_in_mode{};

    bool create_window();

    void close_window() const;

    SDL_Event event{};

    bool ready_to_render;

    void update_tile(const u16 &addr);

public:
    void render_frame();

    inline bool is_ready_to_render() const { return ready_to_render; }


    // SDL Sachen
private:
    const int SCREEN_WIDTH = GB_LCD_SIZE_WIDTH;
    const int SCREEN_HEIGHT = GB_LCD_SIZE_HEIGHT;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;

    // Speicher
private:
    std::vector<u8> v_ram;
    std::vector<u8> sprite_attribute_table;

private:
    // LCD Data
    LCD_Mode lcd_mode{};

    LCD_Register lcd_register{};
public:
    const LCD_Register &get_lcd_register() const { return lcd_register; }

private:

    struct LCD_Control {
        bool lcd_power;          // Bit 7 - LCD Power(0 = Off, 1 = On)
        bool window_tile_map;    // Bit 6 - Window Tile Map (0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
        bool window_enable;      // Bit 5 - Window Enable(0 = Disabled, 1 = Enabled)
        bool bg_window_tileset;  // Bit 4 - BG & Window Tileset(0 = 8800h - 97FFh, 1 = 8000h - 8FFFh)
        bool bg_tile_map;        // Bit 3 - BG Tile Map (0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
        bool sprite_size;        // Bit 2 - Sprite Size(0 = 8x8, 1 = 8x16)
        bool sprites_enabled;    // Bit 1 - Sprites Enabled (0 = Disabled, 1 = Enabled)
        bool bg_enabled;         // Bit 0 - BG Enabled(in DMG) (0 = Disabled, 1 = Enabled)
        void update(PPU *ppu);
    } lcd_control{};

    enum class LCD_State {
        lyc = 6,                // Bit 6 - LYC = LY Coincidence Interrupt (1 = Enable) (Read / Write)
        oam_interrupt = 5,      // Bit 5 - Mode 2 OAM Interrupt (1 = Enable) (Read / Write)
        v_blank_interrupt = 4,  // Bit 4 - Mode 1 V - Blank Interrupt (1 = Enable) (Read / Write)
        h_blank_interrupt = 3,  // Bit 3 - Mode 0 H - Blank Interrupt (1 = Enable) (Read / Write)
        coincidence_flag = 2,   // Bit 2 - Coincidence Flag (0:LYC<>LY, 1 : LYC = LY) (Read Only)
        mode_flag_1 = 1,        // Bit 1 - 0 - Mode Flag (Mode 0 - 3, see below) (Read Only)
        mode_flag_0 = 0         // 0: During H - Blank
        // 1 : During V - Blank
        // 2 : During Searching OAM
        // 3 : During Transferring Data to LCD Driver
    };

    void set_lcd_mode(LCD_Mode mode);

    void load_lcd_mode();

    void set_lcd_state(const LCD_State &s);

    void reset_lcd_state(const LCD_State &s);

    bool get_lcd_state(const LCD_State &s) const;

    // LCD Render Funktionen
    std::vector<Pixel> lcd_buffer;
    std::vector<std::vector<Pixel>> tile_buffer;
public:
    std::vector<std::vector<Pixel>> &get_tile_buffer() { return tile_buffer; }

    Pixel const *get_lcd_buffer() const;

    LCD_Mode mode() const;

// Paletten
    std::vector<std::vector<Pixel>> Palettes = std::vector<std::vector<Pixel >>{
            {{0xE0, 0xF8, 0xD0},
                    {0x88, 0xC0, 0x70},
                    {0x34, 0x68, 0x56},
                    {0x08, 0x18, 0x20}},  // classic grün
            {{175,  203,  70},
                    {121,  170,  109},
                    {34,   111,  95},
                    {8,    41,   85}},  // Forum grün
            {{0xDF, 0xDF, 0xDF},
                    {0xA9, 0xA9, 0xA9},
                    {0x54, 0x54, 0x54},
                    {0x00, 0x0,  0x0}},  // schwarz weiss
            {{255,  255,  255},
                    {192,  192,  192},
                    {96,   96,   96},
                    {0x0,  0x0,  0x0}},  // schwarz weiss
    };
private:

    void buffer_line(const u8 line);


    int current_palette = 0;

private:
    // Daten zum FPS berechnen
    Uint32 fps_last_time = SDL_GetTicks();  // the last recorded time.
    Uint32 fps_current{};                   // the current FPS.
    Uint32 fps_frames = 0;  // frames passed since the last recorded fps.
    char fps_text[8]{};

};

#endif  // !PPU_H_GUARD
