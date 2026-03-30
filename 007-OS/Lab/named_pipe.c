#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define FIFO_FILE "/tmp/myfifo"
#define BUFSIZ 1024

int main() {
    int fd;
    char buffer[BUFSIZ];
    size_t num_bytes;

    mkfifo(FIFO_FILE, 0666); // Create the named pipe (FIFO)

    fd = open(FIFO_FILE, O_WRONLY); // Open the named pipe for writing (producer)
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) { // Producer loop
        printf("Producer: Enter a message (or 'exit' to quit): ");
        fgets(buffer, BUFSIZ, stdin);
        num_bytes = write(fd, buffer, strlen(buffer)); // Write input to the named pipe
        if (num_bytes == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if (strncmp(buffer, "exit", 4) == 0) { // Check for exit condition
            break;  
        }
    }

    close(fd); // Close the named pipe
    unlink(FIFO_FILE); // Remove the named pipe from the file system

    return 0;
}



//producer 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "my_fifo" // Use the same name!
#define BSIZE 1024

int main() {
    mkfifo(FIFO_PATH, 0666);
    
    char message[BSIZE] = "Greetings from Producer";
    printf("Producer: Waiting for Consumer...\n");
    
    int fd = open(FIFO_PATH, O_WRONLY); 
    if(fd < 0) { perror("Open Error"); return 1; }
    
    write(fd, message, sizeof(message));
    printf("Producer: Message sent.\n");

    close(fd);
    // Do not unlink immediately if you want the consumer to find it, 
    // or let the consumer unlink it.
    return 0;
}

//receiver/consumer
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "my_fifo" // Must match Producer
#define BSIZE 1024

int main() {
    printf("Consumer: Waiting for Producer...\n");
    
    int fd = open(FIFO_PATH, O_RDONLY);
    if(fd < 0) { perror("Open Error"); return 1; }
    
    char buffer[BSIZE];
    read(fd, buffer, sizeof(buffer));
    printf("Consumer received: %s\n", buffer);
    
    close(fd);
    unlink(FIFO_PATH); // Clean up the file
    return 0;
}