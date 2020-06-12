#include "CARDRIDGE.h"


CARDRIDGE::CARDRIDGE() {

}
CARDRIDGE::~CARDRIDGE() {

}

bool CARDRIDGE::loadROM(const char* romFile) {
	std::ifstream is(romFile, std::ios::binary);
	if (!is) {
		std::cerr << "Kann die Datei nicht oeffnen : " << romFile << std::endl;
		return false;
	}
	std::vector<u8> rom(std::istreambuf_iterator<char>(is), {});
	this->rom = rom;
	getRomInfo();

	return true;
}


void CARDRIDGE::getRomInfo() {
	if (rom.size() == 0)
		return;
	if (calcCheckSum() != rom[0x14D]) {
		std::cout << "Checkrum FAIL!" << std::endl;
		return;
	}
	std::memcpy(rom_info.title, &rom[0x134], 15);
	rom_info.cgb = (CGBFlag)rom[0x143];
	rom_info.ctype = (CType)rom[0x147];
	rom_info.romsize = (ROMSize)rom[0x148];
	rom_info.ramsize = (RAMSize)rom[0x149];
}

void CARDRIDGE::printRom() {
	int i = 0;
	for (auto s : rom) {
		if ((i++) % 255 == 0) {
			std::cout << std::endl;
		}
		std::cout << std::hex << static_cast<int>(s) << " ";
	}
}

u8 CARDRIDGE::calcCheckSum() {
	if (checkSum == 0) {
		int i = 0x0134;
		while (i <= 0x014C)
			checkSum = checkSum - rom[i++] - 1;

	}
	return checkSum;
}

u8 CARDRIDGE::read(const u16& addr) {
	return rom.at(addr);
}

u8* CARDRIDGE::read_ptr(const u16& addr) {
	return &rom.at(addr);
}
void CARDRIDGE::write(const u16& addr, const u8& data) {
	rom.at(addr) = data;

}