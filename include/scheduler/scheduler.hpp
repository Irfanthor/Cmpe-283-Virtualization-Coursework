#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <vector>

#include "common/types.hpp"

namespace vmm {

class IScheduler {
 public:
  virtual ~IScheduler() = default;
  virtual std::size_t PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>& priorities) = 0;
  virtual const char* Name() const = 0;
};

class RoundRobinScheduler final : public IScheduler {
 public:
  std::size_t PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>& priorities) override;
  const char* Name() const override { return "round_robin"; }

 private:
  std::size_t cursor_ = 0;
};

class PriorityScheduler final : public IScheduler {
 public:
  std::size_t PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>& priorities) override;
  const char* Name() const override { return "priority"; }
};

class MlfqScheduler final : public IScheduler {
 public:
  std::size_t PickNext(const std::vector<VmStatus>& statuses, const std::vector<int>& priorities) override;
  const char* Name() const override { return "mlfq"; }
  void NotifyRan(std::size_t vmIndex);

 private:
  void RebuildQueues(std::size_t vmCount);
  bool initialized_ = false;
  std::deque<std::size_t> high_;
  std::deque<std::size_t> mid_;
  std::deque<std::size_t> low_;
};

}  // namespace vmm
