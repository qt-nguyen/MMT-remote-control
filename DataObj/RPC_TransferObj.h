#pragma once

#include "DataTransferObj.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

class RPC_TransferObj : public DataTransferObj
{

public:
    RPC_TransferObj(std::string ID, DataType dataType, CmdType CMD, std::string data)
    {
        setID(ID);
        _func_type = FuncType::RPC;
        setDataType(dataType);
        setCmdType(CMD);
        setData(data);
    }

    RPC_TransferObj( DataType dataType, CmdType CMD, std::string data)
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

    ~RPC_TransferObj()
    {

    }
};