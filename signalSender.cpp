#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdexcept>

int main(int argc, char* argv[]){
    using std::istringstream;
    pid_t pid; 
    int code; 
    istringstream(argv[1]) >> pid; 
    istringstream(argv[2]) >> code; 
    
    if (kill(pid,0) == 0){
        kill(pid, code);
    } else {
        std::cerr << "ERROR: pid not found" << std::endl;
    }
    return 0;
    
}