#pragma once
// ---------------------------------------------------------------------------
// Picture.h
// Image class using a Bitmap object to represent the image.
//
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include "ip.h"



class Picture
{
private:
	std::wstring _fn;
	int _w, _h;

	DWORD*  _arr;

public:
	Gdiplus::Bitmap* _bmp;

	Picture(void);
	Picture(std::wstring filename);
	~Picture(void);
	

	// Operations
	void getRGBArray(DWORD *);
	void save(std::wstring);
	void save(void);
	Picture* clone(void);

	// Getters & Setters
	std::wstring getFilename(void);
	Gdiplus::Bitmap* getBitmap(void);

	int getHeight();
	int getWidth();

};
