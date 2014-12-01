// ---------------------------------------------------------------------------
// silhouette-erosion.cpp
// Routines to simulate erosion on the object.
//
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include <vector>

#include "silhouette-erosion.h"


bool 
isBg(DWORD p)
{
	BYTE r,g,b;
	getRGB(p, r, g, b);

	return r == CTTE__BG_COLOR_R && CTTE__BG_COLOR_G == g && CTTE__BG_COLOR_B == b;
}

void
erodeSilhouetteMain(Picture *img)
{

}

void 
erodeSilhouette(Picture *img, )
{
	std::vector<pixel_t> vecSilhouette;
	std::vector<pixel_t>::iterator it;

	detectSilhouette (img, vecSilhouette);

	int size = vecSilhouette.size();
	float *erosion = new float [size];

	int width  = img->getWidth();
	int height = img->getHeight();
	DWORD *pixels = new DWORD[width*height];
	
	img->getRGBArray(pixels);

	// iterate vector
	int ix=0;
	for (it=vecSilhouette.begin(); it<vecSilhouette.end(); it++, ix++) {
		
		int x = (*it).x;
		int y = (*it).y;
		int x0, y0;		// boundary of the neighbors of the pixel
		
		int w = CTTE__EROSION_NH_WIDTH;
		int half = int((w-1)/2);
		//int right = width - left;

		// compute boundaries
		if (x - half > 0) {
			if (x + half > 0) 
				x0 = x - half; 
			else
				x0 = x - w;
		} else x0 = 0; 
		
		if (y - half > 0) {
			if (y + half > 0)
				y0 = y - half; 
			else
				y0 = y - w;
		} else y0 = 0;

		int na=0;		// number of pixel in air
		int ns=0;		// number of pixel in stone

		// compute erosion for neighborhood (width x width)
		for (int yi=0; y0+yi<y0+w; yi++)
			for (int xi=0; x0+xi<x0+w; xi++) {

				if (isBg(pixels[(y0+yi)*width + (x0+xi)])) 
					na++;
				else
					ns++;
			} /* for (x0+xi<x0+w) */


		float rho = (float)na/(float)(na+ns);
		erosion[ix] = rho;

	} /* for (it=vecSilhouette) */

	delete [] pixels;


	/* Iterate n times and substract the erosion rate of each pixel to its durability index */

	float *durability = new float [size];
	// init durability (t=0)
	for (int i=0; i<size; i++)
		durability[i] = CTTE__EROSION_DELTA0;
	
	// reduce durability
	for (int i=0; i<size; i++)
		durability[i] -= erosion[i];

	delete [] durability;
	delete [] erosion;
}

void
detectSilhouette (Picture *img, std::vector<pixel_t> &vec)
{
	int width  = img->getWidth();
	int height = img->getHeight();

	DWORD *pixels = new DWORD[width*height];
	
	img->getRGBArray(pixels);

	// Mark background pixels
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++){

			// if the pixels is out of the object check if the pixel 
			// has at least one neighbor that is out of the object (background)
			if (!isBg(pixels[y*width + x])) {

				int x0, y0, wx, hy;		// boundary of the neighbors of the pixel

				// compute boundaries
				if (x==0) x0 = x; else x0 = x - 1;
				if (y==0) y0 = x; else y0 = y - 1;
				if (x==width-1)  wx = x; else wx = x + 1;
				if (y==height-1) hy = y; else hy = y + 1;

				bool found = FALSE;

				// check in the neighborhood if at least of pixel is background
				for (int yi=0; y0+yi<hy && !found; yi++)
					for (int xi=0; x0+xi<wx && !found; xi++) {
						if (isBg(pixels[(y0+yi)*width + (x0+xi)])) {
							pixel_t p;

							p.x = x0+xi;
							p.y = y0+yi;
							vec.push_back(p);

							found = TRUE;
						}
					} /* for (x0+xi<wx) */
			} /* if !isBg(..)*/

		} /* for (x<width) */

	delete [] pixels;
}

