#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "common/types.hpp"

namespace vmm {

class VirtualMemory {
 public:
  explicit VirtualMemory(std::size_t bytes);

  bool ReadWord(uint32_t virtualAddr, int32_t& value, VmFault& fault);
  bool WriteWord(uint32_t virtualAddr, int32_t value, VmFault& fault);
  void MapPage(uint32_t virtualPage, uint32_t physicalPage);
  const std::unordered_map<uint32_t, uint32_t>& PageTable() const { return pageTable_; }
  const std::vector<uint8_t>& Bytes() const { return bytes_; }
  std::vector<uint8_t>& Bytes() { return bytes_; }
  void RestorePageTable(const std::unordered_map<uint32_t, uint32_t>& pageTable);

 private:
  static constexpr uint32_t kPageSize = 4096;

  bool Translate(uint32_t virtualAddr, uint32_t& physicalAddr, VmFault& fault);
  std::vector<uint8_t> bytes_;
  std::unordered_map<uint32_t, uint32_t> pageTable_;
  std::unordered_map<uint32_t, uint32_t> tlb_;
};

}  // namespace vmm
