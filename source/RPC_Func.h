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

class RPC_Func
{
private:

public:
    //Handle a DataObj as request and execute the corresponding function
    std::shared_ptr<DataObj> HandleRequest(DataObj request);

    //list processes
    std::string listPrcs();

    //start a process
    std::string runPrc(std::string prcName);

    //kill a process
    std::string killPrc(const std::string& processName);


};