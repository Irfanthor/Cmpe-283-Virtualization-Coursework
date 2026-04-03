# C++ Virtualization 

This repository includes CMPE 283 coursework and includes a modular hypervisor-style MIPS VM runtime designed to showcase C++ systems engineering and virtualization concepts.

## What This Demonstrates In C++

- Modular architecture with clear subsystem boundaries (`cpu`, `memory`, `vm`, `scheduler`, `migration`, `hypervisor`, `metrics`)
- Modern C++ data modeling with strong types and interfaces (`enum class`, `std::array`, RAII-style composition)
- Testable design via dependency inversion (`IScheduler`, `IMigrationTransport`)
- Build and quality infrastructure using CMake, tests, CI, and sanitizer-ready debug builds

## Virtualization Features Implemented

- Multi-VM execution with pluggable schedulers:
  - round-robin
  - priority-based
  - multi-level feedback queue (MLFQ)
- Per-VM paged memory virtualization:
  - virtual-to-physical translation
  - page-table lookup and TLB cache
  - fault handling (`PageFault`, `AddressOutOfRange`)
- Trap/fault model for illegal instructions and divide-by-zero
- Snapshot object model and snapshot service
- Versioned migration packet serialization with checksum validation
- Migration protocol handshake + retry semantics
- Pre-copy style migration simulation with migration duration metrics

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run

```bash
./build/myvmm round_robin
./build/myvmm priority
./build/myvmm mlfq
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Benchmark/Demo

- End-to-end demo: `scripts/run_demo.sh`
- Basic benchmark harness: `scripts/benchmark.sh`

## Project Layout

- `include/` public interfaces and shared types
- `src/` subsystem implementations
- `tests/` unit/integration-style checks
- `docs/` architecture, memory virtualization, migration protocol, and performance reporting notes
- `.github/workflows/ci.yml` CI build/test pipeline

## Legacy Coursework Files

The original assignment implementations are preserved:

- `project.cc`
- `homework1.cc`
- `homework-2 and 3-client.cc`
- `homework-2 and 3- server.cc`
