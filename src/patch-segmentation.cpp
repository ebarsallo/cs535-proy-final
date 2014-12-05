// ---------------------------------------------------------------------------
// patch-segmentation.cpp
// Tools to patch segment an image based on [FH04] method.
//
// References:
// [FH04] Felzenswalb, P; Huttenlocher, D. Efficient graph-based image 
//   segmentation. Int. J. Comput. Vision 59 (September 2004), 167-181, 3
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "patch-segmentation.h"

int		CTTE__SEG_MIN_SIZE = 1400;		/* Number of pixels in the smallest patch */

// ---------------------------------------------------------------------------
// Routines
// ---------------------------------------------------------------------------

///<summary>Patch segment an image.</summary>
void
segmentImg (Picture *img, int *numPatch)
{
	int xx=0;
	int width  = img->getWidth();
	int height = img->getHeight();

	DWORD bgColor = setBgRGB();

	CTTE__SEG_MIN_SIZE = (int) (height*width / CTTE__SEG_n);

	// Apply gamma correction first
	// filterGammaCorrection(img->_bmp, 1/2.2);
	filterGammaCorrectionMask(img->_bmp, 1/2.2, bgColor);

	// Build an undirected graph upon pixels
	// Since the graph is undirected and assuming that each pixel (node) have 8 neighbors (the pixels just next to it),
	// we only need to construct 4 edges using the following principles:
	//  a) An edge between a pixel (node) and the one right next
	//  b) An edge between a pixel (node) and the one just down below 
	//  c) An edge between a pixel (node) and the one just below to the right.
	//  d) An edge between a pixel (node) and the one just below to the left.
	Graph *g = new Graph(width*height*4);

	DWORD *pixels = new DWORD[width*height];	
	img->getRGBArray(pixels);
	
	// Compute weather tendendy per pixel
	float *dx = new float[width*height];
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++){
			
			int x0, y0, wx, hy;		// boundary of the neighbors of the pixel

			// compute boundaries
			if (x==0) x0 = x; else x0 = x - 1;
			if (y==0) y0 = x; else y0 = y - 1;
			if (x==width-1)  wx = x; else wx = x + 1;
			if (y==height-1) hy = y; else hy = y + 1;

			// get the max_x{v(x)} of the pixel x
			float maxx = 0.0;
			for (int yi=0; y0+yi<hy; yi++)
				for (int xi=0; x0+xi<wx; xi++) {

					DWORD t = getSumRGBColor(pixels[(y0+yi)*width + (x0+xi)]);
					if (t > maxx) maxx = (float)t;

					} /* for (x0+xi<wx) */

			dx[y*width + x] = pow((float)getSumRGBColor(pixels[y*width + x])/maxx, (float)CTTE__SEG_ETA);

		} /* for x<width */


	float w=0.0f;
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++){
			
			// skip if the pixel is background
			// if (isBg(pixels[y*width + x])) continue;

			// pairing pixels right next to each other: (x,y) -> (x+1,y)
			if ( x < (width-1) ) {
				//w = diffIntensity(pixels[(y*width) + x], pixels[(y*width) + x+1]);
				w = dx[(y*width) + x] + dx[(y*width) + x+1];
				g->setEdge(xx, (y*width) + x, (y*width) + x+1, w);
				xx++;
			}

			// pairing pixels with the one below: (x,y) -> (x,y+1)
			if ( y < (height-1) ) {
				//w = diffIntensity(pixels[(y*width) + x], pixels[(y+1)*width + x]);
				w = dx[(y*width) + x] + dx[(y+1)*width + x];
				g->setEdge(xx, (y*width) + x, (y+1)*width + x, w);
				xx++;
			}

			// pairing pixels with the one diagonal right below: (x,y) -> (x+1,y+1)
			if ( (x < (width-1)) && (y < (height-1)) ) {
				//w = diffIntensity(pixels[(y*width) + x], pixels[(y+1)*width + x+1]);
				w = dx[(y*width) + x] + dx[(y+1)*width + x+1];
				g->setEdge(xx, (y*width) + x, (y+1)*width + x+1, w);
				xx++;
			}

			// pairing pixels with the one diagonal left below: (x,y) -> (x-1,y+1)
			if ( (x > 0) && (y < (height-1)) ) {
				//w = diffIntensity(pixels[(y*width) + x], pixels[(y+1)*width + x-1]);
				w = dx[(y*width) + x] + dx[(y+1)*width + x-1];
				g->setEdge(xx, (y*width) + x, (y+1)*width + x-1, w);
				xx++;
			}
		}
	
	delete [] dx;

	// Segment graph
	DisjointSet *ds = segmentGraph(g);

	// Post Process
	// Join componente smaller than the minimun acceptable size
	for (int i=0; i < xx; i++) {
		int a = ds->find(g->getEdge(i).s);
		int b = ds->find(g->getEdge(i).t);
		
		if ((a != b) && ((ds->getSize(a) < CTTE__SEG_MIN_SIZE) || (ds->getSize(b) < CTTE__SEG_MIN_SIZE)))
			ds->join(a, b);
	}
  
	delete g;

	*numPatch = ds->getSize();

  
	// Coloring patch
	if (PARAM__COLORING_PATCH) {
  
		DWORD* colors = new DWORD [width*height];
		for (int i=0; i<width*height; i++) {
			colors[i] = setRandomRGB();
		}


		DWORD* patchs = new DWORD [width*height];
		for (int y=0; y<height; y++)
			for (int x=0; x<width; x++) {
				int c = ds->find(y*width + x);

				if (isBg (pixels[y*width + x]))
					patchs[y*width + x] = setBgRGB();
				else
					patchs[y*width + x] = colors[c];
			}
				
		pixels2Bmp (img->_bmp, patchs);

		delete [] patchs;
		delete [] colors;		  

  } /* if PARAM_COLORING_PATCH */
  
	delete [] pixels;
	delete ds;

}


/// <summary>Get threshold based on the size of the components</summary>
float 
getThreshold(int k, int size)
{
	return (float)k/size;
}


/// <summary>Segmentation algorithm of a graph</summary>
DisjointSet *
segmentGraph(Graph *g)
{
	edge_t *e = g->getEdgeArray();
	int  n  = g->getNumEdges();

	// Step 0. Sort E by non-decreasing edge weight
	std::sort(e, e+n, pred_edge_t);

	// Step 1. Start with a segmentation S^0, where each vertex is in its own component.
	// Init threshold for each component. 
	float *threshold = new float[n];
	for (int i=0; i<n;i++)
		threshold[i] = getThreshold(CTTE__SEG_k, 1);

	// create disjoint-set (forest)
	DisjointSet *forest = new DisjointSet(n);

	// Step 2. Repeat step 3 for q = 1 ...m
	// Step 3. Construct S^q given S^{q-1}.
	for (int i=0; i<n; i++) {
		edge_t *edgeptr = &e[i];

		int s = forest->find(edgeptr->s);
		int t = forest->find(edgeptr->t);

		if (s != t) {
			if ((edgeptr->w <= threshold[s]) && (edgeptr->w <= threshold[t])) {
				forest->join(s,t);
				s = forest->find(s);
				
				// Update threshold
				threshold[s] = edgeptr->w + getThreshold(CTTE__SEG_k, forest->getSize(s));
			} /* if (w <= treshold[s]) && (w <= treshold[t]) */
		} /* if (s != t) */
	}

	delete [] threshold;
	return forest;
}


