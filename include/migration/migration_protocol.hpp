#pragma once

#include <string>

#include "migration/state_serializer.hpp"

namespace vmm {

class IMigrationTransport {
 public:
  virtual ~IMigrationTransport() = default;
  virtual bool Handshake(const std::string& target, uint32_t version) = 0;
  virtual bool SendPacket(const MigrationPacket& packet) = 0;
};

class MigrationProtocol {
 public:
  bool SendWithRetry(IMigrationTransport& transport, const std::string& target, const MigrationPacket& packet,
                     std::string& error) const;
};

}  // namespace vmm
