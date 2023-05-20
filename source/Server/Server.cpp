#include "Server.h"
#include "stdafx.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
    /*
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Khong the khoi tao Winsock." << std::endl;
        exit(1);
    }

    // Tạo socket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Khong the tao socket." << std::endl;
        WSACleanup();
        exit(1);
    }

    // Cấu hình thông tin địa chỉ server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Port để lắng nghe kết nối
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Ràng buộc socket với địa chỉ server
    if (bind(_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Loi khi rong buoc socket." << std::endl;
        closesocket(_serverSocket);
        WSACleanup();
        exit(1);
    }

    // Lắng nghe kết nối từ client
    if (listen(_serverSocket, 1) == SOCKET_ERROR)
    {
        std::cerr << "Loi khi lang nghe ket noi." << std::endl;
        closesocket(_serverSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "Dang cho ket noi tu client..." << std::endl;
    */
}

Server::~Server()
{
    _server.close();
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

    /*
    while (true)
    {
        // Chấp nhận kết nối từ client
        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Loi khi chap nhan ket noi." << std::endl;
            closesocket(_serverSocket);
            WSACleanup();
            exit(1);
        }

        std::cout << "Da ket noi voi client!" << std::endl;

        // Nhận dữ liệu từ client
        std::string request = receiveData(clientSocket);

        // Xử lý yêu cầu từ client bằng cách gọi phương thức handleClientRequest() trong ServerBackend
        std::string response = _backend.createResponseData(_backend.handleClientRequest(request));

        // Gửi phản hồi từ server tới client
        sendData(clientSocket, response);

        // Đóng kết nối với client
        closesocket(clientSocket);
    }
    */
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
        DataObj data;

        mysock.Receive(&data, sizeof(data), 0);
        
        backend.handleClientRequest(data, number_continue);

        //Gui ket qua tinh toan cho client
        mysock.Send(&data, sizeof(data), 0);

        

        if (number_continue == 0)
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


