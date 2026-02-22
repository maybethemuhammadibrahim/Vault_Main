## 3.1.3 Process Control Block (PCB)

Also known as a **task control block**, a PCB is the data structure the operating system uses to represent and track a specific process.

A PCB acts as a central repository for any information that may vary from process to process. It contains several critical pieces of information, including:

* **Process State:** The current status of the process (e.g., *new, ready, running, waiting, halted*).
* **Process Number:** A unique identifier for the process (often called a Process ID or PID).
* **Program Counter:** The exact memory address of the *next* instruction the process needs to execute.
* **CPU Registers:** State information including accumulators, index registers, stack pointers, and condition codes.
* *Crucial Note:* When an interrupt occurs, the OS must save these registers along with the program counter so the process can resume seamlessly later.

* **CPU-Scheduling Information:** Data used to decide when the process gets CPU time (e.g., process priority, pointers to scheduling queues).
* **Memory-Management Information:** Data tracking the memory allocated to the process (e.g., base and limit registers, page tables, or segment tables).
* **Accounting Information:** Usage statistics (e.g., amount of CPU time and real time used, time limits, account or job numbers).
* **I/O Status Information:** A list of I/O devices currently allocated to the process and a list of all open files.

SPAM RIPS.
PPPCCMAI
---

## 1. Process Scheduling

The primary goal of scheduling is to optimize system performance by efficiently assigning the CPU to processes.

* **Multiprogramming:** Keeps multiple processes in memory to ensure some process is always running, maximizing CPU utilization. The number of processes in memory is the **degree of multiprogramming**.
* **Time Sharing:** Switches the CPU rapidly between processes so users can interact with each program seamlessly.
* **Process Behaviors:**
* **I/O-Bound:** Spends more time doing I/O operations than computing (e.g., a word processor waiting for keystrokes).
* **CPU-Bound:** Spends more time doing computations and rarely requests I/O (e.g., rendering a 3D video).



### Scheduling Queues

Processes migrate between different queues throughout their lifecycle.

* **Ready Queue:** Holds processes that are in memory and waiting to execute on the CPU. It is typically structured as a linked list of Process Control Blocks (PCBs).
* **Wait Queue:** Holds processes waiting for a specific event to occur, such as the completion of an I/O request.

### CPU Scheduling & Swapping

* **CPU Scheduler:** Selects a process from the ready queue to run on the CPU. It executes frequently (at least every 100 milliseconds) to prevent any single process from monopolizing the core.
* **Swapping:** If memory is overcommitted, the OS can temporarily move ("swap out") a process from memory to the disk. It is later "swapped in" to resume execution, freeing up main memory in the interim.

---

## 2. Context Switching

When an interrupt occurs, the CPU must pause its current task to run a kernel routine.

* **The Mechanism:** The OS saves the state (context) of the currently running process and loads the saved state of the next process scheduled to run.
* **The Context:** Stored in the process's PCB, this includes CPU register values, process state, and memory-management information.
* **The Cost:** Context switching is **pure overhead**. The system performs no useful work during this time (typically a few microseconds). Speed depends heavily on hardware (e.g., having multiple sets of registers speeds this up).

---

## 3. Process Creation (Key Concept)

Processes can spawn new processes during execution, creating a parent-child hierarchy.

### Process Identification and Hierarchy

* **pid (Process Identifier):** A unique integer assigned to every process.
* **The Root:** In Linux/UNIX, `systemd` (or traditionally `init`) has `pid 1` and is the root parent for all user processes.
* *Subtitle: Figure 3.7 - A typical process tree for the Linux operating system.*

### Resource Allocation & Execution

When a child is created, it needs resources (CPU, memory, files).

* **Resource Sharing:** The parent might share all, some, or no resources with the child. Restricting resources prevents a single user from overloading the system with endless child processes.
* **Execution Options:**
1. Parent and child execute concurrently.
2. Parent waits until some or all of its children terminate.


* **Address-Space Options:**
1. The child is an exact duplicate of the parent (same program and data).
2. The child has an entirely new program loaded into its memory.



### UNIX vs. Windows Creation

* **UNIX/Linux:** Uses `fork()` to create an exact duplicate of the parent. Following the fork, `exec()` is often used by the child to replace its memory space with a new program.
* **Windows:** Uses `CreateProcess()`. Unlike `fork()`, this requires loading a specified program into the child's address space right at creation and requires setting numerous parameters up front.

### C Code Analysis: `fork()`, `exec()`, and `wait()`

Below is a practical example of how a parent creates a child in UNIX, and how the child executes a new command.

```c
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> // Standard inclusion for wait()

int main()
{
    pid_t pid;
    
    /* fork a child process */
    pid = fork();
    
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { /* child process */
        execlp("/bin/ls","ls",NULL);
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete");
    }
    return 0;
}

```

**Step-by-Step Breakdown:**

1. **`pid = fork();`**: The parent process duplicates itself. From this line forward, *two* processes are running concurrently.
2. **`if (pid < 0)`**: Checks for a system failure (e.g., system out of memory).
3. **`else if (pid == 0)`**: This block only executes in the **child process**. The `fork()` command returns `0` to the newly created child.
* **`execlp("/bin/ls","ls",NULL);`**: The child replaces its own duplicated memory space with the `/bin/ls` program (which lists directory contents).


4. **`else`**: This block only executes in the **parent process**. The `fork()` command returns the child's actual PID (an integer > 0) to the parent.
* **`wait(NULL);`**: The parent halts its own execution and moves off the ready queue until the child finishes running the `ls` command.
* **`printf("Child Complete");`**: Once the child terminates, the parent wakes up, prints this message, and finishes.



---

## 4. Process Termination

A process ends by executing its final statement and calling the `exit()` system call, which deallocates its physical/virtual memory, open files, and buffers.

* **Status Return:** The terminating child passes an exit status (usually an integer) back to the parent waiting via the `wait()` call.
* **Forced Termination:** A parent can kill a child process if the child exceeds resource limits, the task is no longer needed, or the OS requires **cascading termination** (where an OS automatically kills all children if a parent terminates).

### Zombies vs. Orphans

* **Zombie Process:** A process that has terminated, but its parent has not yet called `wait()`. Its resources are deallocated, but its entry remains in the OS process table to store the exit status. All terminated processes briefly become zombies.
* **Orphan Process:** A process whose parent has terminated *without* ever calling `wait()`. In UNIX/Linux, these orphans are adopted by `init` or `systemd`, which periodically calls `wait()` to collect their exit statuses and clear them from the process table.

---
## 1. Process Cooperation Overview

Processes in an operating system are either **independent** (do not share data) or **cooperating** (affect or are affected by other processes, sharing data).

**Why allow process cooperation?**

* **Information Sharing:** Multiple applications can access the same data (e.g., copy/paste).
* **Computation Speedup:** Breaking tasks into parallel subtasks (requires multiple CPU cores).
* **Modularity:** Dividing system functions into separate processes/threads.

Cooperating processes require an **Interprocess Communication (IPC)** mechanism.

---

## 2. IPC Model 1: Shared Memory

In this model, cooperating processes agree to remove OS memory restrictions to establish a shared memory region.

* **Speed:** Very fast. Kernel intervention is only required to set up the memory. Subsequent accesses are routine memory reads/writes.
* **Responsibility:** The processes themselves must manage data location, format, and synchronization (ensuring they don't overwrite data simultaneously).

### The Producer-Consumer Problem

A classic paradigm for cooperating processes (similar to the client-server model). A **producer** creates information that a **consumer** reads.

* **Unbounded Buffer:** No size limit; the producer can always create new items.
* **Bounded Buffer:** Fixed size; the producer must wait if the buffer is full, and the consumer must wait if it is empty.

### Bounded Buffer Implementation (Code & Explanation)

The buffer is implemented as a circular array using two logical pointers:

* `in`: Points to the next free position.
* `out`: Points to the first full position.
* **Empty state:** `in == out`
* **Full state:** `((in + 1) % BUFFER_SIZE) == out`

**1. Shared Variables Setup:**

```c
#define BUFFER_SIZE 10
typedef struct {
    /* data fields */
} item;

item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

```

**2. The Producer Process (Figure 3.12):**

```c
item next_produced;
while (true) {
    /* produce an item in next_produced */
    
    /* Wait if the buffer is full */
    while (((in + 1) % BUFFER_SIZE) == out)
        ; /* do nothing */
        
    buffer[in] = next_produced;
    in = (in + 1) % BUFFER_SIZE; // Move pointer forward circularly
}

```

*Explanation:* The producer generates an item. The `while` loop checks if the next position for `in` hits the `out` pointer. If it does, the buffer is full, and the producer loops endlessly (waits) until the consumer frees up space.

**3. The Consumer Process (Figure 3.13):**

```c
item next_consumed;
while (true) {
    /* Wait if the buffer is empty */
    while (in == out)
        ; /* do nothing */
        
    next_consumed = buffer[out];
    out = (out + 1) % BUFFER_SIZE; // Move pointer forward circularly
    
    /* consume the item in next_consumed */
}

```

*Explanation:* The consumer checks if `in` and `out` point to the same slot. If so, the buffer is empty, and it waits. Otherwise, it reads the data at `out` and moves the pointer forward.

---

## 3. IPC Model 2: Message Passing

This model uses OS-provided mechanisms to allow processes to communicate and synchronize without sharing an address space. It is highly effective for distributed systems (e.g., networks, chat programs).

**Core Operations:** `send(message)` and `receive(message)`

* **Fixed-size messages:** Easy for the OS to implement, harder for the programmer.
* **Variable-size messages:** Harder for the OS to implement, easier for the programmer.

### 3.1 Naming (How processes identify each other)

**Direct Communication:** Processes explicitly name each other.

* *Symmetric:* `send(P, message)` and `receive(Q, message)`. Both must name each other.
* *Asymmetric:* `send(P, message)` and `receive(id, message)`. Only the sender names the recipient.
* *Drawback:* Hard-coding names severely limits modularity.

**Indirect Communication:** Messages are sent to and received from **mailboxes (ports)**.

* *Primitives:* `send(A, message)` and `receive(A, message)` (where A is a mailbox).
* *Ownership:* * **Process-owned:** Attached to a process's address space. Disappears when the process terminates.
* **OS-owned:** Independent object. The OS provides system calls to create, use, and delete the mailbox.



### 3.2 Synchronization (Blocking vs. Nonblocking)

Communication design dictates how processes wait for messages.

* **Blocking (Synchronous):**
* *Send:* Sender halts until the message is received.
* *Receive:* Receiver halts until a message is available.


* **Nonblocking (Asynchronous):**
* *Send:* Sender fires off the message and immediately resumes its own execution.
* *Receive:* Receiver checks for a message and gets either a valid message or a null (if empty).


* **Rendezvous:** Occurs when both `send()` and `receive()` are blocking.

### Message-Passing Producer/Consumer Implementation

If we use a blocking `send()` and `receive()`, the complex buffer math from Shared Memory vanishes.

**Producer Process (Figure 3.14):**

```c
message next_produced;
while (true) {
    /* produce an item in next_produced */
    send(next_produced); // Blocks if the queue/link is full
}

```

**Consumer Process (Figure 3.15):**

```c
message next_consumed;
while (true) {
    receive(next_consumed); // Blocks until a message arrives
    /* consume the item in next_consumed */
}

```

### 3.3 Buffering

Regardless of naming, messages reside in a temporary queue governed by three capacity types:

1. **Zero capacity (No buffering):** Queue length is 0. The sender *must* block until the recipient receives the message.
2. **Bounded capacity (Automatic buffering):** Queue length is *n*. The sender only blocks if the queue is full.
3. **Unbounded capacity (Automatic buffering):** Infinite length. The sender never blocks.

---

