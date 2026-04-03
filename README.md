# Virtual Machine Monitor (MIPS) in C++

This repository contains two parts of the same project journey:

1. Original CMPE 283 coursework implementation (single-file milestones)
2. Refactored modular showcase implementation (cleaner architecture for extension and testing)

Together, they demonstrate both practical virtualization concepts and stronger C++ systems design.

## What this project is about

This project implements a MIPS-style instruction-level virtual machine monitor (VMM) in C++.
It can run virtual machines, manage VM state, schedule execution, and support snapshot and migration workflows.

The coursework version shows incremental feature delivery.
The modular version shows how the same ideas can be structured as a maintainable systems codebase.

## Repository at a glance

### A) Legacy coursework files 

- `project.cc`  
  Base hypervisor implementation with multi-VM execution and round-robin slicing.
- `homework1.cc`  
  Adds para-instructions like `DUMP_PROCESSOR_STATE`, `SNAPSHOT`, and snapshot restore.
- `homework-2 and 3-client.cc`  
  Migration source/client side (sends VM state over TCP).
- `homework-2 and 3- server.cc`  
  Migration destination/server side (receives VM state and resumes execution).

These files represent the original assignment milestones and are kept unchanged for reference.

### B) Modular showcase implementation 

- `include/` and `src/` split by subsystem:
  - `cpu`
  - `memory`
  - `vm`
  - `scheduler`
  - `migration`
  - `hypervisor`
  - `metrics`
  - `snapshot`

This version is intended to better showcase C++ architecture and virtualization depth.

## What the modular version adds

### C++ engineering value

- Clear subsystem boundaries and interface-driven design
- Reusable abstractions (`IScheduler`, `IMigrationTransport`)
- Cleaner data modeling (typed VM state, fault types, instruction model)
- Better testability and extensibility
- CMake-based build, CI, and test scaffolding

### Virtualization value

- Multi-VM orchestration with pluggable scheduling policies
- Per-VM virtual memory model (page table + TLB concept)
- Fault and trap handling (illegal opcode, divide-by-zero, page and memory faults)
- Snapshot state model for save and restore flows
- Migration packet versioning, checksum, handshake, and retry logic
- Pre-copy style migration simulation and migration timing metrics

## Build and run (modular implementation)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run with scheduler policy:

```bash
./build/myvmm round_robin
./build/myvmm priority
./build/myvmm mlfq
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

## Scripts and docs

- Demo script: `scripts/run_demo.sh`
- Benchmark helper: `scripts/benchmark.sh`
- Design docs:
  - `docs/architecture.md`
  - `docs/memory-virtualization.md`
  - `docs/migration-protocol.md`
  - `docs/performance-results.md`

## How to read this repo

If you want the coursework progression, start with the legacy files in this order:

1. `project.cc`
2. `homework1.cc`
3. `homework-2 and 3-client.cc` and `homework-2 and 3- server.cc`

If you want the portfolio and system-design version, start with:

1. `include/`
2. `src/hypervisor/`
3. `src/vm/`, `src/memory/`, `src/scheduler/`, `src/migration/`
4. `tests/` and `docs/`

## References

- [MIPS Instruction Set](https://www.dsi.unive.it/~gasparetto/materials/MIPS_Instruction_Set.pdf)
- [MIPS Registers](http://homepage.divms.uiowa.edu/~ghosh/1-28-10.pdf)
