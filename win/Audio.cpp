#include "Audio.hpp"

Audio::Audio(gameboy *g) : gb(g),
						   direct_sound(0), primary_buffer(0), secondary_buffer(0) {
	sound_buffer.samples_per_second = SAMPLES_PER_SECOND;

	running_sample_index = 0;

	bytes_per_sample = sizeof(i16) * 2;
	second_buffer_size = sound_buffer.samples_per_second * bytes_per_sample;

	latency_sample_count = sound_buffer.samples_per_second / 15;

	volume = 500;
}

// void Audio::imgui_view() {
// 	ImGui::Text("Audio");
// 	ImGui::SliderInt("Volume", &volume, 0, 3000);
// }

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
	Wave_Format.nSamplesPerSec = sound_buffer.samples_per_second;
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

	secondary_buffer->Play(0, 0, DSBPLAY_LOOPING);
	win32_clear_sound_buffer(0, second_buffer_size);

	return true;
}

void Audio::win32_fill_sound_buffer(DWORD Byte_to_Lock, DWORD Bytes_to_Write) {
	void *Region_1;
	DWORD Region_1_Size;
	void *Region_2;
	DWORD Region_2_Size;

	auto result = secondary_buffer->Lock(Byte_to_Lock, Bytes_to_Write,
										 &Region_1, &Region_1_Size,
										 &Region_2, &Region_2_Size,
										 0);

	if (FAILED(result)) {
		spdlog::error("AUDIO: Fehler beim Locken 2: {}", result);
	}

	DWORD Region_1_Sample_Count = Region_1_Size / bytes_per_sample;
	DWORD Region_2_Sample_Count = Region_2_Size / bytes_per_sample;
	DWORD Sample_Counts = Region_1_Sample_Count + Region_2_Sample_Count;

	i16 *Dest_Sample = (i16 *)Region_1;
	i16 *Output_Sample = gb->apu.get_output_sample(Sample_Counts);

	for (DWORD SampleIndex = 0; SampleIndex < Region_1_Sample_Count; ++SampleIndex) {
		i16 Sample_ValueL = static_cast<i16>(*Output_Sample++ * volume);
		i16 Sample_ValueR = static_cast<i16>(*Output_Sample++ * volume);
		*Dest_Sample++ = Sample_ValueL;
		*Dest_Sample++ = Sample_ValueR;
		running_sample_index++;
	}

	Dest_Sample = (i16 *)Region_2;
	for (DWORD SampleIndex = 0; SampleIndex < Region_2_Sample_Count; ++SampleIndex) {
		i16 Sample_ValueL = static_cast<i16>(*Output_Sample++ * volume);
		i16 Sample_ValueR = static_cast<i16>(*Output_Sample++ * volume);
		*Dest_Sample++ = Sample_ValueL;
		*Dest_Sample++ = Sample_ValueR;
		running_sample_index++;
	}

	secondary_buffer->Unlock(Region_1, Region_1_Size, Region_2, Region_2_Size);
}

void Audio::win32_clear_sound_buffer(DWORD Byte_to_Lock, DWORD Bytes_to_Write) {

	void *Region_1;
	DWORD Region_1_Size;
	void *Region_2;
	DWORD Region_2_Size;

	auto result = secondary_buffer->Lock(Byte_to_Lock, Bytes_to_Write,
										 &Region_1, &Region_1_Size,
										 &Region_2, &Region_2_Size,
										 0);

	if (FAILED(result)) {
		spdlog::error("AUDIO: Fehler beim Locken: {}", result);
	}

	DWORD Region_1_Sample_Count = Region_1_Size / bytes_per_sample;
	i16 *Sample_Out = (i16 *)Region_1;
	for (DWORD SampleIndex = 0; SampleIndex < Region_1_Sample_Count; ++SampleIndex) {

		*Sample_Out++ = 0;
		*Sample_Out++ = 0;
	}

	DWORD Region_2_Sample_Count = Region_2_Size / bytes_per_sample;
	Sample_Out = (i16 *)Region_2;
	for (DWORD SampleIndex = 0; SampleIndex < Region_2_Sample_Count; ++SampleIndex) {
		*Sample_Out++ = 0;
		*Sample_Out++ = 0;
	}

	secondary_buffer->Unlock(Region_1, Region_1_Size, Region_2, Region_2_Size);
}

void Audio::update(f64 dt) {
	DWORD Play_Cursor;
	DWORD Write_Cursor;

	auto result = secondary_buffer->GetCurrentPosition(&Play_Cursor, &Write_Cursor);
	if (FAILED(result)) {
		spdlog::error("AUDIO: Fehler mit GetCurrentPosition: {}", result);
		return;
	}

	DWORD Byte_To_lock = (running_sample_index * bytes_per_sample) % second_buffer_size;
	DWORD Target_Cursor = (Play_Cursor + (latency_sample_count * bytes_per_sample)) % second_buffer_size;
	DWORD Bytes_To_Write;

	if (Byte_To_lock > Target_Cursor) {
		Bytes_To_Write = (second_buffer_size - Byte_To_lock);
		Bytes_To_Write += Target_Cursor;
	} else {
		Bytes_To_Write = Target_Cursor - Byte_To_lock;
	}

	win32_fill_sound_buffer(Byte_To_lock, Bytes_To_Write);
}
