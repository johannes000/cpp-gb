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
	romintomem();
	return true;
}

void CARDRIDGE::romintomem() {
	auto b = (rom.begin() + 0x0100);
	auto e = (rom.begin() + 0x3FFF);
	u16 i = 0x0100;
	while (b != e) {
		mem->write(i++, *(b++));
	}
}

void CARDRIDGE::getRomInfo() {
	if (rom.size() == 0)
		return;

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
