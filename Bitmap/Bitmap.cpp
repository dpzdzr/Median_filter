#include <string>
#include <optional>

#include "BitmapLoader.h"
#include "BitmapManipulator.h"
#include "BitmapSplitter.h"
#include "BitmapMerger.h"
#include "BitmapAPI.h"

class Bitmap {
public:
	explicit Bitmap(const std::string& fileName)
		: loader(fileName), manipulator(loader.getPixelData()) {
	}

	Bitmap() = delete;

	~Bitmap() = default;

	ProcessedFragment* getProcessedFragments(int numOfFragments) {
		removePadding();
		addFrame();
		return splitter->getProcessedFragments(numOfFragments);
	}

	void mergeFragments(ProcessedFragment* fragments, int numOfFragments, const char* filePath)
	{
		merger.emplace(manipulator.getPixelDataNoPadding(), loader.getHeaderData());
		merger->mergeFragments(fragments, numOfFragments, filePath);
	}

	bool checkIf24Bit() {
		return 24 == loader.getBitCount();
	}

	bool checkIfProperNumOfThreads(int numOfThreads)
	{
		uint32_t num = static_cast<uint32_t>(numOfThreads);
		return num <= loader.getHeight();
	}

private:
	BitmapLoader loader;
	BitmapManipulator manipulator;
	std::optional<BitmapMerger> merger;
	std::optional<BitmapSplitter> splitter;

	void addFrame() {
		manipulator.addFrame();
		splitter.emplace(manipulator.getPixelDataFramed());
	}

	void removePadding() {
		manipulator.removePadding();
	}
};

extern "C" {
	DLL_API Bitmap* CreateBitmap(const char* fileName) 
	{
		return new Bitmap(fileName);
	}

	DLL_API void DestroyBitmap(Bitmap* bitmap) 
	{
		delete bitmap;
	}

	DLL_API bool CheckIf24Bit(Bitmap* bitmap) {
		return bitmap->checkIf24Bit();
	}

	DLL_API bool CheckIfProperNumOfThreads(Bitmap* bitmap, int numOfThreads)
	{
		return bitmap->checkIfProperNumOfThreads(numOfThreads);
	}

	DLL_API ProcessedFragment* GetProcessedFragments(Bitmap* bitmap, int numOfFragments) 
	{
		return bitmap->getProcessedFragments(numOfFragments);
	}

	DLL_API void MergeFragments(Bitmap* bitmap, ProcessedFragment* fragments, int numOfFragments, const char* filePath)
	{
		bitmap->mergeFragments(fragments, numOfFragments, filePath);
	}
}
