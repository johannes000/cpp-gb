// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include <SDL.h>
#include <iostream>
#include "gameboy.h"

int main(int argc, char* args[]) {
	gameboy gb;
	gb.startemu();
	return 0;
}
