#include <cstdint>



using word = uint16_t;
using byte = uint8_t;
using flag = bool;

class CPU
{
public:

    // CPU
    auto powerCPU(void)         -> void;
    auto displayRegisters(void) -> void;

    // memory
    auto readMemory(void)                             -> byte;
    auto loadMemory(word addr)                        -> byte;
    auto storeMemory(word data, byte reg)             -> void;
    auto initializeMem(void)                          -> void;
    auto displayMemory(uint16_t first, uint16_t last) -> void;

    // function pointer
    using fp = auto (CPU::*)(byte) -> byte;

    // instructions
    auto instruction(void) -> void;

    // opcodes that modify values
    auto instructionADC(byte data) -> byte;
    auto instructionAND(byte data) -> byte;
    auto instructionASL(byte data) -> byte;
    auto instructionBIT(byte data) -> byte;
    auto instructionCMP(byte data) -> byte;
    auto instructionCMX(byte data) -> byte;
    auto instructionCMY(byte data) -> byte;
    auto instructionDEC(byte data) -> byte;
    auto instructionEOR(byte data) -> byte;
    auto instructionINC(byte data) -> byte;
    auto instructionLDA(byte data) -> byte;
    auto instructionLSR(byte data) -> byte;
    auto instructionORA(byte data) -> byte;
    auto instructionROL(byte data) -> byte;
    auto instructionROR(byte data) -> byte;
    auto instructionSBC(byte data) -> byte;

    // opcodes for the stack
    auto instructionPushA(void) -> void;
    auto instructionPullA(void) -> void;
    auto instructionPushS(void) -> void;
    auto instructionPullS(void) -> void;

    // opcode that does nothing
    auto instructionNOP(void) -> void;

    // branch operations
    auto instructionBranch(bool decision) -> void;

    // jump operations
    auto instructionJumpAbsolute(void) -> void;
    auto instructionJumpIndirect(void) -> void;

    // jump to/from subroutines operations
    auto instructionJumpSubroutines(void) -> void;
    auto instructionFromSubroutines(void) -> void;

    // status instructions
    auto instructionClear(bool& status) -> void;
    auto instructionSet(bool& status)   -> void;

    // interrupt instructions
    auto instructionInterrupt(void) -> void;
    auto instructionReturnInter(void)  -> void;

    // addressing modes
    auto instructionImmediate(fp instr, byte& reg)                 -> void;
    auto instructionZeroPageRead(fp instr, byte& reg)              -> void;
    auto instructionZeroPageRead(fp instr, byte& reg, byte& index) -> void;
    auto instructionAbsoluteRead(fp instr, byte& reg)              -> void;
    auto instructionAbsoluteRead(fp instr, byte& reg, byte& index) -> void;
    auto instructionIndirectXRead(fp instr, byte& reg)             -> void;
    auto instructionIndirectYRead(fp instr, byte& reg)             -> void;
    auto instructionTransfer(fp instr, byte& to, byte& from)       -> void;
    auto instructionImplied(fp instr, byte& data)                  -> void;

    auto instructionZeroPageStore(byte& reg)                       -> void;
    auto instructionZeroPageStore(byte& reg, byte& index)          -> void;
    auto instructionAbsoluteStore(byte& reg)                       -> void;
    auto instructionAbsoluteStore(byte& reg, byte& index)          -> void;
    auto instructionIndirectXStore(byte& reg)                      -> void;
    auto instructionIndirectYStore(byte& reg)                      -> void;

    auto instructionZeroPageData(fp instr)                         -> void;
    auto instructionZeroPageData(fp instr, byte& index)            -> void;
    auto instructionAbsoluteData(fp instr)                         -> void;
    auto instructionAbsoluteData(fp instr, byte& index)            -> void;
    auto instructionIndirectXData(fp instr)                        -> void;
    auto instructionIndirectYData(fp instr)                        -> void;

    // Stack PC operations
    auto pullPC(void) -> void;
    auto pushPC(void) -> void;



    struct Memory
    {
        constexpr static word size{65534};
        byte ram[size];
    };

    Memory mem;

    word PC; // Program Counter
    byte SP; // Stack Pointer
    byte A;  // Accumulator
    byte X;  // Index Register X
    byte Y;  // Index Register Y

    // Process status
    flag C;  // Carry
    flag Z;  // Zero
    flag I;  // Interrupt Disable
    flag D;  // Decimal Mode
    flag V;  // Overflow
    flag N;  // Negative
};
