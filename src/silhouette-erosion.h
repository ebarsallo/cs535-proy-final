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

bool isBg(DWORD p);

void detectSilhouette (Picture *,  std::vector<pixel_t> &);

void erodeSilhouette(Picture *img);
