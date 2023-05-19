#include "SCR_Obj.h"

std::string SCR_Obj::toString() {
    std::string strData(_data.begin(), _data.end());
    return strData;
}

std::string SCR_Obj::toBitFile(std::string filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return "Failed to open file";
    }
    file.write(_data.data(), _data.size());
    file.close();
    return "File saved successfully";
}

std::string SCR_Obj::toImageFile(std::string filename) {
    // Get the device context of the screen
    HDC hScreenDC = GetDC(NULL);

    // Get the width and height of the screen
    int width = GetDeviceCaps(hScreenDC, HORZRES);
    int height = GetDeviceCaps(hScreenDC, VERTRES);

    // Create a BITMAPFILEHEADER
    BITMAPFILEHEADER bmfHeader;
    bmfHeader.bfType = 0x4D42; // "BM"
    bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + _data.size();
    bmfHeader.bfReserved1 = 0;
    bmfHeader.bfReserved2 = 0;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Create a BITMAPINFOHEADER
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

    // Write the BMP file
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return "Failed to open file";
    }
    file.write((char*)&bmfHeader, sizeof(BITMAPFILEHEADER));
    file.write((char*)&bi, sizeof(BITMAPINFOHEADER));
    file.write(_data.data(), _data.size());
    file.close();

    // Release the device context
    ReleaseDC(NULL, hScreenDC);
    return "Image saved successfully";
}
