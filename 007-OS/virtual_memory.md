# Virtual Memory — Detailed Notes

---

## 1. What is Virtual Memory?

### Formal Definition
Virtual memory is a **memory management technique** that allows a computer to use more memory than is physically available by temporarily transferring data from **RAM** (main memory) to **secondary storage** (hard disk / SSD / NVM device). It separates **logical memory** (what the process sees) from **physical memory** (actual RAM), enabling programs to execute even when they are only **partially loaded** into main memory.

### Intuitive Explanation & Mental Model
> Think of your desk (RAM) as small, and your filing cabinet (disk) as huge. You can't fit all your papers on the desk at once, but you don't need them all at the same time either. Virtual memory lets you keep only the papers you're *currently working on* on the desk, and swap others in from the cabinet when you need them. The process *thinks* it has a massive desk — that's the **virtual address space** — but behind the scenes, the OS is shuffling pages between the desk and the cabinet.

### ⚠️ Interchangeable / Related Terms
| Term | Also Known As / Related To |
|---|---|
| **Virtual memory** | The overall technique (broader concept) |
| **Paging** | One specific mechanism *within* virtual memory |
| **Swap space / Swap device** | The area on secondary storage used to hold pages not in RAM |
| **Secondary memory / Secondary storage / Backing store** | The disk (HDD, SSD, NVM) that holds pages not in main memory |
| **Logical memory / Virtual address space** | What the process sees (may be larger than physical RAM) |
| **Physical memory / Main memory / RAM** | The actual hardware memory |

---

## 2. Advantages of Virtual Memory

### 2.1 Program Size Freedom
- A program is **no longer constrained** by the amount of physical memory available.
- Users can write programs for an **extremely large virtual address space**, simplifying the programming task.
- **Key takeaway:** Programs can be **larger than physical memory**.

### 2.2 Increased Multiprogramming
- Each program takes **less physical memory** (since only needed parts are loaded).
- Therefore, **more programs can run simultaneously**.
- This leads to increased **CPU utilization** and **throughput**, with **no increase** in response time or turnaround time.

### 2.3 Faster Program Execution (Less I/O)
- **Less I/O** is needed to load or swap portions of programs into memory.
- Each program **runs faster** because only the needed pages are loaded, not the entire executable.

### 2.4 Efficient Process Creation
- Virtual memory provides an **efficient mechanism for process creation** (e.g., through copy-on-write with `fork()`).

### ⚠️ Warning: Performance Risk
> Virtual memory may **substantially decrease performance** if it is used carelessly — for example, if the working set of a process does not fit in available RAM, the system will constantly swap pages in and out (**thrashing**).

---

## 3. Shared Memory Advantages (via Page Sharing)

Virtual memory allows **files and memory to be shared** by two or more processes through **page sharing**.

### 3.1 Shared System Libraries
- System libraries (e.g., the **standard C library**) can be shared by several processes by **mapping the shared object** into each process's virtual address space.
- Each process *considers* the library part of its own virtual address space, but the **actual physical pages are shared** across all processes.
- Typically, a shared library is mapped **read-only** into the space of each process linked with it.

### 3.2 Inter-Process Shared Memory
- Two or more processes can **communicate through shared memory**.
- Virtual memory allows one process to create a **region of memory** it can share with another process.
- Both processes see the region as part of **their own** virtual address space, yet the **physical pages are shared**.

### 3.3 Faster Process Creation (fork)
- Pages can be **shared during process creation** with the `fork()` system call, **speeding up** process creation (see Copy-on-Write below).

### Mental Model
> Imagine two students (processes) sitting at separate desks (virtual address spaces) but both looking at the **same shared textbook** (physical pages) on a stand between them. Each student *thinks* the book is on their desk, but there's really only **one physical copy**.

---

## 4. Virtual Memory vs. Paging

| Aspect | Virtual Memory | Paging |
|---|---|---|
| **Scope** | **Broader concept** — covers the entire technique of using disk to simulate additional RAM | **Narrower concept** — one specific mechanism within virtual memory |
| **What it does** | Uses disk storage to **simulate additional RAM** | Specifically refers to **transferring memory pages** between RAM and disk |
| **Relationship** | The umbrella system | **One aspect** of virtual memory |

### Virtual Memory encompasses:
1. **Demand Paging** — loading pages only when needed
2. **Page Fault Handling** — what happens when a needed page isn't in RAM
3. **Memory Allocation Strategies** — deciding how to distribute frames among processes
4. **Page Replacement Algorithms** — deciding which page to evict when memory is full

> **Key distinction:** Paging is a *mechanism*. Virtual memory is a *strategy* that *uses* paging (among other techniques).

---

## 5. Demand Paging

### Formal Definition
Demand paging is a technique where **pages are loaded into memory only when they are demanded** (referenced/needed) during program execution. Pages that are **never accessed** are **never loaded** into physical memory.

### ⚠️ Interchangeable Terms
- **Demand-paged virtual memory** = virtual memory that uses demand paging
- A **demand-paging system** is similar to a **paging system with swapping**, where processes reside in secondary memory

### Intuitive Explanation
> Imagine you're reading a 500-page book, but you only bring pages to your desk **when you actually need to read them**. If Chapter 12 is never needed, its pages never leave the shelf. That's demand paging — load on demand, not in advance.

### Motivating Scenario (from the textbook)
A program starts with a **list of available options** for the user to select. Loading the **entire program** into memory loads executable code for **all options**, even ones the user never selects. With demand paging, only the code for the **selected option** is loaded. This saves memory and speeds up startup.

---

### 5.1 The Valid–Invalid Bit Scheme

The page table uses a **valid–invalid bit** for each entry:

| Bit Value | Meaning |
|---|---|
| **Valid (1)** | The page is **both legal AND in memory** (RAM) |
| **Invalid (0)** | The page is either: **(a)** not in the logical address space of the process (truly invalid / bogus address), OR **(b)** valid but **currently on secondary storage** (not in RAM) |

- A page-table entry for a page **brought into memory** → set as usual (valid).
- A page-table entry for a page **not in memory** → marked **invalid**.
- Marking a page invalid has **no effect** if the process **never attempts to access** that page.

### Mental Model
> The valid-invalid bit is like a label on a filing cabinet drawer: "HERE" (valid = in RAM) or "NOT HERE" (invalid = on disk or doesn't exist). If you never open that drawer, the label doesn't matter.

---

### 5.2 Page Fault Handling — Step by Step

**A page fault is generated** when a program tries to access a page that is **not in memory** (i.e., marked **invalid** in the page table). The paging hardware notices the invalid bit is set and causes a **trap to the operating system**.

#### The Procedure:

```
1. Check the internal table (kept with the PCB) to determine:
   → Was this a VALID or INVALID memory access?

2. If INVALID reference → terminate the process.
   If VALID but page not yet loaded → proceed to page it in.

3. Find a free frame (from the free-frame list).

4. Schedule a secondary storage (disk) operation to READ the
   desired page into the newly allocated frame.

5. [While I/O is happening, the OS can context switch to
    another process to keep the CPU busy.]

6. When the storage read completes → update:
   - The internal table (PCB)
   - The page table (mark the page as valid / in memory)

7. RESTART the instruction that was interrupted by the trap.
   The process can now access the page as though it had
   always been in memory.
```

### Mental Model — Page Fault as a "Delivery Request"
> You're writing an exam and need a reference sheet that's in your locker (disk). You raise your hand (trap). The teacher (OS) checks if your request is legitimate. If yes, a helper fetches the sheet (disk I/O). While waiting, the teacher helps another student (context switch). When the sheet arrives, it's placed on your desk (RAM), the seating chart is updated (page table), and you resume your exam from where you left off (restart instruction).

---

### 5.3 Pure Demand Paging

### Formal Definition
Pure demand paging means **never bringing a page into memory until it is required**. A process starts executing with **no pages in memory at all**.

### How it works:
1. The OS sets the instruction pointer to the **first instruction** of the process.
2. That instruction is on a **non-memory-resident page** → **immediate page fault**.
3. The page is brought in; the process continues executing.
4. It **faults as necessary** until every page it needs is in memory.
5. At that point, it executes with **no more faults**.

### Why it works reasonably well:
> Programs tend to have **locality of reference** — they access a relatively small set of pages during any given period. This means after the initial burst of page faults, the process settles down.

---

### 5.4 Implementation Requirements for Demand Paging

#### Software Requirements:
- A **copy of the entire program must be stored on disk** (so any page can be fetched on demand).
- The OS must maintain **internal tables** (via the PCB) to track which pages are where.

#### Hardware Requirements:
1. **Page Table** — with the ability to mark entries as invalid (via the valid–invalid bit or special protection bits).
2. **Secondary Memory (Swap Device)** — holds pages not present in main memory. Usually a high-speed disk or NVM device. The section of storage used is called **swap space**.

#### Implementation Steps (Detailed):
```
1. Valid bit in page table → 1: in memory, 0: not in memory
2. When a page is referenced but not in memory → trap to OS
3. OS checks the address is valid. If so:
   a. Selects a page to replace (page replacement algorithm)
   b. Invalidates the old page in the page table
   c. Starts loading new page into memory from disk
   d. Context switches to another process while I/O is done
   e. Gets interrupt that page is loaded in memory
   f. Updates the page table entry
   g. Continues the faulting process
```

> **Why continue the faulting process and not the current process?** Because the faulting process was the one that *needed* the page. It was blocked waiting for the I/O, and now that the page is available, it's ready to run. The scheduler may also factor in priority and fairness.

---

### 5.5 Demand Paging: Memory as a Cache for Disk

### Key Insight
> **Demand paging uses memory (RAM) as a cache for the disk.** The page table (memory map) indicates whether each page is on disk or in memory using the **valid bit**. Once a page is brought from disk into memory, the OS updates the page table and the valid bit.

### Performance Requirement
- For efficiency, memory accesses must reference pages that are **in memory the vast majority of the time**.
- Otherwise, the **effective memory access time** will approach that of the disk (which is catastrophically slow compared to RAM).

### The Locality Principle & the 90/10 Rule
- **Locality of reference:** Programs tend to access a small, concentrated set of pages over any short period.
- **Working set:** The set of pages a process is actively using.
- The **working set size** of a process must **fit in memory** and must **stay there**.
- **90/10 Rule:** A program spends **90% of its time** accessing only **10% of its pages**. This is why demand paging works — most of the time, the needed pages are already in RAM.

---

### 5.6 Restarting Instructions After a Page Fault

### Formal Definition
A **crucial requirement** for demand paging is the ability to **restart any instruction** after a page fault. Since the state of the interrupted process (registers, condition code, instruction counter) is saved when the page fault occurs, the process must be restarted in **exactly the same place and state**, except that the desired page is now in memory.

### Where can a page fault occur?
- On the **instruction fetch** → restart by fetching the instruction again.
- While **fetching an operand** → must fetch and decode the instruction again, then fetch the operand.

### Worst-Case Example (Three-Address Instruction)
Consider: **ADD the content of A to B, placing the result in C**

Steps to execute:
```
1. Fetch and decode the instruction (ADD)
2. Fetch A
3. Fetch B
4. Add A and B
5. Store the sum in C
```

If we **fault when storing in C** (because C's page is not in memory):
- Get the desired page, bring it in, correct the page table.
- **Restart the entire instruction:** fetch it again, decode again, fetch both operands again, add again, then store.
- The repeated work is **less than one complete instruction**, and this repetition is only necessary when a page fault occurs.

### Mental Model
> It's like writing an answer on an exam, but your answer sheet's next page is missing. The invigilator brings you a new page. You have to re-read the question and re-calculate, but it's only a small amount of repeated work.

---

## 6. Copy-on-Write (COW)

### Formal Definition
Copy-on-write (COW) is a technique where, after a `fork()` system call, the **parent and child processes initially share the same physical pages**. These shared pages are marked as **copy-on-write pages**. If **either process writes to a shared page**, only then is a **copy of that page created** (the page is duplicated). Pages that **cannot be modified** (e.g., executable code) are simply **shared** without any copy-on-write marking.

### Why COW Exists
- `fork()` creates a child process that is a **duplicate of its parent**.
- Traditionally, `fork()` **copied the parent's entire address space** for the child.
- But many child processes call `exec()` **immediately after creation**, which replaces the address space entirely — making the copy **unnecessary and wasteful**.
- COW **defers** the copy until a write actually happens, saving memory and time.

### How it Works — Step by Step:
```
1. fork() is called.
2. Child gets the SAME physical pages as parent (no copying).
3. All modifiable shared pages are marked "copy-on-write."
4. Read-only pages (e.g., code) are simply shared — no COW needed.
5. If either process WRITES to a COW page:
   → A NEW copy of that page is created for the writing process.
   → The other process retains the original page.
6. Until a write happens, both processes share the same physical memory.
```

### Mental Model
> Two roommates (parent & child) share a single notebook (physical pages). As long as both are only *reading*, one notebook is fine. The moment one roommate wants to *write* something different, they photocopy the page first and write on their own copy. This is copy-on-write.

### Used By
- **Windows**, **Linux**, and **macOS** all use copy-on-write.

---

## 7. Frame Allocation Problem vs. Page Replacement Problem

These are **two distinct but related problems** in virtual memory management:

| Aspect | Frame Allocation Problem | Page Replacement Problem |
|---|---|---|
| **What it asks** | **How many** physical frames should each process get? | **Which page** should be evicted when a new page needs to come in? |
| **When it occurs** | During **initial allocation** of memory to processes | When a **page fault occurs** and there are **no free frames** |
| **Focus** | Distribution of limited frames **among multiple processes** | Selection of a **victim page** to remove from memory |
| **Goal** | Ensure efficient memory usage and minimize page faults | Choose the page whose removal will cause the **least future disruption** |

### Mental Model
> **Frame allocation** is like deciding how many chairs each team gets in a room. **Page replacement** is like deciding which person must stand up to give their chair to a newcomer when all chairs are occupied.

---

## 8. Summary — Quick Reference Card

```
┌──────────────────────────────────────────────────────────┐
│                   VIRTUAL MEMORY                         │
│                                                          │
│  Logical Memory ←──separated from──→ Physical Memory     │
│                                                          │
│  Key Mechanism: DEMAND PAGING                            │
│  → Load pages ONLY when needed                           │
│  → Uses valid-invalid bit in page table                  │
│  → Page fault triggers trap → OS loads page from disk    │
│                                                          │
│  Pure Demand Paging: Start with ZERO pages in memory     │
│                                                          │
│  Memory = Cache for Disk                                 │
│  Working Set must fit in RAM (90/10 Rule)                │
│                                                          │
│  Copy-on-Write: Share pages after fork(),                │
│    copy only on modification                             │
│                                                          │
│  Two Key Problems:                                       │
│    1. Frame Allocation → how many frames per process?    │
│    2. Page Replacement → which page to evict?            │
└──────────────────────────────────────────────────────────┘
```

---

## 9. Glossary of Interchangeable / Synonym Terms

| Term A | Term B (Interchangeable / Synonym) |
|---|---|
| Virtual address space | Logical memory / Logical address space |
| Physical memory | Main memory / RAM / Real memory |
| Secondary memory | Secondary storage / Disk / Backing store |
| Swap space | Swap device / Swap area (the disk section used for paging) |
| Page fault | Page miss (the event when a needed page is not in RAM) |
| Valid bit = 1 | Page is in memory |
| Valid bit = 0 | Page is NOT in memory (on disk or invalid address) |
| Frame | Physical frame / Page frame (a fixed-size block in physical memory) |
| Page | A fixed-size block in logical/virtual memory |
| Demand paging system | Paging system with swapping (similar concept) |
| Working set | The set of pages a process is actively using |
| Copy-on-write (COW) | Lazy copying / Deferred copying (informal names for same idea) |
| Modify bit | Dirty bit (set when a page is written to) |
| Victim page / Victim frame | The page/frame selected for replacement |
| Reference string | The sequence of page numbers accessed by a process |
| Second-chance algorithm | Clock algorithm (circular queue implementation) |
| OPT | MIN (optimal page replacement) |

---

## 10. Page Replacement — Overview

### Formal Definition
Page replacement is a mechanism used in demand paging where the operating system **selects a page currently in physical memory to be evicted (replaced)** with a new page from disk, when there are **no free frames available** to satisfy a page fault.

### Intuitive Explanation & Mental Model
> Imagine a parking lot (RAM) that is completely full. A new car (page) arrives and needs a spot. The parking attendant (OS) must decide which existing car to tow away (evict) to make room. The **page-replacement algorithm** is the rule the attendant uses to pick which car to tow. A good rule tows cars whose owners won't return for a long time; a bad rule tows cars whose owners are coming right back.

### Why Page Replacement is Needed — Over-Allocation

When we increase the **degree of multiprogramming**, we risk **over-allocating memory**:

**Example from the textbook:**
- We run **6 processes**, each **10 pages** in size, but each actually uses only **5 pages**.
- This gives us higher CPU utilization and throughput, with **10 spare frames**.
- **Problem:** Each process may suddenly try to use **all 10** of its pages → needs **60 frames**, but only **40 are available**.
- The same scenario can happen if **I/O buffers** suddenly increase in size.

**Result:** There are **no free frames** on the free-frame list — all memory is in use.

> **Key insight:** If no frame is free, we must **find one that is not currently being used and free it** — this is page replacement.

### How a Frame is Freed
1. Write the frame's contents to **swap space** (secondary storage).
2. Update the **page table** (and all other tables) to indicate that page is **no longer in memory**.
3. The freed frame can now hold the page that caused the fault.

> **Note:** Standard swapping (copying entire processes between memory and swap space) is **no longer used** by most modern operating systems due to the overhead. Instead, most OSes **combine swapping pages with page replacement**.

---

## 11. Page Replacement — Step-by-Step Procedure

```
1. Find the location of the desired page on secondary storage.

2. Find a free frame:
   a. If there IS a free frame → use it.
   b. If there is NO free frame → use a page-replacement algorithm
      to select a VICTIM frame.
   c. Write the victim frame to secondary storage (if necessary);
      update the page and frame tables accordingly.

3. Read the desired page into the newly freed frame;
   update the page and frame tables.

4. Continue the process from where the page fault occurred.
```

### Mental Model — The Complete Flow
> You're at a full library desk. You need a new book (page fault). No empty spot exists. You pick the book you're least likely to need (victim), return it to the shelf (write to disk), grab your new book (read from disk), put it on the desk (load into frame), update the checkout list (page table), and resume reading.

### Performance Cost: Double Page Transfer
- If **no frames are free**, **two page transfers** are required:
  1. **Page-out:** Write the victim page to disk.
  2. **Page-in:** Read the desired page from disk.
- This **doubles the page-fault service time** and increases effective access time accordingly.

---

## 12. The Modify Bit (Dirty Bit) — Optimization

### Formal Definition
The **modify bit** (also called the **dirty bit**) is a bit attached to each page/frame in the hardware. It is **set by the hardware whenever any byte in the page is written to**, indicating the page has been **modified** since it was loaded into memory.

### How It Reduces Overhead

| Modify Bit State | Meaning | Action on Replacement |
|---|---|---|
| **Set (1)** | Page has been **modified** in memory | **Must write** to disk before eviction (page-out needed) |
| **Not set (0)** | Page has **NOT been modified** (or is read-only) | Can be **discarded safely** — no page-out needed |

### Why This Matters
- Without the dirty bit: every replacement needs **2 disk I/Os** (page-out + page-in).
- With the dirty bit: if the page is **clean** (not modified), only **1 disk I/O** is needed (page-in only).
- This **reduces I/O time by one-half** for unmodified pages.

### Special Case: Read-Only Pages
- Pages of **binary code** (executables) are **read-only** → they can **never** be modified.
- These pages can always be **discarded without writing** to disk, since the original copy on disk is still valid.

### Mental Model
> The dirty bit is like a "CHANGED" sticky note on a document. If the note is there, you must save the document before closing. If not, you can just close it — the saved version on disk is already up to date.

---

## 13. Reference Strings & Evaluating Algorithms

### Formal Definition
A **reference string** is the sequence of **page numbers** referenced by a process during execution. It is used to **evaluate page-replacement algorithms** by running them on the string and **counting the number of page faults**.

### How to Generate a Reference String
We apply **two simplifications** to reduce the raw address trace:

1. **Use only page numbers**, not full addresses (page size is fixed by hardware).
2. **Remove consecutive duplicate references** to the same page — once page *p* is in memory, immediate re-references to *p* will not fault.

### Textbook Example
**Raw address sequence** (100 bytes per page):
```
0100, 0432, 0101, 0612, 0102, 0103, 0104, 0101, 0611, 0102,
0103, 0104, 0101, 0610, 0102, 0103, 0104, 0101, 0609, 0102, 0105
```

**Divide each by 100 (page size) → page numbers:**
```
1, 4, 1, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 6, 1, 1
```

**Remove consecutive duplicates → Reference string:**
```
1, 4, 1, 6, 1, 6, 1, 6, 1, 6, 1
```

### Frames vs. Faults Relationship
- As the **number of frames increases**, the **number of page faults decreases** (general trend).
- With **enough frames** to hold all distinct pages, faults drop to a **minimum** (one per unique page).
- With **only 1 frame**, every reference is a replacement → **maximum faults**.

### Standard Reference String for Algorithm Examples
The textbook uses this reference string with **3 frames** for all algorithm comparisons:
```
7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1
```

---

## 14. Page Replacement Algorithms

### 14.1 FIFO (First-In, First-Out) Page Replacement

#### Formal Definition
FIFO associates with each page **the time it was brought into memory**. When a page must be replaced, the **oldest page** (the one that has been in memory the longest) is chosen as the victim.

#### Implementation
- Maintain a **FIFO queue** of all pages currently in memory.
- **Replace** the page at the **head** of the queue (oldest).
- **Insert** newly loaded pages at the **tail** of the queue.
- No need to record exact timestamps — the queue order captures arrival order.

#### Mental Model
> Like a queue at a deli counter — the person who arrived first is served (evicted) first. The newest arrival goes to the back of the line.

#### ⚠️ Belady's Anomaly

**Definition:** For some page-replacement algorithms (including FIFO), the **page-fault rate may INCREASE as the number of allocated frames increases**.

- We would expect more memory → fewer faults, but this is **not always true** with FIFO.
- This counterintuitive phenomenon is called **Belady's anomaly**.

> **Mental Model:** It's like adding more lanes to a highway and getting *worse* traffic — counterintuitive, but it happens with FIFO.

---

### 14.2 OPT / MIN (Optimal Page Replacement)

#### Formal Definition
The optimal algorithm replaces **the page that will not be used for the longest period of time** in the future. It guarantees the **lowest possible page-fault rate** for a fixed number of frames and **never suffers from Belady's anomaly**.

#### Why It's Impractical
- It requires **future knowledge** of the reference string — you need to know which pages will be accessed next.
- Similar to the **SJF CPU-scheduling problem** (needs knowledge of future burst lengths).

#### Why It Matters
- Used as a **benchmark** for comparison studies.
- E.g., "Our new algorithm is within **12.3% of optimal** at worst and within **4.7% on average**."

#### Mental Model
> Like having a crystal ball that shows you which book on your desk you won't need for the longest time — you'd always make the perfect eviction choice. Real life doesn't give you a crystal ball, so OPT is theoretical.

---

### 14.3 LRU (Least Recently Used) Page Replacement

#### Formal Definition
LRU replaces the page that has **not been used for the longest period of time** — i.e., the page whose most recent access is **furthest in the past**. It uses the **recent past as an approximation of the near future**.

#### Key Insight — Relationship to FIFO and OPT

| Algorithm | Looks at... | Uses... |
|---|---|---|
| **FIFO** | Backward in time | When the page was **brought into** memory |
| **OPT** | Forward in time | When the page **will be used** next |
| **LRU** | Backward in time | When the page was **last used** (most recent access) |

> LRU is the "practical middle ground" — it can't see the future like OPT, but it makes a smarter backward-looking decision than FIFO.

#### LRU Does NOT Suffer from Belady's Anomaly
- LRU belongs to the class of **stack algorithms**.
- **Stack algorithm property:** The set of pages in memory for *n* frames is always a **subset** of the set of pages in memory with *n + 1* frames.
- For LRU: the pages in memory are always the *n* most recently referenced pages. Adding a frame means those *n* pages are still included, plus one more.

---

### 14.4 LRU Implementation Methods

LRU requires **hardware assistance** beyond standard TLB registers — the ordering must be updated on **every memory reference**.

#### Method 1: Counters (Time-of-Use Field)

```
- Each page-table entry gets a TIME-OF-USE field.
- The CPU has a logical CLOCK (counter), incremented on every memory reference.
- When a page is referenced → copy the clock value into that page's
  time-of-use field.
- To replace: search the page table for the page with the
  SMALLEST time value (least recently used).
```

**Drawbacks:**
- Requires a **search of the entire page table** on every replacement.
- Requires a **write to memory** (time-of-use field) on every memory access.
- Must maintain times when page tables change (CPU scheduling).
- Must handle **clock overflow**.

#### Method 2: Stack (Doubly Linked List)

```
- Maintain a STACK of page numbers (doubly linked list).
- When a page is referenced:
  → Remove it from its current position in the stack.
  → Place it on TOP of the stack.
- Most recently used page = always at the TOP.
- Least recently used page = always at the BOTTOM.
- To replace: the TAIL pointer already points to the LRU page
  → NO SEARCH needed.
```

**Details:**
- Implemented as a **doubly linked list** with head and tail pointers.
- Removing a page and putting it on top requires changing **at most 6 pointers**.
- Each update is slightly more expensive, but **no search** is needed for replacement.
- Particularly appropriate for **software or microcode** implementations.

#### Why Hardware Assistance is Essential
- Updating the clock/stack must happen on **every single memory reference**.
- If done via software interrupts, it would **slow every memory reference by a factor of at least 10**.
- Few systems can tolerate that level of overhead.

---

## 15. LRU Approximation Algorithms

Since true LRU requires expensive hardware, most systems use **approximations** based on a **reference bit** provided by the hardware.

### How the Reference Bit Works
- Each page has a **reference bit**, initially cleared to **0** by the OS.
- As a process executes, the hardware **sets the bit to 1** whenever the page is referenced.
- After some time, we can determine **which pages have been used** (bit = 1) and **which have not** (bit = 0).
- **Limitation:** We know *whether* a page was used, but **not the order** of use.

---

### 15.1 Additional-Reference-Bits Algorithm

#### Formal Definition
We gain **additional ordering information** by recording reference bits at **regular intervals** (e.g., every 100 ms). Each page has an **8-bit history byte** maintained in a table in memory.

#### How It Works
```
At each timer interrupt (e.g., every 100ms):
1. The OS shifts the reference bit for each page INTO the
   HIGH-ORDER bit of its 8-bit history byte.
2. All other bits shift RIGHT by 1 bit.
3. The LOW-ORDER bit is discarded.
4. The reference bit is then cleared.
```

#### Interpreting the History Byte

| History Byte | Meaning |
|---|---|
| `00000000` | Page has **not been used** for the last 8 time periods |
| `11111111` | Page was used **at least once in every** time period |
| `11000100` | Used **more recently** than a page with `01110111` |

- Interpret the 8-bit bytes as **unsigned integers**.
- The page with the **lowest number** is the LRU page → replace it.
- If values are tied, use **FIFO** to break the tie (or replace all with the smallest value).

#### Mental Model
> Each page keeps a "diary" of its last 8 check-ins. At each check-in, if the page was used, a `1` is stamped at the front, and old entries shift down. Pages with the smallest diary number have been used least recently.

---

### 15.2 Second-Chance Algorithm (Clock Algorithm)

#### Formal Definition
The second-chance algorithm is a **modified FIFO** algorithm. When a page is selected for replacement (by FIFO order), its **reference bit is inspected**:
- If reference bit = **0** → **replace** this page.
- If reference bit = **1** → give the page a **second chance**: clear the bit, reset arrival time to current time, and move to the **next FIFO page**.

#### Key Properties
- A page given a second chance **will not be replaced** until all other pages have been replaced (or given second chances).
- A page **used often enough** to keep its reference bit set will **never be replaced**.
- **Worst case:** If all bits are set, the pointer cycles through the entire queue clearing all bits → **degenerates to FIFO**.

#### Implementation: Circular Queue (Clock)
```
- Pages are arranged in a CIRCULAR QUEUE.
- A POINTER (the "clock hand") indicates which page to examine next.
- When a frame is needed:
  1. Check the page at the pointer.
  2. If reference bit = 0 → REPLACE this page, insert new page here.
  3. If reference bit = 1 → CLEAR the bit, advance the pointer.
  4. Repeat until a victim is found.
```

#### Mental Model
> Imagine a circular conveyor belt of plates (pages). A chef (OS) walks around the belt looking for a cold plate (reference bit = 0). If a plate is hot (bit = 1), the chef lets it cool down (clears the bit) and moves on. Once a cold plate is found, it's replaced with a new dish.

---

### 15.3 Enhanced Second-Chance Algorithm

#### Formal Definition
The enhanced second-chance algorithm considers **both the reference bit (r) and the modify bit (m)** as an **ordered pair (r, m)**, creating **four classes** of pages:

| Class | (r, m) | Meaning | Replacement Priority |
|---|---|---|---|
| **1** | (0, 0) | Neither recently used nor modified | **Best** page to replace |
| **2** | (0, 1) | Not recently used but modified | Not as good — must write out before replacing |
| **3** | (1, 0) | Recently used but clean | Probably will be used again soon |
| **4** | (1, 1) | Recently used and modified | Worst to replace — used soon AND needs write-out |

#### Algorithm — Clock with Classes
- Uses the **same circular queue** as the clock algorithm.
- Instead of just checking the reference bit, examine the **(r, m) class**.
- Replace the **first page encountered in the lowest nonempty class**.
- May need to **scan the circular queue several times**.

#### The Three-Pass Search Process

```
PASS 1 — Looking for (0,0):
  • (0,0) → REPLACE this page immediately.
  • (0,1) → Initiate I/O to WRITE OUT the page, LOCK it in memory
            until I/O completes, CLEAR the modify bit, continue searching.
  • (1,0) or (1,1) → CLEAR the reference bit (page becomes (0,0) or (0,1)).

PASS 2 — If no (0,0) found on first pass:
  • Pages that were (0,1) or (1,0) may now be (0,0) → REPLACE.
  • If a page is still being written out → WAIT for I/O to complete,
    then remove the page.
  • (0,1) pages → treated same as Pass 1 (write out, clear, continue).

PASS 3 — By now, ALL pages will be (0,0):
  • Replace the first page encountered.
```

> The OS goes around **at most three times** searching for a (0,0) page.

#### Key Advantage Over Simple Clock
- Gives **preference to unmodified pages** to reduce the number of I/Os required.
- A modified page requires an extra disk write before it can be evicted, so avoiding those saves time.

#### Mental Model
> Think of the enhanced clock as a hotel housekeeper checking rooms (pages). First priority: empty, clean rooms (0,0) — ready immediately. Second: empty but messy rooms (0,1) — need cleaning first. Third: occupied clean rooms (1,0) — guest might return. Last resort: occupied messy rooms (1,1) — guest might return AND room needs cleaning. The housekeeper does up to three laps, tidying as she goes, until a room is available.

---

## 16. Algorithm Comparison — Summary Table

| Algorithm | Victim Selection Rule | Belady's Anomaly? | Hardware Needed | Practical? |
|---|---|---|---|---|
| **FIFO** | Oldest page (first brought in) | **YES** ⚠️ | Minimal (queue) | Yes, but poor performance |
| **OPT / MIN** | Page not used for longest future time | **No** | Requires future knowledge | **No** (theoretical benchmark) |
| **LRU** | Page not used for longest past time | **No** | Counters or stack + HW assist | Expensive but good |
| **Additional-Reference-Bits** | Page with lowest history byte | Approximation of LRU | Timer interrupt + shift register | Yes |
| **Second-Chance (Clock)** | FIFO + skip if reference bit = 1 | Approximation of LRU | Reference bit + circular queue | Yes, widely used |
| **Enhanced Second-Chance** | Lowest (r, m) class in circular scan | Approximation of LRU | Reference bit + modify bit | Yes, reduces I/O |

### Stack Algorithms — No Belady's Anomaly
> A **stack algorithm** guarantees that the set of pages in memory for *n* frames is always a **subset** of the set for *n + 1* frames. Both **OPT** and **LRU** are stack algorithms. FIFO is **not**.

---

## 17. Key Takeaways — Page Replacement

```
┌──────────────────────────────────────────────────────────────┐
│              PAGE REPLACEMENT — KEY POINTS                    │
│                                                              │
│  WHEN: Page fault + no free frames available                 │
│  WHAT: Select a victim page, evict it, load the new page     │
│                                                              │
│  DIRTY BIT OPTIMIZATION:                                     │
│    • Clean page → discard (1 I/O: page-in only)              │
│    • Dirty page → write out + load (2 I/Os)                  │
│                                                              │
│  ALGORITHM HIERARCHY (best → worst):                         │
│    OPT > LRU > Enhanced Clock > Clock > FIFO                 │
│    (OPT is theoretical; LRU is expensive; Clock is practical)│
│                                                              │
│  BELADY'S ANOMALY: More frames can mean MORE faults (FIFO)   │
│  STACK ALGORITHMS: OPT & LRU are immune to this anomaly      │
│                                                              │
│  REFERENCE STRING: Simplify address trace → page numbers     │
│    → remove consecutive duplicates → evaluate algorithms     │
└──────────────────────────────────────────────────────────────┘
```
