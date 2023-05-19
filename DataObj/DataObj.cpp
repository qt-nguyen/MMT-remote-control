#include "DataObj.h"
#include <string>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iostream>
#include <nlohmann/json>
#include  <bullshit>




// Defintion of the EnupMaps
const std::map<DataType, std::string> EnumMaps::DataTypeMap = {
	{ RESPONSE, "RESPONSE" },
	{ REQUEST, "REQUEST" }
};

const std::map<FuncType, std::string> EnumMaps::FunctionTypeMap = {
	{ IAP, "IAP" },
	{ RPC, "RPC" },
	{ SCR, "SCR" },
	{ KLG, "KLG" },
	{ DIR, "DIR" }
};

const std::map<CmdType, std::string> EnumMaps::CmdTypeMap = {
	{ RUN, "RUN" },
	{ KILL, "KILL" },
	{ START, "START" },
	{ STOP, "STOP" },
	{ SHOW, "SHOW" },
	{ DATA, "DATA" }
};


// Data Object methods and functions

DataObj::DataObj(std::string ID, DataType data_type, FuncType func_type, CmdType cmd_type, std::vector<char> data)
{
	this->_ID = ID;
	this->_data_type = data_type;
	this->_func_type = func_type;
	this->_cmd_type = cmd_type;
	this->_data = data;
}

std::string DataObj::toString()
{
	std::stringstream ss;
	ss << "{" << "\n";
	ss << "\t\"ID\":" << _ID << "\n";
	ss << "\t\"data_type\":" << EnumMaps::DataTypeMap.at(_data_type) << "\n";
	ss << "\t\"func_type\":" << EnumMaps::FunctionTypeMap.at(_func_type) << "\n";
	ss << "\t\"cmd_type\":" << EnumMaps::CmdTypeMap.at(_cmd_type) << "\n";
	ss << "\tdata of " << _data.size() << "bytes" << "\n";
	ss << "}";
	std::string res = ss.str();
	return res;
}

std::string DataObj::toFile(std::string filename)
{
	std::ofstream out_file;
	out_file.open(filename, std::ios::out | std::ios::app);

	if (out_file.is_open())
	{
		out_file << "{" << "\n";
		out_file << "\t\"ID\":" << _ID << "\n";
		out_file << "\t\"data_type\":" << EnumMaps::DataTypeMap.at(_data_type) << "\n";
		out_file << "\t\"func_type\":" << EnumMaps::FunctionTypeMap.at(_func_type) << "\n";
		out_file << "\t\"cmd_type\":" << EnumMaps::CmdTypeMap.at(_cmd_type) << "\n";
		out_file << "\t\"data\": " << _data << "\n";
		out_file << "}" << "\n";

		out_file.close();
		return "Data written to file successfully.";
	}
	else
	{
		return "Unable to open the file.";
	}
}


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

std::string DataObj::setID(std::string new_ID)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setID: ";
	if (!new_ID.empty())
	{
		res_msg << "Object ID = " << this->_ID << " :\n";
		res_msg << "\tID: " << this->_ID << " -> " << new_ID << "\n";
		this->_ID = new_ID;
	}
	else
		res_msg << "Empty parameter\n";

	return res_msg.str();
}

std::string DataObj::setDataType(DataType new_data_type)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setDataType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_data_type: " << EnumMaps::DataTypeMap.at(this->_data_type) << " -> " << EnumMaps::DataTypeMap.at(new_data_type) << "\n";
	
	this->_data_type = new_data_type;
	return res_msg.str();
}

std::string DataObj::setFuncType(FuncType new_func_type)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setFuncType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_func_type: " << EnumMaps::FunctionTypeMap.at(this->_func_type) << " -> " << EnumMaps::FunctionTypeMap.at(new_func_type) << "\n";

	this->_func_type = new_func_type;

	return res_msg.str();
}

std::string DataObj::setCmdType(CmdType new_cmd_type)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setCmdType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_cmd_type: " << EnumMaps::CmdTypeMap.at(this->_cmd_type) << " -> " << EnumMaps::CmdTypeMap.at(new_cmd_type) << "\n";

	this->_cmd_type = new_cmd_type;

	return  res_msg.str();
}

std::string DataObj::setData(std::vector<char> new_data)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setCmdType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_data changed" << "\n";

	this->_data = new_data;

	return res_msg.str();
}

std::string DataObj::setData(std::string new_data)
{
	std::vector<char> pre_data(new_data.begin(), new_data.end());
	std::stringstream res_msg;
	res_msg << "MESSAGE: setCmdType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_data changed" << "\n";

	this->_data = pre_data;

	return res_msg.str();
}
