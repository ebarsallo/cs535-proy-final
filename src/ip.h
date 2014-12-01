#pragma once
// ---------------------------------------------------------------------------
// ip.h
// Image processing
//
// Remarks
// Header file
// ---------------------------------------------------------------------------

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <map>
#include <agents.h>
#include <ppl.h>

#include "tools.h"

using namespace Concurrency;
using namespace Gdiplus;
using namespace std;

// ---------------------------------------------------------------------------
// Operations
// ---------------------------------------------------------------------------

void getRGB(DWORD c, BYTE& r, BYTE& g, BYTE& b);

/// <summary>Set a single colore vlue from the RGB components</summary>
DWORD setRGB(BYTE r, BYTE g, BYTE b);

/// <summary>Loads an image file from disk.</summary>
Bitmap* loadImage(wstring filename);

/// <summary>Process each pixel of an bmp object with a filter function</summary>
void processImgs(Bitmap* bmp, const function <void (DWORD&)>& f);

// ---------------------------------------------------------------------------
// Image filters
// ---------------------------------------------------------------------------

/// <summary>Image filter. Apply a gamma correction of RGB channel.</summary>
Bitmap* filterGammaCorrection(Bitmap* bmp, double gamma);

/// <summary>Image filter. Converts a given image color to grayscale.</summary>
Bitmap* filterGrayscale(Bitmap* bmp);

/// <summary>Image filter. Applies a sephia filter to a given image.</summary>
Bitmap* filterSepia(Bitmap* bmp);

/// <summary>Image Filter. Applies a color mask to each pixel of a given image.</summary>
Bitmap* filterColorMask(Bitmap* bmp, DWORD mask);

/// <summary>Image filter. Darken a given image (to make more obscure) by a specific amount.</summary>
Bitmap* filterDarkEffect(Bitmap* bmp, unsigned int percent);

/// <summary>Get the dominant RGB component in a given image.</summary>
/// <returns>Returns the corresponding color mask for the dominant component.</returns>
DWORD getColorDominance(Bitmap* bmp);

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

/// <summary>Color a bitmap according an specified pattern.</summary>
void pixels2Bmp(Bitmap* bmp, DWORD *pattern);


// ---------------------------------------------------------------------------
// Tools
// ---------------------------------------------------------------------------
void testProcessImgs(const wstring& directory);

// ---------------------------------------------------------------------------
// Classes Definition
// ---------------------------------------------------------------------------

/// <summary>A synchronization primitive which is signaled when its count 
/// reaches zero.</summary>
class CountdownEvent
{
public:
	// constructor
	CountdownEvent(unsigned int count);

	// Decrements the event counter.
	void signal();

	// Increments the event counter.
	void add_count();

	// Blocks the current context until the event is set.
	void wait();

private:
   volatile long _current;		// The current count.
   event _event;				// The event that is set when the counter reaches zero.

   // Disable copy constructor.
   CountdownEvent(const CountdownEvent&);

   // Disable assignment.
   CountdownEvent const & operator=(CountdownEvent const&);
};
