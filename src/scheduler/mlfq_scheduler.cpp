#include "scheduler/scheduler.hpp"

namespace vmm {

void MlfqScheduler::RebuildQueues(std::size_t vmCount) {
  high_.clear();
  mid_.clear();
  low_.clear();
  for (std::size_t i = 0; i < vmCount; ++i) {
    high_.push_back(i);
  }
  initialized_ = true;
}

void MlfqScheduler::NotifyRan(std::size_t vmIndex) {
  low_.push_back(vmIndex);
}

std::size_t MlfqScheduler::PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>&) {
  if (!initialized_) {
    RebuildQueues(statuses.size());
  }

  auto pickFromQueue = [&](std::deque<std::size_t>& q) -> std::size_t {
    while (!q.empty()) {
      const std::size_t candidate = q.front();
      q.pop_front();
      if (candidate < statuses.size() && statuses[candidate] != VmStatus::Halted && statuses[candidate] != VmStatus::Faulted) {
        return candidate;
      }
    }
    return statuses.empty() ? 0 : statuses.size();
  };

  std::size_t idx = pickFromQueue(high_);
  if (idx < statuses.size()) {
    mid_.push_back(idx);
    return idx;
  }
  idx = pickFromQueue(mid_);
  if (idx < statuses.size()) {
    low_.push_back(idx);
    return idx;
  }
  idx = pickFromQueue(low_);
  if (idx < statuses.size()) {
    high_.push_back(idx);
    return idx;
  }
  RebuildQueues(statuses.size());
  return 0;
}

}  // namespace vmm
