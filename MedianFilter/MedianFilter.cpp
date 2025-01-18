/*
Median Filter Implementation in C++
This procedure applies a 3x3 median filter to
a single RGB channel of the image fragment
Input parameters:
uint8_t* input - input image pointer
uint8_t* output - output image pointer
uint32_t width  - image width
uint32_t height  - image height
*/
#ifndef MEDIANFILTERING_H
#define MEDIANFILTERING_H
#include <algorithm>

#include "MedianFilter.h"

class MedianFilter {
public:
	MedianFilter() {}

	static void applyFilter(uint8_t* input, uint8_t* output, uint32_t width, uint32_t height) {
		// misleading variable name - it equals the number pixels added to the every border
		const int halfWindow = 1;

		// counter of the output buffer current pixel
		uint32_t outputIndex = 0;

		// nested loops that iterate through the image fragment gathering surrounding pixels 
		// to make an array of 9 elements, sort it and get the median value
		for (uint32_t y = halfWindow; y < height - halfWindow; y++) {
			for (uint32_t x = halfWindow; x < width - halfWindow; x++) {
				// definition of window that will store surrounding of the current pixel
				uint8_t window[9];
				// counter for index in window
				int count = 0;

				// gathering pixels surrounding currently pointed pixels
				for (int wy = -halfWindow; wy <= halfWindow; wy++) {
					for (int wx = -halfWindow; wx <= halfWindow; wx++) {
						// x position of the currently searched pixel
						int nx = x + wx;
						// y position of the currently searched pixel
						int ny = y + wy;
						// adding calculated pixel to the window
						window[count++] = input[ny * width + nx];
					}
				}

				// sort the elements in the array
				std::sort(window, window + count);
				// save the median to the 
				output[outputIndex++] = window[4];
			}
		}
	}
};

extern "C"
{
	DLL_API void ApplyMedianFilter(uint8_t* input, uint8_t* output, uint32_t width, uint32_t height) {

		MedianFilter().applyFilter(input, output, width, height);
	}
}


#endif //MEDIANFILTERING_H