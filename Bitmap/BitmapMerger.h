/*
BitmapMerger Class Definition in C++
This class is designed for merging processed bitmap fragments back into a single bitmap image.
It supports handling pixel data, bitmap headers, and saving the merged image to a file.

Input parameters:
- PixelData pixelData: The pixel data structure for storing and managing image data.
- std::vector<uint8_t> headerData: A vector containing the header information of the bitmap.

Key functionalities:
- Merges multiple processed fragments into the final bitmap image.
- Copies individual fragments into the appropriate position in the destination bitmap.
- Saves the merged bitmap image to a specified file path.
*/

#ifndef BITMAPMERGER_H
#define BITMAPMERGER_H

#include "BitmapFragments.h"
#include "PixelData.h"
#include "BitmapHeaders.h"

class BitmapMerger
{
public:
	explicit BitmapMerger(PixelData pixelData, std::vector<uint8_t> headerData)
		: pixelData(pixelData), headerData(headerData) {
	}

	~BitmapMerger() = default;

	void mergeFragments(ProcessedFragment* fragments, int numOfFragments, const char* filePath);

private:
	PixelData pixelData;
	std::vector<uint8_t> headerData;
	void copyFragmentToBitmap(const ProcessedFragment& fragment, uint32_t destStartRow);
	void saveToFile(const char* filePath);
};

#endif
