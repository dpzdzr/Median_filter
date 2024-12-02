#ifndef BITMAPFRAGMENTS_H
#define BITMAPFRAGMENTS_H

#include <cstdint>
#include <vector>

struct RawFragment {
    std::vector<uint8_t> data;
    uint32_t startRow;
    uint32_t endRow;
};

struct ProcessedFragment {
    uint8_t* blue;
    uint8_t* green;
    uint8_t* red;
    uint32_t startRow;
    uint32_t endRow;
    uint32_t width;
    uint32_t height;
};

#endif //BITMAPFRAGMENTS_H
