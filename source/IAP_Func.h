#pragma once

#include "DataObj/DataObj.h"
#include "utils.h"

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <iostream>
#include <string>

#include <vector>
#include <algorithm>
#include <set>


#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

// containing functions for Installed AaP

class IAP_Func
{
private:

public:
    std::shared_ptr<DataObj> HandleRequest(DataObj request);

    //list all installed apps
    std::string listApps();

    // Start an app
    std::string startApp(std::string appName);

    // Stop an app
    std::string stopApp(std::string appName);

};