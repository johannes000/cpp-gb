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
	std::vector<u8> card(std::istreambuf_iterator<char>(is), {});
	this->card = card;
	fill_rom_info();

	return true;
}


void CARDRIDGE::fill_rom_info() {
	if (card.size() == 0)
		return;
	if (calcCheckSum() != card[0x14D]) {
		std::cout << "Checkrum FAIL!" << std::endl;
		return;
	}
	std::memcpy(rom_info.title, &card[0x134], 15);
	rom_info.cgb = (CGBFlag)card[0x143];
	rom_info.ctype = (CType)card[0x147];


	rom_info.romsize = (ROMSize)card[0x148];
	switch (rom_info.romsize) {
	case ROMSize::KB32:
		rom_info.rom_banks = 0;
		break;
	case ROMSize::KB64:
		rom_info.rom_banks = 4;
		break;
	case ROMSize::KB128:
		rom_info.rom_banks = 8;
		break;
	case ROMSize::KB256:
		rom_info.rom_banks = 16;
		break;
	case ROMSize::KB512:
		rom_info.rom_banks = 16;
		break;
	case ROMSize::MB1:
		switch (rom_info.ctype) {
		case CType::MBC1:
			rom_info.rom_banks = 63;
			break;
		default:
			rom_info.rom_banks = 64;
			break;
		}
		break;
	case ROMSize::MB2:
		switch (rom_info.ctype) {
		case CType::MBC1:
			rom_info.rom_banks = 125;
			break;
		default:
			rom_info.rom_banks = 128;
			break;
		}
		break;
	case ROMSize::MB4:
		rom_info.rom_banks = 256;
		break;
	case ROMSize::MB8:
		rom_info.rom_banks = 512;
		break;
	case ROMSize::MB1_1:
		rom_info.rom_banks = 72;
		break;
	case ROMSize::MB1_2:
		rom_info.rom_banks = 80;
		break;
	case ROMSize::MB1_5:
		rom_info.rom_banks = 96;
		break;
	default:
		break;
	}

	rom_info.ramsize = (RAMSize)card[0x149];
	switch (rom_info.ramsize) {
	case RAMSize::None:
		rom_info.ram_banks = 0;
		break;
	case RAMSize::KB2:
		rom_info.ram_banks = 0;
		break;
	case RAMSize::KB8:
		rom_info.ram_banks = 0;
		break;
	case RAMSize::KB32:
		rom_info.ram_banks = 4;
		break;
	case RAMSize::KB128:
		rom_info.ram_banks = 16;
		break;
	case RAMSize::KB64:
		rom_info.ram_banks = 8;
		break;
	default:
		break;
	}


}

void CARDRIDGE::printRom() {
	int i = 0;
	for (auto s : card) {
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
			checkSum = checkSum - card[i++] - 1;

	}
	return checkSum;
}

u8 CARDRIDGE::read(const u16& addr) {
	return card.at(addr);
}

u8* CARDRIDGE::read_ptr(const u16& addr) {
	return &card.at(addr);
}
void CARDRIDGE::write(const u16& addr, const u8& data) {
	card.at(addr) = data;

}