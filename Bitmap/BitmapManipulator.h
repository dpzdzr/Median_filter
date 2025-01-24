/*
BitmapManipulator Class Definition in C++
This class provides functionality for manipulating bitmap images, including adding frames, removing padding, and managing pixel data representations.

Input parameters:
- PixelData pixelData: The pixel data of the original bitmap image.

Key functionalities:
- Adds a frame around the bitmap image with customizable borders.
- Removes padding bytes from the bitmap image for precise data processing.
- Provides multiple representations of the pixel data (original, without padding, and framed).
- Includes utility functions for calculating padding and manipulating pixel data for frame generation.
*/

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
