
#include "RPC_Func.h"


std::shared_ptr<DataObj> RPC_Func::HandleRequest(DataObj request)
{
    CmdType cmdType = request.getCmdType();
    std::string data = request.getData_String();
    std::shared_ptr<DataObj> result(new DataObj(utils::CurrentTime(), RESPONSE, RPC, CMD_TYPE, ""));

    if (request.getDataType() != REQUEST || request.getFuncType() != RPC)
    {
        result->setData("MESSAGE PARAMETER ERRORS");
        return result;
    }
    else
    {
        if (cmdType == SHOW)
        {
            result->setData(listPrcs());

            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        if (cmdType == RUN)
        {
            result->setData(runPrc(data));

            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        if (cmdType == KILL)
        {
            result->setData(killPrc(data));

            if (!result->getData_String().empty()) {
                result->setCmdType(CmdType::DATA);
            }

            return result;
        }
        result->setData("MESSAGE PARAMETER ERRORS");
        return result;
    }
}



std::string RPC_Func::listPrcs() {
    std::string result = "";
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return result;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return result;
    }

    // Now walk the snapshot of processes
    do
    {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, pe32.szExeFile, -1, NULL, 0, NULL, NULL);
        std::string str(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, pe32.szExeFile, -1, &str[0], size_needed, NULL, NULL);
        result += str;
        result += "\n";
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return result;

}

std::string RPC_Func::runPrc(std::string Name)
{
    std::string res = "";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    std::wstring PrcName = utils::s2ws(Name);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process.
    if (!CreateProcess(
        NULL,                   // No module name (use command line)
        (LPWSTR)PrcName.c_str(), // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        0,                      // No creation flags
        NULL,                   // Use parent's environment block
        NULL,                   // Use parent's starting directory
        &si,                    // Pointer to STARTUPINFO structure
        &pi)                    // Pointer to PROCESS_INFORMATION structure
        ) {
        DWORD dwError = GetLastError();
        LPWSTR lpMsgBuf = nullptr;
        DWORD size = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&lpMsgBuf,
            0, NULL);

        if (size > 0) {
            // Convert the wide string error message to a string
            std::wstring errorMsg(lpMsgBuf);
            std::string errorMsgStr(errorMsg.begin(), errorMsg.end());

            res += "Run Process failed: " + errorMsgStr;

            // Free the buffer allocated by FormatMessage
            LocalFree(lpMsgBuf);
        }
        else {
            res += "Failed to retrieve error message.";
        }

        return res;
    }

    res += Name + " ran successfully.\n";

    // Wait until the child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return res;
}

std::string RPC_Func::killPrc(std::string name)
{
    std::string resultMsg;

    DWORD processes[1024];
    DWORD cbNeeded;
    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
        resultMsg = "Failed to enumerate processes.";
        return resultMsg;
    }

    DWORD numProcesses = cbNeeded / sizeof(DWORD);
    bool found = false;

    for (DWORD i = 0; i < numProcesses; i++) {
        DWORD processId = processes[i];
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, processId);
        if (hProcess) {
            char processName[MAX_PATH];
            if (GetModuleBaseNameA(hProcess, NULL, processName, sizeof(processName))) {
                std::string processNameStr(processName);
                std::transform(processNameStr.begin(), processNameStr.end(), processNameStr.begin(), ::tolower);
                std::string searchName = name;
                std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

                if (processNameStr.find(searchName) != std::string::npos) {
                    if (TerminateProcess(hProcess, 0)) {
                        resultMsg = "Process '" + processNameStr + "' terminated successfully.";
                        found = true;
                    }
                    else {
                        resultMsg = "Failed to terminate process '" + processNameStr + "'.";
                    }
                    break;
                }
            }
        }

        CloseHandle(hProcess);
    }

    if (!found) {
        resultMsg = "Process '" + name + "' not found.";
    }

    return resultMsg;
}


/*

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

*/