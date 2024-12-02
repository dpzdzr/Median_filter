#include "BitmapMerger.h"

#include <fstream>

void BitmapMerger::mergeFragments(ProcessedFragment* fragments, int numOfFragments, const char* filePath)
{	
	uint32_t currentRow = 0;

	for (int i = 0; i < numOfFragments; i++)
	{
		const ProcessedFragment& fragment = fragments[i];
		copyFragmentToBitmap(fragment, currentRow);

		currentRow += fragment.height;
	}

	saveToFile(filePath);
	//saveToTxtFile();
}
//
//void BitmapMerger::copyFragmentToBitmap(const ProcessedFragment& fragment, uint32_t destStartRow) {
//	uint32_t fragmentWidth = fragment.width - 2;  // Usuñ pierwsz¹ i ostatni¹ kolumnê
//	uint32_t fragmentHeight = fragment.height - 2;  // Usuñ pierwszy i ostatni wiersz
//
//	uint32_t destWidth = pixelData.getWidth();
//
//	for (uint32_t row = 1; row < fragment.height - 1; ++row) // Pomiñ pierwszy i ostatni wiersz
//	{
//		uint32_t destRow = destStartRow + (row - 1);
//
//		for (uint32_t col = 1; col < fragment.width - 1; ++col) // Pomiñ pierwsz¹ i ostatni¹ kolumnê
//		{
//			uint32_t destIndex = (destRow * pixelData.getWidth() + (col - 1)) * 3;
//
//			uint32_t srcIndex = (row * fragment.width + col);
//
//			pixelData.getData()[destIndex] = fragment.blue[srcIndex];
//			pixelData.getData()[destIndex + 1] = fragment.green[srcIndex];
//			pixelData.getData()[destIndex + 2] = fragment.red[srcIndex];
//		}
//	}
//}

void BitmapMerger::copyFragmentToBitmap(const ProcessedFragment& fragment, uint32_t destStartRow) {
	uint32_t fragmentWidth = fragment.width;  // Fragment bez ramek
	uint32_t fragmentHeight = fragment.height;  // Fragment bez ramek

	uint32_t destWidth = pixelData.getWidth();  // Docelowa szerokoœæ obrazu

	for (uint32_t row = 0; row < fragmentHeight; ++row) {
		uint32_t destRow = destStartRow + row;

		for (uint32_t col = 0; col < fragmentWidth; ++col) {
			uint32_t destIndex = (destRow * destWidth + col) * 3;  // *3 dla RGB w docelowym buforze
			uint32_t srcIndex = (row * fragmentWidth + col);  // Bez *3, poniewa¿ kana³y s¹ oddzielne

			// Kopiowanie danych pikseli (R, G, B)
			pixelData.getData()[destIndex] = fragment.blue[srcIndex];       // Kana³ niebieski
			pixelData.getData()[destIndex + 1] = fragment.green[srcIndex]; // Kana³ zielony
			pixelData.getData()[destIndex + 2] = fragment.red[srcIndex];   // Kana³ czerwony
		}
	}
}

void BitmapMerger::saveToFile(const char* filePath)
{
	std::ofstream out(filePath, std::ios::binary);

	if (!out) {
		throw std::ios_base::failure("Failed to open file for writing.");
	}

	// Zapis nag³ówków
	out.write(reinterpret_cast<const char*>(headerData.data()), headerData.size());

	// Obs³uga zapisu pikseli z uwzglêdnieniem wyrównania wierszy (padding)
	uint32_t rowSize = (pixelData.getWidth() * 3 + 3) / 4 * 4;
	uint32_t rawDataSize = rowSize * pixelData.getHeight();

	const uint8_t* data = pixelData.getData().data();

	std::vector<uint8_t> rowWithPadding(rowSize, 0);

	for (uint32_t row = 0; row < pixelData.getHeight(); ++row) {
		// Przepisujemy dane wiersza
		std::memcpy(rowWithPadding.data(), &data[row * pixelData.getWidth() * 3], pixelData.getWidth() * 3);

		// Zapisujemy wiersz z paddingiem
		out.write(reinterpret_cast<const char*>(rowWithPadding.data()), rowSize);
	}

	out.close();
}

//void BitmapMerger::saveToTxtFile()
//{
//	std::ofstream out(R"(C:\Users\Daniel\Desktop\file.txt)");
//
//	if (!out) {
//		throw std::ios_base::failure("Failed to open file for writing.");
//	}
//
//	uint32_t width = pixelData.getWidth();
//	uint32_t height = pixelData.getHeight();
//	const uint8_t* data = pixelData.getData().data();
//
//	for (uint32_t row = 1; row < height - 1; ++row) // Pomiñ pierwszy i ostatni wiersz
//	{
//		for (uint32_t col = 1; col < width - 1; ++col) // Pomiñ pierwsz¹ i ostatni¹ kolumnê
//		{
//			uint32_t pixelIndex = (row * width + col) * 3; // Indeks dla danych RGB
//
//			uint8_t blue = data[pixelIndex];
//			uint8_t green = data[pixelIndex + 1];
//			uint8_t red = data[pixelIndex + 2];
//
//			// Zapisujemy piksel w formacie "R:G:B"
//			out << static_cast<int>(red) << ":"
//				<< static_cast<int>(green) << ":"
//				<< static_cast<int>(blue);
//
//			if (col < width - 2) { // Sprawdzenie przedostatniej kolumny
//				out << " "; // Separator miêdzy pikselami w wierszu
//			}
//		}
//		out << "\n"; // Separator miêdzy wierszami
//	}
//
//	out.close();
//}

