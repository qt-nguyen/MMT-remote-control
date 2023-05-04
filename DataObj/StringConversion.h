#pragma once
#pragma warning(disable : 4996)
#include <string>

namespace StringConversion
{
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& wstr);
}