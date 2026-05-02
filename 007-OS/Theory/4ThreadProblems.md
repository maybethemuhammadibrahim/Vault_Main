### 1. Semantics of `fork()` and `exec()`

**Formal Definitions:**
* **`fork()`:** A UNIX system call that creates a new process (the "child") by duplicating the calling process (the "parent").
* **`exec()`:** A UNIX system call that completely replaces the memory space, code, and data of the current process with a brand-new program.

**The Multithreading Problem:**
In a single-threaded program, `fork()` is simple: duplicate the one process and its one thread. In a multithreaded program, if Process A has 10 threads, and Thread 1 calls `fork()`, what happens to the child process? Does it get 1 thread (a copy of Thread 1) or 10 threads (a copy of the whole environment)?

**The Solution:**
UNIX systems typically handle this by providing two different versions of `fork()`:
1.  A version that duplicates **all threads**.
2.  A version that duplicates **only the thread that called `fork()`**.

**How `exec()` Interacts:**
If any thread inside a process calls `exec()`, the operating system immediately wipes the entire process. All threads are instantly destroyed, and the memory is replaced with the new program. 

**Explicit Example:**
Why do we need two versions of `fork()`? It depends entirely on what the programmer intends to do immediately after the fork.

* **Scenario A: Using `fork()` followed immediately by `exec()`**
    * *Context:* A web browser (multithreaded) wants to open a completely separate PDF reader application.
    * *Action:* The browser calls `fork()` to create a child process, then immediately calls `exec("pdf_reader.exe")` inside that child.
    * *Correct Choice:* Use the **single-thread** `fork()`.
    * *Reasoning:* Duplicating all 10 browser threads into the child process takes CPU time and memory. It is a massive waste of resources because a millisecond later, `exec()` will wipe out all 10 duplicated threads to load the PDF reader.

* **Scenario B: Using `fork()` to run a parallel copy**
    * *Context:* A database server is processing a massive dataset across 8 threads. It needs to create a snapshot backup of its current state while continuing to serve users.
    * *Action:* The server calls `fork()` to create a child process. The child process will save the data to disk and then exit. It does *not* call `exec()`.
    * *Correct Choice:* Use the **all-threads** `fork()`.
    * *Reasoning:* The child process needs an exact replica of the entire environment, including the state of all 8 threads, to accurately write the snapshot to disk.

---

### 2. Signal Handling

**Formal Definition:** A signal is a software interrupt delivered to a process by the operating system to indicate that a specific event has occurred (e.g., memory violation, user pressing Ctrl+C, timer expiration).

**Synchronous vs. Asynchronous Signals:**

| Feature | Synchronous Signals | Asynchronous Signals |
| :--- | :--- | :--- |
| **Origin** | Generated internally by the program executing an illegal or monitored instruction. | Generated externally by the user, the hardware, or another process. |
| **Examples** | Division by zero, illegal memory access (segfault). | Ctrl+C (termination), network disconnect, timer expiring. |
| **Delivery** | Delivered directly to the specific thread that caused the error. | Delivered to the process as a whole. |

**Signal Handlers:**
When a signal arrives, the program must pause and run a specific function to handle it.
1.  **Default Handler:** Provided by the OS kernel (e.g., the default action for dividing by zero is to kill the program).
2.  **User-Defined Handler:** A custom function written by the programmer to override the default action (e.g., instead of crashing on a network disconnect, run a function that tries to reconnect).

**The Multithreading Problem:**
When an asynchronous signal (like Ctrl+C) hits a multithreaded process, which of the 10 running threads should stop to handle it? The OS must choose a routing strategy:
1.  Deliver it to the thread it applies to (usually only works for synchronous signals).
2.  Deliver it to every single thread (inefficient, causes redundant handling).
3.  Deliver it to specific designated threads.
4.  Dedicate one specific thread whose *only* job is to sit and wait for all incoming signals. (This is the most common and logical architectural choice in modern systems).

---

### 3. Thread Cancellation

**Formal Definition:** The act of terminating a target thread before it has naturally completed its task.

**Intuitive Explanation:**
Imagine you search for a file on your hard drive. The UI spawns a thread to scan the disk. Halfway through, you click "Cancel". That thread needs to be stopped.

**Two Approaches:**
1.  **Asynchronous Cancellation:** The operating system terminates the thread immediately, exactly where it stands.
    * *The Danger:* If the thread was in the middle of writing a file, the file is corrupted. If it was holding a synchronization lock (Mutex), that lock is never released, and other threads waiting for it will freeze forever (Deadlock).
2.  **Deferred Cancellation:** The OS sets a "cancellation flag". The thread is allowed to keep running, but it is programmed to periodically check this flag. When it sees the flag is set, it cleans up its resources, releases its locks, and terminates itself gracefully.

---

### 4. Thread-Local Storage (TLS)

**Formal Definition:** A memory management method that uses static or global memory local to a specific thread.

**Intuitive Explanation:**
To understand TLS, you must compare it to the alternatives:
* **Local Variables (Inside a function):** Created when the function starts, destroyed when the function ends. They cannot retain data across multiple function calls.
* **Global/Static Variables:** Persist for the entire life of the program. However, they are shared. If Thread A changes a global variable, Thread B sees the change.

**Thread-Local Storage** sits in the middle. It persists across function calls (like a global variable), but every single thread gets its own private, isolated copy of it. If Thread A changes its TLS variable, Thread B's copy remains untouched.

**Primary Use Case: Thread Pools**
In modern systems, programmers rarely create threads manually. They use a "Thread Pool"—a set of pre-created, generic worker threads managed by the OS. You just hand tasks to the pool.
Because you didn't write the code that creates the thread, you can't easily pass custom arguments to it. If you need a worker thread to maintain a private transaction log or a unique database connection ID across multiple tasks without sharing it with other workers, you declare a TLS variable (`static __thread int transaction_id;`). This guarantees that specific worker has persistent, private data.