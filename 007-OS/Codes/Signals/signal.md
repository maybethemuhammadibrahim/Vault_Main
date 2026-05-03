# Signals in C with Threading — Exam Notes

---

## 1. What is a Signal?

A signal is an **asynchronous notification** sent to a process or thread to notify it of an event. The OS interrupts the target's normal flow and runs a **signal handler** function.

---

## 2. `signal()` — The Simple Way

```c
#include <signal.h>
signal(SIGNUM, handler);
```

| Parameter | Meaning |
|-----------|---------|
| `SIGNUM` | Which signal to catch |
| `handler` | What to do when it arrives |

### Handler values (the 3 options):
```c
signal(SIGINT, my_func);   // Custom function — runs your code
signal(SIGINT, SIG_IGN);   // Ignore — do nothing
signal(SIGINT, SIG_DFL);   // Default — OS default behaviour
```

---

## 3. Common Signal Values (Know These!)

| Signal | Number | Meaning | Default Action | Can Catch/Ignore? |
|--------|--------|---------|----------------|-------------------|
| `SIGINT` | 2 | Ctrl+C from keyboard | Terminate | ✅ Yes |
| `SIGTERM` | 15 | Polite termination request | Terminate | ✅ Yes |
| `SIGUSR1` | 10 | User-defined (you decide) | Terminate | ✅ Yes |
| `SIGUSR2` | 12 | User-defined (you decide) | Terminate | ✅ Yes |
| `SIGKILL` | 9 | Force kill — **cannot be caught** | Terminate | ❌ No |
| `SIGSTOP` | 19 | Pause process — **cannot be caught** | Stop | ❌ No |
| `SIGSEGV` | 11 | Segmentation fault | Core dump | ✅ Yes |
| `SIGCHLD` | 17 | Child process stopped/exited | Ignore | ✅ Yes |
| `SIGALRM` | 14 | Timer expired (`alarm()`) | Terminate | ✅ Yes |

> **Exam tip:** `SIGKILL` and `SIGSTOP` can NEVER be caught, blocked, or ignored. Everything else can be.

---

## 4. What is `SIGINT`?

`SIGINT` = **Signal Interrupt**. Generated when the user presses **Ctrl+C** in the terminal. By default it kills the process, but you can catch it to do cleanup.

```c
void handle_sigint(int sig) {
    printf("Caught Ctrl+C! Cleaning up...\n");
    exit(0);
}
signal(SIGINT, handle_sigint);
```

---

## 5. `SIG_IGN` — Ignoring a Signal

```c
signal(SIGINT,  SIG_IGN);   // Ctrl+C now does nothing
signal(SIGUSR1, SIG_IGN);   // SIGUSR1 silently ignored
```

> Useful for daemons/servers that shouldn't die from accidental Ctrl+C.

---

## 6. `SIGKILL` — The Unkillable Killer

```c
kill(pid, SIGKILL);   // Forces immediate termination — no handler runs
```

- Process gets **no chance** to clean up
- Cannot be caught, blocked, or ignored — ever
- Use only as a last resort

---

## 7. `SIGUSR1` with `kill()` — Sending Between Processes

```c
#include <signal.h>
#include <sys/types.h>

// Send SIGUSR1 to a process by PID
kill(target_pid, SIGUSR1);

// Send to yourself
kill(getpid(), SIGUSR1);
```

**Full example:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Got SIGUSR1!\n");
}

int main() {
    signal(SIGUSR1, handler);
    kill(getpid(), SIGUSR1);   // send to self → handler fires
    return 0;
}
```

---

## 8. `signal()` — Full Template with All Combinations

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/* ── Custom handler ── must match: void name(int) */
void my_handler(int signum) {
    printf("Signal %d received\n", signum);
}

int main() {
    /* Combination 1: Custom handler — your function runs */
    signal(SIGINT,  my_handler);

    /* Combination 2: Ignore — signal silently discarded */
    signal(SIGUSR2, SIG_IGN);

    /* Combination 3: Restore OS default behaviour */
    signal(SIGTERM, SIG_DFL);

    /* Combination 4: Send signal to self */
    kill(getpid(), SIGUSR1);   // if SIGUSR1 has no handler → default = terminate

    /* Combination 5: Register SIGUSR1 then send */
    signal(SIGUSR1, my_handler);
    kill(getpid(), SIGUSR1);   // now my_handler fires

    pause();   // wait for any signal
    return 0;
}
```

---

## 9. `sigaction` — The Robust Way (Preferred in Real Code)

`signal()` is simple but has portability quirks. `sigaction` is the POSIX standard — **more control, more reliable.**

### Structure:
```c
struct sigaction {
    void     (*sa_handler)(int);        // simple handler: void f(int)
    void     (*sa_sa_sigaction)(int, siginfo_t *, void *); // advanced handler
    sigset_t   sa_mask;                 // signals to block WHILE handler runs
    int        sa_flags;                // modifiers (see below)
};
```

### `sa_flags` values:

| Flag | Effect |
|------|--------|
| `0` | No special behaviour |
| `SA_RESTART` | Auto-restart interrupted system calls |
| `SA_SIGINFO` | Use `sa_sigaction` instead of `sa_handler` |
| `SA_RESETHAND` | Reset to `SIG_DFL` after first delivery |
| `SA_NODEFER` | Don't block the signal while handler runs |

### Default Template:

```c
#include <stdio.h>
#include <signal.h>
#include <string.h>   // for memset / sigemptyset

void my_handler(int signum) {
    /* write() is async-signal-safe; printf() technically is not */
    write(STDOUT_FILENO, "Signal caught!\n", 15);
}

int main() {
    struct sigaction sa;

    /* Step 1: Zero out the struct — always do this first */
    memset(&sa, 0, sizeof(sa));          // OR: sigemptyset(&sa.sa_mask);

    /* Step 2: Set handler */
    sa.sa_handler = my_handler;

    /* Step 3: Set mask — block no extra signals during handler */
    sigemptyset(&sa.sa_mask);

    /* Step 4: Set flags */
    sa.sa_flags = SA_RESTART;            // restart syscalls if interrupted

    /* Step 5: Register — sigaction(signal, new, old) */
    /* Pass NULL as 3rd arg if you don't need the old action */
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    /* ── Block SIGUSR2 during SIGINT handler ── */
    sigaddset(&sa.sa_mask, SIGUSR2);     // add to block-set
    sigaction(SIGINT, &sa, NULL);        // re-register with updated mask

    pause();   // wait for signal
    return 0;
}
```

> **Memory aid:** `sigaction` steps = **Zero → Handler → Mask → Flags → Register**

---

## 10. Signals with Threads

### Key rules:
- `signal()` / `sigaction()` applies **process-wide** — any thread can receive it
- `pthread_kill(tid, sig)` sends a signal to a **specific thread**
- In a multi-threaded program, use `sigaction` in **one dedicated thread**, and block signals in all others using `pthread_sigmask`

### Two sending functions:

```c
kill(pid,  SIGUSR1);          // send to whole process — OS picks a thread
pthread_kill(tid, SIGUSR1);   // send to a specific thread
```

---

## 11. Fixed & Corrected Full Example — SIGUSR1 Between Threads

Your original code had several bugs. Here is the corrected version with every fix annotated:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define NUM_THREADS 4

pthread_t threads[NUM_THREADS];

/* ── Signal handler ──*/
void sigusr1_handler(int signum) {
    pid_t tid  = gettid();    // kernel thread ID (Linux only)
    pid_t pid  = getpid();
    pid_t ppid = getppid();

    fprintf(stdout, "Thread (kernel TID=%d) received SIGUSR1 | PID=%d | PPID=%d\n",
            tid, pid, ppid);
}

/* ── Thread function ── */
 
void *thread_function(void *arg) {
    while (1) sleep(1);   // keep thread alive waiting for signals
    return NULL;
}

int main() {
    /* Register handler BEFORE spawning threads */
    signal(SIGUSR1, sigusr1_handler);

    /* Create threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    pid_t tid  = gettid();
    pid_t pid  = getpid();
    pid_t ppid = getppid();

    fprintf(stdout, "PPID=%d | PID=%d | main() kernel TID=%d\n", ppid, pid, tid);

    fprintf(stdout, "Thread IDs: 0=%lu, 1=%lu, 2=%lu, 3=%lu\n",
            (unsigned long)threads[0], (unsigned long)threads[1],
            (unsigned long)threads[2], (unsigned long)threads[3]);

    sleep(1);   // give threads time to start

    /* ── Send SIGUSR1 to the whole process ── */
    printf("Sending SIGUSR1 to whole process (pid=%d)...\n", pid);
    kill(pid, SIGUSR1);

    sleep(1);

    /* ── Send SIGUSR1 to a specific thread (thread index 2) ── */
    printf("Sending SIGUSR1 specifically to thread[2]...\n");
    pthread_kill(threads[2], SIGUSR1);   // targets thread 2 only

    sleep(1);

    /* Join all threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cancel(threads[i]);   // stop the infinite-loop threads
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
```

### Compile & run:
```bash
gcc -o signals signals.c -lpthread
./signals
```

---

## 13. Quick Syntax Cheat Sheet

```
signal(SIG,  handler)          → simple registration
kill(pid,    sig)              → send to process (any thread)
pthread_kill(tid, sig)         → send to specific thread

sigaction(SIG, &sa, NULL)      → robust registration

getpid()    → process ID
getppid()   → parent process ID
gettid()    → kernel thread ID (Linux, #include <sys/types.h>)
pthread_self() → pthread_t of calling thread (opaque, not a pid)
```

```
SIG_DFL   restore default
SIG_IGN   ignore
my_func   custom handler — signature: void f(int)
```

> **Cannot be caught or ignored:** `SIGKILL` (9), `SIGSTOP` (19) — always remember these two.