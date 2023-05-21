#pragma once
#include "ServerBackend.h"
#include "Resource.h"

#include <iostream>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")


class Server
{
private:
    ServerBackend _backend;
    CSocket _server;
    CSocket _s;
    DWORD _threadID;
    HANDLE _threadStatus;
    int _clientID;

public:
    Server();
    ~Server();

    void start();
    friend DWORD WINAPI function_cal(LPVOID arg);


};
