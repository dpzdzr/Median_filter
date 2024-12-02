#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include <memory>

#include "PixelData.h"


class BitmapManipulator {
public:
    explicit BitmapManipulator(const PixelData& pixelData)
        : pixelData(pixelData), pixelDataFramed(std::make_shared<PixelData>()) {
    };

    void addFrame();

    void removePadding();


    void displayPixelData() const { pixelData.displayPixelData(); }
    void displayPixelDataNoPadding() const { pixelDataNoPadding.displayPixelData(); }
    void displayPixelDataFramed() const { pixelDataFramed->displayPixelData(); }
    const PixelData& getPixelData() const { return pixelData; }
    const PixelData& getPixelDataNoPadding() const { return pixelDataNoPadding; }
    //const PixelData &getPixelDataFramed() const { return *pixelDataFramed; }
    std::shared_ptr<PixelData> getPixelDataFramed() const { return pixelDataFramed; }

private:
    PixelData pixelData;
    PixelData pixelDataNoPadding;
    std::shared_ptr<PixelData> pixelDataFramed;

    int calculatePaddingBytesPerRow() const;

    void copyOriginalToFramed(std::vector<uint8_t>& pixelDataFramedVec, const std::vector<uint8_t>& pixelDataNoPadding,
        int borderSize, uint32_t newWidth, uint16_t bytesPerPixel);

    void fillTopBottomBorder(std::vector<uint8_t>& pixelDataFramedVec, uint32_t newWidth, uint32_t newHeight,
        uint16_t bytesPerPixel);

    void fillLeftRightBorder(std::vector<uint8_t>& pixelDataFramedVec, uint32_t newWidth, uint32_t newHeight,
        uint16_t bytesPerPixel);
};


#endif //IMAGEMANIPULATOR_H
