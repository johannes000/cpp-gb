#ifndef AUDIO_H
#define AUDIO_H

#pragma once

#include "windows.h"

class Audio {
public:
	Audio(gameboy *g);

	void audio_view();
	void init_sound(HWND Window);

private:
	gameboy *gb;
};

#endif