// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <SDL.h>
#include "PPU.h"



int main(int argc, char* args[]){
	PPU ppu;
				ppu.createWindow();
	ppu.renderScreen();
	return 0;
}
