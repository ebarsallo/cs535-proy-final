// ---------------------------------------------------------------------------
// Weathering.cpp
// Weathering main class
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "Weathering.h"


Weathering::Weathering(void)
{
}


Weathering::~Weathering(void)
{
}


Weathering::Weathering(wstring filename)
{
	setPhoto(filename);
}


void 
Weathering::setPhoto(wstring filename) 
{
	_img = filename;
	_bmp = loadImage(_img);
}

wstring 
Weathering::getPhoto() 
{
	return _img;
}

void
Weathering::apply (void) 
{
	Bitmap *bmp = _bmp;

	int width  = bmp->GetWidth();
	int height = bmp->GetHeight();

	// Lock the bitmap.
	BitmapData bitmapData;
	//Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	//bmp->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Get the current pixel value.
			DWORD* curr_pixel = image_bits + (y * width) + x;

			// Call the function.
			//f(*curr_pixel);
			//filterGammaCorrection(image_bits);
		}
	}

	// Unlock the bitmap.
	//bmp->UnlockBits(&bitmapData);
}