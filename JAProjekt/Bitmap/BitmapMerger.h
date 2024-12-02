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
	//void saveToTxtFile();
};

#endif
