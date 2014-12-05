// ---------------------------------------------------------------------------
// ip.cpp
// Image processing.
// Filters are implemented using parallel processing and GDI+ library. 
// Based on MSDN documentation.
//
// author. ebarsall
//
// Remarks
// On command line compile using: 
// cl.exe /DUNICODE /EHsc ip.cpp /link gdiplus.lib
// ---------------------------------------------------------------------------
#include "ip.h"


/// <summary>Retrieves the RGB components from a given color value.</summary>
void 
getRGB(DWORD c, BYTE& r, BYTE& g, BYTE& b)
{
	r = static_cast<BYTE>((c & 0x00ff0000) >> 16);
	g = static_cast<BYTE>((c & 0x0000ff00) >> 8);
	b = static_cast<BYTE>((c & 0x000000ff));
}


/// <summary>Set a single colore value from the RGB components</summary>
DWORD 
setRGB(BYTE r, BYTE g, BYTE b)
{
	return (r<<16) | (g<<8) | (b);
}


/// <summary>Loads an image file from disk.</summary>
Gdiplus::Bitmap*
loadImage(std::wstring filename) {
	Gdiplus::Bitmap *bmp = new Gdiplus::Bitmap (filename.c_str());

	return bmp;
}


/// <summary>Process each pixel of an bmp object with a filter function.</summary>
void 
processImg(Gdiplus::Bitmap* bmp, const std::function <void (DWORD&)>& f)
{
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();

	// Lock the bitmap.
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	bmp->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	// Call the function for each pixel in the image.
	Concurrency::parallel_for (0, height, [&, width](int y)
	{      
		for (int x = 0; x < width; ++x)
		{
			// Get the current pixel value.
			DWORD* curr_pixel = image_bits + (y * width) + x;

			// Call the function.
			f(*curr_pixel);
		}
	});

	// Unlock the bitmap.
	bmp->UnlockBits(&bitmapData);
}


/// <summary>Image filter. Converts a given image color to grayscale.</summary>
Gdiplus::Bitmap* 
filterGrayscale(Gdiplus::Bitmap* bmp) 
{
	processImg(bmp, 
		[](DWORD& color) {
			BYTE r, g, b;

			getRGB(color, r, g, b);

			// Set each color component to the average of 
			// the original components.
			BYTE c = (static_cast<WORD>(r) + g + b) / 3;
			color = setRGB(c, c, c);
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a gamma correction of RGB channel.</summary>
Gdiplus::Bitmap* 
filterGammaCorrection(Gdiplus::Bitmap* bmp, double gamma) 
{
	processImg(bmp, 
		[gamma](DWORD& color) {
			BYTE r, g, b;
			double gv = gamma;

			getRGB(color, r, g, b);

			// Apply gamma correction to the pixel.
			BYTE r1 = static_cast<BYTE>(pow(r, gv));
			BYTE g1 = static_cast<BYTE>(pow(g, gv));
			BYTE b1 = static_cast<BYTE>(pow(b, gv));

			color = setRGB(r1, g1, b1);
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a gamma correction of RGB channel if the RGB color is diff than the mask.</summary>
Gdiplus::Bitmap* 
filterGammaCorrectionMask(Gdiplus::Bitmap* bmp, double gamma, DWORD mask) 
{
	processImg(bmp, 
		[gamma, mask](DWORD& color) {
			BYTE r0, g0, b0;
			BYTE rm, gm, bm;
			double gv = gamma;

			getRGB(color, r0, g0, b0);
			getRGB(mask, rm, gm, bm);

			if (r0 == rm && g0 == gm && b0 == bm)
				color = color;
			else {
				// Apply gamma correction to the pixel.
				BYTE r1 = static_cast<BYTE>(pow(r0, gv));
				BYTE g1 = static_cast<BYTE>(pow(g0, gv));
				BYTE b1 = static_cast<BYTE>(pow(b0, gv));

				color = setRGB(r1, g1, b1);
			}
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a sephia filter to a given image.</summary>
Gdiplus::Bitmap* 
filterSepia(Gdiplus::Bitmap* bmp) 
{
	processImg(bmp, 
		[](DWORD& color) {
			BYTE r0, g0, b0;
			getRGB(color, r0, g0, b0);

			WORD r1 = static_cast<WORD>((r0 * .393) + (g0 *.769) + (b0 * .189));
			WORD g1 = static_cast<WORD>((r0 * .349) + (g0 *.686) + (b0 * .168));
			WORD b1 = static_cast<WORD>((r0 * .272) + (g0 *.534) + (b0 * .131));

			color = setRGB(min(0xff, r1), min(0xff, g1), min(0xff, b1));
		}
	);
	return bmp;
}


/// <summary>Image Filter. Applies a color mask to each pixel of a given image.</summary>
Gdiplus::Bitmap* 
filterColorMask(Gdiplus::Bitmap* bmp, DWORD mask)
{
	processImg(bmp, 
		[mask](DWORD& color) {
			color = color & mask;
		}
	);
	return bmp;
}

/// <summary>Image filter. <b>Darken</b> a given image (to make more obscure) by a specific amount.</summary>
Gdiplus::Bitmap* 
filterDarkEffect(Gdiplus::Bitmap* bmp, unsigned int percent)
{
	if (percent > 100)
		throw std::invalid_argument("filterDarkEffect: Percent must be less or equal than 100.");

	double factor = percent / 100.0;

	processImg(bmp, 
		[factor](DWORD& color) {
			BYTE r, g, b;
			getRGB(color, r, g, b);
			r = static_cast<BYTE>(factor*r);
			g = static_cast<BYTE>(factor*g);
			b = static_cast<BYTE>(factor*b);
			color = setRGB(r, g, b);
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a <b>Gaussian</b> blur to a given image.</summary>
Gdiplus::Bitmap* 
filterGaussianBlur(Gdiplus::Bitmap* bmp, DWORD mask)
{
	/* TODO */
}


/// <summary>Apply a laplacian filter.</summary>
void
filterLaplacian (Gdiplus::Bitmap* bmp)
{
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();

	// Lock the bitmap.
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	bmp->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	// iterate each pixel.
	for (int y=1; y < height-1; ++y)
	{      
		for (int x=1; x < width-1; ++x)
		{
			// Get the current pixel value.
			DWORD* curr_pixel = image_bits + (y * width) + x;

			// Apply laplacian filter per RGB component.
			BYTE r,g,b;
			BYTE r1,g1,b1;
			BYTE r2,g2,b2;
			BYTE r3,g3,b3;
			BYTE r4,g4,b4;

			getRGB(*curr_pixel, r, g, b);
			getRGB(*(image_bits + (y * width) + x-1), r1, g1, b1);
			getRGB(*(image_bits + (y * width) + x+1), r1, g1, b1);
			getRGB(*(image_bits + ((y-1) * width) + x), r1, g1, b1);
			getRGB(*(image_bits + ((y+1) * width) + x), r1, g1, b1);

			BYTE dxr = r1 + r2 - 2*r;	BYTE dxg = g1 + g2 - 2*g;	BYTE dxb = b1 + b2 - 2*b;
			BYTE dyr = r3 + r4 - 2*r;	BYTE dyg = g3 + g4 - 2*g;	BYTE dyb = b3 + b4 - 2*b;

			*curr_pixel = setRGB(dxr + dyr, dxg + dyg, dxb + dyb);
		}
	}
	
	// Unlock the bitmap.
	bmp->UnlockBits(&bitmapData);
}


/// <summary>Get the dominant RGB component in a given image.</summary>
/// <returns>Returns the corresponding color mask for the dominant component.</returns>
DWORD 
getColorDominance(Gdiplus::Bitmap* bmp)
{
	// The ProcessImage function processes the image in parallel.
	// The following combinable objects enable the callback function
	// to increment the color counts without using a lock.
	Concurrency::combinable<unsigned int> rc;
	Concurrency::combinable<unsigned int> gc;
	Concurrency::combinable<unsigned int> bc;

	processImg(bmp, 
		[&](DWORD& color) {
			BYTE r, g, b;
			getRGB(color, r, g, b);
			if (r >= g && r >= b)
				rc.local()++;
			else if (g >= r && g >= b)
				gc.local()++;
			else
				bc.local()++;
		}
	);

	// Check which color (r,g,b) is dominant and return the corresponding color mask.
	unsigned int r = rc.combine(std::plus<unsigned int>());
	unsigned int g = gc.combine(std::plus<unsigned int>());
	unsigned int b = bc.combine(std::plus<unsigned int>());

	if (r + r >= g + b)
		return 0x00ff0000;
	else if (g + g >= r + b)
		return 0x0000ff00;
	else
		return 0x000000ff;
}


/// <summary>Get the CLS ID (class identifier) for a given MIME TYPE of an image encoder 
/// (bmp, jpeg, etc).</summary>
int 
getEncoderCLSID(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Error

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == nullptr)
		return -1;  // Error

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // OK
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Error
}


/// diff
/// <summary>Compute difference of intensity between pixels</summary>
float
diffIntensity (DWORD pixel1, DWORD pixel2)
{

	BYTE r1, g1, b1;
	BYTE r2, g2, b2;

	getRGB(pixel1, r1, g1, b1);
	getRGB(pixel2, r2, g2, b2);

	return euclideanDst(r1, g1, b1, r2, g2, b2);
}
 

/// randomRGB
/// <summary>Return a random RGB color.</summary>
DWORD
setRandomRGB ()
{
	BYTE r, g, b;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;

	return setRGB(r, g, b);
}


/// <summary>Return the sum of each RGB component.</summary>
DWORD
getSumRGBColor (DWORD color)
{
	BYTE r, g, b;

	getRGB(color, r, g, b);
	return r + g + b;
}


/// <summary>Color a bitmap according an specified pattern.</summary>
void 
pixels2Bmp(Gdiplus::Bitmap* bmp, DWORD *pattern)
{
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();

	// Lock the bitmap.
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	bmp->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	// iterate each pixel.
	for (int y=0; y < height; ++y)
	{      
		for (int x=0; x < width; ++x)
		{
			// Get the current pixel value.
			DWORD* curr_pixel = image_bits + (y * width) + x;
			*curr_pixel = pattern[(y * width) + x];
		}
	}
	
	// Unlock the bitmap.
	bmp->UnlockBits(&bitmapData);
}


