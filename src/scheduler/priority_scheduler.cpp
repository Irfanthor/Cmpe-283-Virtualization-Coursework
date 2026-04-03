#include "scheduler/scheduler.hpp"

namespace vmm {

std::size_t PriorityScheduler::PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>& priorities) {
  int bestPriority = -1000000;
  std::size_t bestIdx = 0;
  for (std::size_t i = 0; i < statuses.size(); ++i) {
    if (statuses[i] == VmStatus::Halted || statuses[i] == VmStatus::Faulted) {
      continue;
    }
    if (i < priorities.size() && priorities[i] > bestPriority) {
      bestPriority = priorities[i];
      bestIdx = i;
    }
  }
  return bestIdx;
}

}  // namespace vmm
