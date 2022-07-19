#ifndef APU_H
#define APU_H

#pragma once

class APU {
public:
	APU(MEMORY *m);

	u8 read(const u16 &addr);

	void write(const u16 &addr, const u8 &data) const;

	void write_u16(const u16 &addr, const u16 &data) const;

	void apu_view();

private:
	MEMORY *mem;
};

#endif