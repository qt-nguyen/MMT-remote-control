#pragma once

#include  "DataTransferObj.h"
#include <string>
#include <vector>
#include <memory>

class KeyloggerObject : public DataTransferObj
{
public:
	


	// Convert data object to string type
	std::string toString() = 0;

	// Convert data object to a file
	std::string toFile(std::string filename) = 0;




};