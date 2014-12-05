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

	wstring *test = new wstring[10];

	test[0] = L"D:\\Code\\samples\\test\\vase.jpg";
	test[1] = L"D:\\Code\\samples\\test-vase\\vase-2592.jpg";
	test[2] = L"D:\\Code\\samples\\test-vase\\vase-1296.jpg";
	test[3] = L"D:\\Code\\samples\\test-vase\\vase-1024.jpg";
	test[4] = L"D:\\Code\\samples\\test-vase\\vase-800.jpg";
	test[5] = L"D:\\Code\\samples\\test-buddha\\buddha-2592.jpg";
	test[6] = L"D:\\Code\\samples\\test-buddha\\buddha-1296.jpg";
	test[7] = L"D:\\Code\\samples\\test-buddha\\buddha-1024.jpg";
	test[8] = L"D:\\Code\\samples\\test-buddha\\buddha-800.jpg";
	test[9] = L"D:\\Code\\samples\\test-appedit\\vase-2592.jpg";
	
	Weathering *we = new Weathering (test[9]);
	we->apply();

	// Shutdown GDI+.
	GdiplusShutdown(gdiplusToken);

	delete [] test;
	delete we;
}