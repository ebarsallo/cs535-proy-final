#pragma once
// ---------------------------------------------------------------------------
// Weathering.h
// Weathering main class header.
//
// Remarks
// Header file
// ---------------------------------------------------------------------------

#include "ip.h"
#include "Picture.h"
#include "patch-segmentation.h"
#include "silhouette-erosion.h"

class Weathering
{
private:
	//wstring _imgfn;
	//Bitmap* _bmp;

	Picture* _pic;

public:
	Weathering(void);
	Weathering(wstring filename);
	~Weathering(void);

	//void setPhoto(wstring filename);
	//wstring getPhoto();
	void apply(void);
};

