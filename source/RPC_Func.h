#pragma once

#include "DataObj/DataObj.h"
#include "utils.h"

#include <algorithm>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <tchar.h>

#include <Psapi.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
/*

// Huynh Long's code
struct ProcessInfo {
    int pid;
    std::string name;
    char type;
};

struct handle_data {
    unsigned long process_id;
    HWND window_handle;           // ?
};
//------------------------------

*/

class RPC_Func
{
private:

public:
    std::shared_ptr<DataObj> HandleRequest(DataObj request);

    //list processes
    std::string listPrcs();

    //start a process
    std::string runPrc(std::string prcName);


    std::string killPrc(std::string prcName);
    // HUYNH LONG VERSION
   // std::vector<ProcessInfo> get_current_processes();


};