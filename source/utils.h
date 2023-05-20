#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <locale>
#include <codecvt>

namespace utils
{
    std::string CurrentTime();

    std::wstring s2ws(const std::string& str);

    std::string ws2s(const std::wstring& wstr);


}