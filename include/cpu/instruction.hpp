#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace vmm {

enum class Opcode {
  Li,
  Add,
  Addi,
  Sub,
  Mul,
  Div,
  And,
  Or,
  Xor,
  Sll,
  Srl,
  Lw,
  Sw,
  DumpProcessorState,
  Snapshot,
  Migrate,
  Halt,
  Invalid
};

struct Instruction {
  Opcode opcode = Opcode::Invalid;
  int rd = 0;
  int rs = 0;
  int rt = 0;
  int32_t imm = 0;
  std::string label;
};

std::vector<Instruction> ParseProgram(const std::vector<std::string>& lines);

}  // namespace vmm
