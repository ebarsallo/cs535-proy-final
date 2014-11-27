#include "ip.h"
#include "Weathering.h"

int wmain()
{
	
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Perform image processing.
	testProcessImgs(L"D:\\tmp\\samples");

	//Weathering *we = new Weathering (L"D:\\tmp\\samples\\Koala__t.jpg");
	//we->apply();


	// Shutdown GDI+.
	GdiplusShutdown(gdiplusToken);

}