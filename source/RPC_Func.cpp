#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include "RPC_Func.h"
#include "DataObj/DataObj.h"
#include <tlhelp32.h>
#include "utils.h"


std::shared_ptr<DataObj> RPC_Func::listPrcs() {
    std::string result = "";
    std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::RPC, CmdType::SHOW, result));
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
                    result += utils::ws2s(fileName);
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

std::shared_ptr<DataObj> RPC_Func::runPrc(std::string Name)
{
    std::string res = "";
    std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::RPC, CmdType::DATA, res));
    WCHAR szPath[MAX_PATH];
    std::wstring name = utils::s2ws(Name);
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


std::shared_ptr<DataObj> RPC_Func::killPrc(std::string Name)
{
    std::string result = "";
    std::shared_ptr<DataObj> MES(new DataObj(utils::CurrentTime(), DataType::RESPONSE, FuncType::RPC, CmdType::DATA, result));
    std::wstring name = utils::s2ws(Name);
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


// BING CHAT VERSION, ONLY SAVE NAMES
std::vector<std::string> RPC_Func::getRunningProcesses() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    std::vector<std::string> processList;

    // Get the list of process identifiers.
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return processList;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Display the name and process identifier for each process.
    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            // Get a handle to the process.
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                PROCESS_VM_READ,
                FALSE, aProcesses[i]);

            // Get the process name.
            if (hProcess != NULL) {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                    &cbNeeded)) {
                    GetModuleBaseName(hProcess, hMod, szProcessName,
                        sizeof(szProcessName) / sizeof(TCHAR));
                }
            }

            // Add the process name to the list
            if (_tcscmp(szProcessName, TEXT("<unknown>")) != 0) {
                processList.push_back(utils::tcharToString(szProcessName));
            }

            // Release the handle to the process.
            CloseHandle(hProcess);
        }
    }

    return processList;
}


// HUYNH  LONG'S VERSION

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
std::vector<ProcessInfo> RPC_Func::get_current_processes() {
    HANDLE hSnapshot;
    PROCESSENTRY32 pe;
    BOOL hResult;
    std::vector<ProcessInfo> result;

    // snapshot of all processes in the system
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) return {};

    // initializing size: needed for using Process32First
    pe.dwSize = sizeof(PROCESSENTRY32);

    // info about first process encountered in a system snapshot
    hResult = Process32First(hSnapshot, &pe);

    // retrieve information about the processes
    // and exit if unsuccessful
    while (hResult) {
        char type = 0;
        if (IsWindowVisible(find_main_window(pe.th32ProcessID))) {
            type = 1;
        }
        if (pe.th32ProcessID > 4) { //Skip some system processes
            ProcessInfo temp;
            temp.type = type;
            temp.pid = (int)pe.th32ProcessID;
            temp.name = utils::tcharToString(pe.szExeFile);
            result.push_back(temp);
        }
        hResult = Process32Next(hSnapshot, &pe);
    }

    // closes an open handle (CreateToolhelp32Snapshot)
    CloseHandle(hSnapshot);
    return result;
}

//-------


BOOL is_main_window(HWND handle) {
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam) {
    handle_data& data = *(handle_data*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (data.process_id != process_id || !is_main_window(handle))
        return TRUE;
    data.window_handle = handle;
    return FALSE;
}

HWND find_main_window(unsigned long process_id) {
    handle_data data;
    data.process_id = process_id;
    data.window_handle = 0;
    EnumWindows(enum_windows_callback, (LPARAM)&data);
    return data.window_handle;
}

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
std::vector<ProcessInfo> get_current_processes() {
    HANDLE hSnapshot;
    PROCESSENTRY32 pe;
    BOOL hResult;
    std::vector<ProcessInfo> result;

    // snapshot of all processes in the system
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) return {};

    // initializing size: needed for using Process32First
    pe.dwSize = sizeof(PROCESSENTRY32);

    // info about first process encountered in a system snapshot
    hResult = Process32First(hSnapshot, &pe);

    // retrieve information about the processes
    // and exit if unsuccessful
    while (hResult) {
        char type = 0;
        if (IsWindowVisible(find_main_window(pe.th32ProcessID))) {
            type = 1;
        }
        if (pe.th32ProcessID > 4) { //Skip some system processes
            ProcessInfo temp;
            temp.type = type;
            temp.pid = (int)pe.th32ProcessID;
            temp.name = utils::tcharToString(pe.szExeFile);
            result.push_back(temp);
        }
        hResult = Process32Next(hSnapshot, &pe);
    }

    // closes an open handle (CreateToolhelp32Snapshot)
    CloseHandle(hSnapshot);
    return result;
}


// THE REST ARE HUYNH  LONG's

//BOOL PauseResumeThreadList(DWORD dwOwnerPID, bool bResumeThread) {
//    HANDLE hThreadSnap;
//    BOOL bRet = FALSE;
//    THREADENTRY32 te32;
//
//    // Take a snapshot of all threads currently in the system. 
//    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
//    if (hThreadSnap == INVALID_HANDLE_VALUE)
//        return (FALSE);
//
//    // Fill in the size of the structure before using it. 
//    te32.dwSize = sizeof(THREADENTRY32);
//
//    // Walk the thread snapshot to find all threads of the process. 
//    // If the thread belongs to the process, add its information 
//    // to the display list.
//    if (Thread32First(hThreadSnap, &te32)) {
//        do {
//            if (te32.th32OwnerProcessID == dwOwnerPID) {
//                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
//                if (bResumeThread)
//                    ResumeThread(hThread);
//                else
//                    SuspendThread(hThread);
//                CloseHandle(hThread);
//            }
//        } while (Thread32Next(hThreadSnap, &te32));
//        bRet = TRUE;
//    }
//    else
//        bRet = FALSE; // could not walk the list of threads 
//
//    // Do not forget to clean up the snapshot object. 
//    CloseHandle(hThreadSnap);
//
//    return (bRet);
//}
//
//BOOL TerminateProcessEx(DWORD dwProcessId, UINT uExitCode) {
//    DWORD dwDesiredAccess = PROCESS_TERMINATE;
//    BOOL bInheritHandle = FALSE;
//    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
//    if (hProcess == NULL)
//        return FALSE;
//
//    BOOL result = TerminateProcess(hProcess, uExitCode);
//
//    CloseHandle(hProcess);
//
//    return result;
//}
//
//void suspend_process(int processId) {
//    PauseResumeThreadList(processId, 0);
//}
//
//void resume_process(int processId) {
//    PauseResumeThreadList(processId, 1);
//}
//
//void terminate_process(int processId) {
//    TerminateProcessEx(processId, -1);
//}