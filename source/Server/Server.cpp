#include "Server.h"



Server::Server()
{
    HMODULE hModule = ::GetModuleHandle(NULL);
    if (hModule != NULL)
    {
        if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        }
        else
        {
            AfxSocketInit(NULL);
            _clientID = 0;
            _server.Create(4567);
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        _tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
    }

}

Server::~Server()
{
    _server.Close();
}

void Server::start()
{
    do {
        printf("Server lang nghe ket noi tu client\n");
        _server.Listen();
        _server.Accept(_s);

        ++_clientID;
        // Get the IP address and port number of the connected client
        CString ipAddress;
        UINT port;
        _s.GetPeerName(ipAddress, port);
        wprintf(L"Client %d connected from IP address %s\n", _clientID, (LPCTSTR)ipAddress);

        //Chuyển đỏi CSocket thanh Socket
        auto args = new std::pair<int, SOCKET>(_clientID, _s.Detach());
        //Khoi tao thread tuong ung voi moi client Connect vao server.

        //Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
        _threadStatus = CreateThread(NULL, 0, function_cal, args, 0, &_threadID);

    } while (1);

}

DWORD WINAPI function_cal(LPVOID arg)
{

    auto args = (std::pair<int, SOCKET>*)arg;
    int clientID = args->first;
    SOCKET* hConnected = &args->second;
    CSocket mysock;
    mysock.Attach(*hConnected);

    // Get the IP address and port number of the connected client
    CString ipAddress;
    UINT port;
    mysock.GetPeerName(ipAddress, port);

    int number_continue = 0;
    ServerBackend backend;

    do {
        std::shared_ptr<DataObj> data;

        mysock.Receive(&data, sizeof(data), 0);
        
        backend.handleClientRequest(data);

        //Gui ket qua tinh toan cho client
        mysock.Send(&data, sizeof(data), 0);

        if (data->getDataType() == DATA_TYPE)
        {
            wprintf(L"Client %d from %s disconnected from server\n", clientID, (LPCTSTR)ipAddress);
            mysock.Close();
            break;
        }

    } while (number_continue);
    delete args;
    return 0;
    //return 0;
}


