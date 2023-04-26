#include "DataTransferObj.h"
#include <string>
#include <tuple>
#include <sstream>

std::string DataTransferObj::getID()
{
	return this->_ID;
}

DataType DataTransferObj::getDataType()
{
	return this->_data_type;
}

FuncType DataTransferObj::getFuncType()
{
	return this->_func_type;
}

CmdType DataTransferObj::getCmdType()
{
	return this->_cmd_type;
}

std::vector<char> DataTransferObj::getData()
{
	return this->_data;
}

std::string DataTransferObj::setID(std::string new_ID)
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

std::string DataTransferObj::setDataType(DataType new_data_type)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setDataType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_data_type: " << EnumMaps::DataTypeMap.at(this->_data_type) << " -> " << EnumMaps::DataTypeMap.at(new_data_type) << "\n";
	
	this->_data_type = new_data_type;
	return res_msg.str();
}

std::string DataTransferObj::setFuncType(FuncType new_func_type)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setFuncType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_func_type: " << EnumMaps::FunctionTypeMap.at(this->_func_type) << " -> " << EnumMaps::FunctionTypeMap.at(new_func_type) << "\n";

	this->_func_type = new_func_type;

	return res_msg.str();
}

std::string DataTransferObj::setCmdType(CmdType new_cmd_type)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setCmdType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_cmd_type: " << EnumMaps::CmdTypeMap.at(this->_cmd_type) << " -> " << EnumMaps::CmdTypeMap.at(new_cmd_type) << "\n";

	this->_cmd_type = new_cmd_type;

	return  res_msg.str();
}

std::string DataTransferObj::setData(std::vector<char> new_data)
{
	std::stringstream res_msg;
	res_msg << "MESSAGE: setCmdType: ";
	res_msg << "Object ID = " << this->_ID << " :\n";
	res_msg << "\t_data changed" << "\n";

	this->_data = new_data;

	return res_msg.str();
}
