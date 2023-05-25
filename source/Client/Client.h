#include "../DataObj/DataObj.h"


#include "stdafx.h"
#include "afxsock.h"
#include <conio.h>

#include "../Utils/utils.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class Client
{
private:
	DataObj _clientData;
	CSocket _client;
	std::string _IP;

public:
	void getClientData(TCHAR* argv[5]);
	void getClientData();

	Client();
	~Client();

	bool start();
	void process();
};
bool receiveData(CSocket& mysock, size_t& size, char* buffer);