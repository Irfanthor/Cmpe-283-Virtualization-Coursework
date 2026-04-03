#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace vmm {

using RegisterFile = std::array<int32_t, 32>;

struct CpuState {
  RegisterFile regs {};
  int32_t hi = 0;
  int32_t lo = 0;
  std::size_t pc = 0;
};

enum class VmStatus { Ready, Running, Blocked, Halted, Faulted };

enum class FaultType {
  None,
  IllegalOpcode,
  DivideByZero,
  PageFault,
  AddressOutOfRange
};

struct VmFault {
  FaultType type = FaultType::None;
  std::string message;
};

struct VmConfig {
  std::string name;
  std::size_t execSlice = 50;
  int priority = 1;
  std::string programPath;
  std::size_t memoryBytes = 64 * 1024;
};

struct VmSnapshot {
  std::string vmName;
  CpuState cpu;
  std::unordered_map<uint32_t, uint32_t> pageTable;
  std::vector<uint8_t> memoryBytes;
};

struct SchedulerStats {
  std::unordered_map<std::string, std::size_t> instructionsByVm;
  std::unordered_map<std::string, std::size_t> dispatchCountByVm;
};

}  // namespace vmm
