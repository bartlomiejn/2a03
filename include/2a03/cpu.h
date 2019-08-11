#ifndef INC_2A03_CPU_H
#define INC_2A03_CPU_H

#include <cstdint>
#include <functional>
#include <2a03/ppu.h>
#include <2a03/apu.h>

namespace NES
{
	enum AddressingMode
	{
		abs,		///< Absolute
		abs_x,		///< Absolute indexed with X
		abs_y,		///< Absolute indexed with Y
		imm,		///< Immediate
		zp,		///< Zero Page
		zp_x,		///< Zero Page indexed with X
		mode_zp_y,	///< Zero Page indexed with Y
		idx_ind_x,	///< Indexed indirect with X
		ind_idx_y,	///< Indirect indexed with Y
		ind		///< Indirect (used only with JMP)
	};
	
	/// Status Register P union representation
	union StatusRegister
	{
		struct
		{
			bool C : 1; 	///< Carry
			bool Z : 1; 	///< Zero
			bool I : 1; 	///< Interrupt disable
			bool D : 1; 	///< Decimal
			uint8_t B : 2; 	///< Has no effect on CPU, but certain
					///< instructions set it
			bool V : 1;	///< Overflow
			bool N : 1; 	///< Negative (Set using the 7-th bit of
					///< a result value)
		};
		uint8_t status;
	};
	
	/// Ricoh 2A03 CPU emulator
	class CPU
	{
	public:
		// Registers
		// http://wiki.nesdev.com/w/index.php/CPU_registers
		
		uint8_t A; 		///< Accumulator
		uint8_t X, Y; 		///< Index registers
		uint16_t PC;		///< Program counter
		uint8_t S;		///< Stack pointer
		StatusRegister P;	///< Status register
		
		uint8_t ram[0x800]; 	///< RAM
		
		/// Starts the CPU.
		void power_up();
		
		/// Resets the CPU.
		void reset();
		
		/// Executes the next instruction.
		void execute();
	private:
		// Memory access
		
		/// Reads 8 bits of memory at the provided address.
		/// \param addr Address to read from.
		/// \return Byte that has been read.
		uint8_t read(uint16_t addr);
		
		/// Reads 16 bits of memory at the provided address.
		/// \param addr Address to read from
		/// \param is_zp If it's a zero-page address, wrap the most
		/// -significant byte around zero-page.
		/// \return 2 bytes that have been read.
		uint16_t read16(uint16_t addr, bool is_zp = false);
		
		/// Writes a value to the provided address.
		/// \param addr Address to write the value to.
		/// \param val Value to write.
		void write_to(uint16_t addr, uint8_t val);
		
		// Addressing mode functions
		// http://www.obelisk.me.uk/6502/addressing.html
		
		/// Returns the address of the parameter based on the addressing
		/// mode and increments PC based on param length.
		/// \param mode Addressing mode.
		/// \return Parameter address.
		uint16_t param_addr(NES::AddressingMode mode);
		
		/// Retrieves the current instruction parameter based on
		/// the addressing mode and increments PC based on parameter
		/// length.
		/// \param mode Addressing mode to use.
		/// \return Parameter for current instruction.
		uint8_t get_param(AddressingMode mode);
		
		// Auxiliary functions
		
		/// Rotates left the value once.
		uint8_t rot_l(uint8_t value);
		
		/// Rotates right the value once.
		uint8_t rot_r(uint8_t value);
		
		// Instructions
		// http://www.6502.org/tutorials/6502opcodes.html - Docs
		// http://www.qmtpro.com/~nes/misc/nestest.log - Behaviour
		
		// Control transfer
		
		/// Transfer program execution.
		/// \param mode Addressing mode to use.
		void JMP(AddressingMode mode);
		
		/// Jump to subroutine. Pushes the address - 1 of next op on the
		/// stack before transfering control.
		void JSR();
		
		/// Return from subroutine. Pulls two bytes off the stack (low
		/// byte first) and transfers control to that address.
		void RTS();
		
		// Arithmetic / logical
		
		/// Performs bitwise OR with the accumulator.
		/// \param mode Addressing mode of the parameter.
		void ORA(AddressingMode mode);
		
		/// Rotate left the accumulator.
		void ROLA();
		
		/// Rotate left a value at the specified address.
		/// \param mode Addressing mode to use.
		void ROL(AddressingMode mode);
		
		/// Rotate right the accumulator.
		void RORA();
		
		/// Rotate right a value at the specified address.
		/// \param mode Addressing mode to use.
		void ROR(AddressingMode mode);
		
		// Load / store
		
		/// Load register `reg` with memory.
		/// \param reg Register address to load memory to.
		/// \param addr_fn Addressing mode to use.
		void LD(uint8_t &reg, AddressingMode mode);
		
		/// Store register.
		/// \param reg Value to store.
		/// \param mode Addressing mode to use.
		void ST(uint8_t reg, AddressingMode mode);
		
		// Stack
		
		/// Transfer from `reg` to `reg_2`.
		/// \param reg_from Register to transfer from.
		/// \param reg_to Register to transfer to.
		void T(uint8_t &reg_from, uint8_t &reg_to);
		
		/// Push value to stack.
		void PH(uint8_t value);
		
		/// Push P to stack.
		void PH(StatusRegister &p);
		
		/// Pull value from stack.
		/// \param reg_to Register to pull the value to.
		void PL(uint8_t &reg_to);
		
		/// Pull value to status register.
		/// \param p P register to pull the value to.
		void PL(StatusRegister &p);
	};
}

#endif //INC_2A03_CPU_H
