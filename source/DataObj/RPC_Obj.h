#pragma once

#include "DataObj.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

class RPC_Obj: public DataObj
{

public:
    RPC_Obj(std::string ID, DataType dataType, CmdType CMD, std::vector<char> data)
    {
        setID(ID);
        _func_type = FuncType::RPC;
        setDataType(dataType);
        setCmdType(CMD);
        setData(data);
    }

public:
    // Convert data object to string type
    std::string toString();

    // Convert data object to a file
    std::string toFile(std::string filename);

    // List installed apps
    std::string listPrcs();

    // Start an app
    std::string runPrc(std::string prcName);

    // Stop an app
    std::string killPrc(std::string prcName);
};
