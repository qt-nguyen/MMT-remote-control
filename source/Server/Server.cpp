#include "Server.h"

Server::Server()
{
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
}

Server::~Server()
{
    // Đóng kết nối và giải phóng Winsock
    closesocket(_serverSocket);
    WSACleanup();
}

void Server::start()
{
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
}

std::string Server::receiveData(SOCKET clientSocket)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Nhận dữ liệu từ client
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR)
    {
        std::cerr << "Loi khi nhan du lieu tu client." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    std::string data(buffer);
    return data;
}

void Server::sendData(SOCKET clientSocket, std::string response)
{

    // Gửi dữ liệu từ server tới client
    int bytesSent = send(clientSocket, response.c_str(), response.length(), 0);
    if (bytesSent == SOCKET_ERROR)
    {
        std::cerr << "Loi khi gui du lieu tu server toi client." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
}


