/*
PixelData Class Definition in C++
This class encapsulates the pixel data of an image, providing storage for the pixel values, dimensions, bit depth, and padding information.
It offers member functions to manipulate and access pixel data and metadata.
Input parameters:
- std::vector<uint8_t>& data: A vector holding the raw pixel data.
- uint32_t width: The width of the image in pixels.
- uint32_t height: The height of the image in pixels.
- uint16_t bitCount: The bit depth per pixel (e.g., 24).
- bool hasPadding: Flag indicating whether the image data contains padding bytes.
*/

#ifndef PIXELDATA_H
#define PIXELDATA_H

#include <cstdint>
#include <vector>

class PixelData {
public:
    PixelData(std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint16_t bitCount, bool hasPadding)
        : data(data), width(width), height(height), bitCount(bitCount), hasPadding(hasPadding) {
    }
    PixelData() = default;

    void setWidth(const uint32_t width) { this->width = width; }
    void setHeight(const uint32_t height) { this->height = height; }
    void setBitCount(const uint16_t bitCount) { this->bitCount = bitCount; }
    void setPadding(const bool padding) { this->hasPadding = padding; }

    const std::vector<uint8_t>& getData() const { return data; }
    std::vector<uint8_t>& getData() { return data; }
    uint32_t getWidth() const { return width; }
    uint32_t getHeight() const { return height; }
    uint16_t getBitCount() const { return bitCount; }

    void displayPixelData() const;
    PixelData& operator= (const PixelData& other);

private:
    std::vector<uint8_t> data;
    uint32_t width = 0, height = 0;
    uint16_t bitCount = 0;
    bool hasPadding = false;
};

#endif //PIXELDATA_H
