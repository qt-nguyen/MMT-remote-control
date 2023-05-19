#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include "RPC_Func.h"
#include "DataObj/RPC_Obj.h"
#include "DataObj/StringConversion.h"


DataObj* RPC_Func::listPrcs() {
    std::string result = "";
    DataObj* MES = new RPC_Obj(DataType::RESPONSE, CmdType::SHOW, result);
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return MES;

    cProcesses = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

            if (hProcess)
            {
                WCHAR processName[MAX_PATH];
                DWORD len = MAX_PATH;
                if (QueryFullProcessImageName(hProcess, 0, processName, &len))
                {
                    std::wstring fullPath(processName);
                    std::wstring fileName = fullPath.substr(fullPath.find_last_of(L"\\") + 1);
                    result += StringConversion::ws2s(fileName);
                    result += "\n";
                }
                CloseHandle(hProcess);
            }
        }
    }

    // Return the list of installed applications
    return MES;
}

// A funtion to deal with processes I dont know
HANDLE OpenProcessByName(const std::wstring& name)
{
    HANDLE hProcess = NULL;
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        std::wcout << L"Failed to enumerate processes" << std::endl;
        return NULL;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    bool foundProcess = false;
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            // Use a different access level when calling OpenProcess
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);

            if (hProcess)
            {
                WCHAR processName[MAX_PATH];
                DWORD len = MAX_PATH;
                if (QueryFullProcessImageName(hProcess, 0, processName, &len))
                {
                    std::wstring fullPath(processName);
                    std::wstring fileName = fullPath.substr(fullPath.find_last_of(L"\\") + 1);
                    std::wcout << L"Found process: " << fileName << std::endl;
                    if (fileName == name + L".exe")
                    {
                        foundProcess = true;
                        CloseHandle(hProcess);
                        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, aProcesses[i]);
                        if (hProcess == NULL)
                        {
                            std::wcout << L"Failed to open process with terminate access for " << name << std::endl;
                        }
                        return hProcess;
                    }
                }
                CloseHandle(hProcess);
            }
        }
    }

    if (!foundProcess)
    {
        std::wcout << L"Failed to find process for " << name << std::endl;
    }

    return NULL;
}

DataObj* RPC_Func::runPrc(std::string Name)
{
    std::string res = "";
    DataObj* MES = new RPC_Obj(DataType::RESPONSE, CmdType::DATA, res);
    WCHAR szPath[MAX_PATH];
    std::wstring name = StringConversion::s2ws(Name);
    if (SearchPath(NULL, name.c_str(), L".exe", MAX_PATH, szPath, NULL) == 0)
    {
        res = "Failed to find executable file for " + Name;
        res += "\n";
        MES->setData(res);
        return MES;
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessW(szPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        res = "Failed to create process for " + Name;
        res += "\n";
        MES->setData(res);
        return MES;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    res = "Process run successfully.\n";
    MES->setData(res);
    return MES;
}


DataObj* RPC_Func::killPrc(std::string Name)
{
    std::string result = "";
    DataObj* MES = new RPC_Obj(DataType::RESPONSE, CmdType::DATA, result);
    std::wstring name = StringConversion::s2ws(Name);
    HANDLE hProcess = OpenProcessByName(name);
    if (hProcess == NULL)
    {
        result = "Failed to open process for " + Name;
        result += "\n";
        MES->setData(result);
        return MES;
    }

    DWORD dwExitCode;
    GetExitCodeProcess(hProcess, &dwExitCode);
    if (!TerminateProcess(hProcess, dwExitCode))
    {
        result = "Failed to terminate process for " + Name;
        result += "\n";
        MES->setData(result);
        return MES;

    }

    CloseHandle(hProcess);
    
    return MES;
}