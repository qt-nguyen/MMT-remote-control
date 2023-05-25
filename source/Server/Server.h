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

public:
    Server();
    ~Server();

    void start();
    friend DWORD WINAPI function_cal(LPVOID arg);


};
bool sendData(CSocket& mysock, size_t size, char* buffer);