#ifndef AUDIO_H
#define AUDIO_H

#pragma once

#include "dsound.h"
#include <SDL2/SDL.h>

#include "../scr/gameboy.h"
class Audio {
public:
	Audio(gameboy *g);

	void imgui_view();
	bool win32_init_dsound(HWND Window);
	void win32_fill_sound_buffer(DWORD Byte_to_Lock, DWORD Bytes_to_Write);
	void win32_clear_sound_buffer(DWORD Byte_to_Lock, DWORD Bytes_to_Write);

	void update(f64 dt);
	SDL_AudioDeviceID audio_device_id;

private:
	gameboy *gb;
	IDirectSound *direct_sound;
	IDirectSoundBuffer *primary_buffer;
	IDirectSoundBuffer *secondary_buffer;
	SDL_AudioSpec audio_specs;

private:
	sound_output_buffer sound_buffer;

	u32 running_sample_index;
	i32 bytes_per_sample;
	i32 second_buffer_size;
	i32 latency_sample_count;

	i32 volume;
};

#endif