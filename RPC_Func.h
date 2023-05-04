#pragma once

#include "DataTransferObj.h"
#include "RPC_TransferObj.h"

class RPC_Func
{
private:

public:
    //list processes
    DataTransferObj* listPrcs();

    //start a process
    DataTransferObj* runPrc(std::string prcName);

    //delete a process
    DataTransferObj* killPrc(std::string prcName);

};