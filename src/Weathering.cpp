// ---------------------------------------------------------------------------
// Weathering.cpp
// Weathering main class
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "Weathering.h"


/// <summary>Construct a Weathering object upon a image file.</summary>
Weathering::Weathering(wstring filename)
{
	_pic = new Picture(filename);
}


Weathering::~Weathering()
{
	delete _pic;
}



void
Weathering::apply (void) 
{
	wstring fn = _pic->getFilename(); //_imgfn;
	Bitmap *bmp = _pic->getBitmap();  //_bmp;
	int num;

	//DWORD *pixels = new DWORD[_pic->getWidth() * _pic->getHeight()];
	//_pic->getRGBArray(pixels);


	erodeSilhouette(_pic);

	//segmentImg(_pic, &num);



	_pic->save();
	
}