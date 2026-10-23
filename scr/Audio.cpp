#include "Audio.hpp"

Audio::Audio() {
	const SDL_AudioSpec spec{SDL_AUDIO_S16, 2, SAMPLES_PER_SECOND};

	stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
	if (stream_ == nullptr) {
		spdlog::error("Audiogeraet konnte nicht geoeffnet werden: {}", SDL_GetError());
		return;
	}
	SDL_ResumeAudioStreamDevice(stream_);
}

Audio::~Audio() {
	if (stream_ != nullptr)
		SDL_DestroyAudioStream(stream_);
}

auto Audio::push(const s16 *samples, int sample_count) -> bool {
	if (stream_ == nullptr)
		return false;
	return SDL_PutAudioStreamData(stream_, samples, sample_count * static_cast<int>(sizeof(s16)));
}
