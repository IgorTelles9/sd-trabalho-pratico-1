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


int main (int argc, char* argv[]) { 
    using std::cout;
    using std::cerr; 
    using std::string;

    int times;
    std::istringstream(argv[1]) >> times;

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

    char buffer[4096];
    int prev = 1; 
    int curr; 
    srand (time(NULL));
    for(int i =0; i < times + 1; i++){
        memset(buffer, 0, 4096);

        int delta = rand() % 100 + 1; 
        if (i == times){
            curr = 0;
        } else {
            curr = prev + delta;
        }

        string currStr = std::to_string(curr);
        send(clientSocket, currStr.c_str(), currStr.size() + 1, 0); 

        prev = curr;
        

        int bytesRecv = recv(clientSocket, buffer, 4096, 0);
        
        if (bytesRecv < 0){
            cerr << "Connection issue" << "\n";
            break;
        } 
        if(bytesRecv == 0) {
            cout << "The client disconnected" << "\n";
            break;
        }

        cout << "Received: " << string(buffer, 0 ,bytesRecv) << "\n";
        
        //send(clientSocket, buffer, bytesRecv + 1, 0);

    }

    close(clientSocket);

    return 0;
}