// ---------------------------------------------------------------------------
// tools.cpp
// Tools library. Unsorted utility functions.
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include <stdio.h>

#include "tools.h"

/// <summary>Compute the square of a float x (x*x).</summary>
float
square(float x) {
	return x*x;
};


/// euclideanDst
/// <summary>Compute the euclidean distance</summary>
float 
euclideanDst (float x1, float y1, float z1, float x2, float y2, float z2)
{
	return sqrt (square(x1-x2) + square(y1-y2) + square(z1-z2));
}


