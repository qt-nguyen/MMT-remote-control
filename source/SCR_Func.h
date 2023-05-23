#pragma once
#include "DataObj/DataObj.h"
#include "utils.h"

#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <sstream>
#include <d3d9.h>
#include <wincodec.h>
#include <wingdi.h>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Gdiplus.lib")


class SCR_Func
{
public:

	std::string start();
	std::string stop();

	std::vector<char> takeScreenshot();


};