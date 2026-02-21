## 1.1 Defining Operating Systems

There is no universally accepted definition of an operating system (OS), but its fundamental goal is to execute programs, make solving problems easier, and manage hardware resources efficiently.

* **The Core Components:**
* **Kernel:** The central program running at all times on the computer.
* **Middleware:** Software frameworks providing extra services to developers (e.g., databases, multimedia, graphics). This is especially prominent in mobile operating systems like iOS and Android.
* **System Programs:** Software associated with the OS that helps manage the system but isn't necessarily part of the kernel.
* **Application Programs:** All other user-facing programs.


* **Practical Example:** Think about running a demanding, low-spec masterpiece game on an older PC; the OS is the invisible layer efficiently allocating the limited memory and CPU cycles so the game runs smoothly without crashing the system.

## 1.2 Computer-System Organization

A modern general-purpose computer consists of CPUs and device controllers connected via a common bus that provides access to shared memory.

* **Device Controllers:** Hardware components in charge of specific peripheral types (e.g., disk drives, displays). They maintain local buffer storage and special-purpose registers.
* **Device Drivers:** The software interface. Every controller has a driver that understands it and provides the OS with a uniform interface.
* *Example:* When troubleshooting a laptop Wi-Fi card or connecting sensors to a Raspberry Pi, the device driver is what translates the OS's generic network commands into the exact electrical signals the specific hardware requires.



### 1.2.1 Interrupts

Interrupts are hardware signals sent over the system bus that alert the CPU to an event needing immediate attention.

* **The Mechanism:**
1. A device finishes a task (like reading a keystroke) and triggers an interrupt.
2. The CPU stops its current execution and saves its state.
3. The CPU jumps to the **interrupt vector** (a table in low memory) to find the address of the specific **Interrupt Service Routine (ISR)**.
4. The ISR handles the event, restores the CPU state, and resumes the interrupted program.


* **Advanced Features:**
* **Maskable vs. Nonmaskable:** Maskable interrupts can be temporarily ignored by the CPU during critical tasks. Nonmaskable interrupts are reserved for unrecoverable hardware errors.
* **Interrupt Chaining:** Instead of an impossibly large interrupt table, each entry points to a list of handlers that are checked sequentially.
* **Priority Levels:** Allows high-priority interrupts to preempt low-priority ones.



### 1.2.2 Storage Structure

Programs must be in main memory to be executed by the CPU. Because storage varies in cost and performance, it is organized in a hierarchy.

* **Main Memory (RAM/DRAM):** Fast, volatile (loses data when power is cut). The CPU directly loads instructions from here via load/store instructions.
* **Secondary Storage (NVS - Nonvolatile Storage):** Slower, permanent, and large capacity. Acts as an extension of main memory.
* **Electrical (NVM):** Solid State Drives (SSDs), Flash memory. Faster, smaller, more expensive.
* **Mechanical:** Hard Disk Drives (HDDs), Magnetic Tape. Slower, larger capacity, cheaper per byte.


* **Bootstrap Program:** The initial startup code. Because RAM is volatile, this is stored in **EEPROM** or firmware (slow, nonvolatile, rarely written to).

### 1.2.3 I/O Structure

For low-speed devices, the CPU handles interrupts byte-by-byte. For high-speed bulk data movement (like transferring large files to an HDD), this causes massive overhead.

* **Direct Memory Access (DMA):** Solves bulk transfer overhead. The device controller transfers an entire block of data directly to/from main memory without CPU intervention, generating only *one* interrupt per block once finished.

## 1.4 Operating System Operations

### 1.4.1 Multiprogramming and Multitasking

A single program cannot keep the CPU and I/O devices busy simultaneously.

* **Multiprogramming:** Keeps multiple processes in memory. If Process A pauses to wait for I/O, the OS immediately switches the CPU to Process B. The CPU is never idle.
* **Multitasking:** A logical extension where the CPU switches between processes so rapidly that it provides a fast, interactive response time for the user.
* *Example:* This rapid switching is what allows you to concurrently execute complex database development tasks in Oracle SQL Developer while simultaneously listening to music and keeping a web browser open, without noticeable lag.


* **Virtual Memory:** Allows execution of processes that are larger than actual physical memory by abstracting main memory into a large, uniform array.

### 1.4.2 Dual-Mode and Multimode Operation

To protect the system from malicious or buggy programs, hardware supports different execution modes, tracked by a **mode bit**.

* **User Mode (Bit = 1):** Restricted execution on behalf of the user application.
* **Kernel Mode (Bit = 0):** Full privileged execution on behalf of the OS. The system always boots in kernel mode.
* **Privileged Instructions:** Harmful/critical machine instructions (like managing interrupts or I/O control) that can *only* be executed in Kernel Mode.
* *Note:* If you are writing a low-level project in assembly language and attempt to execute a privileged instruction directly in User Mode, the hardware will block it and trap it to the OS as an error.


* **System Calls:** The secure bridge. When a user program needs an OS service, it executes a system call (usually a software interrupt or trap). The hardware switches to Kernel Mode, the OS fulfills the request, and then switches back to User Mode.

### 1.4.3 Timer

Prevents user programs from permanently hogging the CPU (e.g., getting stuck in an infinite loop).

* **Mechanism:** A clock decrements a counter. When the counter hits 0, an interrupt is triggered, automatically transferring control back to the OS.

---
