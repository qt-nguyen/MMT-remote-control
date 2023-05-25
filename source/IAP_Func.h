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
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <set>


#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

// containing functions for Installed AaP
void searchForExeFiles(const std::wstring& searchPath, std::unordered_map<std::wstring, std::wstring>& appPaths);

class IAP_Func
{
private:

public:
    //Handle a DataObj as request and execute the corresponding function
    std::shared_ptr<DataObj> HandleRequest(DataObj request);

    //list all installed apps
    std::string listApps(const std::unordered_map<std::wstring, std::wstring>& appPaths);

    // Start an app
    std::string startApp(const std::string& name, const std::unordered_map<std::wstring, std::wstring>& appPaths);

    // Stop an app
    std::string stopApp(std::string appName);

};