#include "metrics/metrics_tracer.hpp"

namespace vmm {

void MetricsTracer::RecordDispatch(const std::string& vmName) { stats_.dispatchCountByVm[vmName]++; }

void MetricsTracer::RecordInstructions(const std::string& vmName, std::size_t count) {
  stats_.instructionsByVm[vmName] += count;
}

void MetricsTracer::StartMigration() { migrationStart_ = std::chrono::steady_clock::now(); }

void MetricsTracer::EndMigration() {
  lastMigrationDuration_ =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - migrationStart_);
}

}  // namespace vmm
