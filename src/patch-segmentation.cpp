#pragma once
// ---------------------------------------------------------------------------
// patch-segmentation.cpp
// Tools to patch segment an image based on [FH04] method.
//
// References:
// [FH04] Felzenswalb, P; Huttenlocher, D. Efficient graph-based image segmentation. 
//   Int. J. Comput. Vision 59 (September 2004), 167-181, 3
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "patch-segmentation.h"

BitmapData 
segmentImg (Bitmap *img) 
{

	int width  = img->GetWidth();
	int height = img->GetHeight();

	// Lock the bitmap.
	BitmapData bitmapData;
	//Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	//bmp->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Apply gamma correction

	// Build graph upon pixels


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

}
