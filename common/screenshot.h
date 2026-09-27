#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <windows.h>
#include <vector>
#include <iostream>

// Returns true on success; fills `outBmpBytes` with a full BMP file image.
inline bool captureDesktopAsBMP(std::vector<char>& outBmpBytes) {
    HDC screenDC = GetDC(NULL);
    if (!screenDC) {
        std::cerr << "[SCREENSHOT] GetDC(NULL) failed." << std::endl;
        return false;
    }

    int width  = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC memDC = CreateCompatibleDC(screenDC);
    HBITMAP bitmap = CreateCompatibleBitmap(screenDC, width, height);
    if (!memDC || !bitmap) {
        std::cerr << "[SCREENSHOT] Failed to create compatible DC/bitmap." << std::endl;
        ReleaseDC(NULL, screenDC);
        return false;
    }
    HGDIOBJ oldObj = SelectObject(memDC, bitmap);

    if (!BitBlt(memDC, 0, 0, width, height, screenDC, 0, 0, SRCCOPY)) {
        std::cerr << "[SCREENSHOT] BitBlt failed: " << GetLastError() << std::endl;
        SelectObject(memDC, oldObj);
        DeleteObject(bitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);
        return false;
    }

    // Describe the pixel format we want GetDIBits to hand back: 24-bit,
    BITMAPINFOHEADER bi{};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = height;    
    bi.biPlanes = 1;
    bi.biBitCount = 24;        
    bi.biCompression = BI_RGB; 

    int rowSize = ((width * 3 + 3) / 4) * 4;
    DWORD pixelDataSize = rowSize * height;

    std::vector<char> pixelData(pixelDataSize);
    if (!GetDIBits(memDC, bitmap, 0, height, pixelData.data(),
                   reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS)) {
        std::cerr << "[SCREENSHOT] GetDIBits failed." << std::endl;
        SelectObject(memDC, oldObj);
        DeleteObject(bitmap);
        DeleteDC(memDC);
        ReleaseDC(NULL, screenDC);
        return false;
    }

    SelectObject(memDC, oldObj);
    DeleteObject(bitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screenDC);

    // Build the two BMP headers required by the file format spec.
    BITMAPFILEHEADER bfh{};
    bfh.bfType = 0x4D42; // 'BM'
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfh.bfSize = bfh.bfOffBits + pixelDataSize;

    outBmpBytes.clear();
    outBmpBytes.resize(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pixelDataSize);

    char* ptr = outBmpBytes.data();
    memcpy(ptr, &bfh, sizeof(BITMAPFILEHEADER));
    ptr += sizeof(BITMAPFILEHEADER);
    memcpy(ptr, &bi, sizeof(BITMAPINFOHEADER));
    ptr += sizeof(BITMAPINFOHEADER);
    memcpy(ptr, pixelData.data(), pixelDataSize);

    return true;
}

#endif