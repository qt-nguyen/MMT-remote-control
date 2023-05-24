#include "DataObj.h"
#include <string>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


// Defintion of the EnupMaps
const std::map<DataType, std::string> EnumMaps::DataTypeMap = {
	{ DATA_TYPE, "DEFAULT"},
	{ RESPONSE, "RESPONSE" },
	{ REQUEST, "REQUEST" }
};

const std::map<FuncType, std::string> EnumMaps::FunctionTypeMap = {
	{ FUNC_TYPE, "DEFAULT"},
	{ IAP, "IAP" },
	{ RPC, "RPC" },
	{ SCR, "SCR" },
	{ KLG, "KLG" },
	{ DIR, "DIR" }
};

const std::map<CmdType, std::string> EnumMaps::CmdTypeMap = {
	{ CMD_TYPE, "DEFAULT"},
	{ RUN, "RUN" },
	{ KILL, "KILL" },
	{ START, "START" },
	{ STOP, "STOP" },
	{ SHOW, "SHOW" },
	{ DATA, "DATA" }
};


// Data Object methods and functions ------------------------
// Constructors
// Default constructor
DataObj::DataObj()
{
	this->_ID = "";
	this->_data_type = DATA_TYPE;
	this->_func_type = FUNC_TYPE;
	this->_cmd_type = CMD_TYPE;
	this->setData("");
}

// Full constructor with std::vector<char> data
DataObj::DataObj(std::string ID, DataType data_type, FuncType func_type, CmdType cmd_type, std::vector<char> data)
{
	this->_ID = ID;
	this->_data_type = data_type;
	this->_func_type = func_type;
	this->_cmd_type = cmd_type;
	this->_data = data;
}

// Full constructor with string data
DataObj::DataObj(std::string ID, DataType data_type, FuncType func_type, CmdType cmd_type, std::string data)
{
	this->_ID = ID;
	this->_data_type = data_type;
	this->_func_type = func_type;
	this->_cmd_type = cmd_type;
	std::vector<char> vec_data(data.begin(), data.end());
	this->_data = vec_data;
}

// Output Json String
std::string DataObj::toJsonString()
{
	nlohmann::ordered_json res_json;
	res_json["_ID"] = _ID;
	res_json["_data_type"] = EnumMaps::DataTypeMap.at(_data_type);
	res_json["_func_type"] = EnumMaps::FunctionTypeMap.at(_func_type);
	res_json["_cmd_type"] = EnumMaps::CmdTypeMap.at(_cmd_type);
	res_json["_data.size()"] = _data.size();
	std::string res = res_json.dump(4) + "\n";
	return res;
}

// Pseudo connsole format, like this:
// ID123 REQUEST DIR SHOW 233
std::string DataObj::toString()
{
	std::string delim = " ";
	std::stringstream ss;
	ss << _ID << delim;
	ss << EnumMaps::DataTypeMap.at(_data_type) << delim;
	ss << EnumMaps::FunctionTypeMap.at(_func_type) << delim;
	ss << EnumMaps::CmdTypeMap.at(_cmd_type) << delim;
	ss << _data.size() << "\n";
	auto res = ss.str();
	return res;
}

// Json format to file, appending mode
std::string DataObj::toFile(std::string filename)
{
	std::ofstream out_file;
	std::string delim = "\n";
	out_file.open(filename, std::ios::out | std::ios::app);

	if (out_file.is_open())
	{
		nlohmann::ordered_json res_json;
		res_json["_ID"] = _ID;
		res_json["_data_type"] = _data_type;
		res_json["_func_type"] = _func_type;
		res_json["_cmd_type"] = _cmd_type;
		res_json["_data"] = std::string(_data.begin(), _data.end());
		out_file << res_json.dump(4) << delim;
		out_file.close();
	}
	else
	{
		return "Unable to open the file.";
	}
}

std::string DataObj::dataToFile(std::string filename)
{
	std::ofstream out_file; 
	out_file.open(filename, std::ios::binary | std::ios::out);
	if (out_file.is_open())
	{
		out_file.write(this->getDAta_CString(), _data.size());
		out_file.close();
		return "Raw written to file " + filename + " successfully.";
	}
	else
		return "Unable to write data to file " + filename;
}

// Getters
std::string DataObj::getID()
{
	return this->_ID;
}

DataType DataObj::getDataType()
{
	return this->_data_type;
}

FuncType DataObj::getFuncType()
{
	return this->_func_type;
}

CmdType DataObj::getCmdType()
{
	return this->_cmd_type;
}

std::vector<char> DataObj::getData()
{
	return this->_data;
}

std::string DataObj::getData_String()
{
	auto res = std::string(_data.begin(), _data.end());
	return res;
}

char* DataObj::getDAta_CString()
{
	char* res = reinterpret_cast<char*> (&_data[0]);
	return res;
}


// Setters
std::string DataObj::setID(std::string new_ID)
{
	std::stringstream res_msg;
	if (!new_ID.empty())
	{
		res_msg << "Object with ID \"" << this->_ID << "\":\n";
		res_msg << "\t_ID: \"" << this->_ID << "\" ---> \"" << new_ID << "\"\n";
		this->_ID = new_ID;
	}
	else
		res_msg << "Empty parameter\n";

	return res_msg.str();
}

std::string DataObj::setDataType(DataType new_data_type)
{
	std::stringstream res_msg;
	res_msg << "Object with ID \"" << this->_ID << "\":\n";
	res_msg << "\t_data_type: \"" << EnumMaps::DataTypeMap.at(this->_data_type) << "\"--->\"" << EnumMaps::DataTypeMap.at(new_data_type) << "\"\n";
	
	this->_data_type = new_data_type;
	return res_msg.str();
}

std::string DataObj::setFuncType(FuncType new_func_type)
{
	std::stringstream res_msg;
	res_msg << "Object with ID \"" << this->_ID << "\":\n";
	res_msg << "\t_func_type: \"" << EnumMaps::FunctionTypeMap.at(this->_func_type) << "\" ---> \"" << EnumMaps::FunctionTypeMap.at(new_func_type) << "\"\n";

	this->_func_type = new_func_type;

	return res_msg.str();
}

std::string DataObj::setCmdType(CmdType new_cmd_type)
{
	std::stringstream res_msg;
	res_msg << "Object with ID \"" << this->_ID << "\":\n";
	res_msg << "\t_cmd_type: \"" << EnumMaps::CmdTypeMap.at(this->_cmd_type) << " ---> \"" << EnumMaps::CmdTypeMap.at(new_cmd_type) << "\"\n";

	this->_cmd_type = new_cmd_type;

	return  res_msg.str();
}

std::string DataObj::setData(std::vector<char> new_data)
{
	std::stringstream res_msg;
	res_msg << "Object with ID \"" << this->_ID << "\":\n";
	res_msg << "\t_data changed" << "\n";

	this->_data = new_data;

	return res_msg.str();
}

std::string DataObj::setData(std::string new_data)
{
	std::vector<char> pre_data(new_data.begin(), new_data.end());
	std::stringstream res_msg;
	res_msg << "Object with ID \"" << this->_ID << "\":\n";
	res_msg << "\t_data changed" << "\n";
	this->_data = pre_data;
	return res_msg.str();
}

char* DataObj::serialize(size_t& size) const
{
	std::stringstream ss;
	ss << _ID.size() << " " << _ID << " ";
	ss << static_cast<int>(_data_type) << " ";
	ss << static_cast<int>(_func_type) << " ";
	ss << static_cast<int>(_cmd_type) << " ";
	ss << _data.size() << " ";
	for (char c : _data)
		ss << c;
	std::string str = ss.str();
	size = str.size();
	char* buffer = new char[size];
	std::memcpy(buffer, str.data(), size);
	return buffer;
}

DataObj DataObj::deserialize(const char* buffer, size_t size)
{
	std::string str(buffer, size);
	std::stringstream ss(str);
	size_t idSize;
	ss >> idSize;
	ss.ignore();
	std::string ID(idSize, '\0');
	ss.read(&ID[0], idSize);
	int data_type, func_type, cmd_type;
	ss >> data_type >> func_type >> cmd_type;
	size_t dataSize;
	ss >> dataSize;
	std::vector<char> data(dataSize);
	ss.ignore(); // ignore the space character after the data size
	ss.read(data.data(), dataSize);
	return DataObj(ID, static_cast<DataType>(data_type), static_cast<FuncType>(func_type), static_cast<CmdType>(cmd_type), data);
}

