# Architecture

The showcase version separates the hypervisor into reusable C++ modules:

- `src/cpu`: instruction model and parser
- `src/memory`: paged virtual memory with page-table and TLB cache
- `src/vm`: `VirtualMachine` execution, faults, and snapshot state
- `src/scheduler`: pluggable `IScheduler` with round-robin, priority, and MLFQ policies
- `src/migration`: versioned state serialization, checksums, retries, and pre-copy style migration
- `src/hypervisor`: orchestration loop and VM lifecycle control
- `src/metrics`: scheduler and migration metrics

```mermaid
flowchart TD
  cli[CLI_Config] --> hv[HypervisorCore]
  hv --> vmMgr[VmVector]
  hv --> scheduler[IScheduler]
  hv --> migr[MigrationService]
  hv --> metrics[MetricsTracer]
  vmMgr --> vm[VirtualMachine]
  vm --> cpu[InstructionExecutor]
  vm --> mem[VirtualMemoryPager]
  migr --> proto[MigrationProtocol]
  migr --> serializer[StateSerializerV1]
  proto --> transport[IMigrationTransport]
```
