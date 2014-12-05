#pragma once
// ---------------------------------------------------------------------------
// params.h
// Global parameteres.
//
//
// Remarks
// Header file
// ---------------------------------------------------------------------------

// Generals Parameters
#define CTTE__BG_COLOR_R 0				/* R component of defined backgroun color */
#define CTTE__BG_COLOR_G 255			/* G component of defined backgroun color */
#define CTTE__BG_COLOR_B 1				/* B component of defined backgroun color */

// Patch Segmentation Parameters
#define CTTE__SEG_k 100					/* Large K favors bigger patches */
#define CTTE__SEG_n 3200				/* Minimum number of patch in a image */
#define CTTE__SEG_ETA 0.6				/* Constant used to adjust the contrast in weathering tendency */
#define CTTE__SEG_NH_WIDTH 3			/* sqrt(N) of neighbors to compute weather tendency */
#define CTTE__SEG_lap_k 0.01			/* Constant used by the laplacial operator */
#define PARAM__COLORING_PATCH 0			/* Define if the patch segmented are colored in the image */

// Silhouette Erosion Parameters
#define	CTTE__EROSION_EPSILON 0.0002	/* Adjustable speed parameter to erode the silhouette */
#define CTTE__EROSION_DELTA0 0.1		/* Initial durability for each pixel */
#define CTTE__EROSION_NH_WIDTH 7		/* sqrt(N) of neighbors to compute the erosion ratio */
#define CTTE__EROSION_ALFA 0.20			/* Constant used to bled the color of pixel about to be remove from the object (to the one below) */
#define CTTE__EROSION_T 10				/* Time of iteration */