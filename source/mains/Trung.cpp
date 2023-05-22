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

std::vector<std::string> getRunningProcesses() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    std::vector<std::string> processList;

    // Get the list of process identifiers.
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return processList;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Display the name and process identifier for each process.
    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            // Get a handle to the process.
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                PROCESS_VM_READ,
                FALSE, aProcesses[i]);

            // Get the process name.
            if (hProcess != NULL) {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                    &cbNeeded)) {
                    GetModuleBaseName(hProcess, hMod, szProcessName,
                        sizeof(szProcessName) / sizeof(TCHAR));
                }
            }

            // Add the process name to the list
            processList.push_back(utils::tcharToString(szProcessName));

            // Release the handle to the process.
            CloseHandle(hProcess);
        }
    }

    return processList;
}

#include <iostream>

int main()
{
    auto res = getRunningProcesses();
    for (std::string proc_name : res)
        std::cout << proc_name << "\n";



    system("pause");

}
