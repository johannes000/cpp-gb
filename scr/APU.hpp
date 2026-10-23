#ifndef APU_H
#define APU_H

#pragma once

#include "MEMORY.hpp"
#include "utl/type.hpp"

#include <array>

class MEMORY;

class APU {
public:
	APU(MEMORY *m);

	auto read(const u16 &addr) -> u8;

	auto write(const u16 &addr, const u8 &data) -> void;

	auto imgui_view() -> void;
	auto update_tests(i32 Sample_Count) -> void;
	auto update(i32 cycles) -> void;
	auto get_output_sample(i32 Sample_Count) -> i16 *;
	i32 apu_sample_count_;

private:
	MEMORY *mem_;

	auto channel_2_soundeffect() -> void;
	bool play_effect_;

	i32 apu_volume_;
	f32 t_sin_;
	i32 hz_;
	i32 sin_wave_period_;

	const i32 sample_buffer_size_ = 48000 * 2;

	i16 *output_sample_;
	i16 *channel_1_sample_;
	i16 *channel_2_sample_;
	i16 *channel_3_sample_;
	i16 *channel_4_sample_;

	u32 channel_1_running_sample_index_;
	u32 channel_2_running_sample_index_;
	u32 channel_3_running_sample_index_;
	u32 channel_4_running_sample_index_;

	bool imgui_channel_1_on_;
	bool imgui_channel_2_on_;
	bool imgui_channel_3_on_;
	bool imgui_channel_4_on_;

	auto trigger_channel_1() -> void;
	auto trigger_channel_2() -> void;
	auto trigger_channel_3() -> void;
	auto trigger_channel_4() -> void;

	auto clear_channel_samples() -> void;
	auto calc_freq_timer(i32 frequency) -> i32;

	std::vector<std::vector<f32>> wave_duty_table_;

	bool sound_on_;
	bool sinus_test_;

	auto reset() -> void;

private:
	// REGISTER
	u8 nr10_;
	u8 nr11_;
	u8 nr12_;
	u8 nr13_;
	u8 nr14_;
	u8 nr21_;
	u8 nr22_;
	u8 nr23_;
	u8 nr24_;
	u8 nr30_;
	u8 nr31_;
	u8 nr32_;
	u8 nr33_;
	u8 nr34_;
	u8 nr41_;
	u8 nr42_;
	u8 nr43_;
	u8 nr44_;
	u8 nr50_;
	u8 nr51_;
	u8 nr52_;

	//  Sound Channel 1 - Tone & Sweep
	struct Channel_1 {
		bool on_;
		f32 sweep_time_; // Sweep Time
		// 000	sweep off - no freq change
		// 001	7.8 ms (1/128Hz)
		// 010	15.6 ms (2/128Hz)
		// 011	23.4 ms (3/128Hz)
		// 100	31.3 ms (4/128Hz)
		// 101	39.1 ms (5/128Hz)
		// 110	46.9 ms (6/128Hz)
		// 111 54.7 ms(7 / 128Hz)
		bool sweep_decrease_;	 // Sweep Increase/Decrease
								 //  0: Addition    (frequency increases)
								 //  1: Subtraction (frequency decreases)
		i32 nr_of_sweep_shifts_; // Number of sweep shift (n: 0-7)

		i32 wave_pattern_duty_; // Wave Pattern Duty (Read/Write)
								// %00	12.5% (_-------_-------_-------)
								// %01	25% (__------__------__------)
								// %10	50% (____----____----____----) (normal)
								// %11	75% (______--______--______--)

		i32 sound_length_timer_;
		i32 sound_length_data_; // Sound length data (Write Only) (t1: 0-63)
								// Sound Length = (64-t1)(1/256) seconds.
								// The Length value is used only if Bit 6 in NR14 is set.(Counter/consecutive selection)

		i32 envelope_init_volume_; // Initial Volume of envelope (0-0Fh) (0=No Sound)
		bool envelope_increase_;   // Envelope Direction (0=Decrease, 1=Increase)
		i32 envelope_period_;	   // Number of envelope sweep (n: 0-7)
								   // (If zero, stop envelope operation.)
		i32 volume_;

		i32 current_frequency_;
		i32 initial_frequency_;	 // Initial (1=Restart Sound)     (Write Only)
		bool counter_selection_; // Counter/consecutive selection (Read/Write)
								 // (1 = Stop output when length in NR11 expires)
		i32 frequency_;			 // Frequency (Write Only)
	} channel_1_{};

	//  Sound Channel 2 - Tone
	// This sound channel works exactly as channel 1,
	// except that it doesn’t have a Tone Envelope/Sweep Register.
	struct Channel_2 {
		bool on_;

		i32 wave_duty_pattern_; // Wave Pattern Duty (Read/Write)
								// %00	12.5% (_-------_-------_-------)
								// %01	25% (__------__------__------)
								// %10	50% (____----____----____----) (normal)
								// %11	75% (______--______--______--)
		i32 wave_duty_position_;

		i32 sound_length_timer_;
		i32 sound_length_data_; // Sound length data (Write Only) (t1: 0-63)
								// Sound Length = (64-t1)*(1/256) seconds.
								// The Length value is used only if Bit 6 in NR14 is set.(Counter/consecutive selection)

		i32 envelope_init_volume_; // Initial Volume of envelope (0-0Fh) (0=No Sound)
		bool envelope_increase_;   // Envelope Direction (0=Decrease, 1=Increase)
		i32 envelope_period_;	   // Number of envelope sweep (n: 0-7)
								   // (If zero, stop envelope operation.)
		i32 envelope_period_timer_;
		i32 volume_;

		i32 current_frequency_;
		i32 initial_frequency_;	 // Initial (1=Restart Sound)     (Write Only)
		bool counter_selection_; // Counter/consecutive selection (Read/Write)
								 // (1 = Stop output when length in NR11 expires)
		i32 frequency_;			 // Frequency (Write Only)
		i32 frequency_timer_;
	} channel_2_{};
	// Sound Channel 3 - Wave Output
	// This channel can be used to output digital sound,
	// the length of the sample buffer(Wave RAM) is limited to 32 digits.
	// This sound channel can be also used to output normal tones when initializing the Wave RAM
	// by a square wave.This channel doesn’t have a volume envelope register.
	struct Channel_3 {
		bool on_; // Sound Channel 3 Off  (0=Stop, 1=Playback)  (Read/Write)
		i32 sound_length_timer_;
		i32 sound_length_data_; // Sound length (Write only) (t1: 0 - 255)
								// Sound Length = (256-t1)*(1/256) seconds Sound Length = (256-t1)*(1/256) seconds
								// This value is used only if Bit 6 in NR34 is set.This value is used only if Bit 6 in NR34 is set.
		i32 output_level_;		// Select output level (Read/Write)
								// %00	Mute (No sound)
								// %01	100% volume (Produce Wave Pattern RAM Data as it is)
								// %10	50% volume (Produce Wave Pattern RAM data shifted once to the right)
								// %11	25% volume (Produce Wave Pattern RAM data shifted twice to the right)

		i32 current_frequency_;
		i32 initial_frequency_;	 // Initial (1=Restart Sound)     (Write Only)
		bool counter_selection_; // Counter/consecutive selection (Read/Write)
								 // (1 = Stop output when length in NR11 expires)
		i32 frequency_;			 // Frequency (Write Only)

		// This storage area holds 32 4-bit samples that are played back, upper 4 bits first.
		// Wave RAM should only be accessed while CH3 is disabled (NR30 bit 7 reset),
		// otherwise accesses will behave weirdly.
		// On almost all models, the byte will be written at the offset CH3 is currently reading.
		// On GBA, the write will simply be ignored.
		i32 wave_pattern_ram_[sizeof(i8) * 32];
		i32 plot_pattern_[sizeof(i8) * 32];
	} channel_3_{};
	static auto channel_3_imgui_plot_getter(void *, i32) -> f32;

	// Sound Channel 4 - Noise
	// This channel is used to output white noise.
	// This is done by randomly switching the amplitude between high and low at a given frequency.
	// Depending on the frequency the noise will appear “harder” or “softer”.
	// so the that the output becomes more regular,
	// resulting in a limited ability to output Tone instead of Noise.
	struct Channel_4 {
		bool on_;

		i32 sound_length_timer_;
		i32 sound_length_data_; // Sound length (Write only) (t1: 0 - 255)
								// Sound Length = (256-t1)*(1/256) seconds Sound Length = (256-t1)*(1/256) seconds
								// This value is used only if Bit 6 in NR34 is set.This value is used only if Bit 6 in NR34 is set.

		i32 envelope_init_volume_; // Initial Volume of envelope (0-0Fh) (0=No Sound)
		bool envelope_increase_;   // Envelope Direction (0=Decrease, 1=Increase)
		i32 envelope_period_;	   // Number of envelope sweep (n: 0-7)
								   // (If zero, stop envelope operation.)
		i32 volume_;

		// Polynomial Counter (R/W)
		// The amplitude is randomly switched between high and low at the given frequency.
		// A higher frequency will make the noise to appear “softer”.
		// When Bit 3 is set, the output will become more regular,
		// and some frequencies will sound more like Tone than Noise.
		i32 shift_clock_freq_;		 // Shift Clock Frequency (s)
		i32 counter_step_with_;		 // Counter Step/Width (15 bits, 7 bits)
		i32 dividing_ratio_of_freq_; // Dividing Ratio of Frequencies (r)
									 // Frequency = 524288 Hz / r / 2^(s+1) ;For r=0 assume r=0.5 instead
		bool restart_;				 // Initial (1=Restart Sound)     (Write Only)
		bool counter_selection_;	 // Counter/consecutive selection (Read/Write)
									 // (1 = Stop output when length(sound_length) in NR41 expires)
	} channel_4_{};
};

#endif