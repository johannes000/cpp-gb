#include "CARDRIDGE.h"


CARDRIDGE::CARDRIDGE(MEMORY* m) {
	mem = m;
}
CARDRIDGE::~CARDRIDGE() {

}

bool CARDRIDGE::loadROM(const char* romFile) {
	std::ifstream is(romFile, std::ios::binary);
	if (!is) {
		std::cerr << "Kann die Datei nicht oeffnen : " << romFile << std::endl;
		return false;
	}
	std::vector<u8> data(std::istreambuf_iterator<char>(is), {});
	rom = data;

	getRomInfo();
	romintomem();

	return true;
}

void CARDRIDGE::romintomem() {
	romintomem(0x0100, 0x7FFF, 0x0100);
}

void CARDRIDGE::romintomem(u16 romBeg, u16 romEnd, u16 dest) {
	auto b = (rom.begin() + romBeg);
	auto e = (rom.begin() + romEnd);
	u16 i = dest;
	while (b != e) {
		mem->write(i++, *(b++));
	}
}

void CARDRIDGE::getRomInfo() {
	if (rom.size() == 0)
		return;
	if (calcCheckSum() != rom[0x14D]) {
		std::cout << "Checkrum FAIL!" << std::endl;
		return;
	}
	std::memcpy(title, &rom[0x134], 15);
	cgb = (CGBFlag)rom[0x143];
	ctype = (CType)rom[0x147];
	romsize = (ROMSize)rom[0x148];
	ramsize = (RAMSize)rom[0x149];
}

void CARDRIDGE::printRom() {
	int i = 0;
	for (auto s : rom) {
		if ((i++) % 255 == 0) {
			std::cout << std::endl;
		}
		std::cout << static_cast<u8>(s) << " ";
	}
}

u8 CARDRIDGE::readAddr(u16 addr) {
	return rom[addr];
}

u8 CARDRIDGE::calcCheckSum() {
	if (checkSum == 0) {
		int i = 0x0134;
		while (i <= 0x014C)
			checkSum = checkSum - rom[i++] - 1;

	}
	return checkSum;
}
