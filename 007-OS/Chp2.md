## 2.1 Operating-System Services

An OS provides an environment for programs to execute and offers specific services to both users and the system itself.

### User-Facing Services (To make tasks easier)

* **User Interface (UI):** How users interact with the OS (CLI, GUI, or Touch-screen).
* **Program Execution:** Loading a program into memory, running it, and handling normal or abnormal (error) termination.
* **I/O Operations:** Providing safe, efficient means to interact with files or hardware devices, since users cannot control hardware directly.
* **File-System Manipulation:** Creating, deleting, reading, writing, searching, and managing permissions of files and directories.
* **Communications:** Allowing processes to exchange information, either on the same system or across a network. Implemented via:
* *Shared Memory:* Processes read/write to a shared memory segment.
* *Message Passing:* The OS moves packets of information between processes.


* **Error Detection:** Constantly monitoring for and correcting hardware errors (memory parity, power, network drops) and software errors (arithmetic overflows, illegal memory access) to prevent system halts.

### System-Facing Services (To ensure efficient operation)

* **Resource Allocation:** Managing CPU cycles (via scheduling routines), main memory, file storage, and peripheral devices for multiple concurrent processes.
* **Logging:** Tracking resource usage for accounting (billing) or performance statistics (for system administrators to optimize the system).
* **Protection and Security:** * *Protection:* Ensuring all internal access to system resources by concurrent processes is strictly controlled.
* *Security:* Defending the system from external threats (passwords, authentications, network firewalls).



---

## 2.2 User and Operating-System Interface

* **Command Interpreters (CLI):** Also known as **shells** (e.g., bash, C shell). Fetches and executes user commands.
* *Implementation 1:* The shell contains the actual code for the command (makes the shell program very large).
* *Implementation 2 (UNIX/Linux approach):* The shell uses system programs. For example, typing `rm file.txt` simply tells the shell to find the `rm` executable, load it, and pass `file.txt` as a parameter. This keeps the shell small and makes adding new commands easy.


* **Graphical User Interface (GUI):** A mouse-based window-and-menu desktop metaphor. Originated at Xerox PARC (1973), popularized by Apple (Macintosh) and Microsoft (Windows). Open-source UNIX/Linux systems often use KDE or GNOME desktop environments.
* **Touch-Screen Interface:** Gesture-based (swiping, tapping) with software keyboards, driven by the impracticality of physical keyboards/mice on mobile devices (e.g., iOS Springboard).

---

## 2.3 System Calls

System calls are the programmatic interface to OS services, typically written in C/C++ (though low-level hardware access may require assembly language).

* **The Complexity of Direct Calls:** A simple task like copying a file requires dozens of system calls (prompting the user, reading keyboard input, opening the source, creating the destination, reading/writing loops, error handling, and closing files).
* **Application Programming Interfaces (APIs):** Developers rarely write direct system calls. Instead, they use APIs (Windows API, POSIX API for Linux/UNIX, Java API).
* *Benefits:* APIs ensure program portability across different systems and abstract away complex kernel-level details.
* *How it works:* The **Run-Time Environment (RTE)** and standard libraries (like `libc` in C) intercept the API function (e.g., `printf()`), look up the associated system call number in a table, invoke the actual kernel system call (e.g., `write()`), and return the result.


* **Passing Parameters to the OS:**
1. **Registers:** The simplest method, but limited by the number of hardware registers.
2. **Block/Table in Memory:** Parameters are stored in memory, and the *address* of that block is passed in a register. (Linux uses this if there are more than 5 parameters).
3. **Stack:** Parameters are pushed onto the stack by the program and popped off by the OS. (Like the block method, this does not limit parameter count/length).



---

### 2.3.3 Types of System Calls

System calls fall into six primary categories.

#### 1. Process Control

* **Core functions:** `create_process()`, `terminate_process()`, `load()`, `execute()`, `abort()`.
* **Execution control:** `get/set_process_attributes()`, `wait_time()`, `wait_event()`, `signal_event()`.
* **Memory/Locks:** Allocating/freeing memory, acquiring/releasing locks for shared data.
* *Single-tasking vs. Multitasking Context:*
* **Single-tasking (e.g., Arduino):** A boot loader loads one program (sketch) into memory. If another is loaded, it overwrites the first.
* **Multitasking (e.g., FreeBSD/Linux):** The shell runs continuously. It uses `fork()` to create a new process and `exec()` to load a program into it. The shell can either wait for it to finish or let it run in the background. The program uses `exit()` to terminate and return a status code.



#### 2. File Management

* **Core functions:** `create()`, `delete()`, `open()`, `close()`, `read()`, `write()`, `reposition()` (skip/rewind).
* **Attributes:** `get/set_file_attributes()` (name, type, permissions).

#### 3. Device Management

* **Core functions:** `request()`, `release()`, `read()`, `write()`, `reposition()`, logically `attach/detach`.
* *Note:* Because I/O devices (like disk drives) and files require such similar operations, UNIX-like systems merge them into a combined file–device structure, treating devices as special files.

#### 4. Information Maintenance

* **System info:** `get/set_time()`, `get/set_date()`, get system data (OS version, free memory).
* **Debugging aids:** Memory `dump()`, single-step CPU execution traps, time profiling (using timer interrupts to see where a program spends its execution time).
* *Example:* The `strace` command in Linux lists every system call a program executes, which is invaluable for debugging.



#### 5. Communications

* **Message Passing Model:** Requires translating host/process names to IDs (IP addresses/ports). Uses `open/close_connection()`, `read/write_message()`. Client connects; Server (often a background daemon) waits with `accept_connection()`. Great for networking and small data.
* **Shared Memory Model:** Uses `shared_memory_create/attach()`. Removes OS memory restrictions so processes can read/write directly to the same RAM segment. Offers maximum speed but requires complex synchronization to prevent data corruption.

#### 6. Protection

* **Core functions:** `get/set_permission()`, `allow/deny_user()`. Essential for multiuser networked environments to manipulate resource access rights.


## 2.4 System Services (System Utilities)

System services provide a convenient environment for program development and execution. While some are simple interfaces for system calls, others are complex utilities. The user's view of an OS is largely defined by these services rather than the underlying kernel.

* **Core Categories:**
* **File Management:** Create, delete, copy, rename, and list files/directories.
* **Status Information:** Retrieve system data (date, time, memory, disk space) or detailed performance/logging data (often stored in a registry).
* **File Modification:** Text editors and search/transformation commands.
* **Programming-Language Support:** Compilers, assemblers, and interpreters (C, C++, Java, Python).
* **Program Loading and Execution:** Loaders (absolute, relocatable, overlay), linkage editors, and debuggers.
* **Communications:** Creating virtual connections (web browsing, email, remote login, file transfers).
* **Background Services (Daemons):** Processes that launch at boot and run constantly in the background.
* *Practical Example:* A network daemon listening for incoming connections or a process scheduler managing CPU time.


# 2.7 Operating-System Design and Implementation

Designing an OS is highly creative, with requirements shifting drastically based on the target hardware (e.g., a real-time embedded system vs. an enterprise server).

### 2.7.1 Design Goals

* **User Goals:** The system should be convenient, easy to learn, reliable, safe, and fast.
* **System Goals (Developer-facing):** The system should be easy to design, implement, and maintain, while being flexible, error-free, and efficient.

### 2.7.2 Mechanisms and Policies (Crucial Concept)

A fundamental principle of OS design is the **separation of policy from mechanism** to ensure maximum flexibility.

* **Policy:** Determines *what* will be done. (e.g., Deciding that I/O-intensive programs get higher CPU priority).
* **Mechanism:** Determines *how* to do it. (e.g., The actual hardware timer construct used to enforce that priority).
* *Implementation Examples:* * **Microkernels:** Highly separated. The kernel provides basic mechanisms; user programs define policies.
* **Windows/Apple:** Tightly coupled. Mechanism and policy are intertwined to force a uniform global look and feel.
* **Linux:** Flexible. The default CPU scheduler (mechanism) enforces a standard policy, but developers can swap it out.



### 2.7.3 Implementation

Modern operating systems are written primarily in high-level languages (C, C++) rather than purely in assembly.

* **Advantages of High-Level Languages:** Faster development, more compact code, easier debugging, and massive portability across different hardware architectures (Intel, ARM). Modern compilers optimize code so efficiently that speed loss is negligible.
* **Critical Paths:** Only a fraction of OS code requires extreme performance tuning (interrupt handlers, memory managers, CPU schedulers).

---

## 2.8 Operating-System Structure

Modern operating systems are incredibly complex and must be carefully engineered into components or modules.

### 1. Monolithic Structure

* **Concept:** No structure. The entire kernel is a single, static binary running in a single address space (e.g., original UNIX, base Linux).
* **Pros:** Extremely fast communication and low system-call overhead.
* **Cons:** Difficult to implement, debug, and extend. A tightly coupled system where one bug can crash the whole OS.

### 2. Layered Approach

* **Concept:** The OS is broken into strictly ordered levels (Layer 0 = Hardware ... Layer N = UI).
* **Pros:** Extremely easy to debug. You test Layer 1; if it works, you move to Layer 2. Any bug found is isolated to the newest layer.
* **Cons:** Terrible performance. A user request at Layer N must traverse every single layer down to Layer 0, creating massive overhead.

### 3. Microkernels

* **Concept:** Strips the kernel down to bare essentials (minimal process/memory management and communication). Everything else (file systems, drivers) runs as user-level programs. (e.g., Mach, QNX).
* **Pros:** Highly secure and reliable. If a service (like a display driver) crashes in user space, the kernel survives. Very easy to extend.
* **Cons:** Poor performance. Services must communicate indirectly through the kernel using **message passing**, which causes significant overhead.

### 4. Modules (Loadable Kernel Modules - LKMs)

* **Concept:** The best modern methodology. The kernel provides core services but dynamically links additional services (like file systems or USB drivers) during boot or runtime. (e.g., Linux, Windows, macOS).
* **Pros:** Combines the performance of a monolithic kernel (no message passing) with the flexibility of a microkernel (easy to extend without recompiling the whole kernel).

### 5. Hybrid Systems

In practice, almost no OS uses a strict single structure.

* **Linux:** Monolithic (for speed) but modular (for flexibility).
* **Windows:** Monolithic core with microkernel-like subsystems.
* **Apple (macOS and iOS):**
* **Shared Architecture:** Both rely on the **Darwin** kernel environment (a hybrid of the Mach microkernel and the BSD UNIX kernel). They utilize Core Frameworks (media/graphics) and Application Frameworks (Cocoa for macOS, Cocoa Touch for iOS).
* **Key Differences:** macOS runs on Intel architectures and is open to developers. iOS runs on ARM architectures, has aggressive memory/power management, stricter security, and heavily restricted APIs.


