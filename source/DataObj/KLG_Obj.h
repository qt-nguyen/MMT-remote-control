#pragma once

#include "DataObj.h"

#include  "DataObj.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

class KLG_Obj : public DataObj
{
public:
    KLG_Obj(std::string ID, DataType data_type, FuncType func_type, CmdType cmd_type, std::vector<char> data) : DataObj(ID, data_type, func_type, cmd_type, data){}
    KLG_Obj(DataType dataType, FuncType func_type, CmdType cmd_type, std::string data);
    

	std::string toString();
    std::string toFile(std::string filename);


};
