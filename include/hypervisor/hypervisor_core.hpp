#pragma once

#include <memory>
#include <vector>

#include "metrics/metrics_tracer.hpp"
#include "migration/migration_service.hpp"
#include "scheduler/scheduler.hpp"
#include "snapshot/snapshot_service.hpp"
#include "vm/virtual_machine.hpp"

namespace vmm {

class HypervisorCore {
 public:
  HypervisorCore(std::unique_ptr<IScheduler> scheduler, MigrationService& migrationService);
  void AddVm(VirtualMachine vm);
  void Run();
  const MetricsTracer& Metrics() const { return metrics_; }
  MetricsTracer& MutableMetrics() { return metrics_; }

 private:
  std::vector<VirtualMachine> vms_;
  std::unique_ptr<IScheduler> scheduler_;
  SnapshotService snapshotService_;
  MigrationService& migrationService_;
  MetricsTracer metrics_;
};

}  // namespace vmm
