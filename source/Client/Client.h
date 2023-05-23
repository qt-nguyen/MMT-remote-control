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

public:
	void getClientData(TCHAR* argv[5]);
	void getCilentData(DataType dataType, FuncType funcType, CmdType cmdType, std::string data);
	Client();
	~Client();

	void start();
};