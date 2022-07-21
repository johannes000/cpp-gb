#include "Audio.h"

Audio::Audio(gameboy *g) : gb(g), direct_sound(0), primary_buffer(0), secondary_buffer(0) {
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

	DSBUFFERDESC buffer_desc;
	buffer_desc.dwSize = sizeof(DSBUFFERDESC);
	buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	buffer_desc.dwBufferBytes = 0;
	buffer_desc.dwReserved = 0;
	buffer_desc.lpwfxFormat = NULL;
	buffer_desc.guid3DAlgorithm = GUID_NULL;

	result = direct_sound->CreateSoundBuffer(&buffer_desc, &primary_buffer, NULL);
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

	return true;
}
