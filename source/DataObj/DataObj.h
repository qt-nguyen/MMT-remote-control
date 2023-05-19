#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <vector> 
#include <tuple>
#include <map>


enum DataType
{
	RESPONSE,
	REQUEST
};

enum FuncType
{
	IAP, // Installed Apps
	RPC, // Running proceesses
	SCR, // Screen capture
	KLG, // Keylogger
	DIR, // Directory tree
};

enum CmdType
{
	RUN, // Running a specific process / program
	KILL, // Kill process
	START, // Start logging, capturing
	STOP, // Stop logging, capturing
	SHOW, // List running processes
	DATA, // Indicates that this show response data
};

struct EnumMaps {
	static const std::map<DataType, std::string> DataTypeMap;
	static const std::map<FuncType, std::string> FunctionTypeMap;
	static const std::map<CmdType, std::string> CmdTypeMap;
};

// Instruction on how to use EnumMaps 
/*
DataType dt = RESPONSE;
std::cout << EnumMaps::DataTypeMap.at(dt) << std::endl; // Output: RESPONSE

FuncType ft = IAP;
std::cout << EnumMaps::FuncTypeMap.at(ft) << std::endl; // Output: IAP

CmdType ct = RUN;
std::cout << EnumMaps::CmdTypeMap.at(ct) << std::endl; // Output: RUN
*/





class DataObj
{
protected:
	std::string _ID;
	DataType _data_type;
	FuncType _func_type;
	CmdType _cmd_type;
	std::vector<char> _data;
public:
	// Initializer
	DataObj();
	DataObj(std::string ID, DataType data_type, FuncType func_type, CmdType cmd_type, std::vector<char> data);


	// Convert data object to string type
	std::string toString();

	// Convert data object to a file
	std::string toFile(std::string filename);

	//
	std::string getID();
	DataType getDataType();
	FuncType getFuncType();
	CmdType getCmdType();
	std::vector<char> getData();

	// return: announcement about the result of the procedure
	std::string setID(std::string new_ID);
	std::string setDataType(DataType new_data_type);
	std::string setFuncType(FuncType new_func_type);
	std::string setCmdType(CmdType new_cmd_type);
	std::string setData(std::vector<char> new_data);
	std::string setData(std::string new_data);
};
