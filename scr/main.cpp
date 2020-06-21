// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include <iostream>
#include <stdexcept>

#include "gameboy.h"

int main(int argc, char *args[]) {
  try {
    gameboy gb;
    gb.start_emu();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}
