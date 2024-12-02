#include "BitmapSplitter.h"

#include <iostream>

BitmapSplitter::BitmapSplitter(const std::shared_ptr<PixelData>& pixelDataFramed)
    : pixelDataFramed(pixelDataFramed) {
}

BitmapSplitter::~BitmapSplitter() {
    for (auto& fragment : processedFragments) {
        delete[] fragment.blue;
        delete[] fragment.green;
        delete[] fragment.red;
    }
}

ProcessedFragment* BitmapSplitter::getProcessedFragments(int numOfFragments) {
    splitIntoFragments(numOfFragments);
    splitFragmentsIntoChannels();
    return processedFragments.data();
}

void BitmapSplitter::splitIntoFragments(const int numOfThreads) {
    uint32_t rowsPerFragment = pixelDataFramed->getHeight() / numOfThreads;
    uint32_t remainder = pixelDataFramed->getHeight() % numOfThreads;

    uint32_t currentRow = 0;
    for (int i = 0; i < numOfThreads; ++i) {
        uint32_t startRow = currentRow;
        uint32_t endRow = currentRow + rowsPerFragment - 1;

        if (i < remainder) {
            endRow++;
        }

        RawFragment fragment;

        uint32_t fragmentStartRow = (i == 0) ? startRow : startRow - 1;
        uint32_t fragmentEndRow = (i == numOfThreads - 1) ? endRow : endRow + 1;

        fragment.startRow = startRow;
        fragment.endRow = endRow;

        for (uint32_t row = fragmentStartRow; row <= fragmentEndRow; ++row) {
            uint32_t rowStartIndex = row * pixelDataFramed->getWidth() * 3;
            uint32_t rowEndIndex = rowStartIndex + pixelDataFramed->getWidth() * 3;

            fragment.data.insert(fragment.data.end(), pixelDataFramed->getData().begin() + rowStartIndex,
                pixelDataFramed->getData().begin() + rowEndIndex);
        }

        rawFragments.push_back(fragment);
        currentRow = endRow + 1;
    }
}

void BitmapSplitter::splitFragmentsIntoChannels() {
    const uint32_t fragmentWidth = pixelDataFramed->getWidth();

    for (auto& fragment : rawFragments) {
        const uint32_t fragmentHeight = fragment.data.size() / (fragmentWidth * 3);

        ProcessedFragment fragmentChannels{};
        const uint32_t fragmentSize = fragmentHeight * fragmentWidth;
        fragmentChannels.blue = new uint8_t[fragmentSize];
        fragmentChannels.green = new uint8_t[fragmentSize];
        fragmentChannels.red = new uint8_t[fragmentSize];

        for (size_t byte = 0; byte < fragmentSize * 3; byte += 3) {
            fragmentChannels.blue[byte / 3] = fragment.data[byte];
            fragmentChannels.green[byte / 3] = fragment.data[byte + 1];
            fragmentChannels.red[byte / 3] = fragment.data[byte + 2];
        }

        fragmentChannels.height = fragmentHeight;
        fragmentChannels.width = fragmentWidth;
        fragmentChannels.startRow = fragment.startRow;
        fragmentChannels.endRow = fragment.endRow;

        processedFragments.push_back(fragmentChannels);
    }
}
