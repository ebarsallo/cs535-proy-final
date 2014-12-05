// ---------------------------------------------------------------------------
// ip.cpp
// Image processing
//
// Remarks
// Compile using: cl.exe /DUNICODE /EHsc ip.cpp /link gdiplus.lib
// ---------------------------------------------------------------------------
#include "ip.h"


/// <summary>Retrieves the RGB components from a given color value.</summary>
void 
getRGB(DWORD c, BYTE& r, BYTE& g, BYTE& b)
{
	r = static_cast<BYTE>((c & 0x00ff0000) >> 16);
	g = static_cast<BYTE>((c & 0x0000ff00) >> 8);
	b = static_cast<BYTE>((c & 0x000000ff));
}


/// <summary>Set a single colore value from the RGB components</summary>
DWORD 
setRGB(BYTE r, BYTE g, BYTE b)
{
	return (r<<16) | (g<<8) | (b);
}


/// <summary>Loads an image file from disk.</summary>
Bitmap*
loadImage(wstring filename) {
	Bitmap *bmp = new Bitmap (filename.c_str());

	return bmp;
}


/// <summary>Process each pixel of an bmp object with a filter function.</summary>
void 
processImg(Bitmap* bmp, const function <void (DWORD&)>& f)
{
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();

	// Lock the bitmap.
	BitmapData bitmapData;
	Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	bmp->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	// Call the function for each pixel in the image.
	parallel_for (0, height, [&, width](int y)
	{      
		for (int x = 0; x < width; ++x)
		{
			// Get the current pixel value.
			DWORD* curr_pixel = image_bits + (y * width) + x;

			// Call the function.
			f(*curr_pixel);
		}
	});

	// Unlock the bitmap.
	bmp->UnlockBits(&bitmapData);
}


/// <summary>Image filter. Converts a given image color to grayscale.</summary>
Bitmap* 
filterGrayscale(Bitmap* bmp) 
{
	processImg(bmp, 
		[](DWORD& color) {
			BYTE r, g, b;

			getRGB(color, r, g, b);

			// Set each color component to the average of 
			// the original components.
			BYTE c = (static_cast<WORD>(r) + g + b) / 3;
			color = setRGB(c, c, c);
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a gamma correction of RGB channel.</summary>
Bitmap* 
filterGammaCorrection(Bitmap* bmp, double gamma) 
{
	processImg(bmp, 
		[gamma](DWORD& color) {
			BYTE r, g, b;
			double gv = gamma;

			getRGB(color, r, g, b);

			// Apply gamma correction to the pixel.
			BYTE r1 = static_cast<BYTE>(pow(r, gv));
			BYTE g1 = static_cast<BYTE>(pow(g, gv));
			BYTE b1 = static_cast<BYTE>(pow(b, gv));

			color = setRGB(r1, g1, b1);
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a gamma correction of RGB channel if the RGB color is diff than the mask.</summary>
Bitmap* 
filterGammaCorrectionMask(Bitmap* bmp, double gamma, DWORD mask) 
{
	processImg(bmp, 
		[gamma, mask](DWORD& color) {
			BYTE r0, g0, b0;
			BYTE rm, gm, bm;
			double gv = gamma;

			getRGB(color, r0, g0, b0);
			getRGB(mask, rm, gm, bm);

			if (r0 == rm && g0 == gm && b0 == bm)
				color = color;
			else {
				// Apply gamma correction to the pixel.
				BYTE r1 = static_cast<BYTE>(pow(r0, gv));
				BYTE g1 = static_cast<BYTE>(pow(g0, gv));
				BYTE b1 = static_cast<BYTE>(pow(b0, gv));

				color = setRGB(r1, g1, b1);
			}
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a sephia filter to a given image.</summary>
Bitmap* 
filterSepia(Bitmap* bmp) 
{
	processImg(bmp, 
		[](DWORD& color) {
			BYTE r0, g0, b0;
			getRGB(color, r0, g0, b0);

			WORD r1 = static_cast<WORD>((r0 * .393) + (g0 *.769) + (b0 * .189));
			WORD g1 = static_cast<WORD>((r0 * .349) + (g0 *.686) + (b0 * .168));
			WORD b1 = static_cast<WORD>((r0 * .272) + (g0 *.534) + (b0 * .131));

			color = setRGB(min(0xff, r1), min(0xff, g1), min(0xff, b1));
		}
	);
	return bmp;
}


/// <summary>Image Filter. Applies a color mask to each pixel of a given image.</summary>
Bitmap* 
filterColorMask(Bitmap* bmp, DWORD mask)
{
	processImg(bmp, 
		[mask](DWORD& color) {
			color = color & mask;
		}
	);
	return bmp;
}

/// <summary>Image filter. <b>Darken</b> a given image (to make more obscure) by a specific amount.</summary>
Bitmap* 
filterDarkEffect(Bitmap* bmp, unsigned int percent)
{
	if (percent > 100)
		throw invalid_argument("filterDarkEffect: Percent must be less or equal than 100.");

	double factor = percent / 100.0;

	processImg(bmp, 
		[factor](DWORD& color) {
			BYTE r, g, b;
			getRGB(color, r, g, b);
			r = static_cast<BYTE>(factor*r);
			g = static_cast<BYTE>(factor*g);
			b = static_cast<BYTE>(factor*b);
			color = setRGB(r, g, b);
		}
	);
	return bmp;
}


/// <summary>Image filter. Apply a <b>Gaussian</b> blur to a given image.</summary>
Bitmap* 
filterGaussianBlur(Bitmap* bmp, DWORD mask)
{
	/* TODO */
}



/// <summary>Get the dominant RGB component in a given image.</summary>
/// <returns>Returns the corresponding color mask for the dominant component.</returns>
DWORD 
getColorDominance(Bitmap* bmp)
{
	// The ProcessImage function processes the image in parallel.
	// The following combinable objects enable the callback function
	// to increment the color counts without using a lock.
	combinable<unsigned int> rc;
	combinable<unsigned int> gc;
	combinable<unsigned int> bc;

	processImg(bmp, 
		[&](DWORD& color) {
			BYTE r, g, b;
			getRGB(color, r, g, b);
			if (r >= g && r >= b)
				rc.local()++;
			else if (g >= r && g >= b)
				gc.local()++;
			else
				bc.local()++;
		}
	);

	// Check which color (r,g,b) is dominant and return the corresponding color mask.
	unsigned int r = rc.combine(plus<unsigned int>());
	unsigned int g = gc.combine(plus<unsigned int>());
	unsigned int b = bc.combine(plus<unsigned int>());

	if (r + r >= g + b)
		return 0x00ff0000;
	else if (g + g >= r + b)
		return 0x0000ff00;
	else
		return 0x000000ff;
}


/// <summary>Get the CLS ID (class identifier) for a given MIME TYPE of an image encoder 
/// (bmp, jpeg, etc).</summary>
int 
getEncoderCLSID(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = nullptr;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == nullptr)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


/// diff
/// <summary>Compute difference of intensity between pixels</summary>
float
diffIntensity (DWORD pixel1, DWORD pixel2)
{

	BYTE r1, g1, b1;
	BYTE r2, g2, b2;

	getRGB(pixel1, r1, g1, b1);
	getRGB(pixel2, r2, g2, b2);

	return euclideanDst(r1, g1, b1, r2, g2, b2);
}
 

/// randomRGB
/// <summary>Return a random RGB color.</summary>
DWORD
setRandomRGB ()
{
	BYTE r, g, b;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;

	return setRGB(r, g, b);
}


/// <summary>Return the sum of each RGB component.</summary>
DWORD
getSumRGBColor (DWORD color)
{
	BYTE r, g, b;

	getRGB(color, r, g, b);
	return r + g + b;
}


/// <summary>Color a bitmap according an specified pattern.</summary>
void 
pixels2Bmp(Bitmap* bmp, DWORD *pattern)
{
	int width = bmp->GetWidth();
	int height = bmp->GetHeight();

	// Lock the bitmap.
	BitmapData bitmapData;
	Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	bmp->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData);

	// Get a pointer to the bitmap data.
	DWORD* image_bits = (DWORD*)bitmapData.Scan0;

	// Call the function for each pixel in the image.
	for (int y=0; y < height; ++y)
	{      
		for (int x=0; x < width; ++x)
		{
			// Get the current pixel value.
			DWORD* curr_pixel = image_bits + (y * width) + x;
			*curr_pixel = pattern[(y * width) + x];
		}
	}
	
	// Unlock the bitmap.
	bmp->UnlockBits(&bitmapData);
}


// ---------------------------------------------------------------------------
// Classes Implementation
// ---------------------------------------------------------------------------

CountdownEvent::CountdownEvent(unsigned int count = 0) : _current(static_cast<long> (count)) 
{
	// Set the event if the initial count is zero.
	if (_current == 0L)
		_event.set();
}

/// <summary>Decrements the event counter.</summary>
void
CountdownEvent::signal() 
{
	if(InterlockedDecrement(&_current) == 0L) {
		_event.set();
	}
}

/// <summary>Increments the event counter.</summary>
void 
CountdownEvent::add_count() 
{
	if(InterlockedIncrement(&_current) == 1L) {
		_event.reset();
	}
}

/// <summary>Blocks the current context until the event is set.</summary>
void
CountdownEvent::wait() 
{
    _event.wait();
}


// Demonstrates how to set up a message network that performs a series of 
// image processing operations on each JPEG image in the given directory and
// saves each altered image as a Windows bitmap.
void testProcessImgs(const wstring& directory)
{
   // Holds the number of active image processing operations and 
   // signals to the main thread that processing is complete.
   CountdownEvent active(0);

   // Maps Bitmap objects to their original file names.
   map<Bitmap*, wstring> bitmap_file_names;

   //
   // Create the nodes of the network.
   //

   // Loads Bitmap objects from disk.
   transformer<wstring, Bitmap*> load_bitmap(
      [&](wstring file_name) -> Bitmap* {
         Bitmap* bmp = new Bitmap(file_name.c_str());
         if (bmp != nullptr)
            bitmap_file_names.insert(make_pair(bmp, file_name));
         return bmp;
      }
   );

   // Holds loaded Bitmap objects.
   unbounded_buffer<Bitmap*> loaded_bitmaps;

   // Converts images that are authored by Tom to grayscale.
   transformer<Bitmap*, Bitmap*> grayscale(
      [](Bitmap* bmp) {
         return filterGrayscale(bmp);
      },
      nullptr,
      [](Bitmap* bmp) -> bool {
         if (bmp == nullptr)
            return false;

         // Retrieve the artist name from metadata.
         UINT size = bmp->GetPropertyItemSize(PropertyTagArtist);
         if (size == 0)
            // Image does not have the Artist property.
            return false;

         PropertyItem* artistProperty = (PropertyItem*) malloc(size);
         bmp->GetPropertyItem(PropertyTagArtist, size, artistProperty);
         string artist(reinterpret_cast<char*>(artistProperty->value));
         free(artistProperty);

         return (artist.find("Tom ") == 0);
      }
   );

   // Removes the green and blue color components from images that have red as
   // their dominant color.
   transformer<Bitmap*, Bitmap*> colormask(
      [](Bitmap* bmp) {
         return filterColorMask(bmp, 0x00ff0000);
      },
      nullptr,
      [](Bitmap* bmp) -> bool { 
         if (bmp == nullptr)
            return false;
         return (getColorDominance(bmp) == 0x00ff0000);
      }
   );

   // Darkens the color of the provided Bitmap object.
   transformer<Bitmap*, Bitmap*> darken([](Bitmap* bmp) {
      return filterDarkEffect(bmp, 50);
   });

   // Darkens the color of the provided Bitmap object.
   transformer<Bitmap*, Bitmap*> gammaC([](Bitmap* bmp) {
      return filterGammaCorrection(bmp, 1/2.2);
   });

   // Applies sepia toning to the remaining images.
   transformer<Bitmap*, Bitmap*> sepiatone(
      [](Bitmap* bmp) {
         return filterSepia(bmp);
      },
      nullptr,
      [](Bitmap* bmp) -> bool { return bmp != nullptr; }
   );

   // Saves Bitmap objects to disk.
   transformer<Bitmap*, Bitmap*> save_bitmap([&](Bitmap* bmp) -> Bitmap* {
      // Replace the file extension with .bmp.
      wstring file_name = bitmap_file_names[bmp];
      //file_name.replace(file_name.rfind(L'.') + 1, 3, L"bmp");
	  file_name.replace(file_name.rfind(L'.'), 6, L"_x.jpg");

      // Save the processed image.
      CLSID bmpClsid;
      //GetEncoderClsid(L"image/bmp", &bmpClsid);
	  getEncoderCLSID(L"image/jpeg", &bmpClsid);

      bmp->Save(file_name.c_str(), &bmpClsid);
	  
      return bmp;
   });

   // Deletes Bitmap objects.
   transformer<Bitmap*, Bitmap*> delete_bitmap([](Bitmap* bmp) -> Bitmap* {      
      delete bmp;
      return nullptr;
   });

   // Decrements the event counter.
   call<Bitmap*> decrement([&](Bitmap* _) {      
      active.signal();
   });

   //
   // Connect the network.
   //   

   load_bitmap.link_target(&loaded_bitmaps);
   //loaded_bitmaps.link_target(&grayscale);
   //loaded_bitmaps.link_target(&colormask);   
   //colormask.link_target(&darken);
   //loaded_bitmaps.link_target(&sepiatone);
   //loaded_bitmaps.link_target(&decrement);

   //grayscale.link_target(&save_bitmap);
   //darken.link_target(&save_bitmap);
   //sepiatone.link_target(&save_bitmap);

   loaded_bitmaps.link_target(&gammaC);
   gammaC.link_target(&save_bitmap);

   save_bitmap.link_target(&delete_bitmap);
   delete_bitmap.link_target(&decrement);

   // Traverse all files in the directory.
   wstring searchPattern = directory;
   searchPattern.append(L"\\*");

   WIN32_FIND_DATA fileFindData;
   HANDLE hFind = FindFirstFile(searchPattern.c_str(), &fileFindData);
   if (hFind == INVALID_HANDLE_VALUE) 
      return;
   do
   {
      if (!(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         wstring file = fileFindData.cFileName;

         // Process only JPEG files.
         if (file.rfind(L".jpg") == file.length() - 4)
         {
            // Form the full path to the file.
            wstring full_path(directory);
            full_path.append(L"\\");
            full_path.append(file);

            // Increment the count of work items.
            active.add_count();

            // Send the path name to the network.
            send(load_bitmap, full_path);
         }
      }
   }
   while (FindNextFile(hFind, &fileFindData) != 0); 
   FindClose(hFind);

   // Wait for all operations to finish.
   active.wait();
}

