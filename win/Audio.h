#ifndef AUDIO_H
#define AUDIO_H

#pragma once

struct Audio {
	Audio(gameboy *g);

	void audio_view();

	gameboy *gb;
};

#endif