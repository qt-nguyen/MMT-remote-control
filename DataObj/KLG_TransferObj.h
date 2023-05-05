#pragma once

#include "DataObj.h"

#include  "DataObj.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::vector<int> keylog;

class KLG_TransferObj: public DataObj
{
public:
KLG_TransferObj(std::string ID, DataType dataType, CmdType CMD, std::string data)
{
    setID(ID);
    _func_type = FuncType::RPC;
    setDataType(dataType);
    setCmdType(CMD);
    //setData(data);
}

KLG_TransferObj(DataType dataType, CmdType CMD, std::string data)
{
    setID("");
    _func_type = FuncType::RPC;
    setDataType(dataType);
    setCmdType(CMD);
    //setData(data);
}


public:
	// Convert data object to string type
	std::string toString();

	// Convert data object to a file
	std::string toFile(std::string filename);
};