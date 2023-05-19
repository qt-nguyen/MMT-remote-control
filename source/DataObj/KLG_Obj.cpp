#include "KLG_Obj.h"

KLG_Obj::KLG_Obj(DataType dataType, FuncType func_type, CmdType cmd_type, std::string data)
{
    setID("");
    setFuncType(func_type);
    setDataType(dataType);
    setCmdType(cmd_type);
    setData(data);
}

std::string KLG_Obj::toString()
{
    std::stringstream ss;

    // Convert the data to a string
    for (char c : _data)
    {
        ss << c;
    }

    return ss.str();
}

std::string KLG_Obj::toFile(std::string filename)
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