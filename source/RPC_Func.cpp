#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include "RPC_Func.h"

//function to convert a wstring to a string
std::string ws2s(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}


//function to convert a string to a wstring
std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}


DataTransferObj* RPC_Func::listPrcs() {
    std::string result = "";
    DataTransferObj* MES = new RPC_TransferObj(DataType::RESPONSE, CmdType::SHOW, result);
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
                    result += ws2s(fileName);
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

DataTransferObj* RPC_Func::runPrc(std::string Name)
{
    std::string res = "";
    DataTransferObj* MES = new RPC_TransferObj(DataType::RESPONSE, CmdType::RUN, res);
    WCHAR szPath[MAX_PATH];
    std::wstring name = s2ws(Name);
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


DataTransferObj* RPC_Func::killPrc(std::string Name)
{
    std::string result = "";
    DataTransferObj* MES = new RPC_TransferObj(DataType::RESPONSE, CmdType::KILL, result);
    std::wstring name = s2ws(Name);
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