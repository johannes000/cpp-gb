#ifndef TYPE_H_GUARD
#define TYPE_H_GUARD

typedef unsigned char u8;
typedef unsigned short u16;

typedef signed char i8;
typedef signed short i16;

typedef unsigned char Register;

// ADDRESSES
constexpr u16 INTERRUPT_FLAGS_REG = 0xFF0F;
constexpr u16 INTERRUPT_ENABLE_REG = 0xFFFF;

constexpr u16 CARD_ROM = 0x0000;
constexpr u16 CARD_ROM_0 = 0x0000;
constexpr u16 BOOT_ROM_END = 0x00FF;
constexpr u16 CARD_ROM_0_END = 0x3FFF;
constexpr u16 CARD_ROM_END = 0x7FFF;
constexpr u16 BIOS = 0x0000;
constexpr u16 BIOS_END = 0x00FF;
constexpr u16 CARD_HEADER = 0x0100;
constexpr u16 CARD_BANKS = 0x4000;
constexpr u16 CARD_BANKS_END = 0x7FFF;
constexpr u16 GRAPHICS_RAM = 0x8000;
constexpr u16 GRAPHICS_RAM_END = 0x9FFF;
constexpr u16 CARD_EXTERNAL_RAM = 0xA000;
constexpr u16 CARD_EXTERNAL_RAM_END = 0xBFFF;
constexpr u16 WORKING_RAM = 0xC000;
constexpr u16 WORKING_RAM_END = 0xDFFF;
constexpr u16 WORKING_RAM_SHADOW = 0xE000;
constexpr u16 WORKING_RAM_SHADOW_END = 0xFDFF;
constexpr u16 WORKING_RAM_SHADOW_OFFSET = 0x2000;
constexpr u16 SPRITE_INFORMATION = 0xFE00;
constexpr u16 SPRITE_INFORMATION_END = 0xFE9F;
constexpr u16 MEMORY_MAPPED_IO = 0xFF00;
constexpr u16 MEMORY_MAPPED_IO_END = 0xFF7F;
constexpr u16 ZERO_PAGE_RAM = 0xFF80;
constexpr u16 ZERO_PAGE_RAM_END = 0xFFFE;
constexpr u16 IN_BOOT_ADDRESS = 0xFF50;

// LCD
constexpr int GB_LCD_SIZE_WIDTH = 160;
constexpr int GB_LCD_SIZE_HEIGHT = 144;

constexpr unsigned int TILE_WIDTH = 8;
constexpr unsigned int TILE_HEIGHT = 8;
constexpr unsigned int TILE_PER_LINE = 32;
constexpr unsigned int BG_MAP_SIZE = 256;
constexpr unsigned int TILE_BUFFER_WIDTH = 128;
constexpr unsigned int TILE_BUFFER_HEIGHT = 192;


// LCD Register Adressen
constexpr u16 LCD_CONTROL = 0xFF40;
constexpr u16 LCD_STATUS = 0xFF41;

constexpr u16 LCD_SCY = 0xFF42;  // SCY - Scroll Y (R/W)
constexpr u16 LCD_SCX = 0xFF43;  // SCX - Scroll X (R/W)
constexpr u16 LCD_LY = 0xFF44;   // LY - LCDC Y-Coordinate (R)
constexpr u16 LCD_LYC = 0xFF45;  // LYC - LY Compare (R/W)
constexpr u16 LCD_WY = 0xFF4A;   // WY - Window Y Position (R/W)
constexpr u16 LCD_WX = 0xFF4B;   // WX - Window X Position minus 7 (R/W)

constexpr u16 BG_PALETTE_DATA =
        0xFF47;  // BGP - BG Palette Data (R/W) - Non CGB Mode Only
constexpr u16 BG_PALETTE_DATA_0 =
        0xFF48;  // OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only
constexpr u16 BG_PALETTE_DATA_1 =
        0xFF49;  // OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only

// JOYPAD REGISTER
constexpr u16 JOY_P1_REGISTER = 0xFF00;


constexpr u16
        DMA_TRANSFER = 0xFF46;  // DMA Transfer and Start Address (R/W)

constexpr int VERTICAL_REFRESH_CYCLES = 70224;

enum class Flag {
    z = 7,  // Zero Flag
    n = 6,  // Add/Sub-Flag
    h = 5,  // Half Carry Flag
    c = 4   // Carry Flag
};

struct CPU_Register {
    struct {
        union {
            struct {
                u8 F;
                u8 A;
            };
            u16 AF;
        };
    };

    struct {
        union {
            struct {
                u8 C;
                u8 B;
            };
            u16 BC;
        };
    };

    struct {
        union {
            struct {
                u8 E;
                u8 D;
            };
            u16 DE;
        };
    };

    struct {
        union {
            struct {
                u8 L;
                u8 H;
            };
            u16 HL;
        };
    };

    u16 SP;
    u16 PC;
};

// ROM INFO
enum class CGBFlag {
    OFF  [[maybe_unused]] = 0x80,  // - Game supports CGB functions, but works on old gameboys also.
    ON  [[maybe_unused]] = 0xC0,   // - Game works on CGB only(physically the same as 80h).
};

enum class CType {  // Cartridge Type
    ROM_ONLY [[maybe_unused]] = 0x00,
    MBC1  [[maybe_unused]] = 0x01,
    MBC1_RAM [[maybe_unused]] = 0x02,
    MBC1_RAM_BATTERY [[maybe_unused]] = 0x03,
    MBC2  [[maybe_unused]] = 0x05,
    MBC2_BATTERY  [[maybe_unused]] = 0x06,
    ROM_RAM  [[maybe_unused]] = 0x08,
    ROM_RAM_BATTERY  [[maybe_unused]] = 0x09,
    MMM01  [[maybe_unused]] = 0x0B,
    MMM01_RAM  [[maybe_unused]] = 0x0C,
    MMM01_RAM_BATTERY  [[maybe_unused]] = 0x0D,
    MBC3_TIMER_BATTERY  [[maybe_unused]] = 0x0F,
    MBC3_TIMER_RAM_BATTERY  [[maybe_unused]] = 0x10,
    MBC3  [[maybe_unused]] = 0x11,
    MBC3_RAM  [[maybe_unused]] = 0x12,
    MBC3_RAM_BATTERY  [[maybe_unused]] = 0x13,
    MBC5  [[maybe_unused]] = 0x19,
    MBC5_RAM  [[maybe_unused]] = 0x1A,
    MBC5_RAM_BATTERY  [[maybe_unused]] = 0x1B,
    MBC5_RUMBLE  [[maybe_unused]] = 0x1C,
    MBC5_RUMBLE_RAM  [[maybe_unused]] = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY  [[maybe_unused]] = 0x1E,
    MBC6  [[maybe_unused]] = 0x20,
    MBC7_SENSOR_RUMBLE_RAM_BATTERY  [[maybe_unused]] = 022,
    POCKET_CAMERA  [[maybe_unused]] = 0xFC,
    BANDAI_TAMA5  [[maybe_unused]] = 0xFD,
    HuC3  [[maybe_unused]] = 0xFE,
    HUC1_RAM_BATTERY  [[maybe_unused]] = 0xFF

};

enum class ROMSize {
    KB32 = 0x00,   //(no ROM banking)
    KB64 = 0x01,   //(4 banks)
    KB128 = 0x02,  //(8 banks)
    KB256 = 0x03,  //(16 banks)
    KB512 = 0x04,  //(32 banks)
    MB1 = 0x05,    //(64 banks) - only 63 banks used by MBC1
    MB2 = 0x06,    //(128 banks) - only 125 banks used by MBC1
    MB4 = 0x07,    //(256 banks)
    MB8 = 0x08,    //(512 banks)
    MB1_1 = 0x52,  //(72 banks)
    MB1_2 = 0x53,  //(80 banks)
    MB1_5 = 0x54,  //(96 banks)
};
enum class Debug_State {
    STEP,
    RUN
};
enum class RAMSize {
    None = 0x00,
    KB2 = 0x01,
    KB8 = 0x02,
    KB32 = 0x03,   //(4 banks of 8KBytes each)
    KB128 = 0x04,  //(16 banks of 8KBytes each)
    KB64 = 0x05,   // (8 banks of 8KBytes each)
};

enum class DestCode {
    JAP  [[maybe_unused]] = 0x00,
    NON_JAP  [[maybe_unused]] = 0x01,
};

struct Rom_Info {
    char title[15] = "";
    bool SGBFlag = false;

    CGBFlag cgb = CGBFlag::ON;
    CType ctype = CType::MBC1;

    ROMSize rom_size = ROMSize::KB32;
    RAMSize ram_size = RAMSize::None;

    int rom_banks{};
    int ram_banks{};
};

struct debug_instruction {
    u16 addr{};
    const char *mnemonic{};

    u8 bytes{};
    u8 cycles{};

    u16 intermediate{};
};

struct Pixel {
    // SDL_PIXELFORMAT_RGB24
    // Stored in memory as the bytes R, G, B
    // (R at the lowest address, B at the highest address).
    u8 R;
    u8 G;
    u8 B;
};

// 0: During H - Blank
// 1 : During V - Blank
// 2 : OAM Access
// 3 : VRAM Access
enum class LCD_Mode {
    H_Blank,
    V_Blank,
    OAM,
    VRAM,
};

// Bit 7 - Not used
// Bit 6 - Not used
// Bit 5 - P15 Select Button Keys      (0=Select)
// Bit 4 - P14 Select Direction Keys   (0=Select)
// Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
// Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
// Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
// Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
enum class GB_Key {

    Right = 0,
    Left = 1,
    Up = 2,
    Down = 3,
    A = 4,
    B = 5,
    Select = 6,
    Start = 7,

};

void set_bit_if(u8 &value, const u8 &bit, bool condition);

void set_bit_if(u16 &value, const u16 &bit, bool condition);

void set_bit(u8 &value, const u8 &bit);

void set_bit(u16 &value, const u16 &bit);

void reset_bit(u8 &value, const u8 &bit);

void reset_bit(u16 &value, const u16 &bit);

void toggle_bit(u8 &value, const u8 &bit);

void toggle_bit(u16 &value, const u16 &bit);

bool check_bit(const u8 &value, const u8 &bit);

bool check_bit(const u16 &value, const u16 &bit);

#endif  // !TYPE_H_GUARD
