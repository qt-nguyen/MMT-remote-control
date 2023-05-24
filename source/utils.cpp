#include "utils.h"
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <tchar.h>
#include <locale>
#include <wincodec.h>
#include <vector>


namespace utils
{
    std::string CurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time);
        std::stringstream ss;
        ss << std::put_time(now_tm, "%Y-%m-%d-%H-%M-%S");
        return ss.str();
    }

    std::wstring s2ws(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    std::string ws2s(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }



    std::string tcharToString(TCHAR* tcharArray) {
        #ifdef UNICODE
                std::wstring wStr(tcharArray);
                return std::string(wStr.begin(), wStr.end());
        #else
                return std::string(tcharArray);
        #endif
    }

    
    


}