#pragma once

#include "../DataObj/DataObj.h"
#include "../Utils/utils.h"

#include <algorithm>
#include <string>
#include <tlhelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <vector>
#include <sstream>
#include <d3d9.h>
#include <wincodec.h>
#include <wingdi.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Gdiplus.lib")
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