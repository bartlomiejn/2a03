#include <iostream>
#include <2a03/cart/mapper.h>

using namespace NES::iNESv1;

Mapper::Base *Mapper::mapper(NES::iNESv1::Cartridge &cartridge)
{
	uint8_t id = (cartridge.header.flags_7.nib_h << 4)
		     | (cartridge.header.flags_6.nib_l);
	switch (id)
	{
		case Mapper::type_NROM:
			return new NES::iNESv1::Mapper::NROM(cartridge);
		case Mapper::type_MMC1:
			return new NES::iNESv1::Mapper::MMC1(cartridge);
		default:
			std::cerr << "Unimplemented mapper type: " << std::hex
				<< id << "." << std::endl;
			throw UnimplementedType();
	}
}

// NROM

Mapper::NROM::NROM(Cartridge &cartridge) : Mapper::Base(cartridge) {}

uint8_t Mapper::NROM::read(uint16_t addr)
{
	switch (addr)
	{
		case 0x6000 ... 0x7FFF:
			return cartridge.prg_ram[addr - 0x6000];
		case 0x8000 ... 0xBFFF:
			// Low 16KB PRG ROM
			return cartridge.prg_rom[addr - 0x8000];
		case 0xC000 ... 0xFFFF:
			// High 16KB PRG ROM, or mirrored low if 16KB
			if (cartridge.header.prg_rom_pages == 1)
				return cartridge.prg_rom[addr - 0xC000];
			else
				return cartridge.prg_rom[addr - 0x8000];
		default:
			std::cout << "Invalid NROM Mapper memory access: $"
				<< addr << std::endl;
			return 0x0;
	}
}

void Mapper::NROM::write(uint16_t addr, uint8_t val)
{
	switch (addr)
	{
		case 0x6000 ... 0x7FFF:
			cartridge.prg_ram[addr - 0x6000] = val;
			break;
		default:
			break;
	}
}

// MMC1

Mapper::MMC1::MMC1(Cartridge &cartridge) :
	Mapper::Base(cartridge),
	shift_reg(0),
	shift_count(0),
	prg_bank_swap(PRGBankSwap(0)),
	prg_bank_sz(PRGBankSize(0)),
	chr_bank_sz(CHRBankSize(0)),
	prg_bank(0),
	wram_enable(0),
{}

uint8_t Mapper::MMC1::read(uint16_t addr)
{
	switch (addr)
	{
		case 0x6000 ... 0x7FFF:
			// TODO: PRG RAM bankswitching
			return cartridge.prg_ram[addr - 0x6000];
		case 0x8000 ... 0xBFFF:
//			uint16_t l_prg_offset = addr - (uint16_t)0x8000;
//			uint32_t l_prg_addr =
//				l_prg_bank * prg_rom_pagesz + l_prg_offset;
//
//			return cartridge.prg_rom[l_prg_addr];
		case 0xC000 ... 0xFFFF:
//			uint16_t h_prg_offset = addr - (uint16_t)0xC000;
//			uint32_t h_prg_addr =
//				h_prg_bank * prg_rom_pagesz + h_prg_offset;
//
//			return cartridge.prg_rom[h_prg_addr];
		default:
			std::cout << "Invalid MMC1 Mapper memory access: $"
				<< addr << std::endl;
			return 0x0;
	}
}

void Mapper::MMC1::write(uint16_t addr, uint8_t val)
{
	switch (addr)
	{
		case 0x6000 ... 0x7FFF:
			cartridge.prg_ram[addr - 0x6000] = val;
			break;
		case 0x8000 ... 0xFFFF:
			if ((val & 0x80) != 0)
			{
				reset_shift_reg();
				// TODO: Do we reset bank state here as well?
			}
			else
			{
				shift_reg |= (val & 0b1) < shift_count;
				shift_count++;
				
				if (shift_count == 5)
				{
					set_reg(reg_number(addr));
					reset_shift_reg();
				}
			}
			break;
		default:
			std::cerr << "Invalid address passed to MMC1: $"
				  << std::hex << "." << std::endl;
			throw InvalidAddress();
	}
}

void Mapper::MMC1::reset_shift_reg()
{
	shift_reg = 0x0;
	shift_count = 0x0;
}

int Mapper::MMC1::reg_number(uint16_t addr)
{
	switch (addr)
	{
		case 0x8000 ... 0x9FFF:
			return 0;
		case 0xA000 ... 0xBFFF:
			return 1;
		case 0xC000 ... 0xDFFF:
			return 2;
		case 0xE000 ... 0xFFFF:
			return 3;
		default:
			std::cerr << "Invalid address passed to MMC1: $"
				<< std::hex << "." << std::endl;
			throw InvalidAddress();
	}
}

void Mapper::MMC1::set_reg(int reg_number)
{
	switch (reg_number)
	{
		case reg_main_control:
			set_main_ctrl_reg(shift_reg);
			break;
		case reg_l_chrrom:
			// TODO: L CHR ROM register.
			std::cerr << "Unimplemented L CHR ROM Bank Register "
				"access." << std::endl;
			break;
		case reg_h_chrrom:
			// TODO: R CHR ROM register.
			std::cerr << "Unimplemented R CHR ROM Bank Register "
				     "access." << std::endl;
			break;
		case reg_prg_bank:
			set_prg_bank_reg(shift_reg);
			break;
	}
}

void Mapper::MMC1::set_main_ctrl_reg(uint8_t value)
{
	// TODO: PPU mirroring type for Main Control Register.
	switch (value & 0b11)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			break;
	}
	
	prg_bank_swap = PRGBankSwap((value >> 2) & 0b1);
	prg_bank_sz = PRGBankSize((value >> 3) & 0b1);
	chr_bank_sz = CHRBankSize((value >> 4) & 0b1);
}

void Mapper::MMC1::set_prg_bank_reg(uint8_t value)
{
	prg_bank = (uint8_t)(value & 0b1111);
	wram_enable = (bool)((value & 0b10000) >> 4);
}