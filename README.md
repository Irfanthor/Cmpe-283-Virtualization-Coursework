# Hypervisor Development for MIPS Processor Emulation

Masters-level coursework implementing a **MIPS instruction-level emulator** that acts as a hypervisor: it runs multiple virtual machines (VMs) executing MIPS-style assembly, with round-robin scheduling, VM snapshotting, and live migration over the network.

---

## Project Overview

This project is a C++ hypervisor that **emulates a MIPS processor** at the instruction level. It supports:

- **Multiple VMs** running MIPS assembly from text files
- **Round-robin scheduling** with configurable execution slices per VM
- **Para-instructions** for debugging and management: `DUMP_PROCESSOR_STATE`, `SNAPSHOT`, and `MIGRATE`
- **Snapshot save/restore** so VMs can be started from a saved state
- **Live migration** (Homework 2 & 3): VM state is sent over TCP to another host, which continues execution

The codebase is organized by assignment: a base **project**, **Homework 1** (multi-VM + snapshots), and **Homework 2 & 3** (client–server live migration).

---

## Repository Structure

| File | Description |
|------|-------------|
| `project.cc` | Base hypervisor: two VMs, round-robin, config from files. No snapshot or migration. Uses `input_files/`. |
| `homework1.cc` | Homework 1: full standalone hypervisor with two VMs, `DUMP_PROCESSOR_STATE`, `SNAPSHOT`, snapshot restore via `-s`. Uses `input_files_hw3/`. |
| `homework-2 and 3-client.cc` | Client for live migration: runs one VM, executes until `MIGRATE <host>`, then sends VM state to server and exits. Uses Winsock (Windows). |
| `homework-2 and 3- server.cc` | Server for live migration: accepts VM state (registers, PC, binary name), resumes execution from that PC. Uses Winsock (Windows), port 8080. |

---

## Requirements

- **C++ compiler** (e.g. `g++`)
- **project / homework1:** any OS with standard C++ and POSIX (`getopt` for homework1)
- **Homework 2 & 3 (migration):** Windows (Winsock2) for both client and server

---

## Build Instructions

### Project (base hypervisor)

```bash
g++ project.cc -o myvmm
```

### Homework 1 (standalone hypervisor with snapshots)

```bash
g++ homework1.cc -o myvmm
```

### Homework 2 & 3 (live migration)

- **Client:** compile `homework-2 and 3-client.cc` with Winsock2 (e.g. Visual Studio or `g++` on Windows with `-lws2_32`).
- **Server:** compile `homework-2 and 3-server.cc` with Winsock2, same way.

Example (Windows, MinGW-style):

```bash
g++ "homework-2 and 3-client.cc" -o client -lws2_32 -lwsock32
g++ "homework-2 and 3-server.cc" -o server -lws2_32 -lwsock32
```

---

## Configuration

Each VM is defined by a **config file** (one per VM). The hypervisor reads:

- **Path:** `input_files/` (project) or `input_files_hw3/` (homework1 and migration).
- **Format:** one key=value per line:

```
vm_exec_slice_in_instructions=100
vm_binary=path/to/vm_assembly_file
```

- **`vm_exec_slice_in_instructions`** – Number of instructions to run for this VM before switching to the next (round-robin).
- **`vm_binary`** – Path (relative to `input_files/` or `input_files_hw3/`) to the file containing the VM’s assembly instructions.

You must create the `input_files/` or `input_files_hw3/` directory and place config files and assembly files there.

---

## Usage

### Project / Homework 1: Running the hypervisor

Run with two VMs (two config files):

```bash
./myvmm -v <config_vm1> -v <config_vm2>
```

Example:

```bash
./myvmm -v assembly_file_vm1.txt -v assembly_file_vm2.txt
```

Here, `assembly_file_vm1.txt` and `assembly_file_vm2.txt` are **config file names** (not the raw assembly). They live under `input_files/` or `input_files_hw3/` and point to the actual binary (assembly) file via `vm_binary`.

**Start one VM from a snapshot (homework1 only):**

```bash
./myvmm -v config_vm1 -s snapshot_vm1 -v config_vm2
```

`snapshot_vm1` is the snapshot file (e.g. produced by `SNAPSHOT snapshot_vm1` in the assembly).

### Homework 2 & 3: Live migration

1. On the **destination** machine, start the server first (listens on port 8080):
   ```bash
   ./server
   ```
2. On the **source** machine, run the client with one VM:
   ```bash
   ./client -v <config_vm1>
   ```
3. When the VM executes `MIGRATE <hostname_or_ip>`, the client sends its state (registers, PC, binary filename) to that host:port, then exits. The server receives the state and continues execution from the given PC in the given binary.

**Note:** The client code may need to be updated to use the hostname/IP from the `MIGRATE` instruction when connecting, instead of a hardcoded address.

---

## Assembly Input Format

- One **instruction per line**.
- **Operands:** register names use `$` (e.g. `$0`, `$31`). Immediates are decimal.
- **Examples:**
  - `li $1, 42`
  - `add $3, $1, $2`
  - `addi $4, $1, 10`

---

## Supported MIPS Instructions

| Instruction | Description |
|-------------|-------------|
| `li` | Load immediate |
| `add`, `addi` | Add (signed) |
| `addu`, `addiu` | Add unsigned |
| `sub`, `subu` | Subtract |
| `mul` | Multiply (low 32 bits) |
| `mult` | Multiply; result in `hi`, `low` |
| `div` | Divide; quotient in `low`, remainder in `hi` |
| `and`, `andi` | Bitwise AND |
| `or`, `ori` | Bitwise OR |
| `xor` | Bitwise XOR |
| `sll`, `srl` | Shift left/right logical |

Each VM has 32 general-purpose registers plus **hi** and **low** (used by `mult`/`div`).

---

## Para-Instructions

These are special “instructions” in the assembly stream, not real MIPS opcodes:

| Para-instruction | Effect |
|------------------|--------|
| `DUMP_PROCESSOR_STATE` | Prints all registers (R0–R31) and hi/low for that VM. |
| `SNAPSHOT <filename>` | Saves current VM state (all registers, hi, low) to `<filename>`. |
| `MIGRATE <hostname_or_ip>` | (Client only) Sends VM state and PC to the migration server at the given host (port 8080), then exits. Server continues execution. |

---

## Snapshot File Format

Snapshot files (from `SNAPSHOT` or used with `-s`) are text:

```
R0=<value>
R1=<value>
...
R31=<value>
hi=<value>
low=<value>
```

The hypervisor restores registers and hi/low from this file when starting a VM with `-s`.

---

## Homework Summary

| Homework | Focus | Main additions |
|----------|--------|----------------|
| **1** | Multi-VM + snapshots | Two VMs, round-robin, config files, `DUMP_PROCESSOR_STATE`, `SNAPSHOT`, `-s` for restore. |
| **2** | VM snapshotting | Save/restore VM state to/from files; start from snapshot or from beginning of assembly. |
| **3** | Live migration | Client–server over TCP: client sends state + PC + binary name; server resumes execution (Winsock, port 8080). |

---

## References

- [MIPS Instruction Set](https://www.dsi.unive.it/~gasparetto/materials/MIPS_Instruction_Set.pdf)
- [MIPS Registers](http://homepage.divms.uiowa.edu/~ghosh/1-28-10.pdf)

---

## Summary

This coursework implements a MIPS-level hypervisor in C++ with multi-VM scheduling, config-driven execution slices, processor state dump, VM snapshotting, and live migration via a TCP client–server. It illustrates core virtualization concepts: VM state, scheduling, state persistence, and migration across hosts.
