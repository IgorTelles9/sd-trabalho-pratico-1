#include <iostream> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sstream>
#include <time.h>

int isPrime(int number){
    bool isPrime = true;

    for (int i = sqrt(number); i > 1; i--){
        if(number % i == 0){
            return 0;
        }
    }
    return 1; 
}

int main (int argc, char* argv[]) { 
    using std::cout;
    using std::cerr; 
    using std::string;

    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening < 0){
        cerr << "Can't create socket!" << "\n";
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); 

    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) < 0) {
        cerr << "Can't bind to IP/port" << "\n";
        return -2;
    }

    if (listen(listening, SOMAXCONN) < 0){
        cerr << "Can't listen" << "\n";
        return -3;
    }

    cout << "Waiting for connection on port 54000..." << "\n";

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening,(sockaddr*)&client, &clientSize);


    if (clientSocket < 0){
        cerr << "Problem with client connecting!" << "\n";
        return -4;
    }

    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    
    if (result){
        cout << host << " connected on " << svc << "\n";

    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << "\n";
    }

    int bufferSize = 20;
    char buffer[bufferSize];
    int bytesRecv;
    int numberRecv;
    bool running = true; 
    while(running){
        memset(buffer, 0, bufferSize);

        // receive number
        bytesRecv = recv(clientSocket, buffer, bufferSize, 0);
        std::istringstream(string(buffer, bytesRecv)) >> numberRecv;
        if (numberRecv == 0){
            running = false;
        } else {
            // check if number is prime
            string res = std::to_string(isPrime(numberRecv));
            // send response
            // memset(buffer, 0, bufferSize);
            send(clientSocket, res.c_str(), res.size() + 1, 0); 
            
        }

        if (bytesRecv < 0){
            cerr << "Connection issue" << "\n";
            break;
        } 
        if(bytesRecv == 0 || numberRecv == 0) {
            cout << "The client disconnected" << "\n";
            break;
        }
    }

    close(clientSocket);

    return 0;
}