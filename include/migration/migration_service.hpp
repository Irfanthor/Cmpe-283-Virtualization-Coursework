#pragma once

#include <string>

#include "metrics/metrics_tracer.hpp"
#include "migration/migration_protocol.hpp"
#include "migration/state_serializer.hpp"
#include "vm/virtual_machine.hpp"

namespace vmm {

class MigrationService {
 public:
  MigrationService(IMigrationTransport& transport, MetricsTracer& metrics)
      : transport_(transport), metrics_(metrics) {}

  bool PreCopyMigrate(VirtualMachine& vm, const std::string& target, std::string& error);

 private:
  IMigrationTransport& transport_;
  MetricsTracer& metrics_;
  StateSerializer serializer_;
  MigrationProtocol protocol_;
};

}  // namespace vmm
