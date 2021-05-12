#include "gameboy.h"
#include "gtest/gtest.h"
#include <string>

GTEST_TEST(CPUTest, 01_special) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/01-special.gb", true);
	gb.run_for_x_cycles(2500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "01-special\n\n\nPassed\n";

	EXPECT_EQ(result, expected_result);
}

GTEST_TEST(CPUTest, 02_interrupts) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/02-interrupts.gb", true);
	gb.run_for_x_cycles(20000000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "02-interrupts\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 03_op_sp_hl) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/03-op sp,hl.gb", true);
	gb.run_for_x_cycles(2500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "03-op sp,hl\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 04_op_r_imm) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/04-op r,imm.gb", true);
	gb.run_for_x_cycles(5500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "04-op r,imm\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 05_op_rp) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/05-op rp.gb", true);
	gb.run_for_x_cycles(5500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "05-op rp\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 06_ld_r_r) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/06-ld r,r.gb", true);
	gb.run_for_x_cycles(5500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "06-ld r,r\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 07_jr_jp_call_ret_rst) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb", true);
	gb.run_for_x_cycles(2500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "07-jr,jp,call,ret,rst\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 08_misc_instr) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/08-misc instrs.gb", true);
	gb.run_for_x_cycles(2500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "08-misc instrs\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 09_op_r_r) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/09-op r,r.gb", true);
	gb.run_for_x_cycles(15500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "09-op r,r\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 10_bit_ops) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/10-bit ops.gb", true);
	gb.run_for_x_cycles(15500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "10-bit ops\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(CPUTest, 11_op_a_hl) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/11-op a,(hl).gb", true);
	gb.run_for_x_cycles(50000000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "11-op a,(hl)\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(Timing, instr_timing) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/instr_timing/instr_timing.gb", true);
	gb.run_for_x_cycles(5500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "instr_timing\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(MooneyeMBC1, bits_bank1) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/bits_bank1.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, bits_bank2) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/bits_bank2.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, bits_mode) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/bits_mode.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, bits_ramg) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/bits_ramg.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, ram_64kb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/ram_64kb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, ram_256kb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/ram_256kb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, rom_1Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_1Mb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, rom_2Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_2Mb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, rom_4Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_4Mb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, rom_8Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_8Mb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, rom_16Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_16Mb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC1, rom_512kb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc1/rom_512kb.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeBitAcceptance, mem_oam) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/bits/mem_oam.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeBitAcceptance, reg_f) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/bits/reg_f.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeBitAcceptance, unused_hwioGS) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/bits/unused_hwio-GS.gb", true);

	gb.run_for_x_cycles(5500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeDAA, daa) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/instr/daa.gb", true);
	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeOAM_dma, DISABLED_basic) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/oam_dma/basic.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeOAM_dma, DISABLED_reg_read) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/oam_dma/reg_read.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeOAM_dma, DISABLED_sourcesGS) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/acceptance/oam_dma/sources-GS.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_bits_ramg) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/bits_ramg.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_bits_romb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/bits_romb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_bits_unused) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/bits_unused.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_ram) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/ram.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_rom_1Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/rom_1Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_rom_2Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/rom_2Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC2, DISABLED_rom_512kb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc2/rom_512kb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_1Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_1Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_2Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_2Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_4Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_4Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_8Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_8Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_16Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_16Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_32Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_32Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_64Mb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_64Mb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(MooneyeMBC5, rom_512kb) {
	gameboy gb;
	gb.in_test_mode = true;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/mooneye/emulator-only/mbc5/rom_512kb.gb", true);

	gb.run_for_x_cycles(15500000);

	auto R = gb.cpu.get_register();
	bool Result = R.B == 3 && R.C == 5 && R.D == 8 && R.E == 13 && R.H == 21 && R.L == 34;
	EXPECT_TRUE(Result);
}

GTEST_TEST(APUTest, 01_registers) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/01-registers.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "01-registers\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 02_len_ctr) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/02-len ctr.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "02-len ctr\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 03_trigger) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/03-trigger.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "03-trigger\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 04_sweep) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/04-sweep.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "04-sweep\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 05_sweep_details) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/05-sweep details.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "05-sweep details\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 06_overflow_on_trigger) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/06-overflow on trigger.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "06-overflow on trigger\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 07_len_sweep_period_sync) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/07-len sweep period sync.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "07-len sweep period sync\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 08_len_ctr_during_power) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/08-len ctr during power.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "08-len ctr during power\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 09_wave_read_while_on) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/09-wave read while on.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "09-wave read while on\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 10_wave_trigger_while_on) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/10-wave trigger while on.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "10-wave trigger while on\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 11_regs_after_power) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/11-regs after power.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "11-regs after power\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(APUTest, 12_wave_write_while_on) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/dmg_sound/rom_singles/12-wave write while on.gb", true);
	gb.run_for_x_cycles(55500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "12-wave write while on\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
