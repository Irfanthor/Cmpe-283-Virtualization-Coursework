#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cpu/instruction.hpp"
#include "hypervisor/hypervisor_core.hpp"
#include "migration/migration_service.hpp"
#include "scheduler/scheduler.hpp"
#include "vm/virtual_machine.hpp"

namespace {

class LocalTransport final : public vmm::IMigrationTransport {
 public:
  bool Handshake(const std::string&, uint32_t) override { return true; }
  bool SendPacket(const vmm::MigrationPacket&) override { return true; }
};

std::vector<std::string> ReadLines(const std::string& path) {
  std::ifstream in(path);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(in, line)) {
    lines.push_back(line);
  }
  return lines;
}

std::unique_ptr<vmm::IScheduler> BuildScheduler(const std::string& name) {
  if (name == "priority") {
    return std::make_unique<vmm::PriorityScheduler>();
  }
  if (name == "mlfq") {
    return std::make_unique<vmm::MlfqScheduler>();
  }
  return std::make_unique<vmm::RoundRobinScheduler>();
}

}  // namespace

int main(int argc, char** argv) {
  std::string schedulerName = "round_robin";
  if (argc > 1) {
    schedulerName = argv[1];
  }
  std::vector<std::string> demoProgram = {"li $1, 42", "sw $1, 0", "lw $2, 0", "DUMP_PROCESSOR_STATE", "HALT"};
  vmm::VmConfig vmA {.name = "vmA", .execSlice = 3, .priority = 2, .programPath = "inline"};
  vmm::VmConfig vmB {.name = "vmB", .execSlice = 2, .priority = 1, .programPath = "inline"};

  LocalTransport transport;
  vmm::MetricsTracer migrationMetrics;
  vmm::MigrationService migrationService(transport, migrationMetrics);
  vmm::HypervisorCore hypervisor(BuildScheduler(schedulerName), migrationService);
  hypervisor.AddVm(vmm::VirtualMachine(vmA, vmm::ParseProgram(demoProgram)));
  hypervisor.AddVm(vmm::VirtualMachine(vmB, vmm::ParseProgram(demoProgram)));
  hypervisor.Run();

  std::cout << "Scheduler: " << schedulerName << "\n";
  for (const auto& [name, count] : hypervisor.Metrics().Stats().instructionsByVm) {
    std::cout << name << " retired instructions: " << count << "\n";
  }
  return 0;
}
