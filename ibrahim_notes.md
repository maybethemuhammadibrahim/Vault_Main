# Viva Preparation Notes — Muhammad Ibrahim (24K-0649)

## PART 1: WHAT PROBLEM DOES THIS PROJECT SOLVE?

### The Producer-Consumer Problem (in plain english)

Imagine a small table that can hold exactly 5 plates. There are some cooks (producers) making food and placing plates on the table, and some waiters (consumers) picking plates up from the table and serving them.

The rules are:
- A cook CANNOT place a plate if the table is already full (5 plates).
- A waiter CANNOT pick up a plate if the table is empty (0 plates).
- Two people CANNOT touch the table at the exact same time or plates will break (data corruption).

This is the **Producer-Consumer Problem** — a classic Operating Systems concurrency problem. The "table" is a **bounded buffer** (a fixed-size array in memory). The "cooks" are **producer threads** and the "waiters" are **consumer threads**. All threads run at the same time (concurrently), so we need rules (synchronization) to prevent chaos.

### What can go wrong without synchronization?

1. **Race Condition**: Two threads modify the buffer at the same time → corrupted data.
2. **Deadlock**: Two threads wait for each other forever → program freezes.
3. **Starvation**: One thread never gets a turn → unfair.
4. **Buffer Overflow/Underflow**: Writing to a full buffer or reading from an empty one → crash.

### How this project solves it

This project implements the solution in C using **pthreads** (POSIX threads) on Linux. It provides TWO different synchronization approaches:

1. **Semaphore Variant** — uses 3 semaphores (empty, full, mutex)
2. **Monitor Variant** — uses 1 mutex + 2 condition variables

Both approaches guarantee: no race conditions, no deadlocks, no data loss.

---

## PART 2: KEY CONCEPTS YOU MUST KNOW

### What is a Thread?
A thread is a lightweight unit of execution inside a program. Multiple threads share the same memory space but run independently. In this project, each producer and each consumer is a separate thread created with `pthread_create()`.

### What is a Mutex?
A mutex (mutual exclusion lock) is like a key to a room. Only one thread can hold the key at a time. If thread A holds the mutex, thread B must wait until A releases it. This prevents two threads from modifying shared data simultaneously.

### What is a Semaphore?
A semaphore is a counter that threads can increment (`sem_post`) or decrement (`sem_wait`). If the counter is 0 and a thread calls `sem_wait`, that thread goes to sleep until another thread calls `sem_post`. This project uses:
- `sem_empty` (initialized to buffer capacity, e.g. 5) — counts free slots
- `sem_full` (initialized to 0) — counts filled slots
- `sem_mutex` (initialized to 1) — acts as a binary lock for mutual exclusion

### What is a Condition Variable?
A condition variable lets a thread say "wake me up when something changes." Used in the monitor variant. A thread calls `pthread_cond_wait()` to sleep and another thread calls `pthread_cond_signal()` to wake it up.

### What is a Circular Buffer?
An array where the write position wraps around to the beginning when it reaches the end. Think of it as a ring. Position is calculated using modulo: `next = (current + 1) % capacity`.

### Semaphore Protocol (how it works step by step)

**Producer wants to insert an item:**
1. `sem_wait(empty)` — "Is there a free slot?" If yes, decrement empty count. If no, sleep.
2. `sem_wait(mutex)` — "Can I access the buffer?" If yes, lock it. If no, sleep.
3. Insert the item into the buffer.
4. `sem_post(mutex)` — Release the buffer lock.
5. `sem_post(full)` — Tell consumers "there's one more item available."

**Consumer wants to remove an item:**
1. `sem_wait(full)` — "Is there an item?" If yes, decrement full count. If no, sleep.
2. `sem_wait(mutex)` — "Can I access the buffer?" If yes, lock it. If no, sleep.
3. Remove the item from the buffer.
4. `sem_post(mutex)` — Release the buffer lock.
5. `sem_post(empty)` — Tell producers "there's one more free slot."

**Why this order prevents deadlock:** The counting semaphore (empty/full) is ALWAYS acquired BEFORE the binary semaphore (mutex). This means no thread will ever hold the mutex while waiting for a counting semaphore, so no circular waiting can happen.

---

## PART 3: PROJECT FILE STRUCTURE

```
project/
├── include/               # Header files (.h) — declarations
│   ├── buffer.h           # Buffer struct + config constants
│   ├── semaphore_sync.h   # Semaphore declarations
│   ├── monitor_sync.h     # Monitor declarations
│   ├── logger.h           # Logger declarations        ← YOUR FILE
│   └── visualizer.h       # Visualizer declarations    ← YOUR FILE
├── src/                   # Source files (.c) — implementations
│   ├── buffer.c           # Buffer insert/remove logic
│   ├── semaphore_sync.c   # Semaphore init/destroy
│   ├── monitor_sync.c     # Monitor produce/consume
│   ├── producer.c         # Producer thread function
│   ├── consumer.c         # Consumer thread function
│   ├── token_fairness.c   # Anti-starvation mechanism
│   ├── main.c             # Program entry point
│   ├── benchmark.c        # Performance measurement
│   ├── logger.c           # Thread-safe logging        ← YOUR FILE
│   └── visualizer.c       # Live terminal display      ← YOUR FILE
├── tests/                 # Automated tests
│   ├── test_single.c      # Basic 1P/1C test           ← YOUR FILE
│   ├── test_stress.c      # Heavy 8P/8C test           ← YOUR FILE
│   └── test_edge.c        # Edge case tests            ← YOUR FILE
├── Makefile               # Build commands
└── docs/                  # Documentation
```

### How compilation works

The project uses a **Makefile** with multiple build targets:

| Command | What it builds | What flag it sets |
|---------|---------------|-------------------|
| `make semaphore` | Semaphore variant executable | (none) |
| `make monitor` | Monitor variant executable | `-DUSE_MONITOR` |
| `make benchmark` | Benchmark executable | `-DRUN_BENCHMARK` |
| `make test` | All 3 test executables | (none) |

The compiler flag `-DUSE_MONITOR` defines a macro. In `main.c`, the code checks `#ifdef USE_MONITOR` to decide which synchronization functions to call. Same binary, different behavior based on compile-time flag.

All builds use: `gcc -std=c11 -Wall -Wextra -Werror -pthread -Iinclude`
- `-std=c11` = use C11 standard
- `-Wall -Wextra -Werror` = enable all warnings and treat them as errors
- `-pthread` = enable POSIX threads
- `-Iinclude` = look for header files in the `include/` folder

---

## PART 4: YOUR FILES — DETAILED EXPLANATION

---

### FILE 1: `include/logger.h` (Header — Declarations)

This header file defines WHAT the logger can do, without saying HOW. Other files `#include "logger.h"` to use the logger.

**Enums defined:**

```c
typedef enum {
    LOG_ACTION_PRODUCE,   // a producer inserted an item
    LOG_ACTION_CONSUME,   // a consumer removed an item
    LOG_ACTION_FINISHED   // a thread is done working
} log_action_t;
```
An enum is just a way to give names to numbers. `LOG_ACTION_PRODUCE` = 0, `LOG_ACTION_CONSUME` = 1, etc. This makes code readable — instead of writing `action = 0`, we write `action = LOG_ACTION_PRODUCE`.

```c
typedef enum {
    LOG_THREAD_PRODUCER,
    LOG_THREAD_CONSUMER,
    LOG_THREAD_VISUALIZER
} log_thread_type_t;
```
Identifies what type of thread is generating the log entry.

**The log entry struct:**

```c
typedef struct {
    log_thread_type_t thread_type;  // who is logging (producer/consumer)
    int               thread_id;    // which thread number (0, 1, 2...)
    log_action_t      action;       // what happened (produce/consume/finished)
    buffer_item       item;         // what item was involved (-1 if N/A)
    int               buf_count;    // how many items in buffer right now
    int               buf_capacity; // max buffer size
} log_entry_t;
```
Every time something happens (an item is produced or consumed), the thread fills in this struct and passes it to `logger_log()`.

**Functions declared:**
- `logger_init(filepath)` — open the log file
- `logger_log(entry)` — write one event to the log
- `logger_flush()` — force pending writes to disk
- `logger_destroy()` — close everything

---

### FILE 2: `src/logger.c` (Implementation — The Actual Code)

#### Global variables (static = private to this file)

```c
static FILE            *g_log_fp   = NULL;    // file pointer to the trace log
static pthread_mutex_t  g_log_lock = PTHREAD_MUTEX_INITIALIZER; // logger's OWN mutex
static int              g_log_quiet = 0;      // if 1, dont print to terminal
```

**CRITICAL CONCEPT — Why does the logger have its own mutex?**

The buffer already has a mutex (`sem_mutex`). Why not reuse it for logging? Two reasons:

1. **Performance**: Writing to a file (disk I/O) is SLOW. If we held the buffer mutex while writing to a file, all other producers and consumers would be blocked waiting — not because the buffer is busy, but because the logger is slowly writing to disk. The buffer's critical section should be as short as possible.

2. **Deadlock prevention**: If a thread holds `buffer_mutex` and then tries to acquire `logger_mutex`, and another thread holds `logger_mutex` and tries to acquire `buffer_mutex`, we get a deadlock (circular wait). By making sure **the logger mutex is NEVER held while the buffer mutex is held**, we eliminate this risk entirely. In this code, `logger_log()` is called INSIDE the buffer critical section, but the logger acquires and releases its own lock independently — the code is structured so these locks never nest.

#### `logger_init()` function

```c
int logger_init(const char *filepath)
{
    g_log_fp = fopen(filepath, "w");       // open file for writing
    // ... write a header with timestamp ...
    // uses clock_gettime() for nanosecond-precision timestamps
    return 0;
}
```
Opens a log file (e.g., `traces/run_1714123456.log`). The filename includes a Unix timestamp so each run creates a unique file.

#### `logger_log()` function — THE CORE

```c
void logger_log(const log_entry_t *entry)
{
    // 1. Get current time (BEFORE locking, to minimize lock hold time)
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    // 2. Lock the logger's own mutex
    pthread_mutex_lock(&g_log_lock);

    // 3. Write to the trace file
    fprintf(g_log_fp, "[%s] [%s:%d] [%s] [item=%d] [%d/%d]\n", ...);
    fflush(g_log_fp);  // force write to disk immediately

    // 4. Also print to terminal (stderr) if not in quiet mode
    if (!g_log_quiet) {
        fprintf(stderr, "  %s #%d  %-8s  item=%-6d  [%d/%d]\n", ...);
    }

    // 5. Unlock
    pthread_mutex_unlock(&g_log_lock);
}
```

**What each log line looks like:**
```
[14:30:05.123456789] [Producer:0] [PRODUCE] [item=1001] [3/5]
```
This means: at time 14:30:05, Producer #0 produced item 1001, and the buffer now has 3 out of 5 slots filled.

**Why `fflush()`?** Normally, C buffers file writes in memory and writes to disk in batches for performance. But in a concurrent program, if the program crashes, unflushed data is lost. `fflush()` forces the data to disk immediately so we don't lose log entries.

**Why `stderr` and not `stdout`?** `stderr` is unbuffered by default and doesn't interfere with the visualizer (which uses `stdout`). This means log messages and the visual display don't mix up.

#### `logger_destroy()` function

```c
void logger_destroy(void)
{
    fprintf(g_log_fp, "\n=== Trace Log End ===\n");
    fclose(g_log_fp);         // close the file
    g_log_fp = NULL;
    pthread_mutex_destroy(&g_log_lock);  // clean up the mutex
}
```

---

### FILE 3: `include/visualizer.h` (Header)

```c
extern volatile int g_active_producers;  // how many producers are currently active
extern volatile int g_active_consumers;  // how many consumers are currently active
```
`volatile` tells the compiler: "Don't optimize reads of this variable — its value can change at any time from another thread." Without `volatile`, the compiler might cache the value in a register and never re-read it from memory.

`extern` means "this variable is defined somewhere else" (in `visualizer.c`). The header just declares it exists.

**Functions:**
- `visualizer_start(buf, num_prod, num_cons, variant_name)` — start the display thread
- `visualizer_stop()` — stop the display thread
- `visualizer_set_quiet(quiet)` — enable/disable the display

---

### FILE 4: `src/visualizer.c` (Implementation)

#### ANSI Color Codes
```c
#define RST   "\033[0m"   // reset to default color
#define BOLD  "\033[1m"   // bold text
#define GRN   "\033[92m"  // green
#define RED   "\033[91m"  // red
// ... etc
```
These are special escape sequences that terminals understand. When you print `"\033[92m"` followed by text, the text appears green. `"\033[0m"` resets back to normal.

#### The visualizer thread function

```c
static void *visualizer_thread(void *arg)
{
    struct timespec refresh = { .tv_sec = 0, .tv_nsec = 100000000L }; // 100ms

    while (!g_vis_stop) {
        // Read buffer state using ATOMIC loads (no locks!)
        int count = __atomic_load_n(&g_vis_buf->count, __ATOMIC_SEQ_CST);

        // Clear screen and redraw
        printf("\033[H\033[2J");  // ANSI: move cursor to top-left + clear screen

        // Draw a progress bar
        // Draw active thread counts
        // Draw individual buffer slots if buffer is small

        nanosleep(&refresh, NULL);  // sleep 100ms then redraw
    }
    return NULL;
}
```

**CRITICAL CONCEPT — Why `__atomic_load_n` instead of locking?**

`__atomic_load_n(&variable, __ATOMIC_SEQ_CST)` reads a variable atomically — meaning the read is guaranteed to be a single, uninterruptible operation. This is important because:

1. **No deadlock risk**: The visualizer NEVER calls `sem_wait()` on any buffer semaphore. If it did, it could block and prevent producers/consumers from running. Since it only reads (never writes) the buffer state, and uses atomic reads, it cannot interfere with the synchronization layer at all.

2. **Slight lag is acceptable**: The displayed values might be ~100ms behind reality. This is fine for a visual display — we trade perfect accuracy for zero interference with the core system.

3. **`__ATOMIC_SEQ_CST`** stands for "sequentially consistent" — the strongest memory ordering guarantee. It ensures the read sees the most recent write from any thread.

#### The progress bar drawing

```c
int bar_w  = 30;                              // bar is 30 characters wide
int filled = (count * bar_w) / cap;           // how many '#' to draw
// Result: [######........................] 2/5  40%
```

#### `visualizer_start()` and `visualizer_stop()`

```c
int visualizer_start(bounded_buffer_t *buf, int num_prod, int num_cons, const char *variant_name)
{
    // Save parameters in global variables
    g_vis_buf = buf;
    g_vis_stop = 0;
    // Create a new thread that runs visualizer_thread()
    pthread_create(&g_vis_tid, NULL, visualizer_thread, NULL);
    return 0;
}

void visualizer_stop(void)
{
    g_vis_stop = 1;                    // tell the thread to stop
    pthread_join(g_vis_tid, NULL);     // wait for it to finish
}
```

`pthread_join()` blocks the calling thread until the target thread finishes. This ensures clean shutdown — we don't destroy the buffer while the visualizer is still trying to read it.

---

### FILE 5: `tests/test_single.c` — Basic Correctness Test

**What it tests:** 1 producer, 1 consumer, 100 items. The simplest possible scenario.

**How it works step by step:**

```c
// 1. Set up
buffer_init(&buf, 5);        // create buffer with 5 slots
sem_sync_init(5);            // initialize the 3 semaphores
logger_init("traces/test_single.log");

// 2. Create thread arguments
thread_arg_t prod_arg = { .thread_id = 0, .iterations = 100, .buf = &buf };
thread_arg_t cons_arg = { .thread_id = 0, .iterations = 0,   .buf = &buf };
// iterations=0 for consumer means "keep consuming until told to stop"

// 3. Create threads (consumer FIRST, then producer)
pthread_create(&cons_tid, NULL, consumer_fn, &cons_arg);
pthread_create(&prod_tid, NULL, producer_fn, &prod_arg);

// 4. Wait for producer to finish all 100 items
pthread_join(prod_tid, NULL);

// 5. Tell consumer to stop
g_done = 1;               // set the "done" flag
sem_post(&sem_full);       // wake the consumer if it's sleeping on sem_wait(full)

// 6. Wait for consumer to finish
pthread_join(cons_tid, NULL);

// 7. Verify correctness
assert(g_total_produced == 100);   // did we produce exactly 100?
assert(g_total_consumed == 100);   // did we consume exactly 100?
assert(g_total_produced == g_total_consumed);  // do they match?
```

**Why consumer is created FIRST:** If the producer starts first and fills the buffer before the consumer exists, the producer blocks on `sem_wait(empty)`. That's fine — it will unblock once the consumer starts consuming. But creating the consumer first ensures it's ready to consume immediately.

**What `g_done` and cascade `sem_post` do:** When the producer finishes, the consumer might be sleeping on `sem_wait(sem_full)` waiting for items. Setting `g_done = 1` alone isn't enough because the consumer is asleep. So we call `sem_post(&sem_full)` to wake it up. The consumer then checks `g_done` and `buffer_count == 0`, sees everything is done, and exits.

**What `assert()` does:** If the condition is false, the program crashes with an error message. If it's true, nothing happens. This is how we automatically verify correctness.

---

### FILE 6: `tests/test_stress.c` — High Contention Test

**What it tests:** 8 producers, 8 consumers, 1000 items each, 30-second timeout.

**Why this test matters:** With 16 threads all fighting for the same buffer (size 10), any synchronization bug will almost certainly manifest. Race conditions that might not appear with 2 threads will show up with 16.

**The timeout mechanism:**

```c
static void timeout_handler(int sig)
{
    fprintf(stderr, "FAILED: test_stress timed out (>30s) — possible deadlock!\n");
    _exit(EXIT_FAILURE);  // kill the program immediately
}

signal(SIGALRM, timeout_handler);  // register the handler
alarm(30);                          // set a 30-second alarm
```

`alarm(30)` tells the OS: "Send me a SIGALRM signal in 30 seconds." If the test completes before that, we call `alarm(0)` to cancel the alarm. If the test is STILL running after 30 seconds (meaning threads are probably deadlocked), the `timeout_handler` fires and kills the program with an error message.

**This proves the system is deadlock-free:** If 16 threads can process 8000 total items (8 producers × 1000 items) within 30 seconds without hanging, deadlock is extremely unlikely.

**The cascade shutdown for multiple consumers:**

```c
g_done = 1;
for (int i = 0; i < NUM_CONS; i++)
    sem_post(&sem_full);     // wake up EACH consumer
```
With 8 consumers, we need 8 `sem_post` calls — one to wake each sleeping consumer. Each consumer, when it wakes up and sees `g_done == 1` and `buffer_count == 0`, also calls `sem_post(&sem_full)` before exiting (cascade) to wake the next consumer.

---

### FILE 7: `tests/test_edge.c` — Edge Case Tests

This file contains 3 sub-tests that test unusual/extreme scenarios:

#### Test 1: Buffer Size 1
```c
buffer_init(&buf, 1);  // buffer can only hold ONE item at a time
```
**Why this is tricky:** With capacity 1, the producer and consumer must strictly alternate. Producer inserts → consumer removes → producer inserts → etc. There's zero room for error. If the synchronization has any off-by-one bug, this test will catch it.

#### Test 2: Zero Items
```c
thread_arg_t pa = { .iterations = 0, ... };  // producer makes NOTHING
```
**Why this matters:** Tests the shutdown path. The producer immediately finishes (0 iterations), `g_done` is set, and the consumer must wake up and exit cleanly without consuming anything. This tests that the termination logic doesn't assume at least one item was produced.

#### Test 3: Buffer Saturation
```c
buffer_init(&buf, 2);        // tiny buffer: 2 slots
// but iterations = 100      // 100 items to process
```
**Why this matters:** The buffer will fill up and empty repeatedly (100 items through a 2-slot buffer). The producer will block on `sem_wait(empty)` very frequently. This stress-tests the blocking/waking cycle and ensures no items are lost even when the buffer is constantly at capacity.

**All three sub-tests share the same timeout handler** — if any of them deadlock, the 30-second alarm kills the whole program.

---

## PART 5: HOW ALL THE FILES FIT TOGETHER

Here is the execution flow when you run `make semaphore && ./producer_consumer`:

```
main.c
  │
  ├─ buffer_init()         ← allocate the shared circular buffer
  ├─ logger_init()         ← open the trace log file (YOUR CODE)
  ├─ visualizer_start()    ← spawn the display thread (YOUR CODE)
  ├─ sem_sync_init()       ← initialize the 3 semaphores
  │
  ├─ pthread_create() × N  ← spawn consumer threads (consumer.c)
  ├─ pthread_create() × N  ← spawn producer threads (producer.c)
  │
  │   [all threads running concurrently]
  │   producer calls: sem_wait(empty) → sem_wait(mutex) → buffer_insert()
  │                   → logger_log() → sem_post(mutex) → sem_post(full)
  │   consumer calls: sem_wait(full) → sem_wait(mutex) → buffer_remove()
  │                   → logger_log() → sem_post(mutex) → sem_post(empty)
  │   visualizer: reads buffer atomically every 100ms, redraws terminal
  │
  ├─ pthread_join() producers  ← wait for all producers to finish
  ├─ g_done = 1               ← signal consumers to stop
  ├─ sem_post(full) × N       ← wake sleeping consumers
  ├─ pthread_join() consumers  ← wait for all consumers to finish
  │
  ├─ visualizer_stop()    ← stop and join the display thread (YOUR CODE)
  ├─ logger_destroy()     ← close the log file (YOUR CODE)
  ├─ sem_sync_destroy()   ← destroy semaphores
  └─ buffer_destroy()     ← free buffer memory
```

---

## PART 6: VIVA QUESTIONS AND ANSWERS

**Q: Why does the logger have its own mutex?**
A: To prevent deadlock and keep the buffer's critical section short. If we used the buffer's mutex for logging, the slow disk I/O would block all other threads from accessing the buffer. Also, if any thread tried to log while holding the buffer lock, and the logger tried to acquire the buffer lock, we'd get a deadlock.

**Q: What is `__atomic_load_n` and why does the visualizer use it?**
A: It's a GCC built-in that reads a variable atomically (as one uninterruptible operation). The visualizer uses it to read buffer state without acquiring any lock. This means the visualizer can NEVER cause a deadlock or slow down producers/consumers. The trade-off is the display might be ~100ms behind reality, which is acceptable for a visual display.

**Q: How do your tests prove the system is deadlock-free?**
A: The stress test runs 8 producers and 8 consumers (16 threads) processing 8000 items with a 30-second timeout. If the program completes within 30 seconds, it proves no deadlock occurred. If there were a deadlock, threads would wait forever and the alarm would fire, failing the test. The edge tests additionally verify extreme scenarios like buffer size 1 where any sync bug would cause immediate deadlock.

**Q: What is `volatile` and why is it used for `g_active_producers`?**
A: `volatile` tells the compiler that a variable can change at any moment (by another thread or hardware). Without it, the compiler might optimize by caching the value in a CPU register and never re-reading it from RAM, meaning one thread wouldn't see changes made by another thread.

**Q: What does `fflush()` do in the logger?**
A: C normally buffers writes in memory for performance. `fflush()` forces the buffered data to be written to disk immediately. In a concurrent program, if we don't flush and the program crashes, we lose the buffered log entries. Flushing after every log entry ensures we never lose data.

**Q: Why is the consumer created before the producer in tests?**
A: So the consumer is ready to receive items as soon as the producer starts. If the producer started first and the buffer filled up before the consumer was created, the producer would block — which is fine but less efficient for testing.

**Q: What does the cascade `sem_post(sem_full)` in consumer.c do?**
A: When a consumer detects that producers are done and the buffer is empty, it calls `sem_post(&sem_full)` before exiting. This wakes up the NEXT sleeping consumer, who then also checks the done flag and exits, waking the next one, and so on. This chain reaction ensures ALL consumers exit gracefully.

**Q: What does `assert()` do?**
A: `assert(condition)` checks if the condition is true. If it's true, nothing happens. If it's false, the program immediately crashes and prints the file name, line number, and the failed condition. We use it to automatically verify that produced count equals consumed count.

**Q: What does `_exit()` vs `exit()` do in the timeout handler?**
A: `_exit()` terminates the process immediately without running cleanup handlers or flushing stdio buffers. We use it in the signal handler because signal handlers should do minimal work — calling `exit()` (which runs `atexit` handlers and flushes buffers) inside a signal handler can cause undefined behavior if another thread is currently doing I/O.

**Q: What is `PTHREAD_MUTEX_INITIALIZER`?**
A: It's a macro that statically initializes a mutex at compile time. It's equivalent to calling `pthread_mutex_init()` but doesn't require a function call. We use it for the logger mutex because it's a global variable that needs to be ready before any function is called.
