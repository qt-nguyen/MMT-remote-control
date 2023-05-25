#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <locale>
#include <string>
#include <codecvt>
#include <tchar.h>
#include <wincodec.h>
#include <vector>

namespace utils
{
    std::string CurrentTime();

    std::wstring s2ws(const std::string& str);

    std::string ws2s(const std::wstring& wstr);
    
    std::string tcharToString(TCHAR* tcharArray);

    

    
}