#include "Client.h"

bool receiveData(CSocket& mysock, size_t& size, char* buffer)
{
    int bytesReceived = 0;
    while (bytesReceived < size)
    {
        int remainingSize = size - bytesReceived;
        int receiveSize = remainingSize > 1024 ? 1024 : remainingSize; // Nhận tối đa 1024 byte mỗi lần

        int result = mysock.Receive(buffer + bytesReceived, receiveSize, 0);
        if (result == SOCKET_ERROR || result == 0)
        {
            std::cerr << "Error receiving data\n";
            return false;
        }

        bytesReceived += result;
    }

    return true;
}
void Client::getClientData(TCHAR* argv[5])
{
    std::string ID = utils::CurrentTime();
    DataType dataType;
    FuncType funcType;
    CmdType cmdType;
    std::string dataType_Str = utils::tcharToString(argv[1]);
    std::string funcType_Str = utils::tcharToString(argv[2]);
    std::string cmdType_Str = utils::tcharToString(argv[3]);
    std::string data_Str = utils::tcharToString(argv[4]);

    if (dataType_Str == "RESPONSE") dataType = RESPONSE;
    else if (dataType_Str == "REQUEST") dataType = REQUEST;
    else dataType = DATA_TYPE;

    if (funcType_Str == "IAP") funcType = IAP;
    else if (funcType_Str == "RPC") funcType = RPC;
    else if (funcType_Str == "SCR") funcType = SCR;
    else if (funcType_Str == "KLG") funcType = KLG;
    else if (funcType_Str == "DIR") funcType = DIR;
    else funcType = FUNC_TYPE;

    if (cmdType_Str == "RUN") cmdType = RUN;
    else if (cmdType_Str == "KILL") cmdType = KILL;
    else if (cmdType_Str == "START") cmdType = START;
    else if (cmdType_Str == "STOP") cmdType = STOP;
    else if (cmdType_Str == "SHOW") cmdType = SHOW;
    else if (cmdType_Str == "DATA") cmdType = DATA;
    else cmdType = CMD_TYPE;

    _clientData.setID(ID);
    _clientData.setDataType(dataType);
    _clientData.setFuncType(funcType);
    _clientData.setCmdType(cmdType);
    _clientData.setData(data_Str);
}

void Client::getClientData()
{
    int choice;
    std::string ID = utils::CurrentTime();
    DataType dataType = REQUEST;
    FuncType funcType = FUNC_TYPE;
    CmdType cmdType = CMD_TYPE;
    std::string data = "";
    do
    {
        std::cout << "Please select the funtion you want to run : \n"
            " 0 : Exit \n"
            " 1 : IAP  --  Installed Application \n"
            " 2 : RPC  --  Running Processes \n"
            " 3 : SCR  --  Screen Capturing \n"
            " 4 : KLG  --  Keylogger \n"
            " 5 : DIR  --  Directory Tree \n";
        do
        {
            std::cout << "\n --> YOUR CHOICE : ";
            std::cin >> choice;
            if (choice > 5 || choice < 0) std::cout << "\nWRONG FUNCTION! PLEASE INPUT AGAIN.\n";
        } while (choice > 5 || choice < 0);

        std::cout << "\nWhat does this function do ? Please select a command...\n\n 0 : Go Back \n";
        if (choice == 1) {
            funcType = IAP;
            std::cout << " 1 : SHOW  --> Show list of installed apps \n"
                " 2 : START --> Start an app \n"
                " 3 : STOP  --> Close an app (if opened) \n";

            do
            {
                std::cout << "\n --> YOUR CHOICE : ";
                std::cin >> choice;
                if (choice > 3 || choice < 0) std::cout << "\nWRONG COMMAND! PLEASE INPUT AGAIN.\n";
            } while (choice > 3 || choice < 0);
            if (choice == 0) {
                funcType = FUNC_TYPE;
            }
            else
            {
                if (choice == 1)
                {
                    cmdType = SHOW;
                }
                else
                {
                    if (choice == 2)
                    {
                        cmdType = START;
                    }
                    else if (choice == 3)
                    {
                        cmdType = STOP;
                    }
                    std::cout << " Input the name of the app : ";
                    std::cin >> data;
                }
            }
        }
        else if (choice == 2)
        {
            funcType = RPC;
            std::cout << " 1 : SHOW  --> Show list of running processes \n"
                " 2 : RUN --> Run a process \n"
                " 3 : KILL  --> Stop a process (if run) \n";

            do
            {
                std::cout << "\n --> YOUR CHOICE : ";
                std::cin >> choice;
                if (choice > 3 || choice < 0) std::cout << "\nWRONG COMMAND! PLEASE INPUT AGAIN.\n";
            } while (choice > 3 || choice < 0);

            if (choice == 0) {
                funcType = FUNC_TYPE;
            }
            else
            {
                if (choice == 1)
                {
                    cmdType = SHOW;
                }
                else
                {
                    if (choice == 2)
                    {
                        cmdType = RUN;
                    }
                    else if (choice == 3)
                    {
                        cmdType = KILL;
                    }
                    std::cout << " Input the name of the process : ";
                    std::cin >> data;
                }
            }
        }
        else if (choice == 3)
        {
            funcType = SCR;
            std::cout << " 1 : START  --> Start live screen capturing session \n"
                " 2 : STOP  --> Stop live session \n";

            do
            {
                std::cout << "\n --> YOUR CHOICE : ";
                std::cin >> choice;
                if (choice > 2 || choice < 0) std::cout << "\nWRONG COMMAND! PLEASE INPUT AGAIN.\n";
            } while (choice > 2 || choice < 0);

            if (choice == 0) {
                funcType = FUNC_TYPE;
            }
            else
            {
                if (choice == 1)
                {
                    cmdType = START;
                }
                else
                {
                    cmdType = STOP;
                }
            }
        }
        else if (choice == 4)
        {
            funcType = KLG;
            std::cout << " 1 : START  --> Start capturing keys \n"
                " 2 : STOP  --> Stop capturing keys \n";

            do
            {
                std::cout << "\n --> YOUR CHOICE : ";
                std::cin >> choice;
                if (choice > 2 || choice < 1) std::cout << "\nWRONG COMMAND! PLEASE INPUT AGAIN.\n";
            } while (choice > 2 || choice < 1);

            if (choice == 0) {
                funcType = FUNC_TYPE;
            }
            else
            {
                if (choice == 1)
                {
                    cmdType = START;
                }
                else
                {
                    cmdType = STOP;
                }
            }
        }
        else if (choice == 5)
        {
            std::string max_depth = "-1";
            std::string include_file = "0";
            std::string path = "";

            funcType = DIR;
            std::cout << " 1 : SHOW  --> Show directory tree with the following parameters\n"
                " MAX_DEPTH : the maximum depth of the directory tree, if user's input is negative, default = -1 -> show in whole\n"
                " INCLUDE_FILE : specifies whether to include files or not ( 1 = TRUE, 0 = FALSE)\n"
                " PATH : path to the root of the directory, if no input recieved -> default to current directory\n";
            
            if (choice == 0) {
                funcType = FUNC_TYPE;
            }
            else
            {
                cmdType = SHOW;
                std::cout << " MAX_DEPTH = "; std::cin >> max_depth;
                std::cout << "INCLUDE_FILE = "; std::cin >> include_file;
                std::cout << "PATH = "; std::cin >> path;

                data = max_depth + " " + include_file + " " + path;
            }
        }
        else funcType = FUNC_TYPE;
    }while (choice == 0);
    _clientData.setID(ID);
    _clientData.setDataType(dataType);
    _clientData.setFuncType(funcType);
    _clientData.setCmdType(cmdType);
    _clientData.setData(data);
}

Client::Client()
{
    std::cout << "Input IP Address: ";
    std::cin >> _IP;
    std::cout << "Connecting to " << _IP << "...\n";
}

Client::~Client()
{
    _client.Close();
}

bool Client::start()
{

    const int MAX_ATTEMPTS = 5;
    const int DELAY = 500;
    std::wstring wideIP = utils::s2ws(_IP);
    const wchar_t* IP = wideIP.c_str();
    HMODULE hModule = ::GetModuleHandle(NULL);

    if (hModule != NULL)
    {
        if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
        {
            _tprintf(_T("Fatal Error: MFC initialization failed\n"));
            return false;
        }
        else
        {
            AfxSocketInit(NULL);
            
            _client.Create();

            for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt)
            {
                if (_client.Connect(IP, 4567))
                {
                    return true;
                    break;
                }

                Sleep(DELAY);
            }

            std::cerr << "Error: Failed to connect to the server\n";
            return false;
        
        }
    }
    else
    {
        _tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
        return false;
    }
    
}

void Client::process()
{
    int number_continue = 0;
    do
    {
        getClientData();

        fflush(stdin);

        size_t clientSize;
        char* bufferClient = _clientData.serialize(clientSize);
        std::cout << _clientData.toJsonString() << "\n";

        _client.Send(&clientSize, sizeof(clientSize), 0);
        _client.Send(bufferClient, clientSize, 0);
        if (_clientData.getFuncType() == KLG)
        {
            std::cout << "Press any key to stop: ";


            do {
                size_t serverSize;
                _client.Receive(&serverSize, sizeof(serverSize), 0);

                char* bufferServer = new char[serverSize];
                receiveData(_client, serverSize, bufferServer);

                DataObj serverData(DataObj::deserialize(bufferServer, serverSize));
                delete[]bufferServer;
                printf("Du lieu tra ve tu Server: ");
                std::cout << serverData.getData_String() << "\n";
                if (_kbhit()) break;

                _client.Send(&clientSize, sizeof(clientSize), 0);
                _client.Send(bufferClient, clientSize, 0);
                

            } while (true);
            _clientData.setCmdType(STOP);
            bufferClient = _clientData.serialize(clientSize);
            std::cout << _clientData.toJsonString() << "\n";

            _client.Send(&clientSize, sizeof(clientSize), 0);
            _client.Send(bufferClient, clientSize, 0);
        }
        else if (_clientData.getFuncType() == SCR)
        {
            std::cout << "Press any key to stop...\n";

            do {
                size_t serverSize;
                _client.Receive(&serverSize, sizeof(serverSize), 0);

                char* bufferServer = new char[serverSize];
                receiveData(_client, serverSize, bufferServer);

                DataObj serverData(DataObj::deserialize(bufferServer, serverSize));
                delete[]bufferServer;

                std::cout << serverData.getData().size() << "\n";
                std::cout << serverData.dataToFile("capture/" + utils::CurrentTime() + ".png") << "\n";
                if (_kbhit()) break;

                _client.Send(&clientSize, sizeof(clientSize), 0);
                _client.Send(bufferClient, clientSize, 0);


            } while (true);
            _clientData.setCmdType(STOP);
            bufferClient = _clientData.serialize(clientSize);
            std::cout << _clientData.toJsonString() << "\n";

            _client.Send(&clientSize, sizeof(clientSize), 0);
            _client.Send(bufferClient, clientSize, 0);
        }

        size_t serverSize;
        _client.Receive(&serverSize, sizeof(serverSize), 0);

        char* bufferServer = new char[serverSize];
        receiveData(_client, serverSize, bufferServer);

        DataObj serverData(DataObj::deserialize(bufferServer, serverSize));

        printf("Du lieu tra ve tu Server:\n");

        std::cout << serverData.toJsonString() << "\n";
        std::cout << serverData.getData_String() << "\n";
        delete[]bufferServer;
        delete[]bufferClient;

        std::cout << " INPUT 1 to continue, 0 to exit...\n";
        std::cin >> number_continue;
        _client.Send(&number_continue, sizeof(number_continue), 0);
    } while (number_continue == 1);
}