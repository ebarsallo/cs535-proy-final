// ---------------------------------------------------------------------------
// silhouette-erosion.cpp
// Routines to simulate erosion on the object.
//
// author. ebarsall
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include <vector>

#include "silhouette-erosion.h"


/// <summary>If  pixel is going to be erode, bled its intensity onto its below neighbor.</summary>
DWORD 
setBledRGB(DWORD x1, DWORD x0)
{
	BYTE r0,g0,b0;
	BYTE r1,g1,b1;

	getRGB(x0, r0, g0, b0);
	getRGB(x1, r1, g1, b1);

	r1 = (BYTE)(CTTE__EROSION_ALFA*r1 + (1-CTTE__EROSION_ALFA)*r0);
	g1 = (BYTE)(CTTE__EROSION_ALFA*g1 + (1-CTTE__EROSION_ALFA)*g0);
	b1 = (BYTE)(CTTE__EROSION_ALFA*b1 + (1-CTTE__EROSION_ALFA)*b0);

	return setRGB(r1, g1, b1);
}


/// <summary>Main routine to simulate the erosion on silhouette for an object. Iteratively simulate
/// the silhouette erosion process n times.</summary>
void
erodeSilhouetteMain(Picture *img)
{

	int width  = img->getWidth();
	int height = img->getHeight();
	int n;
	int t = CTTE__EROSION_T;

	DWORD *pixels = new DWORD[width*height];		// image
	float *durability = new float[width*height];	// durability index
	
	// get RGB pixel array
	img->getRGBArray(pixels);

	// init durability (t=0)
	for (int i=0; i<width*height; i++)
		durability[i] = (float)CTTE__EROSION_DELTA0;

	// Iterate n times and substract the erosion rate of each pixel to its durability index 
	n = (int)(CTTE__EROSION_EPSILON * sqrt((float)width*height)*t);
	for (int i=0; i<n; i++)
		erodeSilhouette(pixels, durability, width, height, i);

	pixels2Bmp (img->_bmp, pixels);

	delete [] durability;
	delete [] pixels;
}


/// <summary>Simulate the erosion on silhouette for an object (one iteration).</summary>
void 
erodeSilhouette(DWORD *pixels, float *durability, int width, int height, int t)
{
	std::vector<pixel_t> vecSilhouette;
	std::vector<pixel_t>::iterator it;

	detectSilhouette (pixels, vecSilhouette, width, height);

	// debug
	//for (int y=0; y<height; y++)
	//	for (int x=0; x<width; x++){
	//		pixels[y*width + x] = setBgRGB();
	//	}


	// iterate vector
	int ix=0;
	for (it=vecSilhouette.begin(); it<vecSilhouette.end(); it++, ix++) {
		
		int x = (*it).x;
		int y = (*it).y;
		int x0, y0;		// boundary of the neighbors of the pixel
		
		int w = CTTE__EROSION_NH_WIDTH;
		int half = int((w-1)/2);

		// compute boundaries
		if (x - half > 0) {
			if (x + half < width) 
				x0 = x - half; 
			else
				x0 = x - w;
		} else x0 = 0; 
		
		if (y - half > 0) {
			if (y + half < height)
				y0 = y - half; 
			else
				y0 = y - w;
		} else y0 = 0;

		float na=0;		// number of pixel in air
		float ns=0;		// number of pixel in stone

		// compute erosion for neighborhood (width x width)
		for (int yi=0; y0+yi<y0+w; yi++)
			for (int xi=0; x0+xi<x0+w; xi++) {

				if (isBg(pixels[(y0+yi)*width + (x0+xi)])) {
					// if the pixel correspond to upper air: wa=2, lower: wa=0.5, otherwise: wa=1.0 
					float wa;
					if (y0+yi < y) 
						wa = 2.0;
					else if (y0+yi+2 > y) 
						wa = 0.5;
					else wa = 1.0;
					na += wa;
				
				} else
					ns += 1.0;
			} /* for (x0+xi<x0+w) */


		float rho = (float)na/(float)(na+ns);

		// substract erosion from durability
		durability[y * width + x] -= rho;

		// if durability falls bellow zero, drops from object
		if (durability[y * width + x] < 0) {

			// bled intensity to the below neighbor only if the pixel is inside the frame
			if (y+1<height)
				if (!isBg(pixels[(y+1) * width + x]))
					pixels[(y+1) * width + x] = setBledRGB(pixels[(y+1) * width + x], pixels[y * width + x]);

			// erode pixel
			pixels[y * width + x] = setBgRGB(); //setRGB(0,0,0);
		}

	} /* for (it=vecSilhouette) */

}


/// <summary>Detect the pixels which correspond to the silhouette of the object.</summary>
void
detectSilhouette (DWORD *pixels, std::vector<pixel_t> &vec, int width, int height)
{
	// if the pixels is out of the object it should has at least 
	// one neighbor that is out of the object (background)
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++){

			// Process non background pixels.	
			if (!isBg(pixels[y*width + x])) {

				int x0, y0, wx, hy;		// boundary of the neighbors of the pixel

				// compute boundaries
				if (x==0) x0 = x; else x0 = x - 1;
				if (y==0) y0 = x; else y0 = y - 1;
				if (x==width-1)  wx = x; else wx = x + 1;
				if (y==height-1) hy = y; else hy = y + 1;

				bool found = FALSE;

				// check in the neighborhood if at least one pixel is background
				for (int yi=0; y0+yi<hy && !found; yi++)
					for (int xi=0; x0+xi<wx && !found; xi++) {
						if (isBg(pixels[(y0+yi)*width + (x0+xi)])) {
							pixel_t p;

							p.x = x;
							p.y = y;
							vec.push_back(p);

							found = TRUE;
						}
					} /* for (x0+xi<wx) */
			} /* if !isBg(..)*/

		} /* for (x<width) */
}

