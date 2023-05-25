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

//List processes function of RPC_Func, simply list all running processes
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
        std::string str = utils::ws2s(pe32.szExeFile);
        result += str;
        result += "\n";
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return result;
}

//Run process function of RPC_Func, parse the name to be an executable file if possible
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


    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return res;
}

//Stop process of RPC_Func, parse the name get from the function ListPrc() above if possible 
std::string RPC_Func::killPrc(const std::string& processName) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool processFound = false;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return "Failed to create process snapshot";
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return "Failed to retrieve information about first process";
    }

    // Now walk the snapshot of processes
    do
    {
        std::string str = utils::ws2s(pe32.szExeFile);
        if (str == processName) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess == NULL) {
                CloseHandle(hProcessSnap);
                return "Failed to open process";
            }
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            processFound = true;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    if (processFound) {
        return "Processes terminated successfully";
    }
    else {
        return "Processes not found";
    }
}
