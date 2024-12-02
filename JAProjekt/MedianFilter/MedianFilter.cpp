#ifndef MEDIANFILTERING_H
#define MEDIANFILTERING_H
#include <algorithm>

#include "MedianFilter.h"

class MedianFilter {
public:
	MedianFilter() {}

	static void applyFilter(uint8_t* input, uint8_t* output, uint32_t width, uint32_t height) {
		const int halfWindow = 1;

		uint32_t outputIndex = 0;

		for (uint32_t y = halfWindow; y < height - halfWindow; y++) {
			for (uint32_t x = halfWindow; x < width - halfWindow; x++) {
				//if (y < halfWindow || y >= height - halfWindow || x < halfWindow || x >= width - halfWindow) {
				//	//output[y * width + x] = input[y * width + x];
				//	//output[y * width + x] = 999;

				//	continue;
				//}
				uint8_t window[9];
				int count = 0;

				for (int wy = -halfWindow; wy <= halfWindow; wy++) {
					for (int wx = -halfWindow; wx <= halfWindow; wx++) {
						int nx = x + wx;
						int ny = y + wy;

						window[count++] = input[ny * width + nx];
					}
				}

				//std::sort(window, window + count);
				//output[outputIndex++] = window[4];
				int medianIndex = count / 2;
				std::nth_element(window, window + medianIndex, window + count);
				output[outputIndex++] = window[medianIndex];
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