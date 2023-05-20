// Demo_Client.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Demo_Client.h"
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

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// Number of connection attempts
	const int MAX_ATTEMPTS = 10;

	// Delay between connection attempts (in milliseconds)
	const int DELAY = 1000;

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

				// Connection failed, wait before trying again
				Sleep(DELAY);
			}

			if (!connected)
			{
				// Failed to connect to the server after multiple attempts
				std::cerr << "Error: Failed to connect to the server\n";
				nRetCode = -1;
			}


			int number_continue = 0;
			char letter;
			do {
				fflush(stdin); 
				int number_a, number_b, number_result;
				char letter;
				printf("Nhap vao phep toan (+, -): ");
				scanf_s("%c", &letter, 2);

				printf("Nhap vao so thu nhat: ");
				scanf_s("%d", &number_a);

				printf("Nhap vao so thu hai: ");
				scanf_s("%d", &number_b);

				CalculationData data;
				data.setLetter(letter);
				data.setNumA(number_a);
				data.setNumB(number_b);

				client.Send(&data, sizeof(data), 0);

				

				//Nhan ket qua tinh toan tu server
				client.Receive(&number_result, sizeof(number_result), 0);
				printf("Ket qua phep toan %d %c %d = %d\n", number_a, letter, number_b, number_result);

				printf("Nhap 1 de tiep tuc, 0 de thoat: ");
				scanf_s("%d", &number_continue);
				getchar();
				client.Send(&number_continue, sizeof(number_continue), 0);
			}while(number_continue);

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
