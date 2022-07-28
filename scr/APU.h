#ifndef APU_H
#define APU_H

#pragma once

#include <array>
class APU {
public:
	APU(MEMORY *m);

	u8 read(const u16 &addr);

	void write(const u16 &addr, const u8 &data);

	void imgui_view();
	void update(i32 Sample_Count);
	i16 *get_output_sample(i32 Sample_Count);

private:
	MEMORY *mem;

	f32 t_sin;
	i32 sin_wave_period;
	std::vector<i16> channel_1_sample;
	std::vector<i16> channel_2_sample;
	std::vector<i16> channel_3_sample;
	std::vector<i16> channel_4_sample;
	void clear_channel_samples();

	i16 output_sample[48000 * 2];
	bool sound_on;

	void reset();

private:
	// REGISTER
	u8 nr10;
	u8 nr11;
	u8 nr12;
	u8 nr13;
	u8 nr14;
	u8 nr21;
	u8 nr22;
	u8 nr23;
	u8 nr24;
	u8 nr30;
	u8 nr31;
	u8 nr32;
	u8 nr33;
	u8 nr34;
	u8 nr41;
	u8 nr42;
	u8 nr43;
	u8 nr44;
	u8 nr50;
	u8 nr51;
	u8 nr52;

	//  Sound Channel 1 - Tone & Sweep
	struct Channel_1 {
		bool on;
		f32 sweep_time; // Sweep Time
		// 000	sweep off - no freq change
		// 001	7.8 ms (1/128Hz)
		// 010	15.6 ms (2/128Hz)
		// 011	23.4 ms (3/128Hz)
		// 100	31.3 ms (4/128Hz)
		// 101	39.1 ms (5/128Hz)
		// 110	46.9 ms (6/128Hz)
		// 111 54.7 ms(7 / 128Hz)
		bool sweep_decrease;	// Sweep Increase/Decrease
								//  0: Addition    (frequency increases)
								//  1: Subtraction (frequency decreases)
		i32 nr_of_sweep_shifts; // Number of sweep shift (n: 0-7)

		i32 wave_pattern_duty; // Wave Pattern Duty (Read/Write)
							   // %00	12.5% (_-------_-------_-------)
							   // %01	25% (__------__------__------)
							   // %10	50% (____----____----____----) (normal)
							   // %11	75% (______--______--______--)

		f32 sound_length_data; // Sound length data (Write Only) (t1: 0-63)
							   // Sound Length = (64-t1)(1/256) seconds.
							   // The Length value is used only if Bit 6 in NR14 is set.(Counter/consecutive selection)

		i32 envelope_init_volume;  // Initial Volume of envelope (0-0Fh) (0=No Sound)
		bool envelope_increase;	   // Envelope Direction (0=Decrease, 1=Increase)
		i32 nr_of_envelope_sweeps; // Number of envelope sweep (n: 0-7)
								   // (If zero, stop envelope operation.)

		i32 initial_frequency;	// Initial (1=Restart Sound)     (Write Only)
		bool counter_selection; // Counter/consecutive selection (Read/Write)
								// (1 = Stop output when length in NR11 expires)
		i32 frequency;			// Frequency (Write Only)
	} channel_1{};

	//  Sound Channel 2 - Tone
	// This sound channel works exactly as channel 1,
	// except that it doesn’t have a Tone Envelope/Sweep Register.
	struct Channel_2 {
		bool on;

		i32 wave_pattern_duty; // Wave Pattern Duty (Read/Write)
							   // %00	12.5% (_-------_-------_-------)
							   // %01	25% (__------__------__------)
							   // %10	50% (____----____----____----) (normal)
							   // %11	75% (______--______--______--)

		f32 sound_length_data; // Sound length data (Write Only) (t1: 0-63)
							   // Sound Length = (64-t1)*(1/256) seconds.
							   // The Length value is used only if Bit 6 in NR14 is set.(Counter/consecutive selection)

		i32 envelope_init_volume;  // Initial Volume of envelope (0-0Fh) (0=No Sound)
		bool envelope_increase;	   // Envelope Direction (0=Decrease, 1=Increase)
		i32 nr_of_envelope_sweeps; // Number of envelope sweep (n: 0-7)
								   // (If zero, stop envelope operation.)

		i32 initial_frequency;	// Initial (1=Restart Sound)     (Write Only)
		bool counter_selection; // Counter/consecutive selection (Read/Write)
								// (1 = Stop output when length in NR11 expires)
		i32 frequency;			// Frequency (Write Only)
	} channel_2{};

	// Sound Channel 3 - Wave Output
	// This channel can be used to output digital sound,
	// the length of the sample buffer(Wave RAM) is limited to 32 digits.
	// This sound channel can be also used to output normal tones when initializing the Wave RAM
	// by a square wave.This channel doesn’t have a volume envelope register.
	struct Channel_3 {
		bool on;		  // Sound Channel 3 Off  (0=Stop, 1=Playback)  (Read/Write)
		i32 sound_length; // Sound length (Write only) (t1: 0 - 255)
						  // Sound Length = (256-t1)*(1/256) seconds Sound Length = (256-t1)*(1/256) seconds
						  // This value is used only if Bit 6 in NR34 is set.This value is used only if Bit 6 in NR34 is set.
		i32 output_level; // Select output level (Read/Write)
						  // %00	Mute (No sound)
						  // %01	100% volume (Produce Wave Pattern RAM Data as it is)
						  // %10	50% volume (Produce Wave Pattern RAM data shifted once to the right)
						  // %11	25% volume (Produce Wave Pattern RAM data shifted twice to the right)

		i32 initial_frequency;	// Initial (1=Restart Sound)     (Write Only)
		bool counter_selection; // Counter/consecutive selection (Read/Write)
								// (1 = Stop output when length in NR11 expires)
		i32 frequency;			// Frequency (Write Only)

		// This storage area holds 32 4-bit samples that are played back, upper 4 bits first.
		// Wave RAM should only be accessed while CH3 is disabled (NR30 bit 7 reset),
		// otherwise accesses will behave weirdly.
		// On almost all models, the byte will be written at the offset CH3 is currently reading.
		// On GBA, the write will simply be ignored.
		i32 wave_pattern_ram[sizeof(i8) * 32];
		i32 plot_pattern[sizeof(i8) * 32];
	} channel_3{};
	static f32 channel_3_imgui_plot_getter(void *, i32);

	// Sound Channel 4 - Noise
	// This channel is used to output white noise.
	// This is done by randomly switching the amplitude between high and low at a given frequency.
	// Depending on the frequency the noise will appear “harder” or “softer”.
	// so the that the output becomes more regular,
	// resulting in a limited ability to output Tone instead of Noise.
	struct Channel_4 {
		bool on;

		i32 sound_length; // Sound length (Write only) (t1: 0 - 255)
						  // Sound Length = (256-t1)*(1/256) seconds Sound Length = (256-t1)*(1/256) seconds
						  // This value is used only if Bit 6 in NR34 is set.This value is used only if Bit 6 in NR34 is set.

		i32 envelope_init_volume;  // Initial Volume of envelope (0-0Fh) (0=No Sound)
		bool envelope_increase;	   // Envelope Direction (0=Decrease, 1=Increase)
		i32 nr_of_envelope_sweeps; // Number of envelope sweep (n: 0-7)
								   // (If zero, stop envelope operation.)

		// Polynomial Counter (R/W)
		// The amplitude is randomly switched between high and low at the given frequency.
		// A higher frequency will make the noise to appear “softer”.
		// When Bit 3 is set, the output will become more regular,
		// and some frequencies will sound more like Tone than Noise.
		i32 shift_clock_freq;		// Shift Clock Frequency (s)
		i32 counter_step_with;		// Counter Step/Width (15 bits, 7 bits)
		i32 dividing_ratio_of_freq; // Dividing Ratio of Frequencies (r)
									// Frequency = 524288 Hz / r / 2^(s+1) ;For r=0 assume r=0.5 instead
		bool restart;				// Initial (1=Restart Sound)     (Write Only)
		bool counter_selection;		// Counter/consecutive selection (Read/Write)
									// (1 = Stop output when length(sound_length) in NR41 expires)
	} channel_4{};
};

#endif