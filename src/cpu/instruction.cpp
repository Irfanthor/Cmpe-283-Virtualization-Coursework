#include "cpu/instruction.hpp"

#include <sstream>

namespace vmm {

namespace {

int ParseRegister(const std::string& token) {
  if (token.size() < 2 || token[0] != '$') {
    return 0;
  }
  return std::stoi(token.substr(1));
}

std::string TrimComma(std::string token) {
  if (!token.empty() && token.back() == ',') {
    token.pop_back();
  }
  return token;
}

}  // namespace

std::vector<Instruction> ParseProgram(const std::vector<std::string>& lines) {
  std::vector<Instruction> program;
  program.reserve(lines.size());
  for (const auto& line : lines) {
    std::istringstream iss(line);
    std::string op;
    iss >> op;
    if (op.empty() || op[0] == '#') {
      continue;
    }
    Instruction inst {};
    if (op == "li") {
      std::string rd;
      iss >> rd >> inst.imm;
      inst.opcode = Opcode::Li;
      inst.rd = ParseRegister(TrimComma(rd));
    } else if (op == "add") {
      std::string rd, rs, rt;
      iss >> rd >> rs >> rt;
      inst.opcode = Opcode::Add;
      inst.rd = ParseRegister(TrimComma(rd));
      inst.rs = ParseRegister(TrimComma(rs));
      inst.rt = ParseRegister(TrimComma(rt));
    } else if (op == "addi") {
      std::string rd, rs;
      iss >> rd >> rs >> inst.imm;
      inst.opcode = Opcode::Addi;
      inst.rd = ParseRegister(TrimComma(rd));
      inst.rs = ParseRegister(TrimComma(rs));
    } else if (op == "lw") {
      std::string rd;
      iss >> rd >> inst.imm;
      inst.opcode = Opcode::Lw;
      inst.rd = ParseRegister(TrimComma(rd));
    } else if (op == "sw") {
      std::string rs;
      iss >> rs >> inst.imm;
      inst.opcode = Opcode::Sw;
      inst.rs = ParseRegister(TrimComma(rs));
    } else if (op == "DUMP_PROCESSOR_STATE") {
      inst.opcode = Opcode::DumpProcessorState;
    } else if (op == "SNAPSHOT") {
      inst.opcode = Opcode::Snapshot;
      iss >> inst.label;
    } else if (op == "MIGRATE") {
      inst.opcode = Opcode::Migrate;
      iss >> inst.label;
    } else if (op == "HALT") {
      inst.opcode = Opcode::Halt;
    } else if (op == "sub") {
      std::string rd, rs, rt;
      iss >> rd >> rs >> rt;
      inst.opcode = Opcode::Sub;
      inst.rd = ParseRegister(TrimComma(rd));
      inst.rs = ParseRegister(TrimComma(rs));
      inst.rt = ParseRegister(TrimComma(rt));
    } else if (op == "mul") {
      std::string rd, rs, rt;
      iss >> rd >> rs >> rt;
      inst.opcode = Opcode::Mul;
      inst.rd = ParseRegister(TrimComma(rd));
      inst.rs = ParseRegister(TrimComma(rs));
      inst.rt = ParseRegister(TrimComma(rt));
    } else if (op == "div") {
      std::string rs, rt;
      iss >> rs >> rt;
      inst.opcode = Opcode::Div;
      inst.rs = ParseRegister(TrimComma(rs));
      inst.rt = ParseRegister(TrimComma(rt));
    } else {
      inst.opcode = Opcode::Invalid;
      inst.label = op;
    }
    program.push_back(inst);
  }
  return program;
}

}  // namespace vmm
