#pragma once

#include "DataObj/DataObj.h"
#include <string>
#include <tlhelp32.h>

// Huynh Long's code
struct ProcessInfo {
    int pid;
    std::string name;
    char type;
};

struct handle_data {
    unsigned long process_id;
    HWND window_handle;
};
//------------------------------

class RPC_Func
{
private:

public:
    //list processes
    std::shared_ptr<DataObj> listPrcs();



    //start a process
    std::shared_ptr<DataObj> runPrc(std::string prcName);

    //delete a process
    std::shared_ptr<DataObj> killPrc(std::string prcName);


    // BING CHAT VERSION
    std::vector<std::string> getRunningProcesses();

    // HUYNH LONG VERSION
    std::vector<ProcessInfo> get_current_processes();

    HWND RPC_Func::find_main_window(unsigned long process_id);
};