#include "BitmapLoader.h"

#include <fstream>
#include <iostream>
#include <utility>

BitmapLoader::BitmapLoader(std::string fileName)
    : fileName(std::move(fileName)) {
    loadBMP();
}

void BitmapLoader::displayPixelData() const {
    pixelData.displayPixelData();
}

int32_t BitmapLoader::calculateRowBytes() const {
    return ((pixelData.getBitCount() * pixelData.getWidth() + 31) / 32) * 4;
}

bool BitmapLoader::loadBMP() {
    std::ifstream file(fileName, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << fileName << std::endl;
        return false;
    }
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.bfType != 0x4D42) {
        std::cerr << "File is not a BMP file" << std::endl;
        return false;
    }

    uint32_t headerSize = fileHeader.bfOffBits;
    headerData.resize(headerSize);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(headerData.data()), headerSize);


    pixelData.setWidth(infoHeader.biWidth);
    pixelData.setHeight(infoHeader.biHeight);
    pixelData.setBitCount(infoHeader.biBitCount);
    pixelData.setPadding(true);

    const int rowBytes = calculateRowBytes();
    const uint32_t imageSize = rowBytes * pixelData.getHeight();

    std::vector<uint8_t>& data = pixelData.getData();

    data.resize(imageSize);
    file.seekg(fileHeader.bfOffBits, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), imageSize);
    file.close();

    return true;
}
