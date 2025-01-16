#ifndef BITMAPAPI_H
#define BITMAPAPI_H

#ifdef _WIN32
#    ifdef EXPORTING_DLL
#        define DLL_API __declspec(dllexport)
#    else
#        define DLL_API __declspec(dllimport)
#    endif
#else
#    define DLL_API
#endif

class Bitmap;
struct ProcessedFragment;

extern "C" {
    DLL_API Bitmap* CreateBitmap(const char* fileName);
    DLL_API void DestroyBitmap(Bitmap* bitmap);
    DLL_API ProcessedFragment* GetProcessedFragments(Bitmap* bitmap, int numOfFragments);
    DLL_API void MergeFragments(Bitmap* bitmap, ProcessedFragment* fagments, int numOfFragments, const char* filePath);
    DLL_API bool CheckIf24Bit(Bitmap* bitmap);
    DLL_API bool CheckIfProperNumOfThreads(Bitmap* bitmap, int numOfThreads);
}

#endif //BITMAPAPI_H
