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
