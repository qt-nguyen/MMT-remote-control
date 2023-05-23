#include "Client.h"

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	Client client;
	client.getClientData(argv);

	client.start();
}