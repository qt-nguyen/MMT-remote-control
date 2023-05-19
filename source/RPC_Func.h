#pragma once

#include "DataObj/DataObj.h"
#include "DataObj/RPC_Obj.h"
#include <string>

class RPC_Func
{
private:

public:
    //list processes
    DataObj* listPrcs();

    //start a process
    DataObj* runPrc(std::string prcName);

    //delete a process
    DataObj* killPrc(std::string prcName);

};