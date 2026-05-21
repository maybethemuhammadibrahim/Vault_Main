# OS Memory Management – Grouped Numericals
**Textbook:** Operating System Concepts – Silberschatz

---

## Group 1: Effective Access Time (EAT) Calculations

### Formulas

```
EAT = (hit_ratio × memory_access_time) + (miss_ratio × page_fault_service_time)

For TLB systems:
EAT = (TLB_hit_ratio × 1 memory access)
    + (TLB_miss_no_fault × 2 memory accesses)
    + (TLB_miss_with_fault × fault_service_time)

Effective Instruction Time:
EAT = base_cycle
    + P(other_page) × extra_access_cost
    + P(page_fault) × page_fault_time
```

**Drum/Disk page fault time:**
```
Rotation latency   = (60 / RPM) / 2  seconds  (average = half rotation)
Transfer time      = page_size / transfer_rate
Page fault time    = latency + transfer_time
                   (×2 if modified page must be written out first)
```

---

### [10.4] Effective Instruction Time

**Given:**
- CPU cycle = 1 µs
- Extra cost to access another page = 1 µs
- Page size = 1,000 words
- Drum speed = 3,000 RPM → rotation time = 60/3000 = 20 ms per revolution
- Transfer rate = 1,000,000 words/s → transfer time = 1,000 / 1,000,000 = 1 ms
- Average rotational latency = 20/2 = **10 ms**
- Total page-in time = 10 + 1 = **11 ms**
- If page is modified → must write out + read in = 2 × 11 = **22 ms**
- P(accessing another page) = 1% = 0.01
- P(page already in memory | other page accessed) = 80% → P(page NOT in memory) = 20%
- P(page fault) = 0.01 × 0.20 = **0.002**
- P(modified on replacement) = 50%

**Page fault service time:**
```
= 0.50 × 22 ms + 0.50 × 11 ms
= 11 + 5.5
= 16.5 ms = 16,500 µs
```

**EAT:**
```
EAT = 1 µs (base)
    + 0.01 × 1 µs            (extra access cost for other-page refs)
    + 0.002 × 16,500 µs      (page fault penalty)

    = 1 + 0.01 + 33
    = 34.01 µs
```

**Answer: ~34 µs effective instruction time.**

---

### [10.21] Maximum Acceptable Page-Fault Rate

**Given:**
- Page fault service time (empty/clean frame) = 8 ms
- Page fault service time (modified frame) = 20 ms
- Memory access time = 100 ns
- P(modified) = 70%
- Target EAT ≤ 200 ns

**Average page fault time:**
```
= 0.30 × 8 ms + 0.70 × 20 ms
= 2.4 + 14
= 16.4 ms = 16,400,000 ns
```

**EAT formula:**
```
EAT = (1 − p) × 100 ns + p × 16,400,000 ns ≤ 200 ns
```

**Solving:**
```
100 − 100p + 16,400,000p ≤ 200
16,399,900p ≤ 100
p ≤ 100 / 16,399,900
p ≤ 0.0000061  ≈  6.1 × 10⁻⁶
```

**Answer: Page-fault rate must be ≤ ~0.00061% (about 1 in 164,000 accesses).**

---

### [10.36] Effective Memory Access Time with TLB and Page Faults

**Given:**
- Disk access + transfer = 20 ms = 20,000,000 ns
- Memory access time = 1 µs = 1,000 ns
- Two memory accesses needed per reference (page table in memory)
- TLB hit ratio = 80% → 1 memory access
- TLB miss, no fault = 18% → 2 memory accesses
- TLB miss, page fault = 2% → disk access time

**EAT:**
```
EAT = 0.80 × 1,000 ns
    + 0.18 × 2,000 ns
    + 0.02 × 20,000,000 ns

    = 800 + 360 + 400,000
    = 401,160 ns
    ≈ 401.16 µs
```

**Answer: Effective memory access time ≈ 401 µs.**

---

## Group 2: Virtual Address Translation

### Formulas

```
Page number  = virtual_address / page_size        (integer division)
Page offset  = virtual_address mod page_size

Physical address = (frame_number × page_size) + offset

For hex:
  page_size = 256 bytes = 0x100  → last 2 hex digits = offset, rest = page number
  page_size = 4096 bytes = 0x1000 → last 3 hex digits = offset, rest = page number
```

---

### [10.6] 12-bit Addresses, 256-byte Pages – Virtual to Physical

**Page table (given):**

| Page | Frame |
|------|-------|
| 0 | – |
| 1 | 2 |
| 2 | C |
| 3 | A |
| 4 | – |
| 5 | 4 |
| 6 | 3 |
| 7 | – |
| 8 | B |
| 9 | 0 |

Free frames: D, E, F (in order)

With 256-byte pages: last 2 hex digits = offset, first hex digit = page number.

**9EF:**
- Page = 9, Offset = EF
- Frame 9 → 0 → Physical = **0EF**

**111:**
- Page = 1, Offset = 11
- Frame 1 → 2 → Physical = **211**

**700:**
- Page = 7, Offset = 00
- Frame 7 → not in memory → **Page fault** → allocate frame D
- Physical = **D00**

**0FF:**
- Page = 0, Offset = FF
- Frame 0 → not in memory → **Page fault** → allocate frame E
- Physical = **EFF**

---

### [10.18] 12-bit Addresses, 256-byte Pages – With Page Faults & Free Frame Allocation

**Page table (given):**

| Page | Frame |
|------|-------|
| 0 | 4 |
| 1 | B |
| 2 | A |
| 3 | – |
| 4 | – |
| 5 | 2 |
| 6 | – |
| 7 | 0 |
| 8 | C |
| 9 | 1 |

Free frames: 9, F, D (in order)

**0x2A1:**
- Page = 2, Offset = A1
- Frame 2 → A → Physical = **0xAA1**

**0x4E6:**
- Page = 4, Offset = E6
- Frame 4 → not in memory → **Page fault** → allocate frame 9
- Update page table: page 4 → frame 9
- Physical = **0x9E6**

**0x94A:**
- Page = 9, Offset = 4A
- Frame 9 → 1 → Physical = **0x14A**

**0x316:**
- Page = 3, Offset = 16
- Frame 3 → not in memory → **Page fault** → allocate frame F
- Update page table: page 3 → frame F
- Physical = **0xF16**

---

### [10.20] 32-bit Virtual Memory, 22-bit Physical, 4KB Pages

**Given:**
- Virtual address space = 2³² bytes
- Physical memory = 2²² bytes
- Page size = 4,096 bytes = 2¹² bytes
- Virtual address = 0x11123456

**Page number and offset:**
```
Page size = 4096 = 0x1000 → last 3 hex digits = offset

Virtual address: 0x11123456
  Offset      = 0x456
  Page number = 0x11123   (= 69,923 in decimal)
```

**Steps (software vs hardware):**

- **Software (OS):** Maintains the page table in memory. On process start, OS sets up page table base register. On page fault, OS finds a free frame, loads the page from disk, updates the page table, and resumes the process.
- **Hardware (MMU):** On each memory access, the MMU reads the page table base register, looks up page 0x11123, extracts the frame number, and forms the physical address = `frame × 4096 + 0x456`. If the valid bit is 0, the MMU raises a page-fault trap (hands control back to OS).

---

### [10.22] 16-bit Addresses, 4096-byte Pages – LRU with Reference Bits

*(Page table image not provided — general method shown)*

**Page size = 4096 = 0x1000 → last 3 hex digits = offset, first hex digit = page number**

For each virtual address:
1. Extract page number (top 4 bits / first hex digit).
2. Look up page table → get frame number.
3. Physical address = frame × 0x1000 + offset.
4. Set reference bit = 1 for that page.

**Example translations (using method):**

| Virtual Addr | Page | Offset | Physical (frame F → F×0x1000+offset) |
|-------------|------|--------|--------------------------------------|
| 0x621C | 6 | 0x21C | Look up page 6 frame → frame×4096 + 0x21C |
| 0xF0A3 | F | 0x0A3 | Look up page F frame → frame×4096 + 0x0A3 |
| 0xBC1A | B | 0xC1A | Look up page B frame → frame×4096 + 0xC1A |
| 0x5BAA | 5 | 0xBAA | Look up page 5 frame → frame×4096 + 0xBAA |
| 0x0BA1 | 0 | 0xBA1 | Look up page 0 frame → frame×4096 + 0xBA1 |

**b. Example of a page fault:**
Any virtual address whose page number maps to a dash (–) in the page table causes a page fault. E.g., if page 3 has no frame, then `0x3000` causes a page fault.

**c. LRU replacement pool:**
LRU (localized) picks from the frames currently assigned to this process. It evicts whichever frame was least recently used — i.e., the frame whose page has the oldest last-access time (reference bit = 0 for longest).

---

## Group 3: Page Fault Count with Reference Strings

### Formulas

```
Simulate frame contents step by step.
On each reference:
  - If page already in a frame → HIT (no fault)
  - If page not in any frame   → FAULT → load page, replace if needed

LRU replacement: evict the page that was used least recently.
OPT replacement: evict the page that will be used farthest in the future.
```

---

### [10.7] Page Faults for 2D Array Access – LRU, 3 Frames

**Setup:**
- Array A[100][100], integers (assume 1 word each), stored **row-major**.
- A[0][0] is at location 200. Page size = 200 words.
- Page 0 (locations 0–199): process code → always in frame 1.
- Two remaining frames for data pages.
- Data pages: A[0][0]–A[0][99] are on page 1 (loc 200–399), A[1][0]–A[1][99] on page 2, etc. Each row fits exactly in one page.

**a. Column-major loop: `for j … for i … A[i][j] = 0`**

This accesses elements column by column: A[0][0], A[1][0], A[2][0], … A[99][0], A[0][1], …

Each step moves down a column, jumping across rows → jumping across pages every element.

- There are 100 columns × 100 rows = 10,000 accesses.
- Every access hits a different page (since consecutive accesses are from different rows = different pages).
- With only 2 data frames available, LRU constantly evicts → nearly every access is a fault.

**Page faults ≈ 10,000** (one per element, since no page reuse within the 2-frame window).

**b. Row-major loop: `for i … for j … A[i][j] = 0`**

Accesses row by row: A[0][0]–A[0][99], then A[1][0]–A[1][99], …

Each row = 1 page = 100 consecutive accesses. Once a page is loaded, all 100 elements are accessed before the page is evicted.

- 100 rows × 1 page fault per row = **100 page faults** (one fault when each new row's page is first loaded).

---

## Group 4: Page Replacement Algorithm Design

### [10.35] Load-Balancing Page Replacement Algorithm

**a. Algorithm definition:**

- **Counter per frame:** Counts how many distinct pages have been loaded into that frame since some reference point.
- **Initial value:** 0 for all frames at start.
- **When increased:** Each time a new page is loaded into a frame, increment that frame's counter by 1.
- **When decreased:** Counters are NOT decreased (they track cumulative load, not current load). Alternatively, you can reset all counters periodically.
- **Replacement policy:** When a page fault occurs, find the frame with the **smallest counter** (least loaded) and replace the page in that frame. Ties broken arbitrarily (e.g., lowest frame number).

The idea: spread new pages evenly across frames instead of always hitting the same "hot" frames.

---

**b. Page faults for reference string with 4 frames:**

Reference string: `1, 2, 3, 4, 5, 3, 4, 1, 6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 5, 4, 2`

*Simulate: load into smallest-counter frame on fault; counter increments on load.*

| Step | Ref | Frames (f1,f2,f3,f4) | Counters | Fault? |
|------|-----|----------------------|----------|--------|
| 1 | 1 | 1,–,–,– | 1,0,0,0 | ✓ |
| 2 | 2 | 1,2,–,– | 1,1,0,0 | ✓ |
| 3 | 3 | 1,2,3,– | 1,1,1,0 | ✓ |
| 4 | 4 | 1,2,3,4 | 1,1,1,1 | ✓ |
| 5 | 5 | 5,2,3,4 | 2,1,1,1 | ✓ (replace f1, smallest=f1, tie→f1) |
| 6 | 3 | 5,2,3,4 | 2,1,1,1 | – |
| 7 | 4 | 5,2,3,4 | 2,1,1,1 | – |
| 8 | 1 | 5,1,3,4 | 2,2,1,1 | ✓ (replace f2) |
| 9 | 6 | 5,1,6,4 | 2,2,2,1 | ✓ (replace f4) |
| 10 | 7 | 5,1,6,7 | 2,2,2,2 | ✓ (replace f1, tie→f1) |
| 11 | 8 | 8,1,6,7 | 3,2,2,2 | ✓ (replace f1) |
| 12 | 7 | 8,1,6,7 | 3,2,2,2 | – |
| 13 | 8 | 8,1,6,7 | 3,2,2,2 | – |
| 14 | 9 | 8,9,6,7 | 3,3,2,2 | ✓ (replace f2) |
| 15 | 7 | 8,9,6,7 | 3,3,2,2 | – |
| 16 | 8 | 8,9,6,7 | 3,3,2,2 | – |
| 17 | 9 | 8,9,6,7 | 3,3,2,2 | – |
| 18 | 5 | 8,9,5,7 | 3,3,3,2 | ✓ (replace f3) |
| 19 | 4 | 8,9,5,4 | 3,3,3,3 | ✓ (replace f4) |
| 20 | 5 | 8,9,5,4 | 3,3,3,3 | – |
| 21 | 4 | 8,9,5,4 | 3,3,3,3 | – |
| 22 | 2 | 2,9,5,4 | 4,3,3,3 | ✓ (replace f1) |

**Total page faults: 14**

---

**c. Optimal (OPT) minimum page faults – 4 frames:**

Reference string: `1, 2, 3, 4, 5, 3, 4, 1, 6, 7, 8, 7, 8, 9, 7, 8, 9, 5, 4, 5, 4, 2`

OPT: replace the page that will **not be used for the longest time** in the future.

| Step | Ref | Frames | Fault? | Notes |
|------|-----|--------|--------|-------|
| 1 | 1 | 1,–,–,– | ✓ | |
| 2 | 2 | 1,2,–,– | ✓ | |
| 3 | 3 | 1,2,3,– | ✓ | |
| 4 | 4 | 1,2,3,4 | ✓ | |
| 5 | 5 | 1,2,3→5,4 | ✓ | Replace 3? Next use of 1=8, 2=22, 3=6, 4=7 → evict 2 (next use farthest at pos 22) → {1,5,3,4} |
| 6 | 3 | 1,5,3,4 | – | |
| 7 | 4 | 1,5,3,4 | – | |
| 8 | 1 | 1,5,3,4 | – | |
| 9 | 6 | 1,6,3,4 | ✓ | Replace 5 (next use=18, farthest among 1→none,5→18,3→none,4→none; compare: 1 used at 1, 5 at 18, 3 at 6, 4 at 7 future→5 farthest) |
| 10 | 7 | 1,6,7,4 | ✓ | Replace 3 (not used again) → {1,6,7,4} |
| 11 | 8 | 1,6,7→8,4→? | ✓ | Replace 6 (not used again) → {1,8,7,4} |
| 12 | 7 | 1,8,7,4 | – | |
| 13 | 8 | 1,8,7,4 | – | |
| 14 | 9 | 9,8,7,4 | ✓ | Replace 1 (not used again after this) → {9,8,7,4} |
| 15 | 7 | 9,8,7,4 | – | |
| 16 | 8 | 9,8,7,4 | – | |
| 17 | 9 | 9,8,7,4 | – | |
| 18 | 5 | 9,8,5,4 | ✓ | Replace 7 (not used again) → {9,8,5,4} |
| 19 | 4 | 9,8,5,4 | – | |
| 20 | 5 | 9,8,5,4 | – | |
| 21 | 4 | 9,8,5,4 | – | |
| 22 | 2 | 2,8,5,4 | ✓ | Replace 9 (not used again) |

**Minimum page faults with OPT: 11**

---

## Group 5: Buddy System Memory Allocation

### How Buddy System Works

- Memory is split into power-of-2 sized blocks.
- A request of size X is satisfied by the smallest block ≥ X that is a power of 2.
- When a block is freed, if its **buddy** (the adjacent block of the same size it was split from) is also free, they **coalesce** back into a larger block.

**Block sizes used:** 1024, 512, 256, 128, 64, 32, 16, 8 KB (powers of 2)

---

### [10.40] 1024-KB Segment – Allocations then Releases

**Round up each request to next power of 2:**

| Request | Rounded up to |
|---------|--------------|
| 5 KB | 8 KB |
| 135 KB | 256 KB |
| 14 KB | 16 KB |
| 3 KB | 8 KB |
| 12 KB | 16 KB |

---

**Allocation tree (step by step):**

```
Start: [1024 KB free]

Request 8 KB (for 5 KB):
  Split 1024 → 512 + 512
  Split 512  → 256 + 256
  Split 256  → 128 + 128
  Split 128  → 64 + 64
  Split 64   → 32 + 32
  Split 32   → 16 + 16
  Split 16   → 8 + 8
  Allocate left 8 KB  [A=5KB in 8KB block]
  Free: 8, 16, 32, 64, 128, 256, 512 KB blocks

Request 256 KB (for 135 KB):
  Allocate the free 256 KB block  [B=135KB in 256KB block]
  Free: 8, 16, 32, 64, 128, 512 KB blocks

Request 16 KB (for 14 KB):
  Allocate the free 16 KB block  [C=14KB in 16KB block]
  Free: 8, 32, 64, 128, 512 KB blocks

Request 8 KB (for 3 KB):
  Allocate the free 8 KB block (buddy of A)  [D=3KB in 8KB block]
  Free: 32, 64, 128, 512 KB blocks

Request 16 KB (for 12 KB):
  Split 32 → 16 + 16
  Allocate left 16 KB  [E=12KB in 16KB block]
  Free: 16, 64, 128, 512 KB blocks
```

**Memory layout after all allocations:**

```
[1024 KB total]
├── [512 KB] → Free
└── [512 KB]
    ├── [256 KB] → B (135 KB allocated)
    └── [256 KB]
        ├── [128 KB] → Free
        └── [128 KB]
            ├── [64 KB] → Free
            └── [64 KB]
                ├── [32 KB]
                │   ├── [16 KB] → C (14 KB allocated)
                │   └── [16 KB] → E (12 KB allocated)
                └── [32 KB]
                    ├── [16 KB] → Free
                    └── [16 KB]
                        ├── [8 KB] → A (5 KB allocated)
                        └── [8 KB] → D (3 KB allocated)
```

---

**Releases (with coalescing):**

**Release 3 KB (block D, 8 KB):**
- D freed → its buddy A (8 KB) is still allocated → **no coalesce**
- Free: 8 (D's slot), 16, 64, 128, 512 KB

**Release 5 KB (block A, 8 KB):**
- A freed → buddy (D's slot) is now free → **coalesce to 16 KB**
- New 16 KB is buddy of "C's 16 KB" slot? C is still allocated → **no further coalesce**
- Free: 16 (A+D coalesced), 16, 64, 128, 512 KB

**Release 14 KB (block C, 16 KB):**
- C freed → buddy is the 16 KB from A+D coalesce → both free → **coalesce to 32 KB**
- 32 KB buddy is the 32 KB containing E → E still allocated → **no further coalesce**
- Free: 32 (A+D+C coalesced), 64, 128, 512 KB

**Release 12 KB (block E, 16 KB):**
- E freed → 32 KB buddy (A+D+C) is free → **coalesce to 64 KB**
- 64 KB buddy is also free → **coalesce to 128 KB**
- 128 KB buddy is also free → **coalesce to 256 KB**
- 256 KB buddy is B (still allocated) → **no further coalesce**
- Free: 256 KB, 512 KB

*(If B were also released, the full 1024 KB would coalesce back.)*

**Final state: 256 KB free + 512 KB free (= 768 KB free), B (256 KB) still allocated.**

---
*End of document*