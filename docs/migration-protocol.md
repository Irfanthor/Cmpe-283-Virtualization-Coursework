# Migration Protocol

The migration pipeline now uses explicit protocol semantics:

1. `Handshake(target, version)` validates compatibility.
2. VM state is serialized with `StateSerializer` to a versioned packet.
3. Packet checksum is computed and attached.
4. Sender retries delivery up to 3 times on transient failures.
5. Receiver validates checksum and version before restoring state.

The migration service performs a simple pre-copy simulation (multiple short execution rounds before final transfer) and records migration duration metrics.
