#pragma once
#ifndef TYPE_H_GUARD
#define TYPE_H_GUARD

#include <cstdint>

typedef unsigned char u8;
typedef unsigned short u16;

typedef signed char i8;
typedef signed short i16;

const enum class Flag {
	z = 7, // Zero Flag
	n = 6, // Add/Sub-Flag 
	h = 5, // Half Carry Flag
	c = 4, // Carry Flag
};

typedef union {
	struct {
		u8 low;
		u8 high;
	};
	u16 ganz;
}Register;

#endif // !TYPE_H_GUARD

