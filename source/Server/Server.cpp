#include "Server.h"


bool sendData(CSocket& mysock, size_t size, char* buffer)
{
    int bytesSent = 0;
    while (bytesSent < size)
    {
        int remainingSize = size - bytesSent;
        int sendSize = remainingSize > 1024 ? 1024 : remainingSize; // Gửi tối đa 1024 byte mỗi lần

        int result = mysock.Send(buffer + bytesSent, sendSize, 0);
        if (result == SOCKET_ERROR)
        {
            std::cerr << "Error sending data\n";
            return false;
        }

        bytesSent += result;
    }

    return true;
}

Server::Server()
{
}

Server::~Server()
{
 
}

void Server::start()
{ 
    HMODULE hModule = ::GetModuleHandle(NULL);

    if (hModule != NULL)
    {
        if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
        {
            _tprintf(_T("Fatal Error: MFC initialization failed\n"));

        }
        else
        {
            AfxSocketInit(NULL);
            CSocket server, s;
            DWORD threadID;
            HANDLE threadStatus;

            std::map<CString, int> nextClientID;
            server.Create(4567);

            do {
                printf("Server lang nghe ket noi tu client\n");
                server.Listen();
                server.Accept(s);

                CString ipAddress;
                UINT port;
                s.GetPeerName(ipAddress, port);

                int clientID = nextClientID[ipAddress]++;
                wprintf(L"Client %d connected from IP address %s\n", clientID, (LPCTSTR)ipAddress);

                auto args = new std::pair<int, SOCKET>(clientID, s.Detach());
                threadStatus = CreateThread(NULL, 0, function_cal, args, 0, &threadID);

            } while (1);
        }
    }
    else
    {
        _tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
    }


}

DWORD WINAPI function_cal(LPVOID arg)
{
    auto args = (std::pair<int, SOCKET>*)arg;
    int clientID = args->first;
    SOCKET* hConnected = &args->second;
    CSocket server;
    server.Attach(*hConnected);

    CString ipAddress;
    UINT port;
    server.GetPeerName(ipAddress, port);
    ServerBackend backend;
    int number_continue = 0, check = 0;
    const std::chrono::seconds timeout(5);
    server.SetSockOpt(SO_RCVTIMEO, &timeout, sizeof(timeout), SOL_SOCKET);
    do
    {
        fflush(stdin);
        size_t clientSize;
        check = server.Receive(&clientSize, sizeof(clientSize), 0);

        if (check == SOCKET_ERROR)
        {
            int errCode = GetLastError();
            if (errCode == WSAETIMEDOUT)
            {
                wprintf(L"Timeout occurred while waiting for data from client %d\n", clientID);
                break;
            }
        }

        char* bufferClient = new char[clientSize];
        server.Receive(bufferClient, clientSize, 0);

        DataObj clientData(DataObj::deserialize(bufferClient, clientSize));
        delete[] bufferClient;

        std::cout << clientData.toJsonString() << "\n";

        if (clientData.getFuncType() == KLG || clientData.getFuncType() == SCR)
        {
            do
            {
                std::shared_ptr<DataObj> tmp = backend.handleClientRequest(clientData);

                DataObj serverData(tmp->getID(), tmp->getDataType(), tmp->getFuncType(), tmp->getCmdType(), tmp->getData());

                size_t serverSize;
                char* bufferServer = serverData.serialize(serverSize);
                std::cout << serverData.getData().size() << "\n";
                server.Send(&serverSize, sizeof(serverSize), 0);
                sendData(server, serverSize, bufferServer);

                delete[] bufferServer;

                server.Receive(&clientSize, sizeof(clientSize), 0);
                char* bufferClient = new char[clientSize];
                server.Receive(bufferClient, clientSize, 0);

                DataObj clientData(DataObj::deserialize(bufferClient, clientSize));

                delete[] bufferClient;

                if (clientData.getCmdType() == STOP)
                {
                    std::shared_ptr<DataObj> tmp1 = backend.handleClientRequest(clientData);

                    DataObj serverData(tmp1->getID(), tmp1->getDataType(), tmp1->getFuncType(), tmp1->getCmdType(), tmp1->getData());

                    size_t serverSize;
                    char* bufferServer = serverData.serialize(serverSize);

                    std::cout << serverData.toJsonString() << "\n";
                    std::cout << serverData.getData_String() << "\n";

                    server.Send(&serverSize, sizeof(serverSize), 0);
                    sendData(server, serverSize, bufferServer);

                    delete[] bufferServer;
                    break;
                }
                if (clientData.getFuncType() == SCR) Sleep(1000);

            } while (true);
        }

        else
        {
            std::shared_ptr<DataObj> tmp = backend.handleClientRequest(clientData);

            DataObj serverData(tmp->getID(), tmp->getDataType(), tmp->getFuncType(), tmp->getCmdType(), tmp->getData());

            size_t serverSize;
            char* bufferServer = serverData.serialize(serverSize);

            std::cout << serverData.toJsonString() << "\n";
            std::cout << serverData.getData_String() << "\n";

            server.Send(&serverSize, sizeof(serverSize), 0);
            sendData(server, serverSize, bufferServer);

            delete[] bufferServer;

            
        }
        check = server.Receive(&number_continue, sizeof(number_continue), 0);

        if (check == SOCKET_ERROR || number_continue == 0)
        {
            int errCode = GetLastError();
            if (errCode == WSAETIMEDOUT)
            {
                wprintf(L"Timeout occurred while waiting for data from client %d\n", clientID);
                server.Close();
                break;
            }
        }
    } while (number_continue == 1);
    delete args;
    return 0;
}

