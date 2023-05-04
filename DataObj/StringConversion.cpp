#include "StringConversion.h"
#include <locale>
#include <codecvt>

namespace StringConversion
{
    std::wstring s2ws(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    std::string ws2s(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }
}