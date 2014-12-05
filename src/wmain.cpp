#include "ip.h"
#include "Weathering.h"

int wmain()
{
	
	Gdiplus::GdiplusStartupInput	gdiStartup;
	ULONG_PTR						gdiToken;

	// Initialize GDI+.
	Gdiplus::GdiplusStartup(&gdiToken, &gdiStartup, nullptr);

	// Test samples object
	std::wstring *test = new std::wstring[10];

	test[0] = L"D:\\Code\\samples\\test\\vase.jpg";
	test[1] = L"D:\\Code\\samples\\test-vase\\vase-2592.jpg";
	test[2] = L"D:\\Code\\samples\\test-vase\\vase-1296.jpg";
	test[3] = L"D:\\Code\\samples\\test-vase\\vase-1024.jpg";
	test[4] = L"D:\\Code\\samples\\test-vase\\vase-800.jpg";
	test[5] = L"D:\\Code\\samples\\test-buddha\\buddha_green-2592.jpg";
	test[6] = L"D:\\Code\\samples\\test-buddha\\buddha-1296.jpg";
	test[7] = L"D:\\Code\\samples\\test-buddha\\buddha-1024.jpg";
	test[8] = L"D:\\Code\\samples\\test-buddha\\buddha-800.jpg";
	test[9] = L"D:\\Code\\samples\\test-appedit\\vase-2592.jpg";
	
	// Perform weathering effect
	Weathering *we = new Weathering (test[5]);
	we->apply();

	// Shutdown GDI+.
	Gdiplus::GdiplusShutdown(gdiToken);

	delete [] test;
	delete we;
}