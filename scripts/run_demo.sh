#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

echo "=== Round Robin ==="
./build/myvmm round_robin
echo "=== Priority ==="
./build/myvmm priority
echo "=== MLFQ ==="
./build/myvmm mlfq

echo "=== Tests ==="
ctest --test-dir build --output-on-failure
