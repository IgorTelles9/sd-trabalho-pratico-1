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

int main(int argc, char* argv[])
{
    using std::string;
    using std::cout;

    int times;
    std::istringstream(argv[1]) >> times;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return 1;
    }

    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if (connectRes < 0)
    {
        return 2;
    }

    int bufferSize = 20;
    char buffer[bufferSize];
    int prev = 1; 
    int curr; 
    int isPrime;  // 1 if prime
    srand(time(NULL));
    for (int i = 0; i < times + 1; i++){
        memset(buffer, 0, bufferSize);

        // generates random number
        int delta = rand() % 100 + 1;
        if (i == times){
            curr = 0;
        } else {
            curr = prev + delta;
        }

        // send random number
        string currStr = std::to_string(curr); 
        send(sock, currStr.c_str(), currStr.size() + 1, 0); 
        if (curr == 0){
            break;
        }

        // receive response
        int bytesRecv = recv(sock, buffer, bufferSize, 0);
        std::istringstream(string(buffer, bytesRecv)) >> isPrime;

        if(isPrime){
            cout << "The number " << curr << " is prime" << "\n";
        } else { 
            cout << "The number " << curr << " is not prime" << "\n";
        }
        prev = curr;
    };

    close(sock);

    return 0;
}