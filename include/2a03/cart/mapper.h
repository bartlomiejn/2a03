#ifndef INC_2A03_MAPPER_H
#define INC_2A03_MAPPER_H

#include <2a03/cart/ines.h>

namespace NES
{
namespace iNESv1
{
namespace Mapper
{
	class Base;
	
	enum Type
	{
		type_NROM,
		type_MMC1
	};
	
	/// Returns an appropriate mapper type for the cartridge provided.
	/// \param for_cartridge Cartridge to generate a mapper for.
	/// \return Returns a concrete mapper instance based on the ID in the
	/// cartridge.
	Mapper::Base *mapper(NES::iNESv1::Cartridge &for_cartridge);
	
	class Base
	{
	public:
		/// Initializes a Cartridge Mapper instance.
		/// \param cartridge Cartridge to use.
		explicit Base(Cartridge &cartridge) : cartridge(cartridge) {};
		
		virtual ~Base() = default;

		/// Reads a byte of memory at the provided address.
		virtual uint8_t read(uint16_t addr) = 0;
		
		/// Writes a byte of memory to the provided address.
		virtual void write(uint16_t addr, uint8_t val) = 0;
	protected:
		Cartridge &cartridge; ///< Cartridge to map.
	};
	
	class NROM : public Mapper::Base
	{
	public:
		/// Initializes an NROM (iNES Mapper 0) Cartridge Mapper
		/// instance.
		/// \param cartridge Cartridge to use.
		explicit NROM(Cartridge &cartridge);
		
		/// Reads a byte of memory at the provided address.
		uint8_t read(uint16_t addr) final;
		
		/// Writes a byte of memory to the provided address.
		void write(uint16_t addr, uint8_t val) final;
	};
	
	class MMC1 : public Mapper::Base
	{
	public:
		enum MMC1Register
		{ reg_main_ctrl, reg_l_chr_rom, reg_h_chr_rom, reg_prg_bank };
		enum PRGBankSwap { swap_h_prg_bank, swap_l_prg_bank };
		enum PRGBankSize { size_32k, size_16k };
		enum CHRBankSize { size_8k, size_4k };
		
		/// Initializes an MMC1 (iNES Mapper 1) Cartridge Mapper
		/// instance.
		/// \param cartridge Cartridge to use.
		explicit MMC1(Cartridge &cartridge);
		
		/// Reads a byte of memory at the provided address.
		uint8_t read(uint16_t addr) final;
		
		/// Writes a byte of memory to the provided address.
		void write(uint16_t addr, uint8_t val) final;
	private:
		// Shift register contents
		uint8_t shift_reg; 		///< Shift register (SR).
		uint8_t shift_count; 		///< Shift counter.
		
		// Main Control Register
		PRGBankSwap prg_bank_swap; 	///< Decides which PRG Bank is
						///< swappable.
		PRGBankSize prg_bank_sz; 	///< PRG bank size.
		CHRBankSize chr_bank_sz;	///< CHR bank size.
		
		// PRG ROM Bank Register
		uint8_t prg_bank; 		///< PRG ROM bank number.
		bool wram_enable;		///< Decides if WRAM is enabled.
		
		/// Resets the shift register.
		void reset_shift_reg();
		
		/// Shifts first bit of val to SR. If this is a fifth write
		/// uses `addr` to determine the internal register to send the
		/// SR contents to and resets the SR state.
		/// \param addr Address used for the shift.
		/// \param val Value of the register which is shifted into the
		/// SR.
		void set_shift_reg(uint16_t addr, uint8_t val);
		
		/// Returns the number of the accessed register based on the
		/// address used.
		int reg_number(uint16_t addr);
		
		/// Sets a register using contents from the shift register.
		/// \param reg_number Register number to select.
		void set_reg(int reg_number);
		
		/// Sets the main control register using contents from the
		/// shift register.
		void set_main_ctrl_reg(uint8_t value);
		
		/// Sets the PRG ROM bank register using contents from the
		/// shift register.
		void set_prg_bank_reg(uint8_t value);
		
		/// Reads from a 32k PRG ROM bank using provided address.
		/// \param addr Address to read from.
		/// \return Byte of memory.
		uint8_t read_32k_prg_bank(uint16_t addr);
		
		/// Reads from the low 16k PRG ROM bank using provided address.
		/// \param addr Address to read from.
		/// \return Byte of memory.
		uint8_t read_l_16k_prg_bank(uint16_t addr);
		
		/// Reads from the high 16k PRG ROM bank using provided address.
		/// \param addr Address to read from.
		/// \return Byte of memory.
		uint8_t read_h_16k_prg_bank(uint16_t addr);
	};
	
	class UnimplementedType {};
	class InvalidAddress {};
}
}
}

#endif //INC_2A03_MAPPER_H
