#pragma once
// ---------------------------------------------------------------------------
// ip.h
// Filters are implemented using parallel processing (gdi). 
// Based on MSDN documetation.
//
// author. ebarsall
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include <time.h>
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <map>
#include <agents.h>
#include <ppl.h>

#include "tools.h"


// ---------------------------------------------------------------------------
// Operations
// ---------------------------------------------------------------------------

void getRGB(DWORD c, BYTE& r, BYTE& g, BYTE& b);

/// <summary>Set a single colore vlue from the RGB components</summary>
DWORD setRGB(BYTE r, BYTE g, BYTE b);

/// <summary>Loads an image file from disk.</summary>
Gdiplus::Bitmap* loadImage(std::wstring filename);

/// <summary>Process each pixel of an bmp object with a filter function</summary>
void processImgs(Gdiplus::Bitmap* bmp, const std::function <void (DWORD&)>& f);

// ---------------------------------------------------------------------------
// Image filters
// ---------------------------------------------------------------------------

/// <summary>Image filter. Apply a gamma correction of RGB channel.</summary>
Gdiplus::Bitmap* filterGammaCorrection(Gdiplus::Bitmap* bmp, double gamma);

/// <summary>Image filter. Apply a gamma correction of RGB channel if the RGB color is diff than the mask.</summary>
Gdiplus::Bitmap* filterGammaCorrectionMask(Gdiplus::Bitmap* bmp, double gamma, DWORD mask);

/// <summary>Image filter. Converts a given image color to grayscale.</summary>
Gdiplus::Bitmap* filterGrayscale(Gdiplus::Bitmap* bmp);

/// <summary>Image filter. Applies a sephia filter to a given image.</summary>
Gdiplus::Bitmap* filterSepia(Gdiplus::Bitmap* bmp);

/// <summary>Image Filter. Applies a color mask to each pixel of a given image.</summary>
Gdiplus::Bitmap* filterColorMask(Gdiplus::Bitmap* bmp, DWORD mask);

/// <summary>Image filter. Darken a given image (to make more obscure) by a specific amount.</summary>
Gdiplus::Bitmap* filterDarkEffect(Gdiplus::Bitmap* bmp, unsigned int percent);

/// <summary>Apply a lapacian filter.</summary>
void filterLaplacian (Gdiplus::Bitmap* bmp);

/// <summary>Get the dominant RGB component in a given image.</summary>
/// <returns>Returns the corresponding color mask for the dominant component.</returns>
DWORD getColorDominance(Gdiplus::Bitmap* bmp);

/// <summary>Get the CLS ID (class identifier) for a given MIME TYPE of an image encoder 
/// (bmp, jpeg, etc).</summary>
int getEncoderCLSID(const WCHAR* format, CLSID* pClsid);


// ---------------------------------------------------------------------------
// Image processing routines
// ---------------------------------------------------------------------------

/// <summary>Compute difference of intensity between pixels</summary>
float diffIntensity (DWORD pixel1, DWORD pixel2);

/// <summary>Return a random color.</summary>
DWORD setRandomRGB ();

/// <summary>Return the sum of each RGB component.</summary>
DWORD getSumRGBColor (DWORD color);

/// <summary>Color a bitmap according an specified pattern.</summary>
void pixels2Bmp(Gdiplus::Bitmap* bmp, DWORD *pattern);


// ---------------------------------------------------------------------------
// Tools
// ---------------------------------------------------------------------------
