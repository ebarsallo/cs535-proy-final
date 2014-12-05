#pragma once
// ---------------------------------------------------------------------------
// tools.h
// Tools library. Unsorted utility functions.
//
// author. ebarsall
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include <windows.h>
#include <math.h>

#include "ip.h"
#include "params.h"

// ---------------------------------------------------------------------------
// functions
// ---------------------------------------------------------------------------

/// <summary>Compute the square of a float x (x*x).</summary>
float square(float x);

/// <summary>Compute the euclidean distance</summary>
float euclideanDst (float x1, float y1, float z1, float x2, float y2, float z2);

/// <summary>Checks if a pixels has the background intensity.</summary>
bool isBg(DWORD p);

/// <summary>Set a pixel with the defined background color.</summary>
DWORD setBgRGB();
