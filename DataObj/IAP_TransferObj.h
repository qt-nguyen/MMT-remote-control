#pragma once

#include "DataTransferObj.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

class IAP_TransferObj : public DataTransferObj
{

public:
    IAP_TransferObj(std::string ID, DataType dataType, CmdType CMD, std::string data)
    {
        setID(ID);
        _func_type = FuncType::IAP;
        setDataType(dataType);
        setCmdType(CMD);
        setData(data);
    }

    IAP_TransferObj(DataType dataType, CmdType CMD, std::string data)
    {
        setID("");
        _func_type = FuncType::IAP;
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

    ~IAP_TransferObj()
    {

    }
};