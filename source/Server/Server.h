#include "ServerBackend.h"

#include <iostream>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")


class Server
{
private:
    ServerBackend _backend;
    SOCKET _serverSocket;

public:
    Server();
    ~Server();

    void start();
    

private:
    std::string receiveData(SOCKET clientSocket);

    void sendData(SOCKET clientSocket, std::string response);


};
