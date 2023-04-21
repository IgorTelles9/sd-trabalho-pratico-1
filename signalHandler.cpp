#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string.h>
// #include <stdlib.h>
#include <sstream>
#include <stdexcept>


void handlerSignal(int num){
using std::cout;
    switch (num){
        case 2:
            cout << "Signal SIGINT received!" << "\n";
            break;
        case 3: 
            cout << "Signal SIGQUIT received!" << "\n";
            break;
        case 4:
            cout << "Signal SIGILL received!" << "\n";
            cout << "Closing process..." << "\n";
            exit(0);
        default: 
            cout << "testing default" << "\n";
            break;

    } 
}

int main(int argc, char* argv[]){
    using std::cout;
    cout << "pid: " << getpid() << "\n";
    bool isBusy = false;
    if (strcmp(argv[1], "busy") == 0){
        isBusy = true;
    }

    while (isBusy){
        signal(SIGINT, handlerSignal);
        signal(SIGQUIT, handlerSignal);
        signal(SIGILL, handlerSignal);
    }

    while (!isBusy){
        signal(SIGINT, handlerSignal);
        signal(SIGQUIT, handlerSignal);
        signal(SIGILL, handlerSignal);
        pause();
    }

    // signal(SIGINT, handlerSignal);
    // signal(SIGQUIT, handlerSignal);
    // signal(SIGILL, handlerSignal);

    return 0;
    
}