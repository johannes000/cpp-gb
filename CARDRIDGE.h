#pragma once
#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include "type.h"
#include <string>
#include <iostream>
#include <fstream>

class CARDRIDGE {
public:
	CARDRIDGE();
	~CARDRIDGE();

	void loadROM(const std::string& romFile);

public:
};


#endif