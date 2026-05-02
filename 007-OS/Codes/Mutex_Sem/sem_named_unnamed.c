
//----

#include <semaphore.h>

// 1. Declare the semaphore variable
sem_t sem;

void initialize() {
    // 2. Initialize the semaphore
    // Arg 1: pointer to semaphore
    // Arg 2: sharing flag (0 = shared between threads of the same process)
    // Arg 3: initial value (e.g., 1 for a binary semaphore/mutex equivalent)
    sem_init(&sem, 0, 1); 
}

void* worker_thread(void* arg) {
    // 3. Acquire (wait)
    sem_wait(&sem);
    
    /* Critical Section */
    
    // 4. Release (signal)
    sem_post(&sem);
    
    return NULL;
}

---


#include <semaphore.h>
#include <fcntl.h> // For O_* constants
#include <sys/stat.h> // For mode constants

// 1. Declare a pointer for the named semaphore
sem_t *sem;

void initialize() {
    // 2. Create and initialize the semaphore
    // Arg 1: Name of the semaphore
    // Arg 2: O_CREAT flag (create if it doesn't exist)
    // Arg 3: File permissions (0666 = read/write for everyone)
    // Arg 4: Initial value (1)
    sem = sem_open("SEM", O_CREAT, 0666, 1);
}

void process_execution() {
    // 3. Acquire the semaphore
    sem_wait(sem);
    
    /* Critical section */
    
    // 4. Release the semaphore
    sem_post(sem);
}