#include "Audio.h"

Audio::Audio(gameboy *g) : gb(g), direct_sound(0), primary_buffer(0), secondary_buffer(0) {
	samples_per_second = 44100;
	running_sample_index = 0;
	bytes_per_sample = sizeof(i16) * 2;
	second_buffer_size = samples_per_second * bytes_per_sample;
	latency_sample_count = samples_per_second / 15;
}

void Audio::audio_view() {
	ImGui::Text("Audio");
}

bool Audio::win32_init_dsound(HWND Window) {

	HRESULT result = DirectSoundCreate(0, &direct_sound, 0);
	if (SUCCEEDED(result)) {
		spdlog::info("AUDIO: Direct Sound Created");
	} else {
		spdlog::critical("Direct Sound konnte nicht Erstellt werden. Result: {}", result);
		return false;
	}

	result = direct_sound->SetCooperativeLevel(Window, DSSCL_PRIORITY);
	if (SUCCEEDED(result)) {
		spdlog::info("AUDIO: Direct Sound mit Window verbunden");
	} else {
		spdlog::critical("AUDIO: Direct Sound nicht mit Window verbunden");
		return false;
	}

	DSBUFFERDESC Buffer_desc = {};
	Buffer_desc.dwSize = sizeof(DSBUFFERDESC);
	Buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	Buffer_desc.dwBufferBytes = 0;
	Buffer_desc.dwReserved = 0;
	Buffer_desc.lpwfxFormat = NULL;
	Buffer_desc.guid3DAlgorithm = GUID_NULL;

	result = direct_sound->CreateSoundBuffer(&Buffer_desc, &primary_buffer, NULL);
	if (SUCCEEDED(result)) {
		spdlog::info("AUDIO: Primary Buffer erstellt");
	} else {
		spdlog::critical("AUDIO: Primary Buffer konnte nicht erstellt werden. Result: {}", result);
		return false;
	}

	WAVEFORMATEX Wave_Format = {};
	Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
	Wave_Format.nChannels = 2;
	Wave_Format.nSamplesPerSec = 44100;
	Wave_Format.wBitsPerSample = 16;
	Wave_Format.cbSize = 0;
	Wave_Format.nBlockAlign = (Wave_Format.nChannels * Wave_Format.wBitsPerSample) / 8;
	Wave_Format.nAvgBytesPerSec = Wave_Format.nSamplesPerSec * Wave_Format.nBlockAlign;

	result = primary_buffer->SetFormat(&Wave_Format);
	if (SUCCEEDED(result)) {
		spdlog::info("AUDIO: Primary Buffer Wave Format festgelegt");
	} else {
		spdlog::critical("AUDIO: Primary Buffer konnte Wave Format nicht festlegen. Result: {}", result);
		return false;
	}

	Buffer_desc = {};
	Buffer_desc.dwSize = sizeof(DSBUFFERDESC);
	Buffer_desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
	Buffer_desc.dwBufferBytes = second_buffer_size;
	Buffer_desc.lpwfxFormat = &Wave_Format;

	result = direct_sound->CreateSoundBuffer(&Buffer_desc, &secondary_buffer, 0);
	if (SUCCEEDED(result)) {
		spdlog::info("AUDIO: Secondary Buffer erstellt");
	} else {
		spdlog::critical("AUDIO: Secondary Buffer konnte nicht erstellt werden. Result: {}", result);
		return false;
	}

	return true;
}
