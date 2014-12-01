#pragma once
// ---------------------------------------------------------------------------
// tools.h
// Tools library. Unsorted utility functions.
//
// Remarks
// Header file
// ---------------------------------------------------------------------------

#include <windows.h>
#include <math.h>

// ---------------------------------------------------------------------------
// functions
// ---------------------------------------------------------------------------

/// <summary>Compute the square of a float x (x*x).</summary>
float square(float x);

/// <summary>Compute the euclidean distance</summary>
float euclideanDst (float x1, float y1, float z1, float x2, float y2, float z2);
