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
