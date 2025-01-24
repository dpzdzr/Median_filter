/*
BitmapLoader Class Definition in C++
This class is responsible for loading bitmap (.bmp) files, extracting header information, and storing pixel data for further processing.

Input parameters:
- std::string fileName: The name of the bitmap file to be loaded.

Key functionalities:
- Reads and parses the bitmap file, extracting file and info headers.
- Loads pixel data from the file into a PixelData structure.
- Provides access to bitmap metadata (e.g., width, height, bit depth) and pixel data.
- Displays for debugging purposes.
*/


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
