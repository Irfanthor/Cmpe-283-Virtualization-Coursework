#include "migration/migration_service.hpp"

namespace vmm {

bool MigrationService::PreCopyMigrate(VirtualMachine& vm, const std::string& target, std::string& error) {
  metrics_.StartMigration();
  // Simulate iterative pre-copy by taking snapshots in rounds before final commit.
  for (int round = 0; round < 2; ++round) {
    vm.RunForSlice(10);
  }
  const VmSnapshot finalSnapshot = vm.Snapshot();
  const MigrationPacket packet = serializer_.Serialize(finalSnapshot);
  const bool ok = protocol_.SendWithRetry(transport_, target, packet, error);
  metrics_.EndMigration();
  return ok;
}

}  // namespace vmm
