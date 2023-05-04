#include "IAP_Obj.h"
#include "DataObj.h"
#include <tchar.h>
#include <iostream>
#include <string>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <windows.h>
#include <Psapi.h>
#include "StringConversion.h"


std::string IAP_Obj::listApps() {
    std::string result = "";
    HKEY hUninstKey = NULL;
    HKEY hAppKey = NULL;
    WCHAR sAppKeyName[1024];
    WCHAR sSubKey[1024];
    WCHAR sDisplayName[1024];
    const WCHAR* sRoot = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    long lResult = ERROR_SUCCESS;
    DWORD dwType = KEY_ALL_ACCESS;
    DWORD dwBufferSize = 0;

    //Open the "Uninstall" key.
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sRoot, 0, KEY_READ, &hUninstKey) != ERROR_SUCCESS) {
        return "ERROR!";
    }

    for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++) {
        //Enumerate all sub keys...
        dwBufferSize = sizeof(sAppKeyName);
        if ((lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName, &dwBufferSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS) {
            //Open the sub key.
            wsprintf(sSubKey, L"%s\\%s", sRoot, sAppKeyName);
            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_READ, &hAppKey) != ERROR_SUCCESS) {
                RegCloseKey(hAppKey);
                RegCloseKey(hUninstKey);
                return "ERROR!";
            }
            //Get the display name value from the application's sub key.
            dwBufferSize = sizeof(sDisplayName);
            if (RegQueryValueEx(hAppKey, L"DisplayName", NULL, &dwType, (unsigned char*)sDisplayName, &dwBufferSize) == ERROR_SUCCESS) {
                result += StringConversion::ws2s(sDisplayName); // Append the display name to the result string
                result += "\n"; // Add a newline character to separate each display name
            }
            else {
                //Display name value doe not exist, this application was probably uninstalled.
            }
            RegCloseKey(hAppKey);
        }
    }
    RegCloseKey(hUninstKey);

    // Return the list of installed applications
    return result;
}

std::string IAP_Obj::startApp(std::string Name)
{
    std::string res = "";
    std::wstring appName = StringConversion::s2ws(Name);
    DWORD bufferSize = MAX_PATH;
    WCHAR appPath[MAX_PATH];
    HRESULT result = AssocQueryString(ASSOCF_INIT_IGNOREUNKNOWN, ASSOCSTR_EXECUTABLE, appName.c_str(), NULL, appPath, &bufferSize);
    if (SUCCEEDED(result))
    {
        ShellExecute(NULL, L"open", appPath, NULL, NULL, SW_SHOWNORMAL);
        res = Name + " start successfully.";
    }
    else
    {
        res = "Error getting application path. Error code: " + result;
        res += "\n";
    }
    return res;
}


std::string IAP_Obj::stopApp(std::string Name)
{
    std::string result = "";
    std::wstring appName = StringConversion::s2ws(Name);
    // Get the list of process identifiers
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return "Failed to enumerate processes.";
    }

    // Calculate how many process identifiers were returned
    cProcesses = cbNeeded / sizeof(DWORD);

    // Find and close the app
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

            // Get a handle to the process
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

            // Get the process name
            if (hProcess != NULL)
            {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                }
            }

            // Compare the process name with the app name
            if (_tcscmp(szProcessName, appName.c_str()) == 0)
            {
                // Close the app
                TerminateProcess(hProcess, 0);
                result = "App stopped successfully.";
            }

            // Release the handle to the process
            CloseHandle(hProcess);
        }
    }

    return result;
}

std::string IAP_Obj::toString()
{
    std::stringstream ss;

    // Convert the data to a string
    for (char c : _data)
    {
        ss << c;
    }

    return ss.str();
}

std::string IAP_Obj::toFile(std::string filename)
{
    std::string result = "";

    // Open the file
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return "Failed to open file.";
    }

    // Write the data to the file
    file.write(_data.data(), _data.size());

    // Close the file
    file.close();

    result = "Data written to file successfully.";
    return result;
}
