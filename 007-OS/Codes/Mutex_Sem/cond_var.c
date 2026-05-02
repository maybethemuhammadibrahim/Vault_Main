#include <pthread.h>

// 1. Declare Mutex and Condition Variable
pthread_mutex_t mutex;
pthread_cond_t cond_var;

// Shared variables forming the "condition"
int a = 0;
int b = 1;

void initialize() {
    // 2. Initialize both
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);
}

// Thread 1: Waiting for a condition (a == b)
void* waiting_thread(void* arg) {
    pthread_mutex_lock(&mutex);
    
    // Always use a 'while' loop to check the condition, not an 'if'.
    // This protects against "spurious wakeups" (waking up for the wrong reason).
    while (a != b) {
        // Automatically releases mutex and goes to sleep.
        // Re-acquires mutex before waking up.
        pthread_cond_wait(&cond_var, &mutex);
    }
    
    /* Proceed with work knowing a == b */
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

// Thread 2: Modifying data and signaling
void* signaling_thread(void* arg) {
    pthread_mutex_lock(&mutex);
    
    // Modify the shared variables to make the condition true
    a = b; 
    
    // Signal ONE waiting thread to wake up and check the condition
    // Use pthread_cond_broadcast() to wake ALL waiting threads
    pthread_cond_signal(&cond_var);
    
    // Must unlock so the woken thread can actually acquire it
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}