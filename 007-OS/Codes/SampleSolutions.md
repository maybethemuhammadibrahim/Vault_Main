Here are the complete, corrected solutions for all 9 questions, featuring bug fixes, optimized logic, and detailed explanatory comments.

### 1. Multithreaded Array Sorting & Merging
**Fixes:** Changed thread count to 2 (as requested by prompt). Replaced the $O(n^2)$ full-array re-sort at the end with an $O(n)$ merge algorithm to correctly combine the two sorted halves.

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 2
#define SIZE 20

int arr[SIZE];

// Initialize array with descending values to test sorting
void initArr() {
    for (int i = 0; i < SIZE; i++) {
        arr[i] = SIZE - i;
    }
}

// Standard bubble sort applied to a specific subarray segment
void bubbleSort(int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = start; j < end - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Thread function: sorts its designated half of the array
void *workerThread(void *attr) {
    int id = *((int *)attr);
    int start = id * (SIZE / THREADS);
    int end = start + (SIZE / THREADS);
    
    bubbleSort(start, end);
    pthread_exit(NULL);
}

// Merges two contiguous sorted halves of the array in O(n) time
void mergeArray() {
    int mid = SIZE / 2;
    int temp[SIZE];
    int i = 0, j = mid, k = 0;

    // Compare and merge elements from both halves
    while (i < mid && j < SIZE) {
        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    // Append remaining elements from the first half
    while (i < mid) temp[k++] = arr[i++];
    
    // Append remaining elements from the second half
    while (j < SIZE) temp[k++] = arr[j++];

    // Copy the merged temp array back to the original array
    for (int x = 0; x < SIZE; x++) {
        arr[x] = temp[x];
    }
}

int main() {
    initArr();
    pthread_t threads[THREADS];
    int thread_att[THREADS];
    
    // Create threads to sort halves
    for (int i = 0; i < THREADS; i++) {
        thread_att[i] = i; 
        pthread_create(&threads[i], NULL, workerThread, (void *)&thread_att[i]);  
    }
    
    // Wait for sorting to finish
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);  
    }

    // Merge the two sorted halves
    mergeArray();
    
    printf("\nFinal Sorted Data:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Index %d: %d\n", i, arr[i]);
    }

    return 0;
}
```

### 2. Multithreaded Matrix Multiplication
**Fixes:** Cleaned up variables and added comments. The casting logic to return the sum works correctly on POSIX systems.

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 3

int arr1[SIZE][SIZE];
int arr2[SIZE][SIZE];
int result[SIZE][SIZE];

// Hardcode matrix values for quick testing
void hardcode() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            arr1[i][j] = 1;
            arr2[i][j] = 1;
        }
    }
}

// Thread function: calculates one specific row of the result matrix
void *workerThread(void *attr) {
    int id = *((int *)attr); // 'id' represents the row index this thread is responsible for
    int row_total_sum = 0;

    // Matrix multiplication logic for a single row
    for (int j = 0; j < SIZE; j++) {
        int cell_sum = 0;
        for (int k = 0; k < SIZE; k++) {
            cell_sum += arr1[id][k] * arr2[k][j];
        }
        result[id][j] = cell_sum;
        row_total_sum += cell_sum; // Tallying sum for this specific row
    }
    
    // Cast integer to void* pointer size to safely return it without malloc
    return (void *)(long)row_total_sum;
}

int main() {
    hardcode();
    pthread_t threads[SIZE];
    int att[SIZE];
    int rows_sums[SIZE];
    
    // Create one thread per row
    for (int i = 0; i < SIZE; i++) {
        att[i] = i;
        pthread_create(&threads[i], NULL, workerThread, (void *)&att[i]);
    }
    
    void *thread_return;
    printf("\nRow sums returned by threads:\n");
    
    // Join threads and extract the returned row sums
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], &thread_return); 
        rows_sums[i] = (int)(long)thread_return; // Cast back to integer
        printf("Row %d sum: %d\n", i, rows_sums[i]);
    }
    
    printf("\nResult Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("[%d] ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```

### 3. Basic Signal Catching (SIGINT)
**Fixes:** Added variable type for the loop flag. Changed `sleep(1)` to `sleep(2)` to match the prompt's interval constraint.

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// volatile sig_atomic_t ensures safe modification inside a signal handler
volatile sig_atomic_t program_running = 1;

// Handler for Ctrl+C
void signal_handler(int sig) {
    printf("\nSIGINT caught, terminating safely.\n");
    program_running = 0; // Triggers loop exit instead of abrupt kill
}

int main() {
    signal(SIGINT, signal_handler);
    
    while(program_running) {
        printf("Running...\n");
        sleep(2); // Sleeps for 2 seconds per prompt requirement
    }

    return 0; // Clean exit
}
```

### 4. Countdown Timer with Pause/Resume
**Fixes:** Moved `sleep(1)` outside the `if(do_countdown)` block. Previously, pausing the timer caused an infinite loop with no yield, maxing out CPU usage.

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t do_countdown = 1;

// Pause countdown on Ctrl+C
void c_signal_handler(int sig) {
    printf("\nCtrl+C pressed, pausing countdown...\n");
    do_countdown = 0;
}

// Resume countdown on Ctrl+Z
void z_signal_handler(int sig) {
    printf("\nCtrl+Z pressed, resuming countdown...\n");
    do_countdown = 1;
}

int main() {
    int countdown = 10;
    
    // Register signal handlers
    signal(SIGINT, c_signal_handler);
    signal(SIGTSTP, z_signal_handler);
    
    while (countdown >= 0) {
        if (do_countdown) {
            printf("CountDown: %d\n", countdown);
            countdown--;
        } 
        
        // Sleep executes regardless of pause state to prevent CPU spinning
        sleep(1); 
    }
    
    printf("Timer finished.\n");
    return 0;
}
```

### 5. SIGUSR1 Toggle State
**Logic:** Uses a bitwise XOR to toggle the state cleanly inside the signal handler.

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// Global flag to track state
volatile sig_atomic_t paused = 0;

void sigusr1_handler(int sig) {
    paused ^= 1; // Bitwise XOR toggles between 0 and 1
}

int main() {
    signal(SIGUSR1, sigusr1_handler);

    while (1) {
        if (paused) {
            printf("Paused by SIGUSR1\n");
        } else {
            printf("Working...\n");
        }
        sleep(3); // 3 seconds per prompt requirement
    }
    return 0;
}
```

### 6. Dining Philosophers (Deadlock Prevention)
**Logic:** Implements the Resource Hierarchy solution. The final philosopher picks up their right fork first, breaking the circular wait condition that causes deadlocks.

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

sem_t forks[N];

void* philosopher(void* num) {
    int id = *(int*)num;
    int left = id;
    int right = (id + 1) % N;

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        // Deadlock prevention: Force Philosopher 4 to pick up right fork first
        if (id == N - 1) {
            sem_wait(&forks[right]);
            sem_wait(&forks[left]);
        } else {
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);
        }

        printf("Philosopher %d is eating.\n", id);
        sleep(2);

        // Release resources
        sem_post(&forks[left]);
        sem_post(&forks[right]);
    }
    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];

    // Initialize all forks as available (1)
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Spawn philosophers
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
        sem_destroy(&forks[i]);
    }

    return 0;
}
```

### 7. Smart City Parking (Mutex & Semaphores)
**Fixes:** Code was largely correct. Added clear commenting to delineate synchronization primitives.

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define TOTAL_SPACES 4
#define TOTAL_VEHICLES 7

int occupied_slots = 0;
int total_revenue = 0;

sem_t parking_slots;     // Semaphore tracks available physical spaces
pthread_mutex_t lock;    // Mutex protects shared variables (slots, revenue)

void* vehicle_thread(void* arg) {
    int id = *(int*)arg;

    // 1. Request entry (blocks if 4 cars are already inside)
    sem_wait(&parking_slots);

    // 2. Lock mutex to safely update global variables on entry
    pthread_mutex_lock(&lock);
    occupied_slots++;
    printf("Vehicle-%d parked — occupied slots: %d / %d\n", id, occupied_slots, TOTAL_SPACES);
    pthread_mutex_unlock(&lock);

    // 3. Simulate parking duration
    int duration = (rand() % 5) + 2;
    sleep(duration);

    // 4. Lock mutex to safely update global variables on exit
    pthread_mutex_lock(&lock);
    occupied_slots--;
    int fee = (rand() % 151) + 50; // Random fee between 50 and 200
    total_revenue += fee;
    printf("Vehicle-%d departed — occupied slots: %d / %d (Paid: $%d)\n", id, occupied_slots, TOTAL_SPACES, fee);
    pthread_mutex_unlock(&lock);

    // 5. Free up the parking slot for waiting threads
    sem_post(&parking_slots);

    return NULL;
}

int main() {
    pthread_t vehicles[TOTAL_VEHICLES];
    int vehicle_ids[TOTAL_VEHICLES];
    srand(time(NULL));

    // Initialize synchronization primitives
    sem_init(&parking_slots, 0, TOTAL_SPACES);
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < TOTAL_VEHICLES; i++) {
        vehicle_ids[i] = i + 1;
        pthread_create(&vehicles[i], NULL, vehicle_thread, &vehicle_ids[i]);
    }

    for (int i = 0; i < TOTAL_VEHICLES; i++) {
        pthread_join(vehicles[i], NULL);
    }

    printf("\nTotal revenue collected: $%d\n", total_revenue);

    // Clean up
    sem_destroy(&parking_slots);
    pthread_mutex_destroy(&lock);

    return 0;
}
```

### 8. Named Pipes (FIFO) Fire Detection
**Logic:** Establishes IPC via a named pipe. Child writes telemetry data, Parent reads and evaluates.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_NAME "fire_fifo"
#define READINGS 6

int main() {
    int fd;
    int temps[READINGS] = {70, 75, 82, 79, 85, 72};

    // Create Named Pipe (FIFO). Ignore error if it already exists on disk.
    mkfifo(FIFO_NAME, 0666);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // --- CHILD PROCESS (Detector) ---
        fd = open(FIFO_NAME, O_WRONLY); // Blocks until parent opens for reading
        for (int i = 0; i < READINGS; i++) {
            printf("Detector: temperature reading = %d°C\n", temps[i]);
            write(fd, &temps[i], sizeof(int));
            sleep(2);
        }
        close(fd);
        exit(0);
    } else {
        // --- PARENT PROCESS (Controller) ---
        int current_temp;
        int suppression_count = 0;

        fd = open(FIFO_NAME, O_RDONLY); // Blocks until child opens for writing
        
        // Continuously read stream until pipe closes
        while (read(fd, &current_temp, sizeof(int)) > 0) {
            if (current_temp > 80) {
                printf("FIRE ALERT! Activating sprinkler system!\n");
                suppression_count++;
            } else {
                printf("Temperature normal — monitoring continues.\n");
            }
        }
        
        close(fd);
        wait(NULL); // Reap zombie child process

        printf("\nTotal suppression events: %d\n", suppression_count);
        unlink(FIFO_NAME); // Delete FIFO file from filesystem
    }

    return 0;
}
```

### 9. Smart Irrigation System Signals
**Fixes:** Changed interval to `sleep(2)`. Added proper internal signal simulation triggers based on loop increments. 

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// Handler for soil moisture alert
void soil_moisture(int sig) {
    printf("Alert! Soil moisture critically low — activating irrigation!\n");
}

// Handler for clean shutdown
void power_off(int sig) {
    printf("Irrigation system powered off safely...\n");
    exit(0); // Safely terminate
}

int main() {
    // Register handlers
    signal(SIGUSR1, soil_moisture);
    signal(SIGTERM, power_off);
    
    int loop = 0;
    
    while(1) {
        printf("Irrigation System Running...\n");
        sleep(2); // 2 seconds per prompt requirement
        
        // Simulating the receipt of signals internally
        loop++;
        if (loop == 3) {
            printf("[Simulation] Low moisture detected. Triggering SIGUSR1 internally...\n");
            raise(SIGUSR1);
        }
        if (loop == 6) {
            printf("[Simulation] End of day. Triggering SIGTERM internally...\n");
            raise(SIGTERM);
        }
    }
    
    return 0;
}
```