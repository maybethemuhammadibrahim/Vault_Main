#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void) {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;


    int result = pipe(fd); // create a pipe
    if (result == -1) { // create a pipe
        perror("Pipe failed");
        return 1;
    }

    pid = fork();
    if (pid < 0) { /* error occurred */
        perror("Fork Failed");
        return 1;
    }
    if (pid > 0) { /* parent process */
        close(fd[READ_END]);
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
        close(fd[WRITE_END]);
    }
    else { /* child process */
        close(fd[WRITE_END]); // close the unused end of the pipe
        read(fd[READ_END], read_msg, BUFFER_SIZE); // read from the pipe
        fprintf(stderr, "read %s", read_msg);
        close(fd[READ_END]); // close the read end of the pipe
    }

    return 0;
}