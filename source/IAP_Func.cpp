#include "IAP_Func.h"

std::shared_ptr<DataObj> IAP_Func::HandleRequest(DataObj request) {

    CmdType cmdType = request.getCmdType();
    std::string data = request.getData_String();
    std::shared_ptr<DataObj> result(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::IAP, CmdType::CMD_TYPE, ""));

    if (request.getDataType() != DataType::REQUEST || request.getFuncType() != FuncType::IAP)
    {
        result->setData("MESSAGE PARAMETER ERRORS");
        return result;
    }
    else
    {
        if (cmdType == CmdType::SHOW)
        {
            result->setData(listApps());

            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        if (cmdType == CmdType::START)
        {
            result->setData(startApp(data));


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


std::string IAP_Func::listApps() {
    std::vector<std::wstring> appNames; // Vector to store the display names of installed applications
    std::set<std::wstring> uniqueAppNames; // Set to keep track of unique display names
    HKEY hUninstKey = NULL;
    HKEY hAppKey = NULL;
    WCHAR sAppKeyName[1024];
    WCHAR sSubKey[1024];
    WCHAR sDisplayName[1024];
    const WCHAR* sUninstallRoot = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    //const WCHAR* sAppPathsRoot = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths";
    long lResult = ERROR_SUCCESS;
    DWORD dwType = KEY_ALL_ACCESS;
    DWORD dwBufferSize = 0;

    // Check both the HKEY_LOCAL_MACHINE and HKEY_CURRENT_USER hives for uninstall information
    HKEY hives[] = { HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER };
    for (int i = 0; i < 2; i++) {
        // Open the "Uninstall" key.
        if (RegOpenKeyEx(hives[i], sUninstallRoot, 0, KEY_READ, &hUninstKey) != ERROR_SUCCESS) {
            continue;
        }

        for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++) {
            // Enumerate all sub keys...
            dwBufferSize = sizeof(sAppKeyName);
            if ((lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName, &dwBufferSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS) {
                // Open the sub key.
                wsprintf(sSubKey, L"%s\\%s", sUninstallRoot, sAppKeyName);
                if (RegOpenKeyEx(hives[i], sSubKey, 0, KEY_READ, &hAppKey) != ERROR_SUCCESS) {
                    RegCloseKey(hAppKey);
                    RegCloseKey(hUninstKey);
                    continue;
                }
                // Get the display name value from the application's sub key.
                dwBufferSize = sizeof(sDisplayName);
                if (RegQueryValueEx(hAppKey, L"DisplayName", NULL, &dwType, (unsigned char*)sDisplayName, &dwBufferSize) == ERROR_SUCCESS) {
                    // Check if the display name is already in the set of unique display names
                    if (uniqueAppNames.find(sDisplayName) == uniqueAppNames.end()) {
                        // Display name is not in the set, add it to the vector and set
                        appNames.push_back(sDisplayName);
                        uniqueAppNames.insert(sDisplayName);
                    }
                }
                else {
                    // Display name value does not exist, this application was probably uninstalled.
                }
                RegCloseKey(hAppKey);
            }
        }
        RegCloseKey(hUninstKey);
        lResult = ERROR_SUCCESS; // Reset lResult for the next hive
    }
    /*
    // Check the HKEY_LOCAL_MACHINE hive for application paths
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sAppPathsRoot, 0, KEY_READ, &hUninstKey) == ERROR_SUCCESS) {
        for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++) {
            // Enumerate all sub keys...
            dwBufferSize = sizeof(sAppKeyName);
            if ((lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName, &dwBufferSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS) {
                // Check if the application name is already in the set of unique display names
                std::wstring appName(sAppKeyName);
                if (uniqueAppNames.find(appName) == uniqueAppNames.end()) {
                    // Application name is not in the set, add it to the vector and set
                    appNames.push_back(appName);
                    uniqueAppNames.insert(appName);
                }
            }
        }
        RegCloseKey(hUninstKey);
    }
    */
    // Sort the vector of display names in alphabetical order
    std::sort(appNames.begin(), appNames.end());

    // Construct the result string from the sorted vector of display names
    std::wstring resultW;
    for (const auto& appName : appNames) {
        resultW += appName + L"\n";
    }

    // Convert the result string from wide string to narrow string
    std::string result = utils::ws2s(resultW);

    // Return the list of installed applications
    return result;
}


/*

std::string IAP_Func::startApp(std::string Name)
{
    std::string res = "";
    std::wstring appName = utils::s2ws(Name);
    DWORD bufferSize = MAX_PATH;
    WCHAR appPath[MAX_PATH];
    HRESULT result = AssocQueryString(ASSOCF_INIT_IGNOREUNKNOWN, ASSOCSTR_EXECUTABLE, appName.c_str(), NULL, appPath, &bufferSize);
    if (SUCCEEDED(result))
    {
        HINSTANCE hInstance = ShellExecute(NULL, L"open", appPath, NULL, NULL, SW_SHOWNORMAL);
        if ((int)hInstance > 32)
        {
            res = Name + " started successfully.";
        }
        else
        {
            res = "Error starting application. Error code: " + std::to_string((int)hInstance);
            res += "\n";
        }
    }
    else
    {
        res = "Error getting application path. Error code: " + std::to_string(result);
        res += "\n";
    }
    return res;
}
*/

std::string IAP_Func::startApp(std::string Name)
{
    std::string res = "";
    std::wstring appName = utils::s2ws(Name);
    DWORD bufferSize = MAX_PATH;
    WCHAR appPath[MAX_PATH];
    HRESULT result = AssocQueryString(ASSOCF_INIT_IGNOREUNKNOWN, ASSOCSTR_EXECUTABLE, appName.c_str(), NULL, appPath, &bufferSize);
    if (SUCCEEDED(result))
    {
        HINSTANCE hInstance = ShellExecute(NULL, L"open", appPath, NULL, NULL, SW_SHOWNORMAL);
        if ((int)hInstance > 32)
        {
            res = Name + " started successfully.";
        }
        else
        {
            res = "Error starting application. Error code: " + std::to_string((int)hInstance);
            res += "\n";
        }
    }
    else
    {
        // If AssocQueryString fails to find the application path, try using the listApps function to find the application
        std::string appList = listApps();
        std::istringstream iss(appList);
        std::string line;
        bool foundApp = false;
        while (std::getline(iss, line)) {
            if (line == Name) {
                foundApp = true;
                break;
            }
        }
        if (foundApp) {
            // Application was found in the list of installed applications
            HINSTANCE hInstance = ShellExecute(NULL, L"open", appName.c_str(), NULL, NULL, SW_SHOWNORMAL);
            if ((int)hInstance > 32)
            {
                res = Name + " started successfully.";
            }
            else
            {
                res = "Error starting application. Error code: " + std::to_string((int)hInstance);
                res += "\n";
            }
        }
        else {
            // Application was not found in the list of installed applications
            res = "Error getting application path. Error code: " + std::to_string(result);
            res += "\n";
        }
    }
    return res;
}



std::string IAP_Func::stopApp(std::string Name)
{
    std::wstring appName = utils::s2ws(Name);
    HWND hwnd = FindWindow(NULL, appName.c_str());
    if (hwnd != NULL) {
        SendMessage(hwnd, WM_CLOSE, 0, 0);
        return Name + " app closed successfully.";
    }
    else {
        return "Error: Could not find opening app with name " + Name;
    }
}
