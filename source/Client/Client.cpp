#include "Client.h"


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

void Client::getCilentData(DataType dataType, FuncType funcType, CmdType cmdType, std::string data)
{
}

Client::Client()
{
    

}

Client::~Client()
{
    //_client.Close();
}

void Client::start()
{

    const int MAX_ATTEMPTS = 10;
    const int DELAY = 1000;

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
            CSocket client;
            client.Create();
            bool connected = false;

            for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt)
            {
                if (client.Connect(_T("127.0.0.1"), 4567))
                {
                    connected = true;
                    break;
                }

                Sleep(DELAY);
            }

            if (!connected)
            {
                std::cerr << "Error: Failed to connect to the server\n";
            }


            int number_continue = 0;

            fflush(stdin);

            size_t clientSize;
            char* bufferClient = _clientData.serialize(clientSize);
            std::cout << _clientData.toJsonString() << "\n";

            client.Send(&clientSize, sizeof(clientSize), 0);
            client.Send(bufferClient, clientSize, 0);
            if (_clientData.getFuncType() == KLG)
            {
                std::cout << "Press any key to stop: ";

                    
                do {
                    size_t serverSize;
                    client.Receive(&serverSize, sizeof(serverSize), 0);

                    char* bufferServer = new char[serverSize];
                    client.Receive(bufferServer, serverSize, 0);

                    DataObj serverData(DataObj::deserialize(bufferServer, serverSize));
                    delete[]bufferServer;
                    printf("Du lieu tra ve tu Server: ");
                    std::cout << serverData.getData_String() << "\n";
                    if (_kbhit()) break;

                    client.Send(&clientSize, sizeof(clientSize), 0);
                    client.Send(bufferClient, clientSize, 0);
                        
                        
                } while (true);
                _clientData.setCmdType(STOP);
                bufferClient = _clientData.serialize(clientSize);
                std::cout << _clientData.toJsonString() << "\n";

                client.Send(&clientSize, sizeof(clientSize), 0);
                client.Send(bufferClient, clientSize, 0);
            }
            else if (_clientData.getFuncType() == SCR)
            {
                std::cout << "Press any key to stop...\n";

                do {
                    size_t serverSize;
                    client.Receive(&serverSize, sizeof(serverSize), 0);

                    char* bufferServer = new char[serverSize];
                    client.Receive(bufferServer, serverSize, 0);

                    DataObj serverData(DataObj::deserialize(bufferServer, serverSize));
                    delete[]bufferServer;

                    std::cout << serverData.getData().size() << "\n";
                    std::cout << serverData.dataToFile("capture/" + utils::CurrentTime() + ".png") << "\n";
                    if (_kbhit()) break;

                    client.Send(&clientSize, sizeof(clientSize), 0);
                    client.Send(bufferClient, clientSize, 0);


                } while (true);
                _clientData.setCmdType(STOP);
                bufferClient = _clientData.serialize(clientSize);
                std::cout << _clientData.toJsonString() << "\n";

                client.Send(&clientSize, sizeof(clientSize), 0);
                client.Send(bufferClient, clientSize, 0);
            }

            size_t serverSize;
            client.Receive(&serverSize, sizeof(serverSize), 0);

            char* bufferServer = new char[serverSize];
            client.Receive(bufferServer, serverSize, 0);

            DataObj serverData(DataObj::deserialize(bufferServer, serverSize));

            printf("Du lieu tra ve tu Server:\n");

            std::cout << serverData.toJsonString() << "\n";  
            std::cout << serverData.getData_String() << "\n";
            delete[]bufferServer;
            delete[]bufferClient;
        }
    }
    else
    {
        _tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
    }
}
