#ifndef AUDIO_H
#define AUDIO_H

#pragma once

#include "windows.h"
#include "dsound.h"

class Audio {
public:
	Audio(gameboy *g);

	void audio_view();
	bool win32_init_dsound(HWND Window);

	void update(f64 dt);

private:
	gameboy *gb;
	IDirectSound *direct_sound;
	IDirectSoundBuffer *primary_buffer;
	IDirectSoundBuffer *secondary_buffer;

private:
	i32 samples_per_second;
	i32 running_sample_index;
	i32 bytes_per_sample;
	i32 second_buffer_size;
	i32 latency_sample_count;
};

#endif