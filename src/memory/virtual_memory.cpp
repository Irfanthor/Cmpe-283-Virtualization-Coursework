#include "memory/virtual_memory.hpp"

#include <cstring>

namespace vmm {

VirtualMemory::VirtualMemory(std::size_t bytes) : bytes_(bytes, 0) {
  const uint32_t pageCount = static_cast<uint32_t>((bytes + kPageSize - 1) / kPageSize);
  for (uint32_t i = 0; i < pageCount; ++i) {
    pageTable_[i] = i;
  }
}

void VirtualMemory::MapPage(uint32_t virtualPage, uint32_t physicalPage) {
  pageTable_[virtualPage] = physicalPage;
  tlb_[virtualPage] = physicalPage;
}

bool VirtualMemory::Translate(uint32_t virtualAddr, uint32_t& physicalAddr, VmFault& fault) {
  const uint32_t virtualPage = virtualAddr / kPageSize;
  const uint32_t offset = virtualAddr % kPageSize;
  auto tlbIt = tlb_.find(virtualPage);
  uint32_t physicalPage = 0;
  if (tlbIt != tlb_.end()) {
    physicalPage = tlbIt->second;
  } else {
    auto ptIt = pageTable_.find(virtualPage);
    if (ptIt == pageTable_.end()) {
      fault = {FaultType::PageFault, "virtual page not mapped"};
      return false;
    }
    physicalPage = ptIt->second;
    tlb_[virtualPage] = physicalPage;
  }
  physicalAddr = physicalPage * kPageSize + offset;
  if (physicalAddr + sizeof(int32_t) > bytes_.size()) {
    fault = {FaultType::AddressOutOfRange, "address outside VM memory"};
    return false;
  }
  return true;
}

bool VirtualMemory::ReadWord(uint32_t virtualAddr, int32_t& value, VmFault& fault) {
  uint32_t paddr = 0;
  if (!Translate(virtualAddr, paddr, fault)) {
    return false;
  }
  std::memcpy(&value, &bytes_[paddr], sizeof(int32_t));
  return true;
}

bool VirtualMemory::WriteWord(uint32_t virtualAddr, int32_t value, VmFault& fault) {
  uint32_t paddr = 0;
  if (!Translate(virtualAddr, paddr, fault)) {
    return false;
  }
  std::memcpy(&bytes_[paddr], &value, sizeof(int32_t));
  return true;
}

void VirtualMemory::RestorePageTable(const std::unordered_map<uint32_t, uint32_t>& pageTable) {
  pageTable_ = pageTable;
  tlb_.clear();
}

}  // namespace vmm
