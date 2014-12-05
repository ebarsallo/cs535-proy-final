// ---------------------------------------------------------------------------
// Weathering.cpp
// Stone Weathering main class based on [XU11].
//
// References:
// [XU11] Xue, S., Dorsey, J., & Rushmeier, H. (2011, June). Stone weathering 
//   in a photograph. In Computer Graphics Forum (Vol.30, No.4, pp. 189-1196). 
//   Blackwell Publishing Ltd.
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "Weathering.h"


/// <summary>Construct a Weathering object upon a image file.</summary>
Weathering::Weathering(std::wstring filename)
{
	srand(unsigned (time(NULL)) );
	_pic = new Picture(filename);

}


///<summary>Destruct a Weathering object.</summary>
Weathering::~Weathering()
{
	delete _pic;
}


///<summary>Apply weathering effect to a object.</summary>
void
Weathering::apply (void) 
{
	int num;

	segmentImg(_pic, &num);
	erodeSilhouetteMain(_pic);

	_pic->save();
	
}