#include "vm/virtual_machine.hpp"

#include <iostream>

namespace vmm {

VirtualMachine::VirtualMachine(VmConfig config, std::vector<Instruction> program)
    : config_(std::move(config)), program_(std::move(program)), memory_(config_.memoryBytes) {}

int32_t& VirtualMachine::Reg(int idx) { return cpu_.regs[static_cast<std::size_t>(idx)]; }
int32_t VirtualMachine::Reg(int idx) const { return cpu_.regs[static_cast<std::size_t>(idx)]; }

std::size_t VirtualMachine::ExecuteOne(const Instruction& inst) {
  switch (inst.opcode) {
    case Opcode::Li:
      Reg(inst.rd) = inst.imm;
      break;
    case Opcode::Add:
      Reg(inst.rd) = Reg(inst.rs) + Reg(inst.rt);
      break;
    case Opcode::Addi:
      Reg(inst.rd) = Reg(inst.rs) + inst.imm;
      break;
    case Opcode::Sub:
      Reg(inst.rd) = Reg(inst.rs) - Reg(inst.rt);
      break;
    case Opcode::Mul:
      Reg(inst.rd) = Reg(inst.rs) * Reg(inst.rt);
      break;
    case Opcode::Div:
      if (Reg(inst.rt) == 0) {
        lastFault_ = {FaultType::DivideByZero, "division by zero"};
        status_ = VmStatus::Faulted;
        return 0;
      }
      cpu_.lo = Reg(inst.rs) / Reg(inst.rt);
      cpu_.hi = Reg(inst.rs) % Reg(inst.rt);
      break;
    case Opcode::Lw: {
      int32_t value = 0;
      if (!memory_.ReadWord(static_cast<uint32_t>(inst.imm), value, lastFault_)) {
        status_ = VmStatus::Faulted;
        return 0;
      }
      Reg(inst.rd) = value;
      break;
    }
    case Opcode::Sw:
      if (!memory_.WriteWord(static_cast<uint32_t>(inst.imm), Reg(inst.rs), lastFault_)) {
        status_ = VmStatus::Faulted;
        return 0;
      }
      break;
    case Opcode::DumpProcessorState:
      std::cout << "[VM " << config_.name << "] PC=" << cpu_.pc << " R1=" << cpu_.regs[1] << " HI=" << cpu_.hi
                << " LO=" << cpu_.lo << "\n";
      break;
    case Opcode::Snapshot:
      break;
    case Opcode::Migrate:
      pendingMigrationTarget_ = inst.label;
      status_ = VmStatus::Blocked;
      return 0;
    case Opcode::Halt:
      status_ = VmStatus::Halted;
      return 0;
    default:
      lastFault_ = {FaultType::IllegalOpcode, "illegal opcode"};
      status_ = VmStatus::Faulted;
      return 0;
  }
  ++cpu_.pc;
  status_ = VmStatus::Running;
  return 1;
}

std::size_t VirtualMachine::RunForSlice(std::size_t maxInstructions) {
  if (status_ == VmStatus::Halted || status_ == VmStatus::Faulted) {
    return 0;
  }
  status_ = VmStatus::Running;
  std::size_t ran = 0;
  while (ran < maxInstructions && cpu_.pc < program_.size()) {
    const std::size_t advanced = ExecuteOne(program_[cpu_.pc]);
    if (advanced == 0) {
      break;
    }
    ran += advanced;
  }
  if (cpu_.pc >= program_.size()) {
    status_ = VmStatus::Halted;
  } else if (status_ == VmStatus::Running) {
    status_ = VmStatus::Ready;
  }
  return ran;
}

VmSnapshot VirtualMachine::Snapshot() const {
  VmSnapshot snapshot;
  snapshot.vmName = config_.name;
  snapshot.cpu = cpu_;
  snapshot.pageTable = memory_.PageTable();
  snapshot.memoryBytes = memory_.Bytes();
  return snapshot;
}

void VirtualMachine::Restore(const VmSnapshot& snapshot) {
  cpu_ = snapshot.cpu;
  memory_.Bytes() = snapshot.memoryBytes;
  memory_.RestorePageTable(snapshot.pageTable);
  status_ = VmStatus::Ready;
}

}  // namespace vmm
