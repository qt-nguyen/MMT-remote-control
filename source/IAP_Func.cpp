#include "IAP_Func.h"

std::shared_ptr<DataObj> IAP_Func::HandleRequest(DataObj request) {

    CmdType cmdType = request.getCmdType();
    std::string data = request.getData_String();
    std::shared_ptr<DataObj> result(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::IAP, CmdType::CMD_TYPE, ""));

    // Create an unordered_map to store the application paths
    std::unordered_map<std::wstring, std::wstring> appPaths;

    // Define common installation directories to search for executable files
    std::vector<std::wstring> searchPaths = { L"C:\\Program Files", L"C:\\Program Files (x86)" };

    // Search for executable files in all subdirectories of the common installation directories
    for (const auto& searchPath : searchPaths) {
        searchForExeFiles(searchPath, appPaths);
    }

    if (request.getDataType() != DataType::REQUEST || request.getFuncType() != FuncType::IAP)
    {
        result->setData("MESSAGE PARAMETER ERRORS");
        return result;
    }
    else
    {
        if (cmdType == CmdType::SHOW)
        {
            result->setData(listApps(appPaths));

            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        if (cmdType == CmdType::START)
        {
            result->setData(startApp(data, appPaths));


            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        if (cmdType == CmdType::STOP)
        {
            result->setData(stopApp(data));

            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        result->setData("MESSAGE PARAMETER ERRORS");


        return result;
    }
}

void searchForExeFiles(const std::wstring& searchPath, std::unordered_map<std::wstring, std::wstring>& appPaths) {
    // Search for executable files in the specified directory
    WIN32_FIND_DATA findData;
    std::wstring searchPattern = searchPath + L"\\*.exe";
    HANDLE hFind = FindFirstFile(searchPattern.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // Found an executable file, add its name to the map of application paths
                std::wstring appName = findData.cFileName;
                // Remove the .exe extension from the application name
                if (appName.size() > 4 && (appName.substr(appName.size() - 4) == L".exe" || appName.substr(appName.size() - 4) == L".EXE")) {
                    appName = appName.substr(0, appName.size() - 4);
                }
                // Save the executable path of the application
                std::wstring appPath = searchPath + L"\\" + findData.cFileName;
                appPaths[appName] = appPath;
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }

    // Search for subdirectories in the specified directory
    searchPattern = searchPath + L"\\*";
    hFind = FindFirstFile(searchPattern.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // Found a directory, search for executable files in this directory
                std::wstring subDirName = findData.cFileName;
                // Skip the "." and ".." directories
                if (subDirName == L"." || subDirName == L"..") continue;
                std::wstring subDirPath = searchPath + L"\\" + subDirName;
                searchForExeFiles(subDirPath, appPaths);
            }
        } while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }
}


std::string IAP_Func::listApps(const std::unordered_map<std::wstring, std::wstring>& appPaths) {
    static bool cacheValid = false;
    static std::string cachedResult;

    if (cacheValid) {
        // Cache is valid, return the cached result
        return cachedResult;
    }
    else {
        // Cache is not valid, regenerate the list of installed applications
        std::vector<std::wstring> appNames; // Vector to store the display names of installed applications

        // Get the display names of installed applications from the map of application paths
        for (const auto& app : appPaths) {
            appNames.push_back(app.first);
        }

        // Sort the vector of application names in alphabetical order
        std::sort(appNames.begin(), appNames.end());

        // Construct the result string from the sorted vector of application names
        std::wstring resultW;
        for (const auto& appName : appNames) {
            resultW += appName + L"\n";
        }

        // Convert the result string from wide string to narrow string
        cachedResult = utils::ws2s(resultW);

        // Update cache state and return result
        cacheValid = true;
        return cachedResult;
    }
}



std::string IAP_Func::startApp(const std::string& name, const std::unordered_map<std::wstring, std::wstring>& appPaths) {
    static bool cacheValid = false;

    std::string res = "";

    // Convert the application name from narrow string to wide string
    std::wstring appNameW = utils::s2ws(name);

    auto it = appPaths.find(appNameW);
    if (it != appPaths.end()) {
        const std::wstring& appPath = it->second;

        HINSTANCE hInstance = ShellExecute(NULL, L"open", appPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
        if ((int)hInstance > 32) {
            res = name + " started successfully.";
        }
        else {
            res = "Error starting application. Error code: " + std::to_string((int)hInstance);
            res += "\n";
        }
    }
    else {
        // Application not found, try appending .exe extension to application name
        appNameW += L".exe";
        it = appPaths.find(appNameW);
        if (it != appPaths.end()) {
            const std::wstring& appPath = it->second;

            HINSTANCE hInstance = ShellExecute(NULL, L"open", appPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
            if ((int)hInstance > 32) {
                res = name + " started successfully.";
            }
            else {
                res = "Error starting application. Error code: " + std::to_string((int)hInstance);
                res += "\n";
            }
        }
        else {
            res = "Application not found.";
            res += "\n";
        }
    }

    cacheValid = false;
    return res;
}


std::string IAP_Func::stopApp(std::string Name)
{
    std::wstring appName = utils::s2ws(Name);
    HWND hwnd = FindWindow(NULL, NULL);
    while (hwnd != NULL) {
        int length = GetWindowTextLength(hwnd);
        std::wstring title(length, L'\0');
        GetWindowText(hwnd, &title[0], length + 1);
        if (title.find(appName) != std::wstring::npos) {
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            return Name + " app closed successfully.";
        }
        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
    }
    return "Error: Could not find opening app with name " + Name;
}