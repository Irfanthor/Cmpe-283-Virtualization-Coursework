#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "cpu/instruction.hpp"
#include "memory/virtual_memory.hpp"
#include "migration/state_serializer.hpp"
#include "scheduler/scheduler.hpp"
#include "vm/virtual_machine.hpp"

using namespace vmm;

void TestVirtualMemoryReadWrite() {
  VirtualMemory mem(16384);
  VmFault fault;
  assert(mem.WriteWord(0, 99, fault));
  int32_t out = 0;
  assert(mem.ReadWord(0, out, fault));
  assert(out == 99);
}

void TestPageFault() {
  VirtualMemory mem(4096);
  VmFault fault;
  int32_t out = 0;
  const bool ok = mem.ReadWord(8192, out, fault);
  assert(!ok);
  assert(fault.type == FaultType::PageFault);
}

void TestSchedulers() {
  std::vector<VmStatus> statuses = {VmStatus::Ready, VmStatus::Ready};
  std::vector<int> priorities = {1, 10};
  RoundRobinScheduler rr;
  PriorityScheduler pr;
  MlfqScheduler mlfq;
  assert(rr.PickNext(statuses, priorities) == 0);
  assert(pr.PickNext(statuses, priorities) == 1);
  assert(mlfq.PickNext(statuses, priorities) < 2);
}

void TestSerializerRoundTrip() {
  VmSnapshot snap;
  snap.vmName = "vm";
  snap.cpu.pc = 4;
  snap.cpu.regs[1] = 7;
  snap.memoryBytes = std::vector<uint8_t>(32, 11);

  StateSerializer serializer;
  const MigrationPacket packet = serializer.Serialize(snap);
  VmSnapshot restored;
  std::string error;
  assert(serializer.Deserialize(packet, restored, error));
  assert(restored.vmName == "vm");
  assert(restored.cpu.pc == 4);
  assert(restored.cpu.regs[1] == 7);
}

void TestVmMemoryInstructions() {
  VmConfig cfg {.name = "t", .execSlice = 10, .priority = 1, .programPath = "inline", .memoryBytes = 4096};
  std::vector<std::string> lines = {"li $1, 123", "sw $1, 0", "lw $2, 0", "HALT"};
  VirtualMachine vm(cfg, ParseProgram(lines));
  vm.RunForSlice(10);
  assert(vm.Cpu().regs[2] == 123);
}

int main() {
  TestVirtualMemoryReadWrite();
  TestPageFault();
  TestSchedulers();
  TestSerializerRoundTrip();
  TestVmMemoryInstructions();
  std::cout << "All tests passed\n";
  return 0;
}
