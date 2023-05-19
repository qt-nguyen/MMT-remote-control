#pragma once

#include "DataObj.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

//a transfer object support for function dealing with installed app

class IAP_Obj : public DataObj
{

public:
    IAP_Obj(std::string ID, DataType dataType, CmdType CMD, std::string data) : DataObj(ID, dataType, FuncType::IAP, CMD, data) {}

    IAP_Obj(DataType dataType, CmdType CMD, std::string data);

public:
    // Convert data object to string type
    std::string toString();

    // Convert data object to a file
    std::string toFile(std::string filename);

    ~IAP_Obj()
    {

    }
};
