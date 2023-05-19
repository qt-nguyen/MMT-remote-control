#pragma once

#include "DataObj.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

// a  object support Processes Manipulate Functions

class RPC_Obj : public DataObj
{

public:
    RPC_Obj(std::string ID, DataType dataType, CmdType CMD, std::string data) : DataObj(ID, dataType, FuncType::RPC, CMD, data) {}

    RPC_Obj(DataType dataType, CmdType CMD, std::string data)
    {
        setID("");
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

    ~RPC_Obj()
    {

    }
};
