//
// Created by knaub on 25.06.2020.
//

#include "CONTROLS.h"

CONTROLS::CONTROLS(MEMORY* m) :
	mem(m),
	joy_button_direction_selector(0xFF),
	joy_both_states(0xFF),
	joy_both_states_old(0xFF) {
}

u8 CONTROLS::read(const u16& addr) const {
	u8 joy_p1 = 0xFF;
	if (addr == JOY_P1_REGISTER) {
		switch (joy_button_direction_selector & 0xF0u) {
		case 0x10:
			joy_p1 = joy_both_states & 0x0Fu;
			reset_bit(joy_p1, 5);
			set_bit(joy_p1, 4);
			break;
		case 0x20:
			joy_p1 = (joy_both_states & 0xF0u) >> 4u;
			set_bit(joy_p1, 5);
			reset_bit(joy_p1, 4);
			break;
		default:
			return 0xFF;
		}
		return joy_p1;
	}
	else {
		return 0xFF;
	}
}

void CONTROLS::write(const u16& addr, const u8& data) {
	switch (addr) {
	case JOY_P1_REGISTER:
		joy_button_direction_selector = data & 0x30u;
		break;
	default:
		mem->write(addr, data);
		break;
	}
}

void CONTROLS::handle_glwf_inputs(int key, int action) {
	switch (action) {
	case GLFW_PRESS:
	case GLFW_REPEAT:
		switch (key) {
		case GLFW_KEY_UP:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::Up));
			break;
		case GLFW_KEY_DOWN:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::Down));
			break;
		case GLFW_KEY_LEFT:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::Left));
			break;
		case GLFW_KEY_RIGHT:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::Right));
			break;
		case GLFW_KEY_A:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::A));
			break;
		case GLFW_KEY_S:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::B));
			break;
		case GLFW_KEY_ENTER:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::Start));
			break;
		case GLFW_KEY_BACKSPACE:
			reset_bit(joy_both_states, static_cast<u8>(GB_Key::Select));
			break;
		default: // GLFW PRESS
			break;
		}
		break;
	case GLFW_RELEASE:
		switch (key) {
		case GLFW_KEY_UP:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::Up));
			break;
		case GLFW_KEY_DOWN:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::Down));
			break;
		case GLFW_KEY_LEFT:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::Left));
			break;
		case GLFW_KEY_RIGHT:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::Right));
			break;
		case GLFW_KEY_A:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::A));
			break;
		case GLFW_KEY_S:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::B));
			break;
		case GLFW_KEY_ENTER:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::Start));
			break;
		case GLFW_KEY_BACKSPACE:
			set_bit(joy_both_states, static_cast<u8>(GB_Key::Select));
			break;
		default:// GLFW RELEASE
			break;
		}
	default: // switch action
		break;

	}
}

u8 CONTROLS::get_joypad_state() {
	if (joy_both_states != joy_both_states_old)
		joy_both_states_old = joy_both_states;
	return joy_both_states;
}

void CONTROLS::c_view() const {
	ImGui::Begin("Joypad");
	for (int i = 0; i < 8; ++i) {
		ImGui::Text("%1d", check_bit(joy_both_states, i));
		ImGui::SameLine();
	}
	ImGui::End();

}





