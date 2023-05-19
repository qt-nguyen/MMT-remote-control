#include "RPC_Obj.h"

#include <iostream>
#include <string>


RPC_Obj::RPC_Obj(DataType dataType, CmdType CMD, std::string data)
{
    setID("");
    _func_type = FuncType::RPC;
    setDataType(dataType);
    setCmdType(CMD);
    setData(data);
}

std::string RPC_Obj::toString()
{
    std::stringstream ss;

    // Convert the data to a string
    for (char c : _data)
    {
        ss << c;
    }

    return ss.str();
}

std::string RPC_Obj::toFile(std::string filename)
{
    std::string result = "";

    // Open the file
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return "Failed to open file.";
    }

    // Write the data to the file
    file.write(_data.data(), _data.size());

    // Close the file
    file.close();

    result = "Data written to file successfully.";
    return result;
}
