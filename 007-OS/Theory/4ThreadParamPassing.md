### 1. Returning Values from Thread Workers
Threads execute functions that return a `void *` (a generic pointer). To return primitive types, cast the value to a pointer size. To return complex types, return a pointer to heap-allocated memory.

**Pointer Logic & Safe Casting:**
*   **64-bit Size Mismatch:** Pointers are 8 bytes; `int` is 4 bytes. Direct casting causes compiler errors.
*   **The Fix:** Cast `int` -> `intptr_t` -> `void *`. `intptr_t` guarantees an integer type exactly the size of a pointer.
*   **Memory Rule:** Never return a pointer to a local stack variable. It is destroyed when the thread exits.

**Implementation:**
```c
#include <stdint.h>

// Inside Thread: Returning the value
void *worker(void *arg) {
    int sum = 500;
    // Cast int to pointer-sized int, then to void*
    pthread_exit((void *)(intptr_t)sum); 
}

// Inside Main: Receiving the value
void *thread_result;
pthread_join(thread, &thread_result); // Pass address of void*
// Cast void* back to intptr_t, then to int
int final_sum = (int)(intptr_t)thread_result; 
```

### 2. Passing Single Parameters
`pthread_create` accepts one `void *` argument. 

**Pass by Value (Safe for integers/chars):**
Pack the primitive directly into the pointer argument.
```c
int id = 5;
pthread_create(&thread, NULL, worker, (void *)(intptr_t)id);

// Inside Thread:
int my_id = (int)(intptr_t)arg;
```

**Pass by Reference (Pointers):**
Pass the memory address. Ensure the variable is not modified or destroyed by the main thread before the worker reads it.
```c
int *id = malloc(sizeof(int));
*id = 5;
pthread_create(&thread, NULL, worker, (void *)id);

// Inside Thread:
int my_id = *((int *)arg);
free(arg); // Clean up if dynamically allocated
```

### 3. Passing Strings (Without Struct)
A string in C is a `char *` (a pointer to characters). Cast the pointer directly to `void *`.

**Implementation:**
```c
// Inside Main:
char *msg = "Thread message"; // Or malloc'd string
pthread_create(&thread, NULL, worker, (void *)msg);

// Inside Thread:
void *worker(void *arg) {
    char *received_msg = (char *)arg;
    printf("%s\n", received_msg);
    pthread_exit(NULL);
}
```

### 4. Passing Dynamic Arrays (Without Struct)
A dynamic array is a pointer to the first element (e.g., `int *`). Pass it by casting the pointer to `void *`. 

*Constraint:* Because you are only passing the pointer, the thread must know the array size via a global macro or constant.

**Implementation:**
```c
#define SIZE 3

// Inside Main:
int *arr = malloc(SIZE * sizeof(int));
arr[0] = 10; arr[1] = 20; arr[2] = 30;
pthread_create(&thread, NULL, worker, (void *)arr);

// Inside Thread:
void *worker(void *arg) {
    int *received_arr = (int *)arg;
    for(int i = 0; i < SIZE; i++) {
        printf("%d ", received_arr[i]);
    }
    // Main thread or worker must free(received_arr)
    pthread_exit(NULL);
}
```


### 5. Passing Multiple Attributes with Struct Payload
This is the standard and safest method for passing multiple, mixed-type parameters. 

**Rules:**
1. Define a `struct` containing all necessary variables.
2. Allocate the `struct` on the heap using `malloc`.
3. Pass the pointer to the `struct`.
4. The thread casts the `void *` back to the `struct *`.
5. Free the `struct` memory after use.

**Implementation:**
```c
// 1. Define Payload
typedef struct {
    int id;
    char *name;
    int *array;
    int array_size;
} Payload;

// Inside Main:
// 2. Allocate on heap
Payload *data = malloc(sizeof(Payload));
data->id = 1;
data->name = "Worker Node";
data->array_size = 2;
data->array = malloc(2 * sizeof(int));
data->array[0] = 100;
data->array[1] = 200;

// 3. Pass pointer to thread
pthread_create(&thread, NULL, worker, (void *)data);

// Inside Thread:
void *worker(void *arg) {
    // 4. Cast back to struct pointer
    Payload *data = (Payload *)arg;
    
    printf("ID: %d, Name: %s\n", data->id, data->name);
    
    // 5. Clean up heap memory
    free(data->array);
    free(data);
    
    pthread_exit(NULL);
}
```