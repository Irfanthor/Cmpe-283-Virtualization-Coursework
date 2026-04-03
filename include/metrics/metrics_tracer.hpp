#pragma once

#include <chrono>
#include <cstddef>
#include <string>

#include "common/types.hpp"

namespace vmm {

class MetricsTracer {
 public:
  void RecordDispatch(const std::string& vmName);
  void RecordInstructions(const std::string& vmName, std::size_t count);
  void StartMigration();
  void EndMigration();
  const SchedulerStats& Stats() const { return stats_; }
  std::chrono::milliseconds LastMigrationDuration() const { return lastMigrationDuration_; }

 private:
  SchedulerStats stats_;
  std::chrono::steady_clock::time_point migrationStart_ {};
  std::chrono::milliseconds lastMigrationDuration_ {0};
};

}  // namespace vmm
