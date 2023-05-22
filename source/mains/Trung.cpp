///*
//This is Trung's demo to test functionalities and dependencies
//Please exclude this file from project if you want to run your own main demo
//Don't change this file  without informing the author
//*/
//#include <nlohmann/json.hpp>
//#include <iostream>
//#include <string>
//#include <vector>
//#include "../DataObj/DataObj.h"
//#include "../utils.h"
//#include <chrono>
//#include <ctime>
//#include <iomanip>
//#include <filesystem>
//#include "../DIR_Func.h"
//#include "../IAP_Func.h"
//#include "../RPC_Func.h"
//namespace fs = std::filesystem;
//using json = nlohmann::json;
//
//using namespace std;
//
//
//
//#include <tlhelp32.h>
//#include <Windows.h>
//#include <processthreadsapi.h>
//#include <process.h>
//
//int main() {
//    RPC_Func rpc;
//    auto res = rpc.listPrcs();
//    std::cout << res->getData_String();
//
//
//    system("pause");
//    return 0;
//}


#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "../utils.h"

#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <vector>
#include <string>


//void startProcess(std::string processName) {
//    STARTUPINFO si;
//    PROCESS_INFORMATION pi;
//
//    ZeroMemory(&si, sizeof(si));
//    si.cb = sizeof(si);
//    ZeroMemory(&pi, sizeof(pi));
//
//    // Convert processName to TCHAR array
//    TCHAR processNameTCHAR[MAX_PATH];
//#ifdef UNICODE
//    MultiByteToWideChar(CP_ACP, 0, processName.c_str(), -1, processNameTCHAR, MAX_PATH);
//#else
//    strcpy_s(processNameTCHAR, MAX_PATH, processName.c_str());
//#endif
//
//    // Start the child process.
//    if (!CreateProcess(NULL,   // No module name (use command line)
//        processNameTCHAR,      // Command line
//        NULL,           // Process handle not inheritable
//        NULL,           // Thread handle not inheritable
//        FALSE,          // Set handle inheritance to FALSE
//        0,              // No creation flags
//        NULL,           // Use parent's environment block
//        NULL,           // Use parent's starting directory 
//        &si,            // Pointer to STARTUPINFO structure
//        &pi)           // Pointer to PROCESS_INFORMATION structure
//        )
//    {
//        printf("CreateProcess failed (%d).\n", GetLastError());
//        return;
//    }
//
//    // Close process and thread handles.
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//}
//
//int main()
//{
//    startProcess(R"(msinfo32)");
//
//
//    system("pause");
//}

#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <sstream>
#include "../utils.h"



int main()
{



	system("pause");
}