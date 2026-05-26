#include "Audio.hpp"

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

void fill_audio(void *udata, Uint8 *stream, int len) {
	/* Only play if we have data left */
	if (audio_len == 0)
		return;

	/* Mix as much data as possible */
	len = (len > audio_len ? audio_len : len);
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

Audio::Audio() {
	SDL_AudioSpec wanted;
	extern void fill_audio(void *udata, Uint8 *stream, int len);

	/* Set the audio format */
	wanted.freq = 48000;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;   /* 1 = mono, 2 = stereo */
	wanted.samples = 1024; /* Good low-latency value for callback */
	wanted.callback = fill_audio;
	wanted.userdata = NULL;

	/* Open the audio device, forcing the desired format */
	if (SDL_OpenAudio(&wanted, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return;
	}
	return;
}

Audio::~Audio() {
	SDL_CloseAudio();
}
