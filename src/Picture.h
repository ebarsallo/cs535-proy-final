#pragma once
// ---------------------------------------------------------------------------
// Picture.h
// Image class.
//
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include "ip.h"



class Picture
{
private:
	wstring _fn;
	int _w, _h;

	DWORD*  _arr;

public:
	Bitmap* _bmp;

	Picture(void);
	Picture(wstring filename);
	~Picture(void);
	

	// Operations
	void getRGBArray(DWORD *);
	void save(wstring);
	void save(void);

	// Getters & Setters
	wstring getFilename(void);
	Bitmap* getBitmap(void);

	int getHeight();
	int getWidth();

};
