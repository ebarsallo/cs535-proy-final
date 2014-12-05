// ---------------------------------------------------------------------------
// patch-segmentation.cpp
// Tools to patch segment an image based on [FH04] method.
//
// author. ebarsall
//
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

	Picture *picT = img->clone();		// Temporal picture

	// Apply gamma correction first to the object (except to the background)
	filterGammaCorrectionMask(picT->_bmp, 1/2.2, bgColor);

	// Build an undirected graph upon pixels
	// Since the graph is undirected and assuming that each pixel (node) have 8 neighbors (the pixels just next to it),
	// we only need to construct 4 edges using the following principles:
	//  a) An edge between a pixel (node) and the one right next
	//  b) An edge between a pixel (node) and the one just down below 
	//  c) An edge between a pixel (node) and the one just below to the right.
	//  d) An edge between a pixel (node) and the one just below to the left.
	Graph *g = new Graph(width*height*4);

	DWORD *pixels = new DWORD[width*height];	
	picT->getRGBArray(pixels);
	
	// Compute weather tendendy per pixel
	float *dx = new float[width*height];
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++){
			
			int x0, y0;		// boundary of the neighbors of the pixel

			int w = CTTE__SEG_NH_WIDTH;
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


			// get the max_x{v(x)} of the pixel x
			float maxx = 0.0;
			for (int yi=0; y0+yi<y0+w; yi++)
				for (int xi=0; x0+xi<x0+w; xi++) {

					DWORD t = getSumRGBColor(pixels[(y0+yi)*width + (x0+xi)]);
					if (t > maxx) maxx = (float)t;

					} /* for (x0+xi<x0+w) */

			dx[y*width + x] = pow((float)getSumRGBColor(pixels[y*width + x])/maxx, (float)CTTE__SEG_ETA);

		} /* for x<width */

	delete [] pixels; // new

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

	//*numPatch = ds->getSize();
	
	//filterLaplacian(img->_bmp);

	DWORD *pixels2 = new DWORD[width*height];	
	img->getRGBArray(pixels2);


	// Get patchs
	std::hash_map<int,int> map;
	std::hash_map<int,DWORD> mapc;

	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++) {
			int c = ds->find(y*width + x);

			// Do not add the patch if its belong to bg
			DWORD color = pixels2[y*width + x];
			if (!isBg(color)) {
				map[c]  = 0;
				mapc[c] = pixels2[y*width + x];
			}
		} /* y<height -- getPatchs */

	*numPatch = map.size();

	
	int *pm = new int[*numPatch];
	int yy = 0;
	for (std::hash_map<int,int>::iterator iter = map.begin(); iter != map.end(); ++iter){
		pm[yy] = iter->first;
		yy++;
	}

	// Sort patches according to weather tendency
	std::sort(pm, pm+(yy-1));

	// Patches of richer geometry features will be update by those with fewer.
	// (so we need to keep the correspondence)
	int hf = (int)(yy/2);
	for (int i=0; i<yy; i++) {
		if (i>hf)
			map[pm[i]] = pm[(yy-1)-i];
		else 
			map[pm[i]] = 0;
	}

	DWORD* update = new DWORD [width*height];
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++) {
	
			int p0 = ds->find(y*width + x);
			int p1 = map[p0];

			if (p1==0) {
				update[y*width + x] = pixels2[y*width + x];
				continue;
			}

			elem_t e = ds->getElem(p0);
			double dp0 = e.weight / e.size;
			double wp0 = exp(-CTTE__SEG_lap_k*dp0*CTTE__EROSION_T); 

			// Do not update bg pixels
			if (isBg (pixels2[y*width + x]))
				update[y*width + x] = setBgRGB();
			else {

				// Apply interpolation in the Laplacian field in each RGB component.
				BYTE r0,g0,b0;
				BYTE r1,g1,b1;

				getRGB(pixels2[y*width + x], r0, g0, b0);
				getRGB(mapc[p1], r1, g1, b1);

				r0 = r0*wp0 + (1-wp0)*r1;
				g0 = g0*wp0 + (1-wp0)*g1;
				b0 = b0*wp0 + (1-wp0)*b1;

				update[y*width + x] = setRGB(r0,g0,b0);
			}
		}
	
	// Update image
	pixels2Bmp (img->_bmp, update);

	delete [] update;
	delete [] pm;
	
  
	// Coloring patch (should be used only for debugging purpose)
	if (PARAM__COLORING_PATCH) {
  
		DWORD* colors = new DWORD [width*height];
		for (int i=0; i<width*height; i++) {
			colors[i] = setRandomRGB();
		}


		DWORD* patchs = new DWORD [width*height];
		for (int y=0; y<height; y++)
			for (int x=0; x<width; x++) {
				int c = ds->find(y*width + x);

				if (isBg (pixels2[y*width + x]))
					patchs[y*width + x] = setBgRGB();
				else
					patchs[y*width + x] = colors[c];
			}
				
		pixels2Bmp (img->_bmp, patchs);

		delete [] patchs;
		delete [] colors;		  

	} /* if PARAM_COLORING_PATCH */
  
	delete [] pixels2;
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
	//DisjointSet *forest = new DisjointSet(n);
	DisjointSet *forest = new DisjointSet(g);

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


