#include "Audio.h"

Audio::Audio(gameboy *g) : gb(g) {
}

void Audio::audio_view() {
	ImGui::Text("Audio");
}
