#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

for scheduler in round_robin priority mlfq; do
  echo "Running benchmark for ${scheduler}"
  /usr/bin/time -p ./build/myvmm "${scheduler}" >/tmp/vmm_"${scheduler}".log
  echo "--- output (${scheduler}) ---"
  cat /tmp/vmm_"${scheduler}".log
done
