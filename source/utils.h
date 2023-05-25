#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <locale>
#include <codecvt>
#include <tchar.h>


namespace utils
{
    std::string CurrentTime();

    std::wstring s2ws(const std::string& str);

    std::string ws2s(const std::wstring& wstr);
    
    std::string tcharToString(TCHAR* tcharArray);

    

    
}