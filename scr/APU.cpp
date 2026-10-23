#include "APU.hpp"

#include "imgui.h"
#include <cmath>

APU::APU(MEMORY *m) : mem_(m) {
	nr10_ = 0;
	nr11_ = 0;
	nr12_ = 0;
	nr13_ = 0;
	nr14_ = 0;
	nr21_ = 0;
	nr22_ = 0;
	nr23_ = 0;
	nr24_ = 0;
	nr30_ = 0;
	nr31_ = 0;
	nr32_ = 0;
	nr33_ = 0;
	nr34_ = 0;
	nr41_ = 0;
	nr42_ = 0;
	nr43_ = 0;
	nr44_ = 0;
	nr50_ = 0;
	nr51_ = 0;
	nr52_ = 0;

	sound_on_ = false;
	play_effect_ = false;
	sinus_test_ = false;

	apu_volume_ = 10;

	hz_ = 264;
	sin_wave_period_ = 48000 / hz_;
	t_sin_ = 0;

	imgui_channel_1_on_ = true;
	imgui_channel_2_on_ = true;
	imgui_channel_3_on_ = true;
	imgui_channel_4_on_ = true;

	channel_1_ = {};
	channel_2_ = {};
	channel_3_ = {};
	channel_4_ = {};

	channel_1_sample_ = new i16[sample_buffer_size_];
	channel_2_sample_ = new i16[sample_buffer_size_];
	channel_3_sample_ = new i16[sample_buffer_size_];
	channel_4_sample_ = new i16[sample_buffer_size_];

	channel_1_running_sample_index_ = 0;
	channel_2_running_sample_index_ = 0;
	channel_3_running_sample_index_ = 0;
	channel_4_running_sample_index_ = 0;

	apu_sample_count_ = 0;

	output_sample_ = new i16[sample_buffer_size_];

	wave_duty_table_ = {
		{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f},
		{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f},
		{0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f},
		{1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f}};
	clear_channel_samples();
}

auto APU::read(const u16 &addr) -> u8 {
	if (addr >= WAVE_TABLE_START && addr <= WAVE_TABLE_END)
		return channel_3_.wave_pattern_ram_[addr - WAVE_TABLE_START];

	switch (addr) {
		case NR13:
		case NR23:
		case NR31:
		case NR41:
			spdlog::error("AUDIO: Register ist write only wird aber gelesen: 0x{:04X}", addr);
			return 0;
		case NR10:
			return nr10_;
		case NR11:
			return nr11_;
		case NR12:
			return nr12_;
		case NR14:
			return nr14_;
		case NR21:
			return nr21_;
		case NR22:
			return nr22_;
		case NR24:
			return nr24_;
		case NR30:
			return nr30_;
		case NR32:
			return nr32_;
		case NR33:
			return nr33_;
		case NR34:
			return nr34_;
		case NR42:
			return nr42_;
		case NR43:
			return nr43_;
		case NR44:
			return nr44_;
		case NR50:
			return nr50_;
		case NR51:
			return nr51_;
		case NR52:
			return nr52_;
		default:
			spdlog::error("AUDIO: Hier sollte nicht gelesen werden. Addr: 0x{:04X}", addr);
			return 0;
	}
}

auto APU::write(const u16 &addr, const u8 &data) -> void {
	if (addr >= WAVE_TABLE_START && addr <= WAVE_TABLE_END && channel_3_.on_) {
		auto a = (addr - WAVE_TABLE_START) * 2;

		channel_3_.wave_pattern_ram_[a] = data & 0x0F;
		channel_3_.wave_pattern_ram_[a + 1] = (data & 0xF0) >> 4;
		if (addr == WAVE_TABLE_END) {
			for (int i = 0; i < (WAVE_TABLE_END - WAVE_TABLE_START); ++i)
				channel_3_.plot_pattern_[i] = channel_3_.wave_pattern_ram_[i];
		}
		return;
	}

	switch (addr) {
		case NR10: {
			nr10_ = data;

			channel_1_.sweep_time_ = static_cast<f32>((data & 0x70) >> 4u) / 128.f;
			channel_1_.sweep_decrease_ = check_bit(data, 3);
			channel_1_.nr_of_sweep_shifts_ = data & 7u;
			break;
		}
		case NR11: {
			nr11_ = data;

			channel_1_.wave_pattern_duty_ = (data & 0xC0) >> 6u;
			channel_1_.sound_length_data_ = data & 0x3F;
			channel_1_.sound_length_timer_ = 64 - channel_2_.sound_length_data_;
			break;
		}
		case NR12: {
			nr12_ = data;

			channel_1_.envelope_init_volume_ = (data & 0xF0) >> 4u;
			channel_1_.envelope_increase_ = check_bit(data, 3);
			channel_1_.envelope_period_ = data & 3u;
			break;
		}
		case NR13: {
			nr13_ = data;
			i32 upper = channel_1_.frequency_ & 0xFF00;
			channel_1_.frequency_ = upper | data;
			break;
		}
		case NR14: {
			nr14_ = data;
			i32 lower = channel_1_.frequency_ & 0xFF;
			i32 upper = (data & 7) << 8;
			channel_1_.frequency_ = lower | upper;

			channel_1_.counter_selection_ = check_bit(data, 6);
			if (check_bit(data, 7)) {
				sound_on_ = true;
				channel_1_.on_ = true;
				set_bit(nr52_, 0);
			}
			break;
		}
		case NR21: {
			nr21_ = data;
			channel_2_.wave_duty_pattern_ = (data & 0xC0) >> 6; // Bit 7-6
			channel_2_.wave_duty_position_ = 0;
			channel_2_.sound_length_data_ = data & 0x3F;
			channel_2_.sound_length_timer_ = 64 - channel_2_.sound_length_data_;
			break;
		}
		case NR22: {
			nr22_ = data;
			channel_2_.envelope_init_volume_ = (data & 0xF0) >> 4u;
			channel_2_.volume_ = channel_2_.envelope_init_volume_;
			channel_2_.envelope_increase_ = check_bit(data, 3);
			channel_2_.envelope_period_ = data & 0x07;
			// TODO: If zero, stop envelope operation.
			break;
		}
		case NR23: {
			nr23_ = data;
			i32 upper = channel_1_.frequency_ & 0xFF00;
			channel_2_.frequency_ = upper | data;
			break;
		}
		case NR24: {
			nr24_ = data;

			i32 lower = channel_2_.frequency_ & 0xFF;
			i32 upper = (data & 7) << 8;
			channel_2_.frequency_ = lower | upper;
			channel_2_.frequency_timer_ = calc_freq_timer(channel_2_.frequency_);

			channel_2_.counter_selection_ = check_bit(data, 6);

			if (check_bit(data, 7)) {
				sound_on_ = true;
				channel_2_.on_ = true;
				set_bit(nr52_, 1);
				trigger_channel_2();
			}
			break;
		}
		case NR30: {
			nr30_ = data;
			channel_3_.on_ = check_bit(data, 7); // (0 = Stop, 1 = Playback)(Read / Write)
			break;
		}
		case NR31: {
			nr31_ = data;
			break;
		}
		case NR32: {
			nr32_ = data;
			break;
		}
		case NR33: {
			nr33_ = data;
			break;
		}
		case NR34: {
			nr34_ = data;
			if (check_bit(data, 7)) {
				sound_on_ = true;
				channel_3_.on_ = true;
				set_bit(nr52_, 2);
			}
			break;
		}
		case NR41: {
			nr41_ = data;
			break;
		}
		case NR42: {
			nr42_ = data;
			break;
		}
		case NR43: {
			nr43_ = data;
			break;
		}
		case NR44: {
			nr44_ = data;
			if (check_bit(data, 7)) {
				sound_on_ = true;
				channel_4_.on_ = true;
				set_bit(nr52_, 3);
			}
			break;
		}
		case NR50: {
			nr50_ = data;
			break;
		}
		case NR51: {
			nr51_ = data;
			break;
		}
		case NR52: {
			if (check_bit(data, 7))
				set_bit(nr52_, 7);
			else
				reset();
			break;
		}
		default: {
			mem_->write_direct(addr, data);
			break;
		}
	}
}

auto APU::imgui_view() -> void {
	ImGui::PlotLines("Cannel 3", channel_3_imgui_plot_getter, channel_3_.plot_pattern_, 32 * 4, 0.1f, "", -7.f, 7.f, ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SliderInt("Volume", &apu_volume_, 0, 20);

	ImGui::Separator();
	play_effect_ = ImGui::Button("Soundeffect", ImVec2(0, 0));
	ImGui::SliderInt("Sin", &hz_, 50, 500);
	ImGui::Checkbox("Sinus Test", &sinus_test_);

	ImGui::Separator();
	ImGui::Text("Channel 2 Frequenz in Hz: %d", channel_2_.frequency_timer_);

	ImGui::Separator();
	ImGui::Checkbox("Channel 1", &imgui_channel_1_on_);
	ImGui::Checkbox("Channel 2", &imgui_channel_2_on_);
	ImGui::Checkbox("Channel 3", &imgui_channel_3_on_);
	ImGui::Checkbox("Channel 4", &imgui_channel_4_on_);
}

auto APU::update_tests(i32 Sample_Count) -> void {
	if (play_effect_) {
		channel_2_soundeffect();
		play_effect_ = false;
	}
	if (sinus_test_) {
		for (int i = 0; i < Sample_Count; i += 2) {
			f32 Sin_Value = std::sin(t_sin_);
			channel_1_sample_[i] = (i16)(Sin_Value * apu_volume_ * 1000);
			channel_1_sample_[i + 1] = (i16)(Sin_Value * apu_volume_ * 1000);
			t_sin_ += 2.0f * PI * (1.f / (f32)(48000 / hz_));
		}
	}
}

auto APU::update(i32 cycles) -> void {
	static u32 Cycles_since_sample = 0;

	static i32 Frame_Sequencer = 0;
	static i32 Frame_Sequencer_Step = 0;

	while (cycles--) {
		++Cycles_since_sample;
		// FRAME SEQUENCER
		if (Frame_Sequencer == 0 || ++Frame_Sequencer == 8192) {
			if (Frame_Sequencer_Step % 2 == 0 || Frame_Sequencer_Step == 0) {
				// Frame Sequencer Length Counter
				if (--channel_1_.sound_length_timer_ == 0)
					channel_1_.on_ = false;
				if (channel_2_.counter_selection_)
					if (--channel_2_.sound_length_timer_ == 0)
						channel_2_.on_ = false;
			}
			if (Frame_Sequencer_Step == 7) {
				// Volume Envelope
				if (channel_2_.envelope_period_ != 0) {
					if (channel_2_.envelope_period_timer_ > 0)
						--channel_2_.envelope_period_timer_;

					if (channel_2_.envelope_period_timer_ == 0) {
						channel_2_.envelope_period_timer_ = channel_2_.envelope_period_;
						if ((channel_2_.volume_ < 0x0F && !channel_2_.envelope_increase_) ||
							(channel_2_.volume_ > 0 && channel_2_.envelope_increase_)) {
							if (channel_2_.envelope_increase_) {
								--channel_2_.volume_;
							} else {
								++channel_2_.volume_;
							}
						}
					}
				}
			}
			if (Frame_Sequencer_Step == 2 || Frame_Sequencer_Step == 6) {
				// Frequency Sweep
			}
		}
		++Frame_Sequencer_Step %= 8;
		// ENDE FRAME SEQUENCER

		if (--channel_1_.frequency_ < 1) {
			channel_1_.frequency_ -= cycles;
		}

		if (channel_2_.frequency_timer_ > 1 && channel_2_.on_) {
			--channel_2_.frequency_timer_;
		} else {
			channel_2_.frequency_timer_ = calc_freq_timer(channel_2_.frequency_);
			++channel_2_.wave_duty_position_ %= 8;
		}
		channel_3_.frequency_ > 0 ? channel_3_.frequency_ -= cycles : false;

		if (Cycles_since_sample >= CYCLES_PER_SAMPLE) {
			++apu_sample_count_;
			Cycles_since_sample = 0;
			if (channel_2_.on_) {
				f32 sample = wave_duty_table_[channel_2_.wave_duty_pattern_][channel_2_.wave_duty_position_];
				f32 volume = (f32)(channel_2_.volume_);
				i16 output = (i16)(((sample * volume) / 7.5f) - 1.0f) * 10;
				channel_2_sample_[(channel_2_running_sample_index_++) % sample_buffer_size_] = output;
				channel_2_sample_[(channel_2_running_sample_index_++) % sample_buffer_size_] = output;
			} else {
				channel_2_running_sample_index_ += 2;
			}
		}
	}
}

auto APU::get_output_sample(i32 Sample_Count) -> i16 * {
	update_tests(apu_sample_count_);
	for (int i = 0; i < Sample_Count; ++i) {
		i16 c1 = 0, c2 = 0, c3 = 0, c4 = 0;
		if (imgui_channel_1_on_)
			c1 = channel_1_sample_[i];
		if (imgui_channel_2_on_)
			c2 = channel_2_sample_[i];
		if (imgui_channel_3_on_)
			c3 = channel_3_sample_[i];
		if (imgui_channel_4_on_)
			c4 = channel_4_sample_[i];
		output_sample_[i] = (c1 + c2 + c3 + c4) / 4;
	}
	clear_channel_samples();
	return output_sample_;
}

auto APU::channel_2_soundeffect() -> void {
	f32 quick_sin = 0;
	for (int i = 0; i < 3000; i++) {
		f32 Sin_Value = std::sin(quick_sin);
		channel_2_sample_[i] = (i16)(Sin_Value * apu_volume_);
		channel_2_sample_[i + 1] = (i16)(Sin_Value * apu_volume_);
		quick_sin += 2.0f * PI * (1.f / (f32)(48000 / 500));
	}
}

auto APU::trigger_channel_2() -> void {
	channel_2_.on_ = true;
	if (channel_2_.sound_length_timer_ == 0)
		channel_2_.sound_length_timer_ = 64;
	channel_2_.frequency_timer_ = calc_freq_timer(channel_2_.frequency_);
	channel_2_.envelope_period_timer_ = channel_2_.envelope_period_;
	channel_2_.volume_ = channel_2_.envelope_init_volume_;
}

auto APU::clear_channel_samples() -> void {
	for (int i = 0; i < sample_buffer_size_; ++i) {
		channel_1_sample_[i] = 0;
		channel_2_sample_[i] = 0;
		channel_3_sample_[i] = 0;
		channel_4_sample_[i] = 0;
	}
	channel_1_running_sample_index_ = 0;
	channel_2_running_sample_index_ = 0;
	channel_3_running_sample_index_ = 0;
	channel_4_running_sample_index_ = 0;
	apu_sample_count_ = 0;
}

auto APU::calc_freq_timer(i32 frequency) -> i32 {
	return (2048 - frequency) * 4;
}

auto APU::reset() -> void {
	channel_1_ = {};
	channel_2_ = {};
	channel_3_ = {};
	channel_4_ = {};
	nr50_ = 0;
	nr51_ = 0;

	sound_on_ = false;
}

auto APU::channel_3_imgui_plot_getter(void *v, i32 i) -> f32 {
	auto vv = reinterpret_cast<i8 *>(v);
	f32 ret = static_cast<f32>(vv[i % (WAVE_TABLE_END - WAVE_TABLE_START)]);
	return ret;
}
