#include "../DataObj/DataObj.h"
#include "../utils.h"
#include "stdafx.h"
#include "afxsock.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class Client
{
private:
	DataObj _clientData;
	CSocket _client;

public:
	void getClientData(TCHAR* argv[5]);
	void getClientData();

	Client();
	~Client();

	void start();
	void process();
};
bool receiveData(CSocket& mysock, size_t& size, char* buffer);