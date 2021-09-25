#pragma once
#include <basetsd.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Windows.h"

extern UINT32* pPixelData;

class ImageDataHandler
{
public:
	void saveScreenshot(BYTE* pBitmapBits,
		LONG lWidth,
		LONG lHeight,
		WORD wBitsPerPixel,
		const unsigned long& padding_size,
		LPCTSTR lpszFileName);

	void generateBitmap();

	std::unique_ptr<BYTE[]> CreateNewBuffer(unsigned long& padding,
		BYTE* pmatrix,
		const int& width,
		const int& height);

	BYTE* LoadBMP(int* width, int* height, unsigned long* size, LPCTSTR bmpfile);

	void setInt(unsigned char* mem, int data);
};