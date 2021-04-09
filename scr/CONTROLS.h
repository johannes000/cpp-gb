//
// Created by knaub on 25.06.2020.
//

#ifndef CPP_GB_CONTROLS_H
#define CPP_GB_CONTROLS_H

#include "type.h"
#include "MEMORY.h"
#include "Renderer.h"

class MEMORY;

class CONTROLS {
 public:
  CONTROLS(MEMORY *m);

  void write(const u16 &addr, const u8 &data);

  void handle_glwf_inputs(int key, int action);

  [[nodiscard]] u8 read(const u16 &addr) const;

  u8 get_joypad_state();

  void c_view() const;

 private:
  MEMORY *mem;

  u8 joy_button_direction_selector;
  u8 joy_both_states;
  u8 joy_both_states_old;
};

#endif  // CPP_GB_CONTROLS_H
