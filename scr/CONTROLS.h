//
// Created by knaub on 25.06.2020.
//

#ifndef CPP_GB_CONTROLS_H
#define CPP_GB_CONTROLS_H

#include "type.h"
#include "MEMORY.h"
// TODO: Platform Unabhängig machen
#include "../win/Renderer.h"

enum class GB_Key {
	// Bit 7 - Not used
	// Bit 6 - Not used
	// Bit 5 - P15 Select Button Keys      (0=Select)
	// Bit 4 - P14 Select Direction Keys   (0=Select)
	// Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
	// Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
	// Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
	// Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)
	Right = 0,
	Left = 1,
	Up = 2,
	Down = 3,
	A = 4,
	B = 5,
	Select = 6,
	Start = 7,

};

class MEMORY;

class CONTROLS {
public:
	explicit CONTROLS(MEMORY *m);

	void write(const u16 &addr, const u8 &data);

	void handle_glwf_inputs(int key, int action);

	[[nodiscard]] u8 read(const u16 &addr) const;

	u8 get_joypad_state();

private:
	MEMORY *mem;

	u8 joy_button_direction_selector;
	u8 joy_both_states;
	u8 joy_both_states_old;
};

#endif // CPP_GB_CONTROLS_H
