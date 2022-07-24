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
	if (addr >= WAVE_TABLE_START && addr <= WAVE_TABLE_END) {
		channel_3.wave_pattern_ram[addr - WAVE_TABLE_START] = data;
		return;
	}
	switch (addr) {
		case NR10: {
			nr10 = data;

			channel_1.sweep_time = static_cast<f32>((data & 0x70) >> 4u) / 128.f;
			channel_1.sweep_decrease = check_bit(data, 3);
			channel_1.nr_of_sweep_shifts = data & 7u;
		}
		case NR11: {
			nr11 = data;

			channel_1.wave_pattern_duty = (data & 0xC0) >> 6u;
			f32 t1 = static_cast<f32>(data & 0x3F);
			if (channel_1.counter_selection)
				channel_1.sound_length_data = (64.f - t1) * (1.f / 256.f);
		}
		case NR12: {
			nr12 = data;

			channel_1.envelope_init_volume = (data & 0xF0) >> 4u;
			channel_1.envelope_increase = check_bit(data, 3);
			channel_1.nr_of_envelope_sweeps = data & 3u;
		}
		case NR13: {
			nr13 = data;
			i32 upper = channel_1.frequency & 0xFF00;
			channel_1.frequency = upper | data;
		}
		case NR14: {
			nr14 = data;
			i32 lower = channel_1.frequency & 0xFF;
			i32 upper = (data & 3) << 8;
			channel_1.frequency = lower | upper;

			channel_1.counter_selection = check_bit(data, 6);
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_1.channel_on = true;
				set_bit(nr52, 0);
			}
		}
		case NR21: {
			nr21 = data;
		}
		case NR22: {
			nr22 = data;
		}
		case NR23: {
			nr23 = data;
		}
		case NR24: {
			nr24 = data;
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_2.channel_on = true;
				set_bit(nr52, 1);
			}
		}
		case NR30: {
			nr30 = data;
		}
		case NR31: {
			nr31 = data;
		}
		case NR32: {
			nr32 = data;
		}
		case NR33: {
			nr33 = data;
		}
		case NR34: {
			nr34 = data;
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_3.channel_on = true;
				set_bit(nr52, 2);
			}
		}
		case NR41: {
			nr41 = data;
		}
		case NR42: {
			nr42 = data;
		}
		case NR43: {
			nr43 = data;
		}
		case NR44: {
			nr44 = data;
			if (check_bit(data, 7)) {
				sound_on = true;
				channel_4.channel_on = true;
				set_bit(nr52, 3);
			}
		}
		case NR50: {
			nr50 = data;
		}
		case NR51: {
			nr51 = data;
		}
		case NR52: {
			if (check_bit(data, 7))
				set_bit(nr52, 7);
			else
				reset();
		}
		default:
			mem->write_direct(addr, data);
	}
}

void APU::apu_view() {
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
