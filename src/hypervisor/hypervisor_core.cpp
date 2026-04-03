#include "hypervisor/hypervisor_core.hpp"

#include <iostream>

namespace vmm {

HypervisorCore::HypervisorCore(std::unique_ptr<IScheduler> scheduler, MigrationService& migrationService)
    : scheduler_(std::move(scheduler)), migrationService_(migrationService) {}

void HypervisorCore::AddVm(VirtualMachine vm) { vms_.push_back(std::move(vm)); }

void HypervisorCore::Run() {
  while (true) {
    bool allDone = true;
    std::vector<VmStatus> statuses;
    std::vector<int> priorities;
    statuses.reserve(vms_.size());
    priorities.reserve(vms_.size());
    for (const auto& vm : vms_) {
      statuses.push_back(vm.Status());
      priorities.push_back(vm.Priority());
      if (vm.Status() != VmStatus::Halted && vm.Status() != VmStatus::Faulted) {
        allDone = false;
      }
    }
    if (allDone || vms_.empty()) {
      break;
    }
    const std::size_t idx = scheduler_->PickNext(statuses, priorities);
    if (idx >= vms_.size()) {
      continue;
    }
    auto& vm = vms_[idx];
    if (vm.Status() == VmStatus::Halted || vm.Status() == VmStatus::Faulted) {
      continue;
    }
    metrics_.RecordDispatch(vm.Name());
    const std::size_t retired = vm.RunForSlice(vm.Slice());
    metrics_.RecordInstructions(vm.Name(), retired);
    if (vm.PendingMigrationTarget().has_value()) {
      std::string error;
      const auto target = *vm.PendingMigrationTarget();
      if (!migrationService_.PreCopyMigrate(vm, target, error)) {
        std::cerr << "Migration failed for " << vm.Name() << ": " << error << "\n";
      }
      vm.ClearMigrationIntent();
    }
  }
}

}  // namespace vmm
