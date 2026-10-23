#ifndef AUDIO_HPP
#define AUDIO_HPP

#pragma once

#include <SDL3/SDL.h>

#include "utl/type.hpp"

class Audio {
public:
	Audio();
	~Audio();

	Audio(const Audio &) = delete;
	auto operator=(const Audio &) -> Audio & = delete;

	// Stereo-Samples (interleaved, s16) an das Audiogeraet uebergeben
	auto push(const s16 *samples, int sample_count) -> bool;

private:
	SDL_AudioStream *stream_{nullptr};
};

#endif // !AUDIO_HPP
