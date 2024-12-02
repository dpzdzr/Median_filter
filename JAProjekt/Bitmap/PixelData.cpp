#include "PixelData.h"

#include <iomanip>
#include <iostream>

void PixelData::displayPixelData() const {
    const int bytesPerPixel = bitCount / 8;
    const int rowBytes = hasPadding ? (((bitCount * width + 31)) / 32) * 4 : width * bytesPerPixel;

    int count = 0;
    for (const uint8_t pixel : data) {
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(pixel) << ' ';
        count++;
        if (count == rowBytes) {
            std::cout << std::endl;
            count = 0;
        }
        else if (count % bytesPerPixel == 0) {
            std::cout << "| ";
        }
    }
    std::cout << std::endl;
}

PixelData& PixelData::operator=(const PixelData& other) {
    if (this != &other) {
        this->width = other.width;
        this->height = other.height;
        this->bitCount = other.bitCount;
        this->data = other.data;
        this->hasPadding = other.hasPadding;
    }
    return *this;
}
