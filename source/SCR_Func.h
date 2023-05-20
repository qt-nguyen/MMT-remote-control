#pragma once

#include "SCR_Obj.h"
#include <vector>
#include <string>
#include <windows.h>
#include <thread>

class SCR_Func {
private:
    static bool capturing;
    static std::vector<char> screenData;

    static void captureLoop() {
        while (capturing) {
            // Get the device context of the screen
            HDC hScreenDC = GetDC(NULL);
            // Create a compatible device context
            HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

            // Get the width and height of the screen
            int width = GetDeviceCaps(hScreenDC, HORZRES);
            int height = GetDeviceCaps(hScreenDC, VERTRES);

            // Create a compatible bitmap
            HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
            // Select the bitmap into the memory device context
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

            // Bit-block  from the screen device context to the memory device context
            BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
            // Select the old bitmap back into the memory device context
            SelectObject(hMemoryDC, hOldBitmap);

            // Save the bitmap to a vector of char
            BITMAPINFOHEADER bi;
            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = width;
            bi.biHeight = -height;
            bi.biPlanes = 1;
            bi.biBitCount = 32;
            bi.biCompression = BI_RGB;
            bi.biSizeImage = 0;
            bi.biXPelsPerMeter = 0;
            bi.biYPelsPerMeter = 0;
            bi.biClrUsed = 0;
            bi.biClrImportant = 0;

            DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
            screenData.resize(dwBmpSize);
            GetDIBits(hMemoryDC, hBitmap, 0, height, &screenData[0], (BITMAPINFO*)&bi, DIB_RGB_COLORS);

            DataObj* mes = new SCR_Obj(DataType::RESPONSE, CmdType::Data, screenData);

            // Clean up
            DeleteObject(hBitmap);
            DeleteDC(hMemoryDC);
            ReleaseDC(NULL, hScreenDC);

            // Sleep for a while before capturing again
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

public:
    DataObj* startCapture() {
        capturing = true;
        std::string res = "Start capturing... \n";
        std::thread captureThread(captureLoop);
        captureThread.detach();
        DataObj* MES = new SCR_Obj(DataType::RESPONSE, CmdType::START, res);
        return MES;
    }

    DataObj* stopCapture() {
        capturing = false;
        std::string res = "Capture stop \n";
        DataObj* MES = new SCR_Obj(DataType::RESPONSE, CmdType::STOP, res);
        return MES;
    }
};

bool SCR_Func::capturing = false;
std::vector<char> SCR_Func::screenData;