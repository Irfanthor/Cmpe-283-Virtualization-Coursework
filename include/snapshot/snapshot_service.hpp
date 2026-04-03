#pragma once

#include <string>

#include "common/types.hpp"

namespace vmm {

class SnapshotService {
 public:
  bool Save(const VmSnapshot& snapshot, const std::string& path, std::string& error) const;
  bool Load(const std::string& path, VmSnapshot& snapshot, std::string& error) const;
};

}  // namespace vmm
