#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <math.h>
#include <random>

using namespace std;

const int BUFFER_SIZE = 2000; // size of the buffer


int buffer[BUFFER_SIZE]; // buffer to hold items produced
int num_produced = 0; // counter for number of items produced
int in = 0; // index of next empty slot in the buffer


bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    int limit = sqrt(n);
    for (int i = 2; i <= limit; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {


    using std::istringstream;
    int NUM_PRODUCE; // number of items to produce
    istringstream(argv[1]) >> NUM_PRODUCE; 
    
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1, 100); // define the range

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

        // close the read end of the pipe
        close(fd[0]);
        
        int ni = 1;
        while (NUM_PRODUCE > 0) {
            ni += distr(gen);
            int bytes_written = write(fd[1], &ni, sizeof(ni));
            if (bytes_written == -1) {
                cerr << "Error: Unable to write to pipe" << endl;
                return 1;
            }
            NUM_PRODUCE--;
            cout << "Producer sent number: " << ni << endl;
        }

            int zero = 0;
            // write data to the pipe
            write(fd[1], &zero, sizeof(zero));
            

        // close the write end of the pipe
        close(fd[1]);

    } else {
        
        
        // close the write end of the pipe
        close(fd[1]);
        
        // child process
        
         // Read integers from the pipe and print them
        int number;
        int bytes_read;
        while ((bytes_read = read(fd[0], &number, sizeof(number))) > 0) {
            cout << "Consumer received number: " << number << endl;
            if (isPrime(number)){
                cout << "The number "<< number  <<" is Prime!" << endl;
            }
            else{
                cout << "The number " << number <<" is NOT Prime!" << endl;
            }
        }

        if (bytes_read == -1) {
            cerr << "Error: Unable to read from pipe" << endl;
            return 1;
        }

        cout << "Okay its over" << endl;
        // close the read end of the pipe
        close(fd[0]);
        return 0;

    }
        
    return 0;
}
