#pragma once
#define _WINSOCKAPI_    // stops windows.h including winsock.h

#include "ServerBackend.h"
#include "Resource.h"
#include "stdafx.h"
#include "afxsock.h"
#include <iostream>
#include <chrono>
#include <queue>
#include <atomic>
#include <mutex>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class Server
{
private:
    ServerBackend _backend;
    /*CSocket _server;
    CSocket _s;
    DWORD _threadID;
    HANDLE _threadStatus;
    int _clientID;*/

public:
    Server();
    ~Server();

    void start();
    friend DWORD WINAPI function_cal(LPVOID arg);


};
