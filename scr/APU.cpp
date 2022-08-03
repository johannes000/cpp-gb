#include "APU.h"

APU::APU(MEMORY *m) : mem(m) {
	nr10 = 0;
	nr11 = 0;
	nr12 = 0;
	nr13 = 0;
	nr14 = 0;
	nr21 = 0;
	nr22 = 0;
	nr23 = 0;
	nr24 = 0;
	nr30 = 0;
	nr31 = 0;
	nr32 = 0;
	nr33 = 0;
	nr34 = 0;
	nr41 = 0;
	nr42 = 0;
	nr43 = 0;
	nr44 = 0;
	nr50 = 0;
	nr51 = 0;
	nr52 = 0;

	sound_on = false;
	play_effect = false;
	sinus_test = false;

	apu_volume = 10;

	hz = 264;
	sin_wave_period = 48000 / hz;
	t_sin = 0;

	channel_1 = {};
	channel_2 = {};
	channel_3 = {};
	channel_4 = {};

	channel_1_sample = new i16[sample_buffer_size];
	channel_2_sample = new i16[sample_buffer_size];
	channel_3_sample = new i16[sample_buffer_size];
	channel_4_sample = new i16[sample_buffer_size];

	channel_1_running_sample_index = 0;
	channel_2_running_sample_index = 0;
	channel_3_running_sample_index = 0;
	channel_4_running_sample_index = 0;

	output_sample = new i16[sample_buffer_size];

	channel_2_wave_duty_table = {
		{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f},
		{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f},
		{0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f},
		{1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f}};
	clear_channel_samples();
}

u8 APU::read(const u16 &addr) {
	if (addr >= WAVE_TABLE_START && addr <= WAVE_TABLE_END)
		return channel_3.wave_pattern_ram[addr - WAVE_TABLE_START];

	switch (addr) {
		case NR13:
		case NR23:
		case NR31:
		case NR41:
			spdlog::error("AUDIO: Register ist WO wird aber gelesen: {:X}", addr);
			return 0;
		case NR10:
			return nr10;
		case NR11:
			return nr11;
		case NR12:
			return nr12;
		case NR14:
			return nr14;
		case NR21:
			return nr21;
		case NR22:
			return nr22;
		case NR24:
			return nr24;
		case NR30:
			return nr30;
		case NR32:
			return nr32;
		case NR33:
			return nr33;
		case NR34:
			return nr34;
		case NR42:
			return nr42;
		case NR43:
			return nr43;
		case NR44:
			return nr44;
		case NR50:
			return nr50;
		case NR51:
			return nr51;
		case NR52:
			return nr52;
		default:
			spdlog::error("AUDIO: Hier sollte nicht gelesen werden. Addr: {:X} | {} {}", addr, __FILE__, __LINE__);
			return 0;
	}
}

void APU::write(const u16 &addr, const u8 &data) {
	if (addr >= WAVE_TABLE_START && addr <= WAVE_TABLE_END && channel_3.on) {
		auto a = (addr - WAVE_TABLE_START) * 2;

		channel_3.wave_pattern_ram[a] = data & 0x0F;
		channel_3.wave_pattern_ram[a + 1] = (data & 0xF0) >> 4;
		if (addr == WAVE_TABLE_END) {
			for (int i = 0; i < (WAVE_TABLE_END - WAVE_TABLE_START); ++i)
				channel_3.plot_pattern[i] = channel_3.wave_pattern_ram[i];
		}
		return;
	}

	switch (addr) {
		case NR10: {
			nr10 = data;

			channel_1.sweep_time = static_cast<f32>((data & 0x70) >> 4u) / 128.f;
			channel_1.sweep_decrease = check_bit(data, 3);
			channel_1.nr_of_sweep_shifts = data & 7u;
			break;
		}
		case NR11: {
			nr11 = data;

			channel_1.wave_pattern_duty = (data & 0xC0) >> 6u;
			f32 t1 = static_cast<f32>(data & 0x3F);
			if (channel_1.counter_selection)
				channel_1.sound_length_data = (64.f - t1) * (1.f / 256.f);
			break;
		}
		case NR12: {
			nr12 = data;

			channel_1.envelope_init_volume = (data & 0xF0) >> 4u;
			channel_1.envelope_increase = check_bit(data, 3);
			channel_1.nr_of_envelope_sweeps = data & 3u;
			break;
		}
		case NR13: {
			nr13 = data;
			i32 upper = channel_1.frequency & 0xFF00;
			channel_1.frequency = upper | data;
			break;
		}
		case NR14: {
			nr14 = data;
			i32 lower = channel_1.frequency & 0xFF;
			i32 upper = (data & 7) << 8;
			channel_1.frequency = lower | upper;

			channel_1.counter_selection = check_bit(data, 6);
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_1.on = true;
				set_bit(nr52, 0);
			}
			break;
		}
		case NR21: {
			nr21 = data;
			channel_2.wave_duty_pattern = (data & 0xC0) >> 6; // Bit 7-6
			channel_2.sound_length_data = (data & 0x3F);
			break;
		}
		case NR22: {
			nr22 = data;
			channel_2.envelope_init_volume = data & 0xF0;
			channel_2.envelope_increase = check_bit(data, 3);
			channel_2.nr_of_envelope_sweeps = data & 0x07;
			// TODO: If zero, stop envelope operation.
			break;
		}
		case NR23: {
			nr23 = data;
			i32 upper = channel_1.frequency & 0xFF00;
			channel_2.frequency = upper | data;
			break;
		}
		case NR24: {
			nr24 = data;

			i32 lower = channel_2.frequency & 0xFF;
			i32 upper = (data & 7) << 8;
			channel_2.frequency = lower | upper;
			channel_2.frequency_timer = (2048 - channel_2.frequency) * 4;

			channel_2.counter_selection = check_bit(data, 6);

			if (check_bit(data, 7)) {
				sound_on = true;
				channel_2.on = true;
				set_bit(nr52, 1);
			}
			break;
		}
		case NR30: {
			nr30 = data;
			channel_3.on = check_bit(data, 7); // (0 = Stop, 1 = Playback)(Read / Write)
			break;
		}
		case NR31: {
			nr31 = data;
			break;
		}
		case NR32: {
			nr32 = data;
			break;
		}
		case NR33: {
			nr33 = data;
			break;
		}
		case NR34: {
			nr34 = data;
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_3.on = true;
				set_bit(nr52, 2);
			}
			break;
		}
		case NR41: {
			nr41 = data;
			break;
		}
		case NR42: {
			nr42 = data;
			break;
		}
		case NR43: {
			nr43 = data;
			break;
		}
		case NR44: {
			nr44 = data;
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_4.on = true;
				set_bit(nr52, 3);
			}
			break;
		}
		case NR50: {
			nr50 = data;
			break;
		}
		case NR51: {
			nr51 = data;
			break;
		}
		case NR52: {
			if (check_bit(data, 7))
				set_bit(nr52, 7);
			else
				reset();
			break;
		}
		default: {
			mem->write_direct(addr, data);
			break;
		}
	}
}

void APU::imgui_view() {
	ImGui::PlotLines("Cannel 3", channel_3_imgui_plot_getter, channel_3.plot_pattern, 32 * 4, 0.1f, "", -7.f, 7.f, ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
	ImGui::SliderInt("Volume", &apu_volume, 0, 200);

	ImGui::Separator();
	play_effect = ImGui::Button("Soundeffect", ImVec2(0, 0));
	ImGui::SliderInt("Sin", &hz, 50, 500);
	ImGui::Checkbox("Sinus Test", &sinus_test);

	ImGui::Separator();
	ImGui::Text("Channel 2 Frequenz in Hz: %d", channel_2.frequency_timer);
}

void APU::update_tests(i32 Sample_Count) {
	if (play_effect) {
		channel_2_soundeffect();
		play_effect = false;
	}
	if (sinus_test) {
		for (int i = 0; i < Sample_Count * 2; i += 2) {
			f32 Sin_Value = sinf(t_sin);
			channel_1_sample[i] = (i16)(Sin_Value * apu_volume);
			channel_1_sample[i + 1] = (i16)(Sin_Value * apu_volume);
			t_sin += 2.0f * PI * (1.f / (f32)(48000 / hz));
		}
	}
}

void APU::update(i32 cycles) {
	static u32 cycles_since_sample_1 = 0;
	static u32 cycles_since_sample_2 = 0;
	static u32 cycles_since_sample_3 = 0;
	static u32 cycles_since_sample_4 = 0;
	while (cycles--) {
		if (--channel_1.frequency < 1) {
			channel_1.frequency -= cycles;
		}
		if (channel_2.frequency_timer > 1) {
			--channel_2.frequency_timer;
			++cycles_since_sample_2;
		} else {
			channel_2.frequency_timer = (2048 - channel_2.frequency) * 4;
			++channel_2.wave_duty_position %= 8;
		}
		channel_3.frequency > 0 ? channel_3.frequency -= cycles : false;

		++cycles_since_sample_1;
		++cycles_since_sample_3;
		++cycles_since_sample_4;
		if (cycles_since_sample_2 > CYCLES_PER_SAMPLE && channel_2.on) {
			cycles_since_sample_2 = 0;
			i16 sample = (i16)(channel_2_wave_duty_table[channel_2.wave_duty_pattern][channel_2.wave_duty_position] * 20);
			channel_2_sample[(channel_2_running_sample_index++) % sample_buffer_size] = sample;
			channel_2_sample[(channel_2_running_sample_index++) % sample_buffer_size] = sample;
		}
	}
}

i16 *APU::get_output_sample(i32 Sample_Count) {
	update_tests(Sample_Count);

	for (int i = 0; i < Sample_Count * 2; ++i) {
		auto c1 = channel_1_sample[i];
		auto c2 = channel_2_sample[i];
		auto c3 = channel_3_sample[i];
		auto c4 = channel_4_sample[i];

		output_sample[i] = (c1 + c2 + c3 + c4) / 4;
	}
	clear_channel_samples();
	return output_sample;
}

void APU::channel_2_soundeffect() {
	f32 quick_sin = 0;
	for (int i = 0; i < 3000; i++) {
		f32 Sin_Value = sinf(quick_sin);
		channel_2_sample[i] = (i16)(Sin_Value * apu_volume);
		channel_2_sample[i + 1] = (i16)(Sin_Value * apu_volume);
		quick_sin += 2.0f * PI * (1.f / (f32)(48000 / 500));
	}
}

void APU::clear_channel_samples() {
	for (int i = 0; i < sample_buffer_size; ++i) {
		channel_1_sample[i] = 0;
		channel_2_sample[i] = 0;
		channel_3_sample[i] = 0;
		channel_4_sample[i] = 0;
	}
	channel_1_running_sample_index = 0;
	channel_2_running_sample_index = 0;
	channel_3_running_sample_index = 0;
	channel_4_running_sample_index = 0;
}

void APU::reset() {
	channel_1 = {};
	channel_2 = {};
	channel_3 = {};
	channel_4 = {};
	nr50 = 0;
	nr51 = 0;

	sound_on = false;
}

f32 APU::channel_3_imgui_plot_getter(void *v, i32 i) {
	auto vv = reinterpret_cast<i8 *>(v);
	f32 ret = static_cast<f32>(vv[i % (WAVE_TABLE_END - WAVE_TABLE_START)]);
	return ret;
}
