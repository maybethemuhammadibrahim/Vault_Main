#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      
#include <unistd.h>     
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>   
#include <sys/stat.h>  
#include <errno.h>  

int main() {
    char *name = "OS";
    int sizeArr;
    printf("Enter number of entries: ");
    scanf("%d", &sizeArr);

    int *arr = malloc(sizeArr * sizeof(int));
    for(int i = 0; i < sizeArr; i++) {
        printf("%d: ", i+1);
        scanf("%d", &arr[i]);
    }

    int SIZE = sizeArr * sizeof(int);

    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd < 0) { perror("shm"); return 1; }

    if(ftruncate(fd, SIZE) < 0) { perror("ftruncate"); return 1; }

    int *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) { perror("mmap"); return 1; }

    // write array to shared memory
    for(int i = 0; i < sizeArr; i++) {
        ptr[i] = arr[i];
    }

    pid_t pid = fork();
    if(pid < 0) { perror("fork"); return 1; }
    else if(pid == 0) {
        // child reads shared memory
        int fd_c = shm_open(name, O_RDONLY, 0666);
        if(fd_c < 0) { perror("shm"); return 1; }

        int *child_ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd_c, 0);
        if(child_ptr == MAP_FAILED) { perror("mmap"); return 1; }

        printf("Child read: ");
        for(int i = 0; i < sizeArr; i++) {
            printf("%d ", child_ptr[i]);
        }
        printf("\n");

        close(fd_c);
        munmap(child_ptr, SIZE);
    }
    else {
        // parent can continue or wait for child
        wait(NULL); // optional
    }

    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(name); // clean up
    free(arr);

    return 0;
}