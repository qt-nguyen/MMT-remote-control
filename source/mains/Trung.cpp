///*
//This is Trung's demo to test functionalities and dependencies
//Please exclude this file from project if you want to run your own main demo
//Don't change this file  without informing the author
//*/
//#include <nlohmann/json.hpp>
//#include <iostream>
//#include <string>
//#include <vector>
//#include "../DataObj/DataObj.h"
//#include "../utils.h"
//#include <chrono>
//#include <ctime>
//#include <iomanip>
//#include <filesystem>
//#include "../DIR_Func.h"
//#include "../IAP_Func.h"
//#include "../RPC_Func.h"
//namespace fs = std::filesystem;
//using json = nlohmann::json;
//
//using namespace std;
//
//
//
//#include <tlhelp32.h>
//#include <Windows.h>
//#include <processthreadsapi.h>
//#include <process.h>
//
//int main() {
//    RPC_Func rpc;
//    auto res = rpc.listPrcs();
//    std::cout << res->getData_String();
//
//
//    system("pause");
//    return 0;
//}


#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "../utils.h"

#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <vector>
#include <string>


//void startProcess(std::string processName) {
//    STARTUPINFO si;
//    PROCESS_INFORMATION pi;
//
//    ZeroMemory(&si, sizeof(si));
//    si.cb = sizeof(si);
//    ZeroMemory(&pi, sizeof(pi));
//
//    // Convert processName to TCHAR array
//    TCHAR processNameTCHAR[MAX_PATH];
//#ifdef UNICODE
//    MultiByteToWideChar(CP_ACP, 0, processName.c_str(), -1, processNameTCHAR, MAX_PATH);
//#else
//    strcpy_s(processNameTCHAR, MAX_PATH, processName.c_str());
//#endif
//
//    // Start the child process.
//    if (!CreateProcess(NULL,   // No module name (use command line)
//        processNameTCHAR,      // Command line
//        NULL,           // Process handle not inheritable
//        NULL,           // Thread handle not inheritable
//        FALSE,          // Set handle inheritance to FALSE
//        0,              // No creation flags
//        NULL,           // Use parent's environment block
//        NULL,           // Use parent's starting directory 
//        &si,            // Pointer to STARTUPINFO structure
//        &pi)           // Pointer to PROCESS_INFORMATION structure
//        )
//    {
//        printf("CreateProcess failed (%d).\n", GetLastError());
//        return;
//    }
//
//    // Close process and thread handles.
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//}
//
//int main()
//{
//    startProcess(R"(msinfo32)");
//
//
//    system("pause");
//}

#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <sstream>
#include "../utils.h"
#include <d3d9.h>
#include <wincodec.h>
#include <wingdi.h>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Gdiplus.lib")

int main()
{
	HDC hScreenDC = GetDC(nullptr); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	// Get width and height of the current screen
	int width = GetDeviceCaps(hScreenDC, HORZRES);
	int height = GetDeviceCaps(hScreenDC, VERTRES);

	
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);

	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Save the bitmap to a file
	Gdiplus::Bitmap bitmap(hBitmap, nullptr);
	CLSID clsid;
	CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &clsid); // PNG encoder
	bitmap.Save(L"screenshot.png", &clsid);

	// Convert the bitmap to a std::vector<char>
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height; // top-down
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	std::vector<char> data(width * height * 4);
	GetDIBits(hMemoryDC, hBitmap, 0, height, data.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// Use the data
	// ...

	// Shutdown GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);



}