#include "migration/migration_protocol.hpp"

namespace vmm {

bool MigrationProtocol::SendWithRetry(IMigrationTransport& transport, const std::string& target,
                                      const MigrationPacket& packet, std::string& error) const {
  if (!transport.Handshake(target, packet.version)) {
    error = "handshake failed";
    return false;
  }
  for (int attempt = 0; attempt < 3; ++attempt) {
    if (transport.SendPacket(packet)) {
      return true;
    }
  }
  error = "packet send retries exhausted";
  return false;
}

}  // namespace vmm
