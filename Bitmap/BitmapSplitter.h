/*
BitmapSplitter Class Definition in C++
This class is responsible for splitting a bitmap image into smaller fragments for further processing.
It utilizes the PixelData class for managing pixel data and supports both raw and processed fragment handling.

Input parameters:
- std::shared_ptr<PixelData>& pixelDataFramed: A shared pointer to the framed pixel data of the bitmap.

Key functionalities:
- Splits the bitmap into fragments using a specified number of threads.
- Separates fragments into individual color channels.
- Provides access to raw and processed fragments for further operations.
*/


#ifndef BITMAPSPLITTER_H
#define BITMAPSPLITTER_H

#include <memory>

#include "PixelData.h"
#include "BitmapFragments.h"

class BitmapSplitter {
public:
    explicit BitmapSplitter(const std::shared_ptr<PixelData>& pixelDataFramed);

    ~BitmapSplitter();

    void displayPixelDataFramed() const { pixelDataFramed->displayPixelData(); }

    std::vector<RawFragment> getRawFragments() const { return rawFragments; }

    ProcessedFragment* getProcessedFragments(int numOfFragments);

private:
    std::vector<RawFragment> rawFragments;
    std::vector<ProcessedFragment> processedFragments;
    std::shared_ptr<PixelData> pixelDataFramed;

    void splitIntoFragments(int numOfThreads);

    void splitFragmentsIntoChannels();
};

#endif //BITMAPSPLITTER_H
