#include "cpu.hpp"
#include <iostream>

int main(int argc, char* args[])
{
  (void)argc;
  (void)args;

  CPU cpu{};
  cpu.PC = 0x00;
  cpu.SP = 0xFF;
  cpu.N =1;
  cpu.V =0;
  cpu.D =1;
  cpu.I =1;
  cpu.Z =1;
  cpu.C =1;
  cpu.mem.ram[0] = 0x08; // Pull Status
  cpu.mem.ram[1] = 0x28; // Push Status
  cpu.instruction();
  cpu.displayRegisters();
  cpu.N =0;
  cpu.V =1;
  cpu.D =0;
  cpu.I =0;
  cpu.Z =0;
  cpu.C =0;
  cpu.instruction();
  cpu.displayRegisters();
  return 0;
}
