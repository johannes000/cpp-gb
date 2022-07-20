#ifndef APU_H
#define APU_H

#pragma once

class APU {
public:
	APU(MEMORY *m);

	u8 read(const u16 &addr);

	void write(const u16 &addr, const u8 &data) const;

	void write_u16(const u16 &addr, const u16 &data) const;

	void apu_view();

private:
	MEMORY *mem;

private:
	// REGISTER
	union Channel_1_Sweep_register {
		u8 value;
		struct {
			u8 n_sweep_shift_0 : 1; // Bit 2-0 - Number of sweep shift (n: 0-7)
			u8 n_sweep_shift_1 : 1; //
			u8 n_sweep_shift_2 : 1; //
			u8 sweep_inc_dec : 1;	// Bit 3 - Sweep Increase/Decrease
									// 0: Addition    (frequency increases)
									// 1: Subtraction (frequency decreases)
			u8 sweep_time_0 : 1;	// Bit 6-4 - Sweep Time
			u8 sweep_time_1 : 1;	//
			u8 sweep_time_2 : 1;	//
			u8 sweep_time_3 : 1;	//
		};
	} chan_1_sweep_register{};
};

#endif