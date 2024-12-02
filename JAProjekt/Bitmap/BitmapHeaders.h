#ifndef BITMAPHEADERS_H
#define BITMAPHEADERS_H

#include <cstdint>

#pragma pack(push, 1)

struct BITMAPFILEHEADER {
    uint16_t bfType; // File type is always BM which is 0x4D42
    uint32_t bfSize; // Size of the files (in bytes)
    uint16_t bfReserved1; // Reserved, always 0
    uint16_t bfReserved2; // Reserved, always 0
    uint32_t bfOffBits; // Start position of pixel data (bytes from beginning of the file)
};

#pragma pack(pop)

struct BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biPixelsPerMeter;
    int32_t biYPixelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

#endif //BITMAPHEADERS_H
