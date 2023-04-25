#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <vector> 


enum DataType
{
	RESPONSE,
	REQUEST
};

enum FunctionType
{
	IAP, // Installed Apps
	RPC, // Running proceesses
	SCR, // Screen capture
	KLG, // Keylogger
	DIR, // Directory tree
};

enum CommandType
{
	RUN, // Running a specific process / program
	KILL, // Kill process
	START, // Start logging, capturing
	STOP, // Stop logging, capturing
	SHOW, // List running processes
	DATA, // Indicates that this show response data
};



class DataTransferObj
{
protected:
	std::string _ID;
	DataType _data_type;
	FunctionType _func_type;
	CommandType _cmd_type;
	std::vector<char> _data;
public:
	// Convert data object to string type
	virtual std::string toString();

	// Convert data object to a file
	virtual std::string toFile(std::string filename);

	//
	std::string getID();
	std::string getDataType();
	std::string getFunctionType();
	std::string getCommandType();
	std::vector<char> getData();

	// return: announcement about the result of the procedure
	std::string setID(std::string new_ID);
	std::string setDataType(DataType new_data_type);
	std::string setFunctionType(FunctionType new_function_type);
	std::string setCommandType(CommandType new_command_type);
	std::string setData(std::vector<char> new_data);
};

class FAther...
	class Son: public Father


	Father* John = new Son()