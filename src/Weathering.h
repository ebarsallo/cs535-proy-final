#pragma once
// ---------------------------------------------------------------------------
// Weathering.h
// Weathering main class header.
//
// Remarks
// Header file
// ---------------------------------------------------------------------------

#include "ip.h"

class Weathering
{
private:
	wstring _img;
	Bitmap* _bmp;

public:
	Weathering(void);
	Weathering(wstring filename);
	~Weathering(void);

	void setPhoto(wstring filename);
	wstring getPhoto();
	void apply(void);
};

