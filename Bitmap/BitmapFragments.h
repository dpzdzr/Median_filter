/*
BitmapFragments Definitions in C++
This header file defines the structures for handling fragments of bitmap images, both in raw and processed forms.

Structures:
- RawFragment:
  - Represents a raw fragment of a bitmap image.
  - Contains a vector of pixel data and metadata about the fragment's row range (start and end).

- ProcessedFragment:
  - Represents a processed fragment of a bitmap image, separated into color channels (blue, green, and red).
  - Includes metadata about the fragment's dimensions (width and height) and row range (start and end).

These structures are used for parallel processing and manipulation of bitmap image fragments.
*/


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
