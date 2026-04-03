# Memory Virtualization

Each VM owns a virtual address space backed by a private byte array.

- Page size: 4 KiB
- Translation: virtual page -> physical page from page table
- Fast path: TLB cache for recently translated pages
- Faults:
  - page fault when no mapping exists
  - address-out-of-range when translated physical address exceeds backing store

`lw` and `sw` instructions operate on virtual addresses and pass through this translation path.
