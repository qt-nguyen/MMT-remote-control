#pragma once

#include "DataObj.h"
#include <fstream>
#include <windows.h>

class SCR_Obj : public DataObj
{
public:
    SCR_Obj(std::string ID, DataType data_type, CmdType cmd_type, std::string data)
        : DataObj(ID, data_type, FuncType::SCR, cmd_type, data) {}


    SCR_Obj(DataType dataType, CmdType CMD, std::string data)
    {
        setID("");
        _func_type = FuncType::SCR;
        setDataType(dataType);
        setCmdType(CMD);
        setData(data);
    }
    std::string toString();
    std::string toBitFile(std::string filename);
    std::string toImageFile(std::string filename);
};