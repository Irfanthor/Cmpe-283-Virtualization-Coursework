#include "scheduler/scheduler.hpp"

namespace vmm {

std::size_t RoundRobinScheduler::PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>&) {
  if (statuses.empty()) {
    return 0;
  }
  for (std::size_t i = 0; i < statuses.size(); ++i) {
    const std::size_t candidate = (cursor_ + i) % statuses.size();
    if (statuses[candidate] == VmStatus::Ready || statuses[candidate] == VmStatus::Running ||
        statuses[candidate] == VmStatus::Blocked) {
      cursor_ = (candidate + 1) % statuses.size();
      return candidate;
    }
  }
  return 0;
}

}  // namespace vmm
