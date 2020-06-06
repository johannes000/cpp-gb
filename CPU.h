#pragma once
#ifndef CPU_H_GUARD
#define CPU_H_GUARD

#include <iostream>
#include "type.h"
#include <vector>

class CPU {
public:
	CPU();

private:

	//REGISTER
	Register AF; // Accumulator & Flags
	Register BC; // BC
	Register DE; // DE
	Register HL; // HL

	Register sp; // stack pointer
	Register pc; // program counter/pointer

public:
	u16 getAF() const;
	u16 getBC() const;
	u16 getDE() const;
	u16 getHL() const;

	u8 getA() const;
	u8 getF() const;
	u8 getC() const;
	u8 getD() const;
	u8 getE() const;
	u8 getH() const;
	u8 getL() const;

	//FLAGS
public:
	enum flag {
		z_flag = 7, // Zero Flag
		n_flag = 6, // Add/Sub-Flag 
		h_flag = 5, // Half Carry Flag
		c_flag = 4, // Carry Flag
	};

	void setFlag(const flag);		//setze flag auf true egal was
	void clearFlag(const flag);		//setze flag auf false egal was
	void toggleFlag(const flag);	//toggled die flag
	bool checkFlag(const flag);		//gibt flag status wieder

private:
	struct instruction {
		const char* mnemonic;
		u8 bytes;
		u8 cycles;
	};


	//OPCODES
	//TODO: OPJumpliste füllen mit pointern für jeden opcode
	//LD und ADD und andere häufig benutze ops einzelnen funktionen zuteielen
	//die opcode funktionen führen diese dann mit anderen parametern aus
public:
	void OPCODEONE();

	std::vector<instruction> instructions{
		{"NOP",2,2},
		{"NOP",2,2},
		{"NOP",2,2},
	};
	void parseOPCode(u8&);



};
#endif // !CPU_H_GUARD
