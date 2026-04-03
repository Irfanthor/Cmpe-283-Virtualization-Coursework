#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "common/types.hpp"

namespace vmm {

struct MigrationPacket {
  uint32_t version = 1;
  uint32_t checksum = 0;
  std::vector<uint8_t> payload;
};

class StateSerializer {
 public:
  MigrationPacket Serialize(const VmSnapshot& snapshot) const;
  bool Deserialize(const MigrationPacket& packet, VmSnapshot& snapshot, std::string& error) const;

 private:
  uint32_t ComputeChecksum(const std::vector<uint8_t>& bytes) const;
};

}  // namespace vmm
