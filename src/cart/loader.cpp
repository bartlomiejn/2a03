#include <2a03/cart/loader.h>
#include <fstream>
#include <iostream>

using namespace NES::iNESv1;

static const unsigned int prg_rom_pagesz = 16384; ///< PRG ROM page size - 16KB.
static const unsigned int chr_rom_pagesz = 8192;  ///< CHR ROM page size - 8KB.
static const unsigned int trainer_abssz = 512;	  ///< Trainer absolute size - 512B.

/// Checks if magic number is valid. Modifies the input iterator.
static bool is_magic_valid(std::string::iterator &iter)
{
	std::string magic;
	for (int i = 0; i < 4; i++)
	{
		magic += *iter;
		iter++;
	}
	return magic == "NES\x1A";
}

/// Generates an iNESv1 header. Modifies the input iterator.
static Header get_inesv1_header(std::string::iterator &iter)
{
	auto prg_rom_sz = static_cast<uint8_t>(*iter);
	iter++;
	auto chr_rom_sz = static_cast<uint8_t>(*iter);
	iter++;
	Byte6 flags_6 = { .byte = static_cast<uint8_t>(*iter) };
	iter++;
	Byte7 flags_7 = { .byte = static_cast<uint8_t>(*iter) };
	iter++;
	auto prg_ram_sz = static_cast<uint8_t>(*iter);
	iter++;
	Byte9 flags_9 = { .byte = static_cast<uint8_t>(*iter) };
	iter++;
	return Header(
		prg_rom_sz, chr_rom_sz, flags_6, flags_7, prg_ram_sz, flags_9);
}

Cartridge NES::iNESv1::load(std::string &&filename)
{
	// Attempt to open the file
	std::ifstream fstream(filename);
	if (!fstream)
	{
		std::cerr << "Invalid cartridge filename to load: " << filename
			<< "." << std::endl;
		throw InvalidFile();
	}
	
	// Get the file contents
	std::string fstring(
		(std::istreambuf_iterator<char>(fstream)),
		std::istreambuf_iterator<char>());
	std::string::iterator fstr_iter = fstring.begin();
	
	// Verify the iNES magic number
	if (!is_magic_valid(fstr_iter))
	{
		std::cerr << "Invalid magic number in " << filename
			<< " header. Probably not an iNES ROM." << std::endl;
		throw InvalidMagicNumber();
	}

	// Generate header based on provided file
	Header header = get_inesv1_header(fstr_iter);
	
	// Generate cartridge object based on the header
	unsigned int trainer_sz = header.flags_6.has_trainer ? trainer_abssz : 0;
	unsigned int prg_rom_sz = prg_rom_pagesz * header.prg_rom_pages;
	unsigned int chr_rom_sz = chr_rom_pagesz * header.chr_rom_pages;
	Cartridge cart(
		header,
		std::make_unique<uint8_t[]>(trainer_sz),
		std::make_unique<uint8_t[]>(prg_rom_sz),
		std::make_unique<uint8_t[]>(chr_rom_sz));

	// If trainer is available, copy trainer memory first
	if (header.flags_6.has_trainer)
	{
		for (int i = 0; i < trainer_sz; i++)
		{
			cart.trainer[i] = static_cast<uint8_t>(*fstr_iter);
			fstr_iter++;
		}
	}
	
	// Copy PRG ROM
	for (int i = 0; i < prg_rom_sz; i++)
	{
		cart.prg_rom[i] = static_cast<uint8_t>(*fstr_iter);
		fstr_iter++;
	}
	
	// Copy CHR ROM
	for (int i = 0; i < chr_rom_sz; i++)
	{
		cart.chr_rom[i] = static_cast<uint8_t>(*fstr_iter);
		fstr_iter++;
	}
	
	std::cout << filename << " header loaded successfully."
		<< std::endl;
	
	return cart;
}