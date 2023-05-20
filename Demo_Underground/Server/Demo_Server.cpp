// Demo_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Demo_Server.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CalculationData {
private:
	char _letter;
	int _number_a;
	int _number_b;
public:
	char getLetter() { return _letter; }
	int getNumA() { return _number_a; }
	int getNumB() { return _number_b; }
	void setLetter(char l) { _letter = l; }
	void setNumA(int a) { _number_a = a; }
	void setNumB(int b) { _number_b = b; }
};

// The one and only application object

CWinApp theApp;

using namespace std;

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

	do {
		fflush(stdin); 
		int number_result;

		CalculationData data;
		mysock.Receive(&data, sizeof(data), 0);

		char letter = data.getLetter();
		int number_a = data.getNumA();
		int number_b = data.getNumB();

		//So sanh neu client muon thuc hien phep cong
		if(letter == '+')
			number_result = number_a + number_b;
		else if (letter == '-')
			number_result = number_a  - number_b;
			
		//Gui ket qua tinh toan cho client
		mysock.Send(&number_result, sizeof(number_result), 0);

		//Nhan number xem client co tiep tuc hay khong
		mysock.Receive(&number_continue, sizeof(number_continue), 0);


		if (mysock.Receive(&letter, sizeof(letter), 0) == 0)
		{
			wprintf(L"Client %d from %s disconnected from server\n", clientID, (LPCTSTR)ipAddress);
			mysock.Close();
			break;
		}

	}while(number_continue);
	delete args;
	return 0;
	//return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			AfxSocketInit(NULL);
			CSocket server, s;
			DWORD threadID;
			HANDLE threadStatus;

			int clientID = 0;

			server.Create(4567);
			do {
				printf("Server lang nghe ket noi tu client\n");
				server.Listen();
				server.Accept(s);

				++clientID;
				// Get the IP address and port number of the connected client
				CString ipAddress;
				UINT port;
				s.GetPeerName(ipAddress, port);
				wprintf(L"Client %d connected from IP address %s\n", clientID, (LPCTSTR)ipAddress);

				//Chuyển đỏi CSocket thanh Socket
				auto args = new std::pair<int, SOCKET>(clientID, s.Detach());
				//Khoi tao thread tuong ung voi moi client Connect vao server.
				
				//Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
				threadStatus = CreateThread(NULL, 0, function_cal, args, 0, &threadID);

			}while(1);
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


