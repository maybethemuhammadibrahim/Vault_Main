### 1. Thread Libraries and Pthreads

**Formal Definition:** A thread library provides a programming interface (API) for creating and managing threads. POSIX Threads (Pthreads) is a specific IEEE standard (1003.1c) that defines a C-language API for thread execution and synchronization.

**Intuitive Explanation:**
A thread library is the control panel you use in your code to manage your "workers" (threads). 

Pthreads is just a universally agreed-upon blueprint. It dictates what the "buttons" (functions) on the control panel must be named and what arguments they must accept. It does *not* dictate how those buttons are wired to the hardware. The developers of the specific operating system (Linux, macOS) write the underlying code that makes the Pthreads API actually interact with their specific kernel. 

---

### 2. Core Pthread Functions

Before writing multithreaded code, you must understand the primary functions used to manage the thread lifecycle.

#### A. Creating Threads
**Function:** `int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);`

* **What it is:** The function that spawns a new thread. The main program starts with one default thread. This function creates additional ones.
* **Parameters:**
    * `thread`: A pointer to a variable that will store the unique ID of the newly created thread.
    * `attr`: Attributes for the thread (like stack size). Usually set to `NULL` for default behavior.
    * `start_routine`: The specific C function the thread will execute. It must accept a `void *` and return a `void *`.
    * `arg`: The argument you want to pass to the `start_routine`. Set to `NULL` if no arguments are needed.

#### B. Terminating Threads
**Function:** `void pthread_exit(void *retval);`

* **What it is:** The function a thread calls to shut itself down cleanly. 
* **Parameters:**
    * `retval`: A pointer to the return value you want to send back to the thread that "joins" this one.
* **Termination Triggers:** A thread terminates if it reaches the end of its function naturally, calls `pthread_exit`, gets cancelled by another thread (`pthread_cancel`), or if the entire process terminates (e.g., `main()` finishes and calls `exit()`).

    * *Critical Note:* If the `main()` function finishes and returns normally *without* explicitly calling `pthread_exit()` itself, it kills the entire process, violently terminating any other threads that are still running.

    * *Critical Note:* `pthread_cancel` does not violently kill the thread instantly. It requests termination. To fully clean up the cancelled thread's resources, you still must use `pthread_join()` on it afterward, unless the thread is detached.

#### C. Joining Threads (Detailed Explanation)
**Function:** `int pthread_join(pthread_t thread, void **retval);`

**Intuitive Explanation of Joining:**
Imagine `main()` is a manager who hires a worker (creates a thread) to calculate a massive spreadsheet. 
* If the manager finishes their own immediate tasks and leaves the building (the `main()` function returns), the building locks down. The worker is instantly terminated, even if the spreadsheet is only half-finished.
* **Joining** is the solution. `pthread_join` forces the manager to sit in a chair and wait. The manager's execution is suspended. They cannot leave the building until the worker finishes the spreadsheet and hands it over. 
* If the worker was *already* finished before the manager sat down to wait, the manager instantly takes the result and continues. 

* **Parameters:**
    * `thread`: The specific ID of the thread you are waiting for.
    * `retval`: A pointer to capture the value returned by the thread when it called `pthread_exit`.

##### C. Joining vs. Detaching Threads
**Functions:**
* `int pthread_join(pthread_t thread, void **retval);`: Forces the calling thread (usually `main`) to wait until the specified thread finishes.
* `int pthread_detach(pthread_t thread);`: Marks a thread as independent. 
    * *What it is:* A detached thread runs in the background. When it finishes, the system automatically reclaims its resources. You cannot (and do not need to) call `pthread_join` on a detached thread.

#### D. Thread Synchronization (Overview)
Used to prevent data corruption when multiple threads access shared resources.
* **Mutexes (Locks):** Ensure only one thread accesses a resource at a time.
    * `pthread_mutex_init()`, `pthread_mutex_lock()`, `pthread_mutex_unlock()`, `pthread_mutex_destroy()`
* **Condition Variables:** Allow threads to wait until a specific condition becomes true.
    * `pthread_cond_init()`, `pthread_cond_wait()`, `pthread_cond_signal()`, `pthread_cond_broadcast()`, `pthread_cond_destroy()`

#### E. Thread Attributes
Manage specific behaviors of a thread (like starting it in a detached state automatically).
* `pthread_attr_init()`: Initializes an attribute object.
* `pthread_attr_setdetachstate()`: Sets whether the thread will be joinable or detached upon creation.
* `pthread_attr_destroy()`: Destroys the attribute object once the thread is created.


---

### 3. Code Examples

Below are the examples provided, corrected for compilation (added missing standard libraries, math headers, and macros). 

#### Example 1: Basic Creation and Joining
**Purpose:** Demonstrates spawning a single thread, doing a simple task, and forcing `main()` to wait for it using `pthread_join`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *helloWorld(void *vargp) {
    sleep(1);
    printf("Hello World \n");
    return NULL;
}

int main() {
    pthread_t thread_id;
    printf("Before Thread\n");
    
    // Spawn the worker
    pthread_create(&thread_id, NULL, helloWorld, NULL);
    
    // Manager waits here until the worker is done
    pthread_join(thread_id, NULL);
    
    printf("After Thread\n");
    exit(0);
}
```

#### Example 2: Creating Multiple Threads
**Purpose:** Demonstrates using a loop to spawn multiple threads and pass a unique identifier (the loop index) to each one.

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *PrintHello(void *threadid) {
    long tid;
    tid = (long) threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc; 
    long t;
    
    for(t=0; t<NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", t);
        // Cast the integer 't' to a void pointer to pass it as an argument
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    /* Last thing that main() should do to allow threads to finish if not joining */
    pthread_exit(NULL);
}
```

#### Example 3: Thread Attributes and Joining Computations
**Purpose:** Demonstrates threads doing heavy computation, explicitly setting thread attributes to ensure they are "joinable", and retrieving status codes upon completion.

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Required for sin() and tan()

#define NUM_THREADS 4

void *BusyWork(void *t) {
    int i;
    long tid;
    double result = 0.0;
    tid = (long)t;
    printf("Thread %ld starting...\n", tid);
    
    for (i = 0; i < 1000000; i++) {
        result = result + sin(i) * tan(i);
    }
    
    printf("Thread %ld done. Result = %e\n", tid, result);
    pthread_exit((void*) t); // Return the thread ID as the exit status
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc; 
    long t; 
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (t = 0; t < NUM_THREADS; t++) {
        printf("Main: creating thread %ld\n", t);
        rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_join(thread[t], &status);
        if (rc) {
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        // Cast the returned status back to a long integer to read it
        printf("Main: completed join with thread %ld having status %ld\n", t, (long)status);
    }
    
    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
}
```

#### Example 4: Detaching a Thread
**Purpose:** Demonstrates allowing a thread to run independently without blocking `main()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Required for sleep()

void* threadFunction(void* arg) {
    printf("Thread is running...\n");
    sleep(2);
    printf("Thread is finishing...\n");
    return NULL;
}

int main() {
    pthread_t thread;
    int result;
    
    // Create a new thread
    result = pthread_create(&thread, NULL, threadFunction, NULL);
    if (result != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Detach the thread so it cleans up automatically
    result = pthread_detach(thread);
    if (result != 0) {
        fprintf(stderr, "Error detaching thread\n");
        return 1;
    }

    // Main thread continues to run immediately without waiting
    printf("Main thread is continuing...\n");
    sleep(3); // Main thread does its own work
    printf("Main thread is finishing...\n");
    return 0;
}
```

#### Example 5: Using Shared Global Memory
**Purpose:** Demonstrates passing a command-line argument to a thread to compute a sum and storing the result in a globally shared variable.

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; /* This data is shared by the thread(s) */
void *runner(void *param); /* Threads call this function */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return -1;
    }

    pthread_t tid; /* The thread identifier */
    pthread_attr_t attr; /* Set of thread attributes */

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]); // argv[1] is passed as param
    pthread_join(tid, NULL);

    printf("sum = %d\n", sum);
    return 0;
}

void *runner(void *param) {
    int i, upper = atoi(param);
    sum = 0; // Accessing global variable

    for (i = 1; i <= upper; i++)
        sum += i;

    pthread_exit(0);
}
```

#### Example 6:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

int global_array[ARRAY_SIZE]; // Shared array

// Function to initialize the array with random values
void initialize_array() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        global_array[i] = rand() % 1000;
    }
}

// Function to find the sum of elements in a portion of the array
void *sum_array(void *arg) {
    int thread_id = *(int *)arg;
    int start = thread_id * (ARRAY_SIZE / NUM_THREADS);
    int end = start + (ARRAY_SIZE / NUM_THREADS);
    int sum = 0;

    // Calculate the sum of elements in the assigned portion of the array
    for (int i = start; i < end; ++i) {
        sum += global_array[i];
    }

    return (void *)(long)sum; // Return the sum as a void pointer
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    void *thread_results[NUM_THREADS];
    long total_sum = 0;

    // Initialize the array with random values
    initialize_array();

    // Create threads to compute the sum of array elements
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, sum_array, (void *)&thread_args[i]);
    }

    // Join threads and collect results
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], &thread_results[i]);
        total_sum += (long)thread_results[i]; // Accumulate the partial sums
    }

    printf("Total sum of array elements: %ld\n", total_sum);

    return 0;
}

```

#### Example 7: Passing String Arguments & Joining Multiple
**Purpose:** Demonstrates spawning multiple threads, passing string literals as arguments, and verifying successful creation/joining using `assert`.

```c
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

void *mythread(void *arg) {
    // Cast the void pointer back to a string and print
    printf("%s\n", (char *) arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    int rc;
    printf("main: begin\n");
    
    // Create threads passing "A" and "B"
    rc = pthread_create(&p1, NULL, mythread, "A"); 
    assert(rc == 0);
    
    rc = pthread_create(&p2, NULL, mythread, "B"); 
    assert(rc == 0);
    
    // Join waits for both threads to finish sequentially
    rc = pthread_join(p1, NULL); 
    assert(rc == 0);
    
    rc = pthread_join(p2, NULL); 
    assert(rc == 0);
    
    printf("main: end\n");
    return 0;
}
```


### 1. Thread Execution and Non-Determinism

**Formal Definition:** Non-determinism in multithreading means that the execution order of threads is not predictable and is entirely controlled by the operating system's scheduler, not the programmer's code sequence.

**Intuitive Explanation:**
When you write a standard, single-threaded program, it executes line 1, then line 2, then line 3. It is perfectly predictable. 

When you use `pthread_create`, you are no longer executing a function; you are asking the operating system to hire a new worker and assign them a task. 
1. `main` creates Thread 1. 
2. `main` creates Thread 2.

Just because Thread 1 was created first does not mean it starts working first. The OS scheduler is a black box. It might:
* Run Thread 1 immediately (Table 26.2).
* Pause Thread 1, run Thread 2 entirely, then go back to Thread 1 (Table 26.3).
* Run `main` until it hits `pthread_join`, and only then wake up the threads (Table 26.1).

This means every time you run the exact same program, you might get a different output order (e.g., "A" then "B", or "B" then "A"). 

Use the visualization below to simulate how the OS scheduler randomly dictates the execution timeline of your threads.

---

### 2. Memory Control Keywords

When dealing with multiple threads, you must strictly control how variables are stored and accessed in memory.

#### The `volatile` Keyword
**Formal Definition:** A type qualifier that tells the compiler that a variable's value may be changed at any time by entities outside the program's immediate control (like hardware registers or other threads).

**Intuitive Explanation:**
Compilers are designed to make your code run fast. If a compiler sees a loop counting to 10 million, it will normally load the `counter` variable into the CPU's ultra-fast internal register, do all the math there, and only write the final result back to the main RAM once the loop is done. 

In a multithreaded environment, this is catastrophic. If Thread A has a copy of `counter` in its CPU register, and Thread B has a copy in a *different* CPU register, they cannot see each other's updates. 

Adding `volatile` disables this optimization. It forces the CPU to read the variable directly from the main RAM before every single operation, and write it directly back to the main RAM after every single operation.

#### The `static` Keyword
**Formal Definition:** A keyword that controls the lifetime and linkage (visibility) of a variable.

**Intuitive Explanation:**
`static` has two completely different meanings depending on where you type it:

1. **Inside a function (Local Static):**
   * Normally, variables inside a function are destroyed when the function finishes. 
   * A `static` local variable is created once when the program starts and is *never destroyed*. If you leave the function and come back, the variable remembers its last value.
2. **Outside a function (Global Static):**
   * Normally, a global variable can be accessed by any other file in your entire project.
   * A `static` global variable is strictly locked to the file it was declared in. It acts as a private global variable for that specific C file. In the provided code, `static volatile int counter = 0;` ensures no other C files can accidentally tamper with this specific counter.

---

### 3. The Shared Counter Problem (Race Conditions)

The final code snippet demonstrates a fundamental disaster in concurrency known as a **Race Condition**. 

```c
static volatile int counter = 0;

void *
mythread(void *arg)
{
    printf("%s: begin\n", (char *) arg);
    int i;
    for (i = 0; i < 1e7; i++) {
        counter = counter + 1;
    }
    printf("%s: done\n", (char *) arg);
    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t p1, p2;
    printf("main: begin (counter = %d)\n", counter);
    Pthread_create(&p1, NULL, mythread, "A");
    Pthread_create(&p2, NULL, mythread, "B");

    // join waits for the threads to finish
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("main: done with both (counter = %d)\n", counter);
    return 0;
}
```

**Code Breakdown:**
* `static volatile int counter = 0;`: A shared global variable, forced to be read/written directly to RAM.
* `mythread`: A function that loops 10 million times, doing `counter = counter + 1`.
* `main`: Spawns two threads (A and B) that both run `mythread` simultaneously.

**The Added Value / Crucial Explanation:**
You might logically expect the final output to be `20000000` (10 million from A + 10 million from B). **It will almost certainly not be.** The final number will be random and lower than 20 million.

Why? Because `counter = counter + 1` is not a single, atomic action. It is three separate steps at the hardware level:
1. **Read** the current value of `counter` from RAM.
2. **Add** 1 to that value inside the CPU.
3. **Write** the new value back to RAM.

Because of the OS scheduling non-determinism we discussed earlier, Thread A and Thread B can overlap:
* Thread A reads `counter` (value is 50).
* *The OS pauses Thread A and switches to Thread B.*
* Thread B reads `counter` (value is still 50).
* Thread B adds 1 and writes it back (value is now 51).
* *The OS switches back to Thread A.*
* Thread A adds 1 to *its* copied value of 50, and writes it back (value is 51).

Even though both threads completed a loop iteration, the counter only went up by 1 instead of 2. An update was overwritten and permanently lost. This proves that merely creating threads is insufficient; programmers must actively synchronize them (using Mutexes) to protect shared data.