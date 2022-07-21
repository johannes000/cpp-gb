#ifndef AUDIO_H
#define AUDIO_H

#pragma once

// #define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID lpGUID, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
// typedef DIRECT_SOUND_CREATE(direct_sound_create);

#include "windows.h"
#include "dsound.h"

class Audio {
public:
	Audio(gameboy *g);

	void audio_view();
	bool win32_init_dsound(HWND Window);

private:
	gameboy *gb;
	IDirectSound *direct_sound;
	IDirectSoundBuffer *primary_buffer;
	IDirectSoundBuffer *secondary_buffer;
};

#endif