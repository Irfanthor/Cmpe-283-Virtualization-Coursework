#include "snapshot/snapshot_service.hpp"

#include <fstream>

namespace vmm {

bool SnapshotService::Save(const VmSnapshot& snapshot, const std::string& path, std::string& error) const {
  std::ofstream out(path, std::ios::binary);
  if (!out) {
    error = "failed to open snapshot file for write";
    return false;
  }
  out << snapshot.vmName << "\n";
  out << snapshot.cpu.pc << " " << snapshot.cpu.hi << " " << snapshot.cpu.lo << "\n";
  for (const int32_t reg : snapshot.cpu.regs) {
    out << reg << " ";
  }
  out << "\n";
  out << snapshot.memoryBytes.size() << "\n";
  for (uint8_t b : snapshot.memoryBytes) {
    out << static_cast<int>(b) << " ";
  }
  out << "\n";
  return true;
}

bool SnapshotService::Load(const std::string& path, VmSnapshot& snapshot, std::string& error) const {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    error = "failed to open snapshot file for read";
    return false;
  }
  std::getline(in, snapshot.vmName);
  in >> snapshot.cpu.pc >> snapshot.cpu.hi >> snapshot.cpu.lo;
  for (auto& reg : snapshot.cpu.regs) {
    in >> reg;
  }
  std::size_t size = 0;
  in >> size;
  snapshot.memoryBytes.assign(size, 0);
  for (std::size_t i = 0; i < size; ++i) {
    int val = 0;
    in >> val;
    snapshot.memoryBytes[i] = static_cast<uint8_t>(val);
  }
  return true;
}

}  // namespace vmm
