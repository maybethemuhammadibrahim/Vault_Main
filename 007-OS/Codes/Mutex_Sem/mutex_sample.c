#include <pthread.h>

// 1. Declare the mutex
pthread_mutex_t mutex;

void initialize() {
    // 2. Initialize: Arg 1 is pointer to mutex, Arg 2 is attributes (NULL = default)
    pthread_mutex_init(&mutex, NULL); 
}

void* worker_thread(void* arg) {
    // 3. Acquire the lock (blocks if unavailable)
    pthread_mutex_lock(&mutex);
    
    /* Critical Section */
    
    // 4. Release the lock
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}
