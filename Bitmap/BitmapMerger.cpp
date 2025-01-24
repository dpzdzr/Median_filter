#include "BitmapMerger.h"

#include <fstream>

void BitmapMerger::mergeFragments(ProcessedFragment* fragments, int numOfFragments, const char* filePath)
{
	uint32_t currentRow = 0;

	for (int i = 0; i < numOfFragments; i++)
	{
		const ProcessedFragment& fragment = fragments[i];
		copyFragmentToBitmap(fragment, currentRow);

		currentRow += fragment.height;
	}

	saveToFile(filePath);
}


void BitmapMerger::copyFragmentToBitmap(const ProcessedFragment& fragment, uint32_t destStartRow) {
	uint32_t fragmentWidth = fragment.width;
	uint32_t fragmentHeight = fragment.height;

	uint32_t destWidth = pixelData.getWidth();

	for (uint32_t row = 0; row < fragmentHeight; ++row) {
		uint32_t destRow = destStartRow + row;

		for (uint32_t col = 0; col < fragmentWidth; ++col) {
			uint32_t destIndex = (destRow * destWidth + col) * 3;
			uint32_t srcIndex = (row * fragmentWidth + col);

			pixelData.getData()[destIndex] = fragment.blue[srcIndex];
			pixelData.getData()[destIndex + 1] = fragment.green[srcIndex];
			pixelData.getData()[destIndex + 2] = fragment.red[srcIndex];
		}
	}
}

void BitmapMerger::saveToFile(const char* filePath)
{
	std::ofstream out(filePath, std::ios::binary);

	if (!out) {
		throw std::ios_base::failure("Failed to open file for writing.");
	}

	out.write(reinterpret_cast<const char*>(headerData.data()), headerData.size());

	uint32_t rowSize = (pixelData.getWidth() * 3 + 3) / 4 * 4;
	uint32_t rawDataSize = rowSize * pixelData.getHeight();

	const uint8_t* data = pixelData.getData().data();

	std::vector<uint8_t> rowWithPadding(rowSize, 0);

	for (uint32_t row = 0; row < pixelData.getHeight(); ++row) {
		std::memcpy(rowWithPadding.data(), &data[row * pixelData.getWidth() * 3], pixelData.getWidth() * 3);

		out.write(reinterpret_cast<const char*>(rowWithPadding.data()), rowSize);
	}

	out.close();
}