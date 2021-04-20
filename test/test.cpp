#include "gameboy.h"
#include "gtest/gtest.h"
#include <string>

GTEST_TEST(CPUTest, 01_special) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/01-special.gb", true);
	gb.run_for_x_cycles(2500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "01-special\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

GTEST_TEST(CPUTest, 02_interrupts) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/02-interrupts.gb", true);
	gb.run_for_x_cycles(1000000);
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
	gb.run_for_x_cycles(2500000);
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
	gb.run_for_x_cycles(15500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "11-op a,(hl)\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}
GTEST_TEST(Timing, instr_timing) {
	gameboy gb;
	gb.load_rom("C:/Users/knaub/source/repos/testroms/instr_timing/instr_timing.gb", true);
	gb.run_for_x_cycles(15500000);
	std::string result = gb.mem.TestOutput;
	std::string expected_result = "instr_timing\n\n\nPassed\n";

	EXPECT_EQ(expected_result, result);
}

int main() {
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
