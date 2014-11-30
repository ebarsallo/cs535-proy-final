#include "ip.h"
#include "Weathering.h"

int wmain()
{
	
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Perform image processing.
	//testProcessImgs(L"D:\\tmp\\samples");

	//Weathering *we = new Weathering (L"D:\\Code\\samples\\Koala.jpg");
	Weathering *we = new Weathering (L"D:\\Code\\samples\\vase.jpg");
	we->apply();


	// Shutdown GDI+.
	GdiplusShutdown(gdiplusToken);

	delete we;
}