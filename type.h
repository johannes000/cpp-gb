#pragma once
#ifndef TYPE_H_GUARD
#define TYPE_H_GUARD

#include <cstdint>

typedef unsigned char u8;
typedef unsigned short u16;

typedef signed char i8;
typedef signed short i16;


typedef union {
	struct {
		u8 low;
		u8 high;
	};
	u16 ganz;
}Register;



#endif // !TYPE_H_GUARD

