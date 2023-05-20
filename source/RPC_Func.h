#pragma once

#include "DataObj/DataObj.h"
#include <string>

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

};