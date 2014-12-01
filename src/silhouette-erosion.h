#pragma once
// ---------------------------------------------------------------------------
// silhoutte-erosion.h
// Routines to simulate erosion on the object.
//
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include "Picture.h"
#include "params.h"



// ---------------------------------------------------------------------------
// Data structures
// ---------------------------------------------------------------------------

struct pixel_s {
	int x;
	int y;
};
typedef pixel_s pixel_t;

// ---------------------------------------------------------------------------
// Routines
// ---------------------------------------------------------------------------

/// <summary>Checks if a pixels has the background intensity.</summary>
bool isBg(DWORD p);

/// <summary>Set a pixel with the defined background color.</summary>
DWORD setBgRGB();

/// <summary>If  pixel is going to be erode, bled its intensity onto its below neighbor.</summary>
DWORD setBledRGB(DWORD, DWORD);

/// <summary>Main routine to simulate the erosion on silhouette for an object. Iteratively simulate
/// the silhouette erosion process n times.</summary>
void erodeSilhouetteMain(Picture *img);

/// <summary>Simulate the erosion on silhouette for an object (one iteration).</summary>
void erodeSilhouette(DWORD *, float *, int, int, int);

/// <summary>Detect the pixels which correspond to the silhouette of the object.</summary>
void detectSilhouette (DWORD *, std::vector<pixel_t> &, int, int);
