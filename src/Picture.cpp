// ---------------------------------------------------------------------------
// Picture.cpp
// Image class using a Bitmap object to represent the image.
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "Picture.h"


/// Picture
/// <summary>Constructor of a image store in disk</summary>
Picture::Picture(std::wstring filename)
{
	_bmp = loadImage(filename);
	_fn  = filename;

	_h = _bmp->GetHeight();
	_w = _bmp->GetWidth();
	//_arr  = new DWORD[_h*_w];
}


///<summary>Destroy a Picture object.</summary>
Picture::~Picture()
{
	//if (_bmp != nullptr) delete _bmp;
}

/// getRGBArray
/// <summary>Get an array with the RGB intensity of each pixel.</summary>
void
Picture::getRGBArray(DWORD *arr)
{
	int height = _h;
	int width  = _w;

	
	// Lock the bitmap.
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, width, height);
	_bmp->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	// Call the function for each pixel in the image.
	Concurrency::parallel_for (0, height, [&, width](int y)
	{      
		for (int x = 0; x < width; ++x)
		{
			// Get the current pixel value.
			DWORD* pixel = image_bits + (y * width) + x;
			arr[(y * width) + x] = *pixel;
		}
	});

	// Unlock the bitmap.
	_bmp->UnlockBits(&bitmapData);
	//return _arr;
}


/// save
/// <summary>Save a picture to disk</summary>
void
Picture::save()
{
	std::wstring fn = _fn;
	CLSID bmpClsid;

	getEncoderCLSID(L"image/jpeg", &bmpClsid);
	fn.replace(fn.rfind(L"."), 1, L"_out.");

    _bmp->Save(fn.c_str(), &bmpClsid);
}


/// getFilename
/// <summary>Return picture's filename.</summary>
std::wstring 
Picture::getFilename(void)
{
	return _fn;
}


/// getBitmap
/// <summary>Return the Bitmap object.</summary>
Gdiplus::Bitmap* 
Picture::getBitmap(void)
{
	return _bmp;
}


/// getHeight
/// <summary>Return the image's heigth </summary>
int 
Picture::getHeight()
{
	return _h;
}


/// getWidth
/// <summary>Return the image's width </summary>
int 
Picture::getWidth()
{
	return _w;
}

