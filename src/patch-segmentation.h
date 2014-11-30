#pragma once
// ---------------------------------------------------------------------------
// patch-segmentation.h
// Tools to patch segment an image based on [FH04] method.
//
// References:
// [FH04] Felzenswalb, P; Huttenlocher, D. Efficient graph-based image segmentation. 
//   Int. J. Comput. Vision 59 (September 2004), 167-181, 3
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include <algorithm>

#include "ip.h"
#include "Picture.h"
#include "DisjointSet.h"
#include "tools.h"

// ---------------------------------------------------------------------------
// Constants and Parameters
// ---------------------------------------------------------------------------

#define CTTE__k 500
#define CTTE__MIN_SIZE 20
#define PARAM__COLORING_PATCH 1


// ---------------------------------------------------------------------------
// Routines
// ---------------------------------------------------------------------------

void segmentImg (Picture*, int *);
DisjointSet *segmentGraph(Graph*);
float getThreshold(int, int);
void coloringPatch(Bitmap* bmp, DWORD *pattern);

