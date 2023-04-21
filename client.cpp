#include <iostream> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>
#include <math.h>

std::string isPrime(int number){
    bool isPrime = true;

    for (int i = sqrt(number); i > 1; i--){
        if(number % i == 0){
            isPrime = false;
        }
    }

    std::ostringstream resp;
    if(isPrime){
        resp << number << " is a prime number!";
    } else {
        resp << number << " is not a prime number!";
    }

    return resp.str();
}


int main(){
    using std::string;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        return 1;
    }

    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes < 0){
        return 2;
    } 

    char buffer[4096];
    int numberRecv;
    bool running = true;

    while(running) {
        memset(buffer, 0, 256);
        int bytesRecv = recv(sock, buffer, 256, 0);

        std::istringstream(string(buffer, bytesRecv)) >> numberRecv;

        if (numberRecv == 0){
           running = false; 
        } else { 
            string primeRes = isPrime(numberRecv);
            int sendRes = send(sock, primeRes.c_str(), primeRes.size() + 1, 0);
        }
    };

    close(sock);

    return 0;
}