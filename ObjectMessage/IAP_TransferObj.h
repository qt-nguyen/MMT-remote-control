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

public:
    // Convert data object to string type
    std::string toString();

    // Convert data object to a file
    std::string toFile(std::string filename);

    // List installed apps
    std::string listApps();

    // Start an app
    std::string startApp(std::string appName);

    // Stop an app
    std::string stopApp(std::string appName);
};