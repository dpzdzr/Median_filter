#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#ifdef _WIN32
#    ifdef EXPORTING_DLL
#        define DLL_API __declspec(dllexport)
#    else
#        define DLL_API __declspec(dllimport)
#    endif
#else
#    define DLL_API
#endif

#include <cstdint>

class MedianFilter;

extern "C" {
	DLL_API void ApplyMedianFilter(uint8_t* input, uint8_t* output, uint32_t width, uint32_t height);
}

#endif