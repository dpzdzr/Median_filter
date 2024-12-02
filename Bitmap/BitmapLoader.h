#ifndef BITMAPLOADER_H
#define BITMAPLOADER_H

#include <cstdint>
#include <string>
#include <vector>

#include "BitmapHeaders.h"
#include "PixelData.h"

class BitmapLoader {
public:
	explicit BitmapLoader(std::string fileName);

	bool loadBMP();

	void displayPixelData() const;

	const PixelData& getPixelData() const { return pixelData; }
	std::vector<uint8_t>& getPixelDataVec() { return pixelData.getData(); }
	uint32_t getWidth() const { return pixelData.getWidth(); }
	uint32_t getHeight() const { return pixelData.getHeight(); }
	uint16_t getBitCount() const { return pixelData.getBitCount(); }

	BITMAPFILEHEADER getFileHeader() { return fileHeader; }
	BITMAPINFOHEADER getInfoHeader() { return infoHeader; }
	std::vector<uint8_t> getHeaderData() { return headerData; }

private:
	int calculateRowBytes() const;

	BITMAPFILEHEADER fileHeader{};
	BITMAPINFOHEADER infoHeader{};
	std::vector<uint8_t> headerData;

	const std::string fileName;
	PixelData pixelData;
};


#endif //BITMAPLOADER_H
