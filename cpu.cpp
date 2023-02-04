#include <iostream>
#include <cstring>
#include <iomanip>
#include "cpu.hpp"

// CPU
auto CPU::powerCPU(void) -> void
{
    A = X = Y = 0;
    SP = 0xFF;
    C = D = V = N = 0;
    I = 1;

    initializeMem();

    PC = loadMemory(0xFFFC);
    PC |= (loadMemory(0xFFFD) << 8);
}

auto CPU::displayRegisters(void) -> void
{
    std::cout << "A:  " << std::hex << static_cast<int16_t>(A) << "\n";
    std::cout << "X:  " << std::hex << static_cast<int16_t>(X) << "\n";
    std::cout << "Y:  " << std::hex << static_cast<int16_t>(Y) << "\n";
    std::cout << "PC: " << std::hex << static_cast<int16_t>(PC) << "\n";
    std::cout << "SP: " << std::hex << static_cast<int16_t>(SP) << "\n";
    std::cout << "Status: \n";
    std::cout << "N: "   << (N ? 1 : 0)
              << "  V: " << (V ? 1 : 0)
              << "  D: " << (D ? 1 : 0)
              << "  I: " << (I ? 1 : 0)
              << "  Z: " << (Z ? 1 : 0)
              << "  C: " << (C ? 1 : 0)
              << "\n";
}

// memory
auto CPU::readMemory(void) -> byte
{
    return mem.ram[PC++];
}

auto CPU::loadMemory(word addr) -> byte
{
    return mem.ram[addr];
}

auto CPU::storeMemory(word data, byte reg) -> void
{
    mem.ram[data] = reg;
}

auto CPU::initializeMem(void) -> void
{
    memset(mem.ram, 0x00, mem.size);
}

auto CPU::displayMemory(uint16_t first, uint16_t last) -> void
{
    std::cout.fill('0');
    for (auto i = first; i < last; i++) {
        if (i % 16 == 0 && i != 0)
            std::cout << "\n";
        std::cout << std::setw(2) << std::hex << static_cast<int16_t>(mem.ram[i]) << " ";
    }
    std::cout << "\n";
}

// instructions
auto CPU::instruction(void) -> void
{
    byte opcode = readMemory();
    switch(opcode) {
        case 0x00: instructionInterrupt(); break;                              // BRK
        case 0x01: instructionIndirectXRead(&CPU::instructionORA, A); break;   // ORA (Indirect,X)
        case 0x05: instructionZeroPageRead(&CPU::instructionORA, A); break;    // ORA Zero Page
        case 0x06: instructionZeroPageData(&CPU::instructionASL); break;       // ASL Zero Page
        case 0x08: instructionPushS(); break;                                  // PHP
        case 0x09: instructionImmediate(&CPU::instructionORA, A); break;       // ORA Immediate
        case 0x0A: instructionImplied(&CPU::instructionASL, A); break;         // ASL Accumulator
        case 0x0D: instructionAbsoluteRead(&CPU::instructionORA, A); break;    // ORA Absolute
        case 0x0E: instructionAbsoluteData(&CPU::instructionASL); break;       // ASL Absolute
        case 0x10: instructionBranch(!N); break;                               // BPL
        case 0x11: instructionIndirectYRead(&CPU::instructionORA, A); break;   // ORA (Indirect),Y
        case 0x15: instructionZeroPageRead(&CPU::instructionORA, A, X); break; // ORA Zero Page,X
        case 0x16: instructionZeroPageData(&CPU::instructionASL, X); break;    // ASL Zero Page,X
        case 0x18: instructionClear(C); break;                                 // CLC
        case 0x19: instructionAbsoluteRead(&CPU::instructionORA, A, Y); break; // ORA Absolute,Y
        case 0x1D: instructionAbsoluteRead(&CPU::instructionORA, A, X); break; // ORA Absolute,X
        case 0x1E: instructionAbsoluteData(&CPU::instructionASL, X); break;    // ASL Absolute,X
        case 0x20: instructionJumpSubroutines(); break;                        // JSR
        case 0x21: instructionIndirectXRead(&CPU::instructionADC, A); break;   // AND (Indirect,X)
        case 0x24: instructionZeroPageRead(&CPU::instructionBIT, A); break;    // BIT Zero Page
        case 0x25: instructionZeroPageRead(&CPU::instructionADC, A); break;    // AND Zero Page
        case 0x26: instructionZeroPageData(&CPU::instructionROL); break;       // ROL Zero Page
        case 0x28: instructionPullS(); break;                                  // PLP
        case 0x29: instructionImmediate(&CPU::instructionADC, A); break;       // AND Immediate
        case 0x2A: instructionImplied(&CPU::instructionROL, A); break;         // ROL Accumulator
        case 0x2C: instructionAbsoluteRead(&CPU::instructionBIT, A); break;    // BIT Absolute
        case 0x2D: instructionAbsoluteRead(&CPU::instructionADC, A); break;    // AND Absolute
        case 0x2E: instructionAbsoluteData(&CPU::instructionROL); break;       // ROL Absolute
        case 0x30: instructionBranch(N); break;                                // BMI
        case 0x31: instructionIndirectYRead(&CPU::instructionADC, A); break;   // AND (Indirect),Y
        case 0x35: instructionZeroPageRead(&CPU::instructionADC, A, X); break; // AND Zero Page,X
        case 0x36: instructionZeroPageData(&CPU::instructionROL, X); break;    // ROL Zero Page,X
        case 0x38: instructionSet(C); break;                                   // SEC
        case 0x39: instructionAbsoluteRead(&CPU::instructionADC, A, Y); break; // AND Absolute,Y
        case 0x3D: instructionAbsoluteRead(&CPU::instructionADC, A, X); break; // AND Absolute,X
        case 0x3E: instructionAbsoluteData(&CPU::instructionROL, X); break;    // ROL Absolute,X
        case 0x40: instructionReturnInter(); break;                            // RTI
        case 0x41: instructionIndirectXRead(&CPU::instructionEOR, A); break;   // EOR (Indirect,X)
        case 0x45: instructionZeroPageRead(&CPU::instructionEOR, A); break;    // EOR Zero Page
        case 0x46: instructionZeroPageData(&CPU::instructionLSR); break;       // LSR Zero Page
        case 0x48: instructionPushA(); break;                                  // PHA
        case 0x49: instructionImmediate(&CPU::instructionEOR, A); break;       // EOR Immediate
        case 0x4A: instructionImplied(&CPU::instructionLSR, A); break;         // LSR Accumulator
        case 0x4C: instructionJumpAbsolute(); break;                           // JMP Absolute
        case 0x4D: instructionAbsoluteRead(&CPU::instructionEOR, A); break;    // EOR Absolute
        case 0x4E: instructionAbsoluteData(&CPU::instructionLSR); break;       // LSR Absolute
        case 0x50: instructionBranch(!V); break;                               // BVC
        case 0x51: instructionIndirectYRead(&CPU::instructionEOR, A); break;   // EOR (Indirect),Y
        case 0x55: instructionZeroPageRead(&CPU::instructionEOR, A, X); break; // EOR Zero Page,X
        case 0x56: instructionZeroPageRead(&CPU::instructionLSR, X); break;    // LSR Zero Page,X
        case 0x58: instructionClear(I); break;                                 // CLI
        case 0x59: instructionAbsoluteRead(&CPU::instructionEOR, A, Y); break; // EOR Absolute,Y
        case 0x5D: instructionAbsoluteRead(&CPU::instructionEOR, A, X); break; // EOR Absolute,X
        case 0x5E: instructionAbsoluteData(&CPU::instructionLSR, X); break;    // LSR Absolute,X
        case 0x60: instructionFromSubroutines(); break;                        // RTS
        case 0x61: instructionIndirectXRead(&CPU::instructionADC, A); break;   // ADC (Indirect,X)
        case 0x65: instructionZeroPageRead(&CPU::instructionADC, A); break;    // ADC Zero Page
        case 0x66: instructionZeroPageData(&CPU::instructionROR); break;       // ROR Zero Page
        case 0x68: instructionPullA(); break;                                  // PLA
        case 0x69: instructionImmediate(&CPU::instructionADC, A); break;       // ADC Immediate
        case 0x6A: instructionImplied(&CPU::instructionROR, A); break;         // ROR Accumulator
        case 0x6C: instructionJumpIndirect(); break;                           // JMP Indirect
        case 0x6D: instructionAbsoluteRead(&CPU::instructionADC, A); break;    // ADC Absolute
        case 0x6E: instructionAbsoluteData(&CPU::instructionROR); break;       // ROR Absolute
        case 0x70: instructionBranch(V); break;                                // BVS
        case 0x71: instructionIndirectYRead(&CPU::instructionADC, A); break;   // ADC (Indirect),Y
        case 0x75: instructionZeroPageRead(&CPU::instructionADC, A, X); break; // ADC Zero Page,X
        case 0x76: instructionZeroPageData(&CPU::instructionROR, X); break;    // ROR Zero Page,X
        case 0x78: instructionSet(I); break;                                   // SEI
        case 0x79: instructionAbsoluteRead(&CPU::instructionADC, A, Y); break; // ADC Absolute,Y
        case 0x7D: instructionAbsoluteRead(&CPU::instructionADC, A, X); break; // ADC Absolute,X
        case 0x7E: instructionAbsoluteData(&CPU::instructionROR, X); break;    // ROR Absolute,X
        case 0x81: instructionIndirectXStore(A); break;                        // STA (Indirect,X)
        case 0x84: instructionZeroPageStore(Y); break;                         // STY Zero Page
        case 0x85: instructionZeroPageStore(A); break;                         // STA Zero Page
        case 0x86: instructionZeroPageStore(X); break;                         // STX Zero Page
        case 0x88: instructionImplied(&CPU::instructionDEC, Y); break;         // DEY
        case 0x8A: instructionTransfer(&CPU::instructionLDA, A, X); break;     // TXA
        case 0x8C: instructionAbsoluteStore(Y); break;                         // STY Absolute
        case 0x8D: instructionAbsoluteStore(A); break;                         // STA Absolute
        case 0x8E: instructionAbsoluteStore(X); break;                         // STX Absolute
        case 0x90: instructionBranch(!C); break;                               // BBC
        case 0x91: instructionIndirectYStore(A); break;                        // STA (Indirect),Y
        case 0x94: instructionZeroPageStore(Y, X); break;                      // STY Zero Page,X
        case 0x95: instructionZeroPageStore(A, X); break;                      // STA Zero Page,X
        case 0x96: instructionZeroPageStore(X, Y); break;                      // STX Zero Page,Y
        case 0x98: instructionTransfer(&CPU::instructionLDA, A, Y); break;     // TYA
        case 0x99: instructionAbsoluteStore(A, Y); break;                      // STA Absolute,Y
        case 0x9A: instructionTransfer(&CPU::instructionLDA, SP, X); break;    // TXS
        case 0x9D: instructionAbsoluteStore(A, X); break;                      // STA Absolute,X
        case 0xA0: instructionImmediate(&CPU::instructionLDA, Y); break;       // LDY Immediate
        case 0xA1: instructionIndirectXRead(&CPU::instructionLDA, A); break;   // LDA (Indirect,X)
        case 0xA2: instructionImmediate(&CPU::instructionLDA, X); break;       // LDX Immediate
        case 0xA4: instructionZeroPageRead(&CPU::instructionLDA, Y); break;    // LDY Zero Page
        case 0xA5: instructionZeroPageRead(&CPU::instructionLDA, A); break;    // LDA Zero Page
        case 0xA6: instructionZeroPageRead(&CPU::instructionLDA, X); break;    // LDX Zero Page
        case 0xA8: instructionTransfer(&CPU::instructionLDA, Y, A); break;     // TAY
        case 0xA9: instructionImmediate(&CPU::instructionLDA, A); break;       // LDA Immediate
        case 0xAA: instructionTransfer(&CPU::instructionLDA, X, A); break;     // TAX
        case 0xAC: instructionAbsoluteRead(&CPU::instructionLDA, Y); break;    // LDY Absolute
        case 0xAD: instructionAbsoluteRead(&CPU::instructionLDA, A); break;    // LDA Absolute
        case 0xAE: instructionAbsoluteRead(&CPU::instructionLDA, X); break;    // LDX Absolute
        case 0xB0: instructionBranch(C); break;                                // BCS
        case 0xB1: instructionIndirectYRead(&CPU::instructionLDA, A); break;   // LDA (Indirect),Y
        case 0xB4: instructionZeroPageRead(&CPU::instructionLDA, Y, X); break; // LDY Zero Page,X
        case 0xB5: instructionZeroPageRead(&CPU::instructionLDA, A, X); break; // LDA Zero Page,X
        case 0xB6: instructionZeroPageRead(&CPU::instructionLDA, X, Y); break; // LDX Zero Page,Y
        case 0xB8: instructionClear(V); break;                                 // CLV
        case 0xB9: instructionAbsoluteRead(&CPU::instructionLDA, A, Y); break; // LDA Absolute,Y
        case 0xBA: instructionTransfer(&CPU::instructionLDA, X, SP); break;    // TSX
        case 0xBC: instructionAbsoluteRead(&CPU::instructionLDA, Y, X); break; // LDY Absolute,Y
        case 0xBD: instructionAbsoluteRead(&CPU::instructionLDA, A, X); break; // LDA Absolute,X
        case 0xBE: instructionAbsoluteRead(&CPU::instructionLDA, X, Y); break; // LDX Absolute,X
        case 0xC0: instructionImmediate(&CPU::instructionCMP, Y); break;       // CPY Immediate
        case 0xC1: instructionIndirectXRead(&CPU::instructionCMP, A); break;   // CMP (Indirect,X)
        case 0xC4: instructionZeroPageRead(&CPU::instructionCMP, Y); break;    // CPY Zero Page
        case 0xC5: instructionZeroPageRead(&CPU::instructionCMP, A); break;    // CMP Zero Page
        case 0xC6: instructionZeroPageData(&CPU::instructionDEC); break;       // DEC Zero Page
        case 0xC8: instructionImplied(&CPU::instructionINC, Y); break;         // INY
        case 0xC9: instructionImmediate(&CPU::instructionCMP, A); break;       // CMP Immediate
        case 0xCA: instructionImplied(&CPU::instructionDEC, X); break;         // DEX
        case 0xCC: instructionAbsoluteRead(&CPU::instructionCMP, Y); break;    // CPY Absolute
        case 0xCD: instructionAbsoluteRead(&CPU::instructionCMP, A); break;    // CMP Absolute
        case 0xCE: instructionAbsoluteData(&CPU::instructionDEC); break;       // DEC Absolute
        case 0xD0: instructionBranch(!Z); break;                               // BNE
        case 0xD1: instructionIndirectYRead(&CPU::instructionCMP, A); break;   // CMP (Indirect),Y
        case 0xD6: instructionZeroPageData(&CPU::instructionDEC, X); break;    // DEC Zero Page,X
        case 0xD8: instructionClear(D); break;                                 // CLD
        case 0xD9: instructionAbsoluteRead(&CPU::instructionCMP, A, Y); break; // CMP Absolute,Y
        case 0xD5: instructionZeroPageRead(&CPU::instructionCMP, A, X); break; // CMP Zero Page,X
        case 0xDD: instructionAbsoluteRead(&CPU::instructionCMP, A, X); break; // CMP Absolute,X
        case 0xDE: instructionAbsoluteRead(&CPU::instructionDEC, X); break;    // DEC Absolute,X
        case 0xE0: instructionImmediate(&CPU::instructionCMP, X); break;       // CPX Immediate
        case 0xE1: instructionIndirectXRead(&CPU::instructionSBC, A); break;   // SBC (Indirect,X)
        case 0xE4: instructionZeroPageRead(&CPU::instructionCMP, X); break;    // CPX Zero Page
        case 0xE5: instructionZeroPageRead(&CPU::instructionSBC, A); break;    // SBC Zero Page
        case 0xE6: instructionZeroPageData(&CPU::instructionINC); break;       // INC Zero Page
        case 0xE8: instructionImplied(&CPU::instructionINC, X); break;         // INX
        case 0xE9: instructionImmediate(&CPU::instructionSBC, A); break;       // SBC Immediate
        case 0xEA: instructionNOP(); break;                                    // NOP
        case 0xEC: instructionAbsoluteRead(&CPU::instructionCMP, X); break;    // CPX Absolute
        case 0xED: instructionAbsoluteRead(&CPU::instructionSBC, A); break;    // SBC Absolute
        case 0xEE: instructionAbsoluteData(&CPU::instructionINC); break;       // INC Absolute
        case 0xF0: instructionBranch(Z); break;                                // BEQ
        case 0xF1: instructionIndirectYRead(&CPU::instructionSBC, A); break;   // SBC (Indirect),Y
        case 0xF5: instructionZeroPageRead(&CPU::instructionSBC, A, X); break; // SBC Zero Page,X
        case 0xF6: instructionZeroPageData(&CPU::instructionINC, X); break;    // INC Zero Page,X
        case 0xF8: instructionSet(D); break;                                   // SED
        case 0xF9: instructionAbsoluteRead(&CPU::instructionSBC, A, Y); break; // SBC Absolute,Y
        case 0xFD: instructionAbsoluteRead(&CPU::instructionSBC, A, X); break; // SBC Absolute,X
        case 0xFE: instructionAbsoluteData(&CPU::instructionINC, X); break;    // INC Absolute,X
        default:
            std::cerr << "Wrong opcode: " << std::hex << static_cast<int16_t>(opcode) << "\n";
            exit(EXIT_FAILURE);
    }
}

// opcodes that modify values
auto CPU::instructionADC(byte data) -> byte
{
    // it's a word so we can detective if bit 9 is 1(Carry) or 0
    byte temp = (C ? 1 : 0);
    word res;
    res = A + data + temp;
    // if A and data have 0(1) in their msb, then (A^data) = 0(1)
    // and we want to know the opposite of it. So ~(A^data) = 1 if they had
    // different sigs
    // then we compare if res and A have the same msb
    // and it will be 1 if they don't have the same sign
    // and we AND it with ~(A ^ data) so it will be 1 if there was
    // an overflow and 0 if not
    // since we only care about the msb, we mask it with 0x80
    V = (~(A ^ data) & (res ^ A) & (0x80));
    N = res & 0x0080;
    C = res & 0x0100;
    Z = (res == 0);

    return res;
}

auto CPU::instructionAND(byte data) -> byte
{
    byte res = data & A;

    N = res & 0x80;
    Z = (res == 0);

    return res;
}

auto CPU::instructionASL(byte data) -> byte
{
    byte res = (data << 1);
    Z = (res == 0);
    N = res & 0x80;
    C = data & 0x80;

    return res;
}

auto CPU::instructionBIT(byte data) -> byte
{
    byte res = (A & data);
    Z = (res == 0);
    V = data & 0x40;
    N = data & 0x80;

    return A;
}

auto CPU::instructionCMP(byte data) -> byte
{
    word res = A - data;
    C = res & 0x0100;
    Z = (A == data);
    N = res & 0x80;

    return A;
}

auto CPU::instructionCMX(byte data) -> byte
{
    word res = X - data;
    C = res & 0x0100;
    Z = (X == data);
    N = res & 0x80;

    return X;
}

auto CPU::instructionCMY(byte data) -> byte
{
    word res = Y - data;
    C = res & 0x0100;
    Z = (Y == data);
    N = res & 0x80;

    return Y;
}

// can be used for DEX and DEY too
auto CPU::instructionDEC(byte data) -> byte
{
    data--;
    Z = (data == 0);
    N = data & 0x80;

    return data;
}


auto CPU::instructionEOR(byte data) -> byte
{
    byte res = A ^ data;
    Z = (res == 0);
    N = res & 0x80;

    return res;
}

// can be used for INX and INY too
auto CPU::instructionINC(byte data) -> byte
{
    data++;
    Z = (data == 0);
    N = data & 0x80;

    return data;
}

// also works for LDX and LDY
auto CPU::instructionLDA(byte data) -> byte
{
    Z = (data == 0);
    N = data & 0x40;
    return data;
}

auto CPU::instructionLSR(byte data) -> byte
{
    byte res = (data >> 1);
    Z = (res == 0);
    N = res & 0x80;
    C = data & 0x01;

    return res;
}

auto CPU::instructionORA(byte data) -> byte
{
    byte res = A | data;
    Z = (res == 0);
    N = res & 0x80;

    return res;
}

auto CPU::instructionROL(byte data) -> byte
{
    byte res = (data << 1) | C;
    Z = (res == 0);
    N = res & 0x80;
    C = data & 0x80;

    return res;
}

auto CPU::instructionROR(byte data) -> byte
{
    byte temp = (C ? (1 << 7) : 0);
    byte res = (data >> 1) | temp;
    Z = (res == 0);
    N = res & 0x80;
    C = (data & 0x01);

    return res;
}

auto CPU::instructionSBC(byte data) -> byte
{
    // same idea as in ADC, but converting data to ~data first
    byte temp = (C ? 1 : 0);
    word res;
    data = ~data;
    res = A + data + temp;
    V = (~(A ^ data) & (res ^ A) & (0x80)) >> 7;
    N = res & 0x0080;
    C = res & 0x0100;
    Z = (res == 0);

    return res;
}

// opcodes for the stack
auto CPU::instructionPushA(void) -> void
{
    storeMemory(0x0100 | SP--, A);
}

auto CPU::instructionPullA(void) -> void
{
    A = loadMemory(0x0100 | ++SP);
}

auto CPU::instructionPushS(void) -> void
{
    byte status = 0x00 | (N ? 1 : 0) << 7 | (V ? 1 : 0) << 6 | (D ? 1 : 0) << 3 |
        (I ? 1 : 0) << 2 | (Z ? 1 : 0) << 1 | (C ? 1 : 0);
    storeMemory(0x0100 | SP--, status);
}

auto CPU::instructionPullS(void) -> void
{
    auto status = loadMemory(0x0100 | ++SP);
    N = status & 0x80;
    V = status & 0x40;
    D = status & 0x08;
    I = status & 0x04;
    Z = status & 0x02;
    C = status & 0x01;
}

// opcode that does nothing
auto CPU::instructionNOP(void) -> void
{
}

// branch operations
auto CPU::instructionBranch(bool decision) -> void
{
    if (decision) {
        auto dest = readMemory();
        PC += dest;
    } else {
        readMemory();
    }
}

// jump operations
auto CPU::instructionJumpAbsolute(void) -> void
{
    word dest = readMemory();
    dest |= (readMemory() << 8);
    PC = dest;

}

auto CPU::instructionJumpIndirect(void) -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);
    byte loc = loadMemory(addr);
    word dest = loc;
    loc++; // so it wraps around in case of being 0xFF;
           // See: https://www.nesdev.org/obelisk-6502-guide/reference.html#JMP
    dest |= (loadMemory(loc) << 8);
    PC = dest;
}

// jump to/from subroutines operations
auto CPU::instructionJumpSubroutines(void) -> void
{
    word dest = readMemory();
    dest |= (readMemory() << 8);
    PC--; // so we can get to the position
          // of the the the last readMemory above
    pushPC();
    PC = dest;
}

auto CPU::instructionFromSubroutines(void) -> void
{
    pullPC();
    PC++; // so we can go to the next instruction
          // after the last readMemory in instructionJumpSubroutines
}

// status instructions
auto CPU::instructionClear(bool& status) -> void
{
    status = 0;
}

auto CPU::instructionSet(bool& status)   -> void
{
    status = 1;
}




// interrupt instructions
auto CPU::instructionInterrupt(void) -> void
{
    pushPC();
    instructionPushS();
    PC = loadMemory(0xFFFE);
    PC |= (loadMemory(0xFFFF) << 8);
}

auto CPU::instructionReturnInter(void) -> void
{
    instructionPullS();
    pullPC();

}

// addressing modes
auto CPU::instructionImmediate(fp instr, byte& reg) -> void
{
    reg = (this->*instr)(readMemory());
}


auto CPU::instructionZeroPageRead(fp instr, byte& reg)  -> void
{
    auto zero = readMemory();
    reg = (this->*instr)(loadMemory(zero));
}

auto CPU::instructionZeroPageRead(fp instr, byte& reg, byte& index) -> void
{
    auto zero = readMemory();
    zero += index; // to avoid the sum be higher than 1 byte
    reg = (this->*instr)(loadMemory(zero));
}

auto CPU::instructionAbsoluteRead(fp instr, byte& reg)  -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);

    reg = (this->*instr)(loadMemory(addr));
}

auto CPU::instructionAbsoluteRead(fp instr, byte& reg, byte& index)  -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);
    reg = (this->*instr)(loadMemory(addr + index));
}

auto CPU::instructionIndirectXRead(fp instr, byte& reg) -> void
{
    auto zero = readMemory();
    word addr  = loadMemory(zero + X);
    addr |= (loadMemory(zero + X + 1) << 8);

    reg = (this->*instr)(loadMemory(addr));
}

auto CPU::instructionIndirectYRead(fp instr, byte& reg) -> void
{
    auto zero = readMemory();
    word addr = loadMemory(zero);
    addr |= (loadMemory(zero + 1) << 8);
    reg = (this->*instr)(loadMemory(addr + Y));
}

auto CPU::instructionTransfer(fp instr, byte& to, byte& from) -> void
{
    to = (this->*instr)(from);
}

auto CPU::instructionImplied(fp instr, byte& data) -> void
{
    data = (this->*instr)(data);
}

auto CPU::instructionZeroPageStore(byte& reg)  -> void
{
    auto zero = readMemory();
    storeMemory(loadMemory(zero), reg);
}

auto CPU::instructionZeroPageStore(byte& reg, byte& index)          -> void
{
    auto zero = readMemory();
    zero += index;
    storeMemory(loadMemory(zero), reg);

}

auto CPU::instructionAbsoluteStore(byte& reg)                       -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);
    storeMemory(loadMemory(addr), reg);
}

auto CPU::instructionAbsoluteStore(byte& reg, byte& index)          -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);
    storeMemory(loadMemory(addr + index), reg);
}

auto CPU::instructionIndirectXStore(byte& reg)                      -> void
{
    auto zero = readMemory();

    word addr  = loadMemory(zero + X);
    addr |= (loadMemory(zero + X + 1) << 8);
    storeMemory(addr, reg);
}

auto CPU::instructionIndirectYStore(byte& reg)                      -> void
{
    auto zero = readMemory();
    word addr = loadMemory(zero);
    addr |= (loadMemory(zero + 1) << 8);

    storeMemory(addr + Y, reg);
}

auto CPU::instructionZeroPageData(fp instr)                         -> void
{
    auto zero = readMemory();
    auto data = loadMemory(zero);
    storeMemory((this->*instr)(data), data);
}

auto CPU::instructionZeroPageData(fp instr, byte& index)            -> void
{
    auto zero = readMemory();
    zero += index;
    auto data = loadMemory(zero);
    storeMemory((this->*instr)(data), data);
}

auto CPU::instructionAbsoluteData(fp instr)                         -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);
    auto data = loadMemory(addr);
    storeMemory((this->*instr)(data), data);
}

auto CPU::instructionAbsoluteData(fp instr, byte& index)           -> void
{
    word addr = readMemory();
    addr |= (readMemory() << 8);
    auto data = loadMemory(addr + index);
    storeMemory((this->*instr)(data), data);
}

auto CPU::instructionIndirectXData(fp instr)                        -> void
{
    auto zero = readMemory();
    word addr = loadMemory(zero + X);
    addr |= (loadMemory(zero + X +  1) << 8);
    auto data = loadMemory(addr);
    storeMemory((this->*instr)(data), data);
}

auto CPU::instructionIndirectYData(fp instr)                        -> void
{
    auto zero = readMemory();
    word addr = loadMemory(zero);
    addr |= (loadMemory(zero + 1) << 8);
    auto data = loadMemory(addr + Y);
    storeMemory((this->*instr)(data), data);
}

// Stack PC operations
auto CPU::pushPC(void) -> void
{
    storeMemory(0x0100 | SP--, (PC >> 8));
    storeMemory(0x0100 | SP--, PC);
}

auto CPU::pullPC(void) -> void
{
    PC = loadMemory(0x0100 | SP++);
    PC |= (loadMemory(0x0100 | SP++) << 8);
}
