#include "BitmapManipulator.h"

int BitmapManipulator::calculatePaddingBytesPerRow() const {
    return (4 - (pixelData.getBitCount() / 8 * pixelData.getWidth()) % 4) % 4;
}

void BitmapManipulator::copyOriginalToFramed(std::vector<uint8_t>& pixelDataFramedVec,
    const std::vector<uint8_t>& pixelDataNoPaddingVec, int borderSize,
    uint32_t newWidth, uint16_t bytesPerPixel) {
    for (size_t y = 0; y < pixelDataNoPadding.getHeight(); y++) {
        for (size_t x = 0; x < pixelDataNoPadding.getWidth(); x++) {
            const uint32_t originalIndex = (y * pixelDataNoPadding.getWidth() + x) * bytesPerPixel;
            const uint32_t framedIndex = ((y + borderSize) * newWidth + (x + borderSize)) * bytesPerPixel;

            for (int channel = 0; channel < bytesPerPixel; ++channel) {
                pixelDataFramedVec[framedIndex + channel] = pixelDataNoPaddingVec[originalIndex + channel];
            }
        }
    }
}

void BitmapManipulator::fillTopBottomBorder(std::vector<uint8_t>& pixelDataFramedVec, uint32_t newWidth,
    uint32_t newHeight, uint16_t bytesPerPixel) {
    for (size_t x = 0; x < newWidth; x++) {
        //top border
        const uint32_t secondRowIndex = (newWidth + x) * bytesPerPixel;
        const uint32_t topBorderIndex = (x * bytesPerPixel);

        for (int channel = 0; channel < bytesPerPixel; ++channel) {
            pixelDataFramedVec[topBorderIndex + channel] = pixelDataFramedVec[secondRowIndex + channel];
        }

        //bottom border
        uint32_t secondLastRowIndex = ((newHeight - 2) * newWidth + x) * bytesPerPixel;
        uint32_t bottomBorderIndex = ((newHeight - 1) * newWidth + x) * bytesPerPixel;

        for (int channel = 0; channel < bytesPerPixel; ++channel) {
            pixelDataFramedVec[bottomBorderIndex + channel] = pixelDataFramedVec[secondLastRowIndex + channel];
        }
    }
}

void BitmapManipulator::fillLeftRightBorder(std::vector<uint8_t>& pixelDataFramedVec, uint32_t newWidth,
    uint32_t newHeight, uint16_t bytesPerPixel) {
    for (size_t y = 0; y < newHeight; y++) {
        //left border
        uint32_t secondColumnIndex = (y * newWidth + 1) * bytesPerPixel;
        uint32_t leftBorderIndex = y * newWidth * bytesPerPixel;

        for (int channel = 0; channel < bytesPerPixel; ++channel) {
            pixelDataFramedVec[leftBorderIndex + channel] = pixelDataFramedVec[secondColumnIndex + channel];
        }

        //right border
        uint32_t secondLastColumnIndex = (y * newWidth + (newWidth - 2)) * bytesPerPixel;
        uint32_t rightBorderIndex = (y * newWidth + (newWidth - 1)) * bytesPerPixel;

        for (int channel = 0; channel < bytesPerPixel; ++channel) {
            pixelDataFramedVec[rightBorderIndex + channel] = pixelDataFramedVec[secondLastColumnIndex + channel];
        }
    }
}

void BitmapManipulator::addFrame() {
    const int borderSize = 1;
    const uint32_t newWidth = pixelDataNoPadding.getWidth() + 2 * borderSize;
    const uint32_t newHeight = pixelDataNoPadding.getHeight() + 2 * borderSize;
    const uint16_t bytesPerPixel = pixelDataNoPadding.getBitCount() / 8;

    pixelDataFramed->setHeight(newHeight);
    pixelDataFramed->setWidth(newWidth);
    pixelDataFramed->setBitCount(pixelDataNoPadding.getBitCount());
    pixelDataFramed->setPadding(false);
    pixelDataFramed->getData().resize(newWidth * newHeight * bytesPerPixel);
    std::fill(pixelDataFramed->getData().begin(), pixelDataFramed->getData().end(), 0);
    std::vector<uint8_t>& pixelDataFramedVec = pixelDataFramed->getData();
    const std::vector<uint8_t>& pixelDataNoPaddingVec = pixelDataNoPadding.getData();

    copyOriginalToFramed(pixelDataFramedVec, pixelDataNoPaddingVec, borderSize, newWidth, bytesPerPixel);
    fillTopBottomBorder(pixelDataFramedVec, newWidth, newHeight, bytesPerPixel);
    fillLeftRightBorder(pixelDataFramedVec, newWidth, newHeight, bytesPerPixel);
}

void BitmapManipulator::removePadding() {
    const size_t paddingBytesPerRow = calculatePaddingBytesPerRow();
    const size_t rowWidthWithPadding = pixelData.getWidth() * 3 + paddingBytesPerRow;
    const size_t rowWidthWithoutPadding = pixelData.getWidth() * 3;

    if (paddingBytesPerRow == 0) {
        pixelDataNoPadding = pixelData;
    }
    else {
        pixelDataNoPadding.getData().reserve(pixelData.getHeight() * rowWidthWithoutPadding);

        for (int row = 0; row < pixelData.getHeight(); ++row) {
            const size_t rowStartIndex = row * rowWidthWithPadding;
            pixelDataNoPadding.getData().insert(pixelDataNoPadding.getData().end(),
                pixelData.getData().begin() + rowStartIndex,
                pixelData.getData().begin() + rowStartIndex + rowWidthWithoutPadding);
        }
        pixelDataNoPadding.setHeight(pixelData.getHeight());
        pixelDataNoPadding.setWidth(pixelData.getWidth());
        pixelDataNoPadding.setBitCount(pixelData.getBitCount());
        pixelDataNoPadding.setPadding(false);
    }
}