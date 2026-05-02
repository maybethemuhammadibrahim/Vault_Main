#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // For O_CREAT, O_RDWR
#include <sys/shm.h>    // shm functions
#include <sys/stat.h>   // mode constants
#include <sys/mman.h>   // mmap
#include <unistd.h>     // ftruncate
#include <sys/types.h>

int main() {
    const int SIZE = 4096;             // size of shared memory object
    const char *name = "OS";           // name of the shared memory object

    const char *message0 = "Welcome ";   // first string
    const char *message1 = "World!";   // second string

    int fd;        // shared memory file descriptor
    char *ptr;     // pointer to shared memory object

    // 1. Create shared memory object
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // 2. Configure the size of the shared memory object
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // 3. Memory map the shared memory object
    ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // 4. Write to shared memory
    sprintf(ptr, "%s", message0);
    ptr += strlen(message0);           // move pointer forward
    sprintf(ptr, "%s", message1);
    ptr += strlen(message1);/// end of the msg

    printf("Data written to shared memory: Hello World!\n");

    // 5. Close the shared memory object
    close(fd);

    return 0;
}


//Consumer code
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const int SIZE = 4096;
    const char *name = "OS";
    int fd = shm_open(name, O_RDONLY, 0666);

    char *ptr = (char *) mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    printf("Data read from shared memory: %s\n", ptr);

    close(fd);
    return 0;
}