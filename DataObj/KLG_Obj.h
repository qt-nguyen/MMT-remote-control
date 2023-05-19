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
	KLG_Obj(DataType dataType, FuncType func_type, std::string data) 
    {
        setFuncType(func_type);
        setDataType(dataType);
        setData(data);
    }

	std::string toString();
    std::string toFile(std::string filename);


};
