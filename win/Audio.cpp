#include "Audio.h"

Audio::Audio(gameboy *g) : gb(g), direct_sound(0), primary_buffer(0), secondary_buffer(0) {
	samples_per_second = 48000;
	running_sample_index = 0;
	bytes_per_sample = sizeof(i16) * 2;
	second_buffer_size = samples_per_second * bytes_per_sample;
	latency_sample_count = samples_per_second / 15;
}

void Audio::imgui_view() {
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
	Wave_Format.nSamplesPerSec = samples_per_second;
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

void Audio::update(f64 dt) {
	secondary_buffer->Play(0, 0, DSBPLAY_LOOPING);
	i32 Volume = 1000;
	i32 Hz = 144;
	i32 Square_Wave_Period = samples_per_second / Hz;
	i32 Half_Square_Wave_Period = Square_Wave_Period / 2;

	DWORD Play_Cursor;
	DWORD Write_Cursor;

	auto result = secondary_buffer->GetCurrentPosition(&Play_Cursor, &Write_Cursor);
	if (FAILED(result)) {
		spdlog::error("AUDIO: Fehler mit GetCurrentPosition: {}", result);
		return;
	}

	void *Region_1;
	DWORD Region_1_Size;
	void *Region_2;
	DWORD Region_2_Size;

	DWORD Byte_To_lock = (running_sample_index * bytes_per_sample) % second_buffer_size;
	DWORD Bytes_To_Write;
	if (Byte_To_lock >= Play_Cursor) {
		Bytes_To_Write = (second_buffer_size - Byte_To_lock);
		Bytes_To_Write += Play_Cursor;
	} else {
		Bytes_To_Write = Play_Cursor - Byte_To_lock;
	}

	result = secondary_buffer->Lock(Byte_To_lock, Bytes_To_Write,
									&Region_1, &Region_1_Size,
									&Region_2, &Region_2_Size,
									0);

	if (FAILED(result)) {
		spdlog::error("AUDIO: Fehler beim Locken: {}", result);
	}

	i16 *Sample_Out = (i16 *)Region_1;
	DWORD Region_1_Sample_Count = Region_1_Size / bytes_per_sample;
	DWORD Region_2_Sample_Count = Region_2_Size / bytes_per_sample;

	for (DWORD SampleIndex = 0; SampleIndex < Region_1_Sample_Count; ++SampleIndex) {
		i16 Sample_Value = (running_sample_index / Half_Square_Wave_Period) % 2 ? Volume : -Volume;
		*Sample_Out++ = Sample_Value;
		*Sample_Out++ = Sample_Value;
		++running_sample_index;
	}
	Sample_Out = (i16 *)Region_2;

	for (DWORD SampleIndex = 0; SampleIndex < Region_2_Sample_Count; ++SampleIndex) {
		i16 Sample_Value = (running_sample_index / Half_Square_Wave_Period) % 2 ? Volume : -Volume;
		*Sample_Out++ = Sample_Value;
		*Sample_Out++ = Sample_Value;
		++running_sample_index;
	}

	secondary_buffer->Unlock(Region_1, Region_1_Size, Region_2, Region_2_Size);
}
