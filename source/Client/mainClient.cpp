#include "Client.h"

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	std::cout << "------------------- POWERED BY TNT - NETWORKING GROUP ---------------------\n\n";
	Client client;
	if (!client.start()) {
		system("pause");
	}
	else client.process();
	
}