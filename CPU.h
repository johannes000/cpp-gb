#pragma once
#ifndef CPU_H_GUARD
#define CPU_H_GUARD

#include <cstdint>
#include <iostream>

class CPU {
public:
	CPU();

private:

	//REGISTER
	typedef union {
		struct {
			uint8_t low;
			uint8_t high;
		};
		uint16_t ganz;
	}Register;

	Register AF; // Accumulator & Flags
	Register BC; // BC
	Register DE; // DE
	Register HL; // HL

	uint16_t sp; // stack pointer
	uint16_t pc; // program counter/pointer

public:
	uint16_t getAF();
	uint16_t getBC();
	uint16_t getDE();
	uint16_t getHL();
	uint8_t getA();
	uint8_t getF();
	uint8_t getC();
	uint8_t getD();
	uint8_t getE();
	uint8_t getH();
	uint8_t getL();

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

public:
	void OPCODEONE();
};
#endif // !CPU_H_GUARD
