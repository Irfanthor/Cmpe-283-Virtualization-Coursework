cmake_minimum_required(VERSION 3.20)
project(vmm_showcase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(VMM_ENABLE_SANITIZERS "Enable ASAN/UBSAN in Debug" ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND VMM_ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

include_directories(include)

add_library(vmm_core
  src/cpu/cpu_state.cpp
  src/cpu/instruction.cpp
  src/memory/virtual_memory.cpp
  src/vm/virtual_machine.cpp
  src/scheduler/round_robin_scheduler.cpp
  src/scheduler/priority_scheduler.cpp
  src/scheduler/mlfq_scheduler.cpp
  src/metrics/metrics_tracer.cpp
  src/snapshot/snapshot_service.cpp
  src/migration/state_serializer.cpp
  src/migration/migration_protocol.cpp
  src/migration/migration_service.cpp
  src/hypervisor/hypervisor_core.cpp
)

add_executable(myvmm src/main.cpp)
target_link_libraries(myvmm PRIVATE vmm_core)

enable_testing()
add_executable(vmm_tests tests/test_main.cpp)
target_link_libraries(vmm_tests PRIVATE vmm_core)
add_test(NAME vmm_tests COMMAND vmm_tests)
