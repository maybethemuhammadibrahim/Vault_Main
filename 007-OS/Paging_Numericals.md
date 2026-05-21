# Paging Numericals — Formulas, Techniques & Solved Questions

---

## Common Formulas & Techniques

These formulas are used repeatedly across all paging problems. **Master these first.**

### Core Formulas

| # | Formula | What It Gives You |
|---|---------|-------------------|
| 1 | `Logical Address Bits = log₂(Number of Pages × Page Size)` | Total bits in a logical address |
| 2 | `Physical Address Bits = log₂(Number of Frames × Frame Size)` | Total bits in a physical address |
| 3 | `Page Offset Bits = log₂(Page Size)` | Bits needed to address within a page/frame |
| 4 | `Page Number Bits = Logical Address Bits − Offset Bits` | Bits for the page number portion |
| 5 | `Frame Number Bits = Physical Address Bits − Offset Bits` | Bits for the frame number portion |
| 6 | `Page Number = floor(Address ÷ Page Size)` | Which page a decimal address falls in |
| 7 | `Page Offset = Address mod Page Size` | Position within that page |
| 8 | `Conventional PT Entries = 2^(Page Number Bits)` | Entries in a single-level page table (one per virtual page) |
| 9 | `Inverted PT Entries = 2^(Frame Number Bits)` | Entries in inverted page table (one per physical frame) |
| 10 | `Max Physical Memory = Number of Frames × Frame Size` | Total addressable physical memory |
| 11 | `EAT = hit_ratio × (TLB_time + mem_time) + (1 − hit_ratio) × (TLB_time + mem_time + mem_time)` | Effective Access Time with TLB |

### Key Technique: Converting to Powers of 2

Almost every paging problem boils down to expressing things as powers of 2, then counting bits.

| Value | Power of 2 | Bits |
|-------|-----------|------|
| 1 KB | 2¹⁰ | 10 bits |
| 2 KB | 2¹¹ | 11 bits |
| 4 KB | 2¹² | 12 bits |
| 8 KB | 2¹³ | 13 bits |
| 1 MB | 2²⁰ | 20 bits |
| 16 MB | 2²⁴ | 24 bits |
| 32 MB | 2²⁵ | 25 bits |
| 64 MB | 2²⁶ | 26 bits |
| 128 MB | 2²⁷ | 27 bits |
| 256 MB | 2²⁸ | 28 bits |
| 512 MB | 2²⁹ | 29 bits |
| 1 GB | 2³⁰ | 30 bits |
| 4 GB | 2³² | 32 bits |

**Quick rule:** `log₂(X)` = "how many bits do I need to represent X things?" If X = 2^k, the answer is k.

---

## Pattern 1: Bits in Logical & Physical Address

### How to Solve (Plain English Steps)

1. **Find the total logical address space size:**
   - Multiply: `Number of Pages × Page Size` = total logical space in bytes/words
   - Convert to a power of 2

2. **Logical address bits** = `log₂(Total Logical Space)`
   - Alternatively: `log₂(Pages) + log₂(Page Size)` — since log of a product = sum of logs

3. **Find the total physical address space size:**
   - Multiply: `Number of Frames × Frame Size` (frame size = page size, always)
   - Convert to a power of 2

4. **Physical address bits** = `log₂(Total Physical Space)`
   - Alternatively: `log₂(Frames) + log₂(Frame Size)`

**Relevant formulas:** #1, #2, #3 from the table above.

---

### Worked Example (Covers Everything)

> **Q:** Consider a logical address space of **128 pages** of **2,048 bytes each**, mapped onto a physical memory of **64 frames**.
> a. How many bits in the logical address?
> b. How many bits in the physical address?
> c. How many bits for the page number? For the offset? For the frame number?

**Solution:**

**Step 1 — Express everything as powers of 2:**
- 128 pages = 2⁷
- 2,048 bytes = 2¹¹ (this is the page size = frame size)
- 64 frames = 2⁶

**Step 2 — Logical address bits:**
- Total logical space = 2⁷ × 2¹¹ = 2¹⁸ bytes
- **Logical address = 18 bits**

**Step 3 — Physical address bits:**
- Total physical space = 2⁶ × 2¹¹ = 2¹⁷ bytes
- **Physical address = 17 bits**

**Step 4 — Breakdown:**
- Offset bits = log₂(page size) = log₂(2¹¹) = **11 bits** (same in both logical and physical)
- Page number bits = 18 − 11 = **7 bits** (matches: 2⁷ = 128 pages ✓)
- Frame number bits = 17 − 11 = **6 bits** (matches: 2⁶ = 64 frames ✓)

```
Logical:  [ Page Number (7 bits) | Offset (11 bits) ] = 18 bits total
Physical: [ Frame Number (6 bits) | Offset (11 bits) ] = 17 bits total
```

---

### Question 1.1

> Consider a logical address space of **64 pages** of **1,024 words each**, mapped onto a physical memory of **32 frames**.
> a. How many bits in the logical address?
> b. How many bits in the physical address?

**Solution:**

Express as powers of 2:
- 64 pages = 2⁶
- 1,024 words = 2¹⁰ (page size)
- 32 frames = 2⁵

**(a) Logical address bits:**
- Total logical space = 2⁶ × 2¹⁰ = 2¹⁶
- **Logical address = 16 bits**

**(b) Physical address bits:**
- Total physical space = 2⁵ × 2¹⁰ = 2¹⁵
- **Physical address = 15 bits**

```
Logical:  [ Page # (6 bits) | Offset (10 bits) ] = 16 bits
Physical: [ Frame # (5 bits) | Offset (10 bits) ] = 15 bits
```

---

### Question 1.2

> Consider a logical address space of **256 pages** with a **4-KB page size**, mapped onto a physical memory of **64 frames**.
> a. How many bits in the logical address?
> b. How many bits in the physical address?

**Solution:**

Express as powers of 2:
- 256 pages = 2⁸
- 4 KB = 2¹² bytes (page size)
- 64 frames = 2⁶

**(a) Logical address bits:**
- Total = 2⁸ × 2¹² = 2²⁰
- **Logical address = 20 bits**

**(b) Physical address bits:**
- Total = 2⁶ × 2¹² = 2¹⁸
- **Physical address = 18 bits**

```
Logical:  [ Page # (8 bits) | Offset (12 bits) ] = 20 bits
Physical: [ Frame # (6 bits) | Offset (12 bits) ] = 18 bits
```

---

### Question 1.3

> Consider a logical address space of **2,048 pages** with a **4-KB page size**, mapped onto a physical memory of **512 frames**.
> a. How many bits in the logical address?
> b. How many bits in the physical address?

**Solution:**

Express as powers of 2:
- 2,048 pages = 2¹¹
- 4 KB = 2¹² bytes
- 512 frames = 2⁹

**(a) Logical address bits:**
- Total = 2¹¹ × 2¹² = 2²³
- **Logical address = 23 bits**

**(b) Physical address bits:**
- Total = 2⁹ × 2¹² = 2²¹
- **Physical address = 21 bits**

```
Logical:  [ Page # (11 bits) | Offset (12 bits) ] = 23 bits
Physical: [ Frame # (9 bits)  | Offset (12 bits) ] = 21 bits
```

---

## Pattern 2: Page Number & Offset from Decimal Address

### How to Solve (Plain English Steps)

Given a **decimal address** and a **page size**, find the page number and offset:

1. **Convert page size to bytes** (if not already). Example: 1 KB = 1,024 bytes.
2. **Page Number** = `floor(Address ÷ Page Size)`
   - This is integer division (drop the remainder)
3. **Page Offset** = `Address mod Page Size`
   - This is the remainder of the division
4. **Verify:** `Page Number × Page Size + Offset = Original Address`

**Relevant formulas:** #6, #7 from the table above.

**Why this works:** Each page covers a contiguous range of `Page Size` bytes. Page 0 covers addresses 0 to (PageSize−1), Page 1 covers PageSize to (2×PageSize−1), etc. Dividing tells you which chunk, and the remainder tells you where within that chunk.

---

### Worked Example (Covers Everything)

> **Q:** Assuming a **2-KB page size**, what are the page numbers and offsets for:
> a. 5,000
> b. 16,384

**Solution:**

Page size = 2 KB = 2,048 bytes

**(a) Address 5,000:**
- Page Number = floor(5,000 ÷ 2,048) = floor(2.441) = **2**
- Offset = 5,000 mod 2,048 = 5,000 − (2 × 2,048) = 5,000 − 4,096 = **904**
- Verify: 2 × 2,048 + 904 = 4,096 + 904 = 5,000 ✓

**(b) Address 16,384:**
- Page Number = floor(16,384 ÷ 2,048) = floor(8.0) = **8**
- Offset = 16,384 mod 2,048 = 16,384 − (8 × 2,048) = 16,384 − 16,384 = **0**
- Verify: 8 × 2,048 + 0 = 16,384 ✓

---

### Question 2.1

> Assuming a **1-KB page size**, what are the page numbers and offsets for:
> a. 3085
> b. 42095
> c. 215201
> d. 650000
> e. 2000001

Page size = 1 KB = 1,024 bytes

**(a) Address = 3,085:**
- Page Number = floor(3,085 ÷ 1,024) = floor(3.013) = **3**
- Offset = 3,085 − (3 × 1,024) = 3,085 − 3,072 = **13**
- Verify: 3 × 1,024 + 13 = 3,072 + 13 = 3,085 ✓

**(b) Address = 42,095:**
- Page Number = floor(42,095 ÷ 1,024) = floor(41.1084) = **41**
- Offset = 42,095 − (41 × 1,024) = 42,095 − 41,984 = **111**
- Verify: 41 × 1,024 + 111 = 41,984 + 111 = 42,095 ✓

**(c) Address = 215,201:**
- Page Number = floor(215,201 ÷ 1,024) = floor(210.157) = **210**
- Offset = 215,201 − (210 × 1,024) = 215,201 − 215,040 = **161**
- Verify: 210 × 1,024 + 161 = 215,040 + 161 = 215,201 ✓

**(d) Address = 650,000:**
- Page Number = floor(650,000 ÷ 1,024) = floor(634.765) = **634**
- Offset = 650,000 − (634 × 1,024) = 650,000 − 649,216 = **784**
- Verify: 634 × 1,024 + 784 = 649,216 + 784 = 650,000 ✓

**(e) Address = 2,000,001:**
- Page Number = floor(2,000,001 ÷ 1,024) = floor(1,953.126) = **1,953**
- Offset = 2,000,001 − (1,953 × 1,024) = 2,000,001 − 1,999,872 = **129**
- Verify: 1,953 × 1,024 + 129 = 1,999,872 + 129 = 2,000,001 ✓

| Address | Page Number | Offset |
|---------|-------------|--------|
| 3,085 | 3 | 13 |
| 42,095 | 41 | 111 |
| 215,201 | 210 | 161 |
| 650,000 | 634 | 784 |
| 2,000,001 | 1,953 | 129 |

---

### Question 2.2

> Assuming a **1-KB page size**, what are the page numbers and offsets for:
> a. 21205
> b. 164250
> c. 121357
> d. 16479315
> e. 27253187

Page size = 1 KB = 1,024 bytes

**(a) Address = 21,205:**
- Page Number = floor(21,205 ÷ 1,024) = floor(20.708) = **20**
- Offset = 21,205 − (20 × 1,024) = 21,205 − 20,480 = **725**
- Verify: 20 × 1,024 + 725 = 20,480 + 725 = 21,205 ✓

**(b) Address = 164,250:**
- Page Number = floor(164,250 ÷ 1,024) = floor(160.400) = **160**
- Offset = 164,250 − (160 × 1,024) = 164,250 − 163,840 = **410**
- Verify: 160 × 1,024 + 410 = 163,840 + 410 = 164,250 ✓

**(c) Address = 121,357:**
- Page Number = floor(121,357 ÷ 1,024) = floor(118.513) = **118**
- Offset = 121,357 − (118 × 1,024) = 121,357 − 120,832 = **525**
- Verify: 118 × 1,024 + 525 = 120,832 + 525 = 121,357 ✓

**(d) Address = 16,479,315:**
- Page Number = floor(16,479,315 ÷ 1,024) = floor(16,093.081) = **16,093**
- Offset = 16,479,315 − (16,093 × 1,024) = 16,479,315 − 16,479,232 = **83**
- Verify: 16,093 × 1,024 + 83 = 16,479,232 + 83 = 16,479,315 ✓

**(e) Address = 27,253,187:**
- Page Number = floor(27,253,187 ÷ 1,024) = floor(26,614.440) = **26,614**
- Offset = 27,253,187 − (26,614 × 1,024) = 27,253,187 − 27,252,736 = **451**
- Verify: 26,614 × 1,024 + 451 = 27,252,736 + 451 = 27,253,187 ✓

| Address | Page Number | Offset |
|---------|-------------|--------|
| 21,205 | 20 | 725 |
| 164,250 | 160 | 410 |
| 121,357 | 118 | 525 |
| 16,479,315 | 16,093 | 83 |
| 27,253,187 | 26,614 | 451 |

---

## Pattern 3: Page Table Entries, Inverted Table & Max Physical Memory

### How to Solve (Plain English Steps)

**For conventional (single-level) page table entries:**
1. Find the number of bits for the virtual address (given or computed)
2. Find the offset bits = `log₂(Page Size)`
3. Page number bits = Virtual address bits − Offset bits
4. **Entries = 2^(Page Number Bits)** — one entry per virtual page

**For inverted page table entries:**
1. Find the number of bits for the physical address (given or computed)
2. Frame number bits = Physical address bits − Offset bits
3. **Entries = 2^(Frame Number Bits)** — one entry per physical frame

**For max physical memory:**
- **Max Physical Memory = Number of Frames × Frame Size = 2^(Physical Address Bits)**

**Relevant formulas:** #3, #4, #5, #8, #9, #10 from the table above.

**Key insight:** Conventional page table is sized by the **virtual** address space (can be huge). Inverted page table is sized by **physical** memory (usually much smaller).

---

### Worked Example (Covers Everything)

> **Q:** A system has a **26-bit virtual address**, an **18-bit physical address**, and a **4-KB page size**.
> a. Entries in a conventional page table?
> b. Entries in an inverted page table?
> c. Maximum physical memory?

**Solution:**

**Step 1 — Offset bits:**
- Page size = 4 KB = 2¹² → **Offset = 12 bits**

**Step 2 — (a) Conventional page table:**
- Page number bits = 26 − 12 = **14 bits**
- Entries = 2¹⁴ = **16,384 entries**

**Step 3 — (b) Inverted page table:**
- Frame number bits = 18 − 12 = **6 bits**
- Entries = 2⁶ = **64 entries**

**Step 4 — (c) Max physical memory:**
- 2¹⁸ bytes = **256 KB**
- Or equivalently: 64 frames × 4 KB = 256 KB ✓

---

### Question 3.1

> The **BTV** OS has a **21-bit virtual address**, a **16-bit physical address**, and a **2-KB page size**.
> a. Entries in a conventional page table?
> b. Entries in an inverted page table?
> c. Maximum physical memory?

**Solution:**

Offset bits = log₂(2 KB) = log₂(2¹¹) = **11 bits**

**(a) Conventional page table:**
- Page number bits = 21 − 11 = **10 bits**
- Entries = 2¹⁰ = **1,024 entries**

**(b) Inverted page table:**
- Frame number bits = 16 − 11 = **5 bits**
- Entries = 2⁵ = **32 entries**

**(c) Max physical memory:**
- 2¹⁶ bytes = **64 KB**
- Or: 32 frames × 2 KB = 64 KB ✓

---

### Question 3.2

> The **MPV** OS has a **24-bit virtual address**, a **20-bit physical address**, and a **4-KB page size**.
> a. Entries in a conventional page table?
> b. Entries in an inverted page table?
> c. Maximum physical memory?

**Solution:**

Offset bits = log₂(4 KB) = log₂(2¹²) = **12 bits**

**(a) Conventional page table:**
- Page number bits = 24 − 12 = **12 bits**
- Entries = 2¹² = **4,096 entries**

**(b) Inverted page table:**
- Frame number bits = 20 − 12 = **8 bits**
- Entries = 2⁸ = **256 entries**

**(c) Max physical memory:**
- 2²⁰ bytes = **1 MB**
- Or: 256 frames × 4 KB = 1 MB ✓

---

### Question 3.3

> A system has a **32-bit logical address**, **4-KB page size**, and supports up to **512 MB** of physical memory.
> a. Entries in a conventional page table?
> b. Entries in an inverted page table?

**Solution:**

Offset bits = log₂(4 KB) = log₂(2¹²) = **12 bits**

Physical address bits: 512 MB = 2²⁹ bytes → **29 bits**

**(a) Conventional page table:**
- Page number bits = 32 − 12 = **20 bits**
- Entries = 2²⁰ = **1,048,576 entries** (about 1 million)

**(b) Inverted page table:**
- Frame number bits = 29 − 12 = **17 bits**
- Entries = 2¹⁷ = **131,072 entries**

---

### Question 3.4

> A system has a **32-bit logical address**, **8-KB page size**, and supports up to **1 GB** of physical memory.
> a. Entries in a conventional page table?
> b. Entries in an inverted page table?

**Solution:**

Offset bits = log₂(8 KB) = log₂(2¹³) = **13 bits**

Physical address bits: 1 GB = 2³⁰ bytes → **30 bits**

**(a) Conventional page table:**
- Page number bits = 32 − 13 = **19 bits**
- Entries = 2¹⁹ = **524,288 entries**

**(b) Inverted page table:**
- Frame number bits = 30 − 13 = **17 bits**
- Entries = 2¹⁷ = **131,072 entries**

---

## Pattern 4: Effective Access Time (EAT) with TLB

### How to Solve (Plain English Steps)

1. **Identify the memory access time** (time for one RAM access)
2. **Without TLB:** Every paged reference needs **2 memory accesses** (one for page table + one for data)
   - Paged memory reference time = `2 × memory access time`
3. **With TLB:** Use the EAT formula:
   - On a **TLB hit**: Time = `TLB lookup time + 1 memory access` (for the data)
   - On a **TLB miss**: Time = `TLB lookup time + 1 memory access (page table) + 1 memory access (data)`
   - **EAT** = `hit_ratio × (TLB_time + mem_time) + (1 − hit_ratio) × (TLB_time + mem_time + mem_time)`

**Relevant formula:** #11 from the table above.

> **Note:** Some problems assume TLB lookup time is negligible (0 ns) or folded into the memory access. Read the problem carefully to see if a TLB lookup time is given separately.

---

### Worked Example

> **Q:** Memory access = 60 ns, TLB lookup = 5 ns, hit ratio = 90%.
> a. Paged memory reference time without TLB?
> b. EAT with TLB?

**Solution:**

**(a) Without TLB:**
- 2 × 60 = **120 ns**

**(b) With TLB:**
- TLB hit path: 5 (TLB) + 60 (data) = 65 ns
- TLB miss path: 5 (TLB) + 60 (page table) + 60 (data) = 125 ns
- EAT = 0.90 × 65 + 0.10 × 125 = 58.5 + 12.5 = **71 ns**

---

### Question 4.1

> Consider a paging system with the page table stored in memory.
> a. If a memory reference takes **50 nanoseconds**, how long does a paged memory reference take?
> b. If we add TLBs, and **75%** of all page-table references are found in the TLBs, what is the effective memory reference time? (TLB lookup = **2 ns** if entry is present.)

**Solution:**

**(a) Paged memory reference without TLB:**
- Need 2 memory accesses: one for page table + one for data
- Time = 50 + 50 = **100 ns**

**(b) EAT with TLB (75% hit ratio, TLB lookup = 2 ns):**

- **TLB hit path** (75% of the time):
  - TLB lookup (2 ns) + memory access for data (50 ns) = **52 ns**
- **TLB miss path** (25% of the time):
  - TLB lookup that fails (2 ns) + memory access for page table (50 ns) + memory access for data (50 ns) = **102 ns**

> Note: We include the TLB lookup time even on a miss because the TLB is always checked first.

- **EAT** = 0.75 × 52 + 0.25 × 102
- EAT = 39 + 25.5
- **EAT = 64.5 ns**

Compared to 100 ns without TLB, this is a **35.5% improvement** even with only a 75% hit ratio.

---

## Quick Reference: Problem-Solving Checklist

```
┌─────────────────────────────────────────────────────────────┐
│  STEP 0: Convert everything to powers of 2                  │
│          (1 KB = 2¹⁰, 4 KB = 2¹², 1 MB = 2²⁰, etc.)       │
├─────────────────────────────────────────────────────────────┤
│  For "how many bits?" problems:                             │
│    → Logical bits  = log₂(pages) + log₂(page_size)         │
│    → Physical bits = log₂(frames) + log₂(frame_size)       │
│    → Offset bits   = log₂(page_size)                       │
├─────────────────────────────────────────────────────────────┤
│  For "page number & offset?" problems:                      │
│    → Page #  = floor(address ÷ page_size)                   │
│    → Offset  = address mod page_size                        │
│    → Verify: page# × page_size + offset = address          │
├─────────────────────────────────────────────────────────────┤
│  For "how many entries?" problems:                          │
│    → Conventional PT = 2^(virtual_bits − offset_bits)       │
│    → Inverted PT     = 2^(physical_bits − offset_bits)      │
├─────────────────────────────────────────────────────────────┤
│  For EAT problems:                                          │
│    → Without TLB: 2 × mem_time                              │
│    → With TLB: weight hit and miss paths by probability     │
└─────────────────────────────────────────────────────────────┘
```

---

*End of Paging Numericals*
