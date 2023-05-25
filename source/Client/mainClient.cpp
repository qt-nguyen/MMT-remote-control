#include "Client.h"

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	Client client;
	client.start();

	client.process();

}