#include "Client.h"

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	Client client;
	if (!client.start()) {
		system("pause");
	}
	else client.process();
	
}