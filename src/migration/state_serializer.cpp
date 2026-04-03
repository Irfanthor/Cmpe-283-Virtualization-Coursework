#include "migration/state_serializer.hpp"

#include <cstring>

namespace vmm {

namespace {

template <typename T>
void Append(std::vector<uint8_t>& out, const T& value) {
  const auto* ptr = reinterpret_cast<const uint8_t*>(&value);
  out.insert(out.end(), ptr, ptr + sizeof(T));
}

template <typename T>
void Read(const std::vector<uint8_t>& in, std::size_t& offset, T& value) {
  std::memcpy(&value, in.data() + offset, sizeof(T));
  offset += sizeof(T);
}

}  // namespace

MigrationPacket StateSerializer::Serialize(const VmSnapshot& snapshot) const {
  MigrationPacket packet;
  packet.payload.clear();
  const uint32_t nameLen = static_cast<uint32_t>(snapshot.vmName.size());
  Append(packet.payload, nameLen);
  packet.payload.insert(packet.payload.end(), snapshot.vmName.begin(), snapshot.vmName.end());
  Append(packet.payload, snapshot.cpu.pc);
  Append(packet.payload, snapshot.cpu.hi);
  Append(packet.payload, snapshot.cpu.lo);
  for (int32_t reg : snapshot.cpu.regs) {
    Append(packet.payload, reg);
  }
  const uint32_t memSize = static_cast<uint32_t>(snapshot.memoryBytes.size());
  Append(packet.payload, memSize);
  packet.payload.insert(packet.payload.end(), snapshot.memoryBytes.begin(), snapshot.memoryBytes.end());
  packet.checksum = ComputeChecksum(packet.payload);
  return packet;
}

bool StateSerializer::Deserialize(const MigrationPacket& packet, VmSnapshot& snapshot, std::string& error) const {
  if (packet.version != 1) {
    error = "unsupported migration version";
    return false;
  }
  if (packet.checksum != ComputeChecksum(packet.payload)) {
    error = "migration checksum mismatch";
    return false;
  }
  std::size_t offset = 0;
  uint32_t nameLen = 0;
  Read(packet.payload, offset, nameLen);
  snapshot.vmName.assign(reinterpret_cast<const char*>(packet.payload.data() + offset), nameLen);
  offset += nameLen;
  Read(packet.payload, offset, snapshot.cpu.pc);
  Read(packet.payload, offset, snapshot.cpu.hi);
  Read(packet.payload, offset, snapshot.cpu.lo);
  for (auto& reg : snapshot.cpu.regs) {
    Read(packet.payload, offset, reg);
  }
  uint32_t memSize = 0;
  Read(packet.payload, offset, memSize);
  snapshot.memoryBytes.resize(memSize);
  std::memcpy(snapshot.memoryBytes.data(), packet.payload.data() + offset, memSize);
  return true;
}

uint32_t StateSerializer::ComputeChecksum(const std::vector<uint8_t>& bytes) const {
  uint32_t hash = 2166136261u;
  for (const auto b : bytes) {
    hash ^= b;
    hash *= 16777619u;
  }
  return hash;
}

}  // namespace vmm
