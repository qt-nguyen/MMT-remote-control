#include "SCR_Func.h"
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


std::vector<char> SCR_Func::takeScreenshot()
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
	




	return " ";
}
