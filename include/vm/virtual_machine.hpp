#pragma once

#include <optional>
#include <string>
#include <vector>

#include "common/types.hpp"
#include "cpu/instruction.hpp"
#include "memory/virtual_memory.hpp"

namespace vmm {

class VirtualMachine {
 public:
  VirtualMachine(VmConfig config, std::vector<Instruction> program);

  const std::string& Name() const { return config_.name; }
  std::size_t Slice() const { return config_.execSlice; }
  int Priority() const { return config_.priority; }
  VmStatus Status() const { return status_; }
  const VmFault& LastFault() const { return lastFault_; }
  CpuState& Cpu() { return cpu_; }
  const CpuState& Cpu() const { return cpu_; }
  VirtualMemory& Memory() { return memory_; }
  const VirtualMemory& Memory() const { return memory_; }

  std::size_t RunForSlice(std::size_t maxInstructions);
  VmSnapshot Snapshot() const;
  void Restore(const VmSnapshot& snapshot);
  std::optional<std::string> PendingMigrationTarget() const { return pendingMigrationTarget_; }
  void ClearMigrationIntent() { pendingMigrationTarget_.reset(); }

 private:
  std::size_t ExecuteOne(const Instruction& inst);
  int32_t& Reg(int idx);
  int32_t Reg(int idx) const;

  VmConfig config_;
  std::vector<Instruction> program_;
  CpuState cpu_;
  VirtualMemory memory_;
  VmStatus status_ = VmStatus::Ready;
  VmFault lastFault_;
  std::optional<std::string> pendingMigrationTarget_;
};

}  // namespace vmm
