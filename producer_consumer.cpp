#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

const int BUFFER_SIZE = 20; // size of the buffer
int NUM_PRODUCE = 3; // number of items to produce

int buffer[BUFFER_SIZE]; // buffer to hold items produced
int num_produced = 0; // counter for number of items produced
int in = 0; // index of next empty slot in the buffer




int main() {
    
    //sem_t mutex;
    //sem_init(&mutex, 1, 1);
    sem_t* mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    //sem_t empty;
    //sem_init(&empty, 1, BUFFER_SIZE);
    sem_t* empty = sem_open("/empty", O_CREAT, 0644, BUFFER_SIZE);

    //sem_t full;
    //sem_init(&full, 1, 0);
    sem_t* full = sem_open("/full", O_CREAT, 0644, 0);

    int fd[2];
    pid_t pid;
    char buf[BUFFER_SIZE];

    if (pipe(fd) == -1) {
        cerr << "Error creating pipe" << endl;
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        cerr << "Error forking process" << endl;
        exit(EXIT_FAILURE);

    } else if (pid > 0) {
        // parent process

        sem_t* mutex = sem_open("/mutex", 0);
        sem_t* empty = sem_open("/empty", 0);
        sem_t* full = sem_open("/full", 0);

        // close the read end of the pipe
        close(fd[0]);
        

        while (NUM_PRODUCE > 0 ) {
            sem_wait(empty);

            sem_wait(mutex);
            // write data to the pipe
            write(fd[1], "Hello, world!", 14);
            sem_post(mutex);
            sem_post(full);
            NUM_PRODUCE--;
        }

        sem_wait(mutex);
            // write data to the pipe
            write(fd[1], "0", 2);
            sem_post(mutex);

        // close the write end of the pipe
        close(fd[1]);

        exit(EXIT_SUCCESS);
    } else {
        
        sem_t* mutex = sem_open("/mutex", 0);
        sem_t* empty = sem_open("/empty", 0);
        sem_t* full = sem_open("/full", 0);
        // close the write end of the pipe
        close(fd[1]);
        
        // child process
        
         while (1){
            cout << "jfezes entrei "  << endl;
            sem_wait(full);
            cout << "jfezes entrei 2"  << endl;
            sem_wait(mutex);
            read(fd[0], buf, 14);
            sem_post(mutex);
            sem_post(empty);
            cout << "Child process received: " << buf << endl;

        }
        

        cout << "Child process received: " << buf << endl;
        // close the read end of the pipe
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
