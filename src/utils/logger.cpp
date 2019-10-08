#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <2a03/utils/logger.h>

using namespace NES;

CPULogger::CPULogger(CPU &cpu, MemoryBus &bus) :
	cpu(cpu),
	bus(bus),
	logs(),
	is_cout_each_line_enabled(true)
{}

void CPULogger::log()
{
	using namespace std;
	
	string line;
	stringstream ss;
	int opcode;
	
	// PC as a 4-char wide hex string.
	ss << setfill('0') << setw(4) << hex << (int)cpu.PC << "  ";
	line += string(ss.str());
	ss.str(string());
	
	// Current opcode as hex.
	opcode = (int)bus.read(cpu.PC);
	ss << setfill('0') << setw(2) << hex << opcode << " ";
	line += string(ss.str());
	ss.str(string());
	
	// TODO: Fill opcode parameters here instead of empty space.
	line += "       ";
	
	// Decode opcode to string.
	line += decode(opcode);
	
	// TODO: Decode opcode param / addressing mode here.
	line += "                             ";
	
	// CPU register status.
	ss << "A:" << setfill('0') << setw(2) << hex << (int)cpu.A << " ";
	ss << "X:" << setfill('0') << setw(2) << hex << (int)cpu.X << " ";
	ss << "Y:" << setfill('0') << setw(2) << hex << (int)cpu.Y << " ";
	ss << "P:" << setfill('0') << setw(2) << hex << (int)cpu.P.status
		<< " ";
	ss << "SP:" << setfill('0') << setw(2) << hex << (int)cpu.S << " ";
	ss << "PPU:???,???" << " ";
	ss << "CYC:" << dec << (int)cpu.cycles;
	line += string(ss.str());
	ss.str(string());
	
	// Convert to uppercase chars.
	transform(line.begin(), line.end(), line.begin(),
		[](char c) -> char { return (char)toupper(c); });
	
	// Push to log storage.
	logs.push_back(line);
	
	// Push to cout if requested.
	if (is_cout_each_line_enabled)
		cout << line << endl;
}

std::string CPULogger::decode(uint8_t opcode)
{
	switch (opcode) {
		case 0x10: return "BPL";
		case 0x30: return "BMI";
		case 0x50: return "BVC";
		case 0x70: return "BVS";
		case 0x90: return "BCC";
		case 0xB0: return "BCS";
		case 0xD0: return "BNE";
		case 0xF0: return "BEQ";
		case 0x4C: return "JMP";
		case 0x6C: return "JMP";
		case 0x20: return "JSR";
		case 0x60: return "RTS";
		case 0x40: return "RTI";
		case 0x69: return "ADC";
		case 0x65: return "ADC";
		case 0x75: return "ADC";
		case 0x6D: return "ADC";
		case 0x7D: return "ADC";
		case 0x79: return "ADC";
		case 0x61: return "ADC";
		case 0x71: return "ADC";
		case 0x29: return "AND";
		case 0x25: return "AND";
		case 0x35: return "AND";
		case 0x2D: return "AND";
		case 0x3D: return "AND";
		case 0x39: return "AND";
		case 0x21: return "AND";
		case 0x31: return "AND";
		case 0x0A: return "ASL A";
		case 0x06: return "ASL";
		case 0x16: return "ASL";
		case 0x0E: return "ASL";
		case 0x1E: return "ASL";
		case 0x24: return "BIT";
		case 0x2C: return "BIT";
		case 0xC9: return "CMP";
		case 0xC5: return "CMP";
		case 0xD5: return "CMP";
		case 0xCD: return "CMP";
		case 0xDD: return "CMP";
		case 0xD9: return "CMP";
		case 0xC1: return "CMP";
		case 0xD1: return "CMP";
		case 0xE0: return "CPX";
		case 0xE4: return "CPX";
		case 0xEC: return "CPX";
		case 0xC0: return "CPY";
		case 0xC4: return "CPY";
		case 0xCC: return "CPY";
		case 0xC6: return "DEC";
		case 0xD6: return "DEC";
		case 0xCE: return "DEC";
		case 0xDE: return "DEC";
		case 0x49: return "EOR";
		case 0x45: return "EOR";
		case 0x55: return "EOR";
		case 0x4D: return "EOR";
		case 0x5D: return "EOR";
		case 0x59: return "EOR";
		case 0x41: return "EOR";
		case 0x51: return "EOR";
		case 0x18: return "CLC";
		case 0x38: return "SEC";
		case 0x58: return "CLI";
		case 0x78: return "SEI";
		case 0xB8: return "CLV";
		case 0xD8: return "CLD";
		case 0xF8: return "SED";
		case 0x4A: return "LSR A";
		case 0x46: return "LSR";
		case 0x56: return "LSR";
		case 0x4E: return "LSR";
		case 0x5E: return "LSR";
		case 0x09: return "ORA";
		case 0x05: return "ORA";
		case 0x15: return "ORA";
		case 0x0D: return "ORA";
		case 0x1D: return "ORA";
		case 0x19: return "ORA";
		case 0x01: return "ORA";
		case 0x11: return "ORA";
		case 0x2A: return "ROL A";
		case 0x26: return "ROL";
		case 0x36: return "ROL";
		case 0x2E: return "ROL";
		case 0x3E: return "ROL";
		case 0x6A: return "ROR A";
		case 0x66: return "ROR";
		case 0x76: return "ROR";
		case 0x6E: return "ROR";
		case 0x7E: return "ROR";
		case 0xE9: return "SBC";
		case 0xE5: return "SBC";
		case 0xF5: return "SBC";
		case 0xED: return "SBC";
		case 0xFD: return "SBC";
		case 0xF9: return "SBC";
		case 0xE1: return "SBC";
		case 0xF1: return "SBC";
		case 0xA9: return "LDA";
		case 0xA5: return "LDA";
		case 0xB5: return "LDA";
		case 0xAD: return "LDA";
		case 0xBD: return "LDA";
		case 0xB9: return "LDA";
		case 0xA1: return "LDA";
		case 0xB1: return "LDA";
		case 0xA2: return "LDX";
		case 0xA6: return "LDX";
		case 0xB6: return "LDX";
		case 0xAE: return "LDX";
		case 0xBE: return "LDX";
		case 0xA0: return "LDY";
		case 0xA4: return "LDY";
		case 0xB4: return "LDY";
		case 0xAC: return "LDY";
		case 0xBC: return "LDY";
		case 0x85: return "STA";
		case 0x95: return "STA";
		case 0x8D: return "STA";
		case 0x9D: return "STA";
		case 0x99: return "STA";
		case 0x81: return "STA";
		case 0x91: return "STA";
		case 0x86: return "STX";
		case 0x96: return "STX";
		case 0x8E: return "STX";
		case 0x84: return "STY";
		case 0x94: return "STY";
		case 0x8C: return "STY";
		case 0xAA: return "TAX";
		case 0x8A: return "TXA";
		case 0xA8: return "TAY";
		case 0x98: return "TYA";
		case 0xCA: return "DEX";
		case 0xE8: return "INX";
		case 0x88: return "DEY";
		case 0xC8: return "INY";
		case 0x9A: return "TXS";
		case 0xBA: return "TSX";
		case 0x48: return "PHA";
		case 0x08: return "PHP";
		case 0x68: return "PLA";
		case 0x28: return "PLP";
		case 0xEA: return "NOP";
		default: return "???";
	}
}

std::optional<AddressingMode> CPULogger::addr_mode(uint8_t opcode)
{
	switch (opcode) {
		// De facto mode is relative for each conditional branch opcode
		case 0x10: return { AddressingMode::imm };
		case 0x30: return { AddressingMode::imm };
		case 0x50: return { AddressingMode::imm };
		case 0x70: return { AddressingMode::imm };
		case 0x90: return { AddressingMode::imm };
		case 0xB0: return { AddressingMode::imm };
		case 0xD0: return { AddressingMode::imm };
		case 0xF0: return { AddressingMode::imm };
		case 0x4C: return { AddressingMode::abs };
		case 0x6C: return { AddressingMode::ind };
		case 0x20: return { AddressingMode::abs };
		case 0x60: return std::nullopt;
		case 0x40: return std::nullopt;
		case 0x69: return { AddressingMode::imm };
		case 0x65: return { AddressingMode::zp };
		case 0x75: return { AddressingMode::zp_x };
		case 0x6D: return { AddressingMode::abs };
		case 0x7D: return { AddressingMode::abs_x };
		case 0x79: return { AddressingMode::abs_y };
		case 0x61: return { AddressingMode::idx_ind_x };
		case 0x71: return { AddressingMode::ind_idx_y };
		case 0x29: return { AddressingMode::imm };
		case 0x25: return { AddressingMode::zp };
		case 0x35: return { AddressingMode::zp_x };
		case 0x2D: return { AddressingMode::abs };
		case 0x3D: return { AddressingMode::abs_x };
		case 0x39: return { AddressingMode::abs_y };
		case 0x21: return { AddressingMode::idx_ind_x };
		case 0x31: return { AddressingMode::ind_idx_y };
		case 0x0A: return std::nullopt;
		case 0x06: return { AddressingMode::zp };
		case 0x16: return { AddressingMode::zp_x };
		case 0x0E: return { AddressingMode::abs };
		case 0x1E: return { AddressingMode::abs_x };
		case 0x24: return { AddressingMode::zp };
		case 0x2C: return { AddressingMode::abs };
		case 0xC9: return { AddressingMode::imm };
		case 0xC5: return { AddressingMode::zp };
		case 0xD5: return { AddressingMode::zp_x };
		case 0xCD: return { AddressingMode::abs };
		case 0xDD: return { AddressingMode::abs_x };
		case 0xD9: return { AddressingMode::abs_y };
		case 0xC1: return { AddressingMode::idx_ind_x };
		case 0xD1: return { AddressingMode::ind_idx_y };
		case 0xE0: return { AddressingMode::imm };
		case 0xE4: return { AddressingMode::zp };
		case 0xEC: return { AddressingMode::abs };
		case 0xC0: return { AddressingMode::imm };
		case 0xC4: return { AddressingMode::zp };
		case 0xCC: return { AddressingMode::abs };
		case 0xC6: return { AddressingMode::zp };
		case 0xD6: return { AddressingMode::zp_x };
		case 0xCE: return { AddressingMode::abs };
		case 0xDE: return { AddressingMode::abs_x };
		case 0x49: return { AddressingMode::imm };
		case 0x45: return { AddressingMode::zp };
		case 0x55: return { AddressingMode::zp_x };
		case 0x4D: return { AddressingMode::abs };
		case 0x5D: return { AddressingMode::abs_x };
		case 0x59: return { AddressingMode::abs_y };
		case 0x41: return { AddressingMode::idx_ind_x };
		case 0x51: return { AddressingMode::ind_idx_y };
		case 0x18: return std::nullopt;
		case 0x38: return std::nullopt;
		case 0x58: return std::nullopt;
		case 0x78: return std::nullopt;
		case 0xB8: return std::nullopt;
		case 0xD8: return std::nullopt;
		case 0xF8: return std::nullopt;
		case 0x4A: return std::nullopt;
		case 0x46: return { AddressingMode::zp };
		case 0x56: return { AddressingMode::zp_x };
		case 0x4E: return { AddressingMode::abs };
		case 0x5E: return { AddressingMode::abs_x };
		case 0x09: return { AddressingMode::imm };
		case 0x05: return { AddressingMode::zp };
		case 0x15: return { AddressingMode::zp_x };
		case 0x0D: return { AddressingMode::abs };
		case 0x1D: return { AddressingMode::abs_x };
		case 0x19: return { AddressingMode::abs_y };
		case 0x01: return { AddressingMode::idx_ind_x };
		case 0x11: return { AddressingMode::ind_idx_y };
		case 0x2A: return std::nullopt;
		case 0x26: return { AddressingMode::zp };
		case 0x36: return { AddressingMode::zp_x };
		case 0x2E: return { AddressingMode::abs };
		case 0x3E: return { AddressingMode::abs_x };
		case 0x6A: return std::nullopt;
		case 0x66: return { AddressingMode::zp };
		case 0x76: return { AddressingMode::zp_x };
		case 0x6E: return { AddressingMode::abs };
		case 0x7E: return { AddressingMode::abs_x };
		case 0xE9: return { AddressingMode::imm };
		case 0xE5: return { AddressingMode::zp };
		case 0xF5: return { AddressingMode::zp_x };
		case 0xED: return { AddressingMode::abs };
		case 0xFD: return { AddressingMode::abs_x };
		case 0xF9: return { AddressingMode::abs_y };
		case 0xE1: return { AddressingMode::idx_ind_x };
		case 0xF1: return { AddressingMode::ind_idx_y };
		case 0xA9: return { AddressingMode::imm };
		case 0xA5: return { AddressingMode::zp };
		case 0xB5: return { AddressingMode::zp_x };
		case 0xAD: return { AddressingMode::abs };
		case 0xBD: return { AddressingMode::abs_x };
		case 0xB9: return { AddressingMode::abs_y };
		case 0xA1: return { AddressingMode::idx_ind_x };
		case 0xB1: return { AddressingMode::ind_idx_y };
		case 0xA2: return { AddressingMode::imm };
		case 0xA6: return { AddressingMode::zp };
		case 0xB6: return { AddressingMode::zp_y };
		case 0xAE: return { AddressingMode::abs };
		case 0xBE: return { AddressingMode::abs_y };
		case 0xA0: return { AddressingMode::imm };
		case 0xA4: return { AddressingMode::zp };
		case 0xB4: return { AddressingMode::zp_x };
		case 0xAC: return { AddressingMode::abs };
		case 0xBC: return { AddressingMode::abs_x };
		case 0x85: return { AddressingMode::zp };
		case 0x95: return { AddressingMode::zp_x };
		case 0x8D: return { AddressingMode::abs };
		case 0x9D: return { AddressingMode::abs_x };
		case 0x99: return { AddressingMode::abs_y };
		case 0x81: return { AddressingMode::idx_ind_x };
		case 0x91: return { AddressingMode::ind_idx_y };
		case 0x86: return { AddressingMode::zp };
		case 0x96: return { AddressingMode::zp_y };
		case 0x8E: return { AddressingMode::abs };
		case 0x84: return { AddressingMode::zp };
		case 0x94: return { AddressingMode::zp_x };
		case 0x8C: return { AddressingMode::abs };
		case 0xAA: return std::nullopt;
		case 0x8A: return std::nullopt;
		case 0xA8: return std::nullopt;
		case 0x98: return std::nullopt;
		case 0xCA: return std::nullopt;
		case 0xE8: return std::nullopt;
		case 0x88: return std::nullopt;
		case 0xC8: return std::nullopt;
		case 0x9A: return std::nullopt;
		case 0xBA: return std::nullopt;
		case 0x48: return std::nullopt;
		case 0x08: return std::nullopt;
		case 0x68: return std::nullopt;
		case 0x28: return std::nullopt;
		case 0xEA: return std::nullopt;
		default: return std::nullopt;
	}
}

std::string CPULogger::templ_for_mode(AddressingMode addr_mode)
{
	switch (addr_mode)
	{
		case abs:
			return "${{OPERAND}}";
		case abs_x:
			return "${{OPERAND}},X";
		case abs_y:
			return "${{OPERAND}},Y";
		case imm:
			return "#${{OPERAND}}";
		case zp:
			return "${{OPERAND}}";
		case zp_x:
			return "${{OPERAND}},X";
		case zp_y:
			return "${{OPERAND}},Y";
		case idx_ind_x:
			return "(${{OPERAND}},X)";
		case ind_idx_y:
			return "(${{OPERAND}}),Y";
		case ind:
			return "${{OPERAND}}";
		default:
			return "";
		
	}
}

uint8_t CPULogger::operand_len(NES::AddressingMode addr_mode)
{
	switch (addr_mode)
	{
		case abs:
		case abs_x:
		case abs_y:
			return 2;
		case zp:
		case zp_x:
		case zp_y:
		case imm:
		case idx_ind_x:
		case ind_idx_y:
		case ind:
			return 1;
		default:
			return 0;
		
	}
}