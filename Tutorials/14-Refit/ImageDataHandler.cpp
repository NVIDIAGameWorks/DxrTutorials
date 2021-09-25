#pragma once
#include <algorithm>
#include <memory>
#include "ImageDataHandler.h"
#include <vector>

// Save the bitmap to a bmp file  
void ImageDataHandler::saveScreenshot(BYTE* pBitmapBits,
	LONG lWidth,
	LONG lHeight,
	WORD wBitsPerPixel,
	const unsigned long& padding_size,
	LPCTSTR lpszFileName)
{
	// Some basic bitmap parameters  
	unsigned long headers_size = sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER);

	unsigned long pixel_data_size = lHeight * ((lWidth * (wBitsPerPixel / 8)) + padding_size);

	BITMAPINFOHEADER bmpInfoHeader = { 0 };

	// Set the size  
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);

	// Bit count  
	bmpInfoHeader.biBitCount = wBitsPerPixel;

	// Use all colors  
	bmpInfoHeader.biClrImportant = 0;

	// Use as many colors according to bits per pixel  
	bmpInfoHeader.biClrUsed = 0;

	// Store as un Compressed  
	bmpInfoHeader.biCompression = BI_RGB;

	// Set the height in pixels  
	bmpInfoHeader.biHeight = lHeight;

	// Width of the Image in pixels  
	bmpInfoHeader.biWidth = lWidth;

	// Default number of planes  
	bmpInfoHeader.biPlanes = 1;

	// Calculate the image size in bytes  
	bmpInfoHeader.biSizeImage = pixel_data_size;

	BITMAPFILEHEADER bfh = { 0 };

	// This value should be values of BM letters i.e 0x4D42  
	// 0x4D = M 0×42 = B storing in reverse order to match with endian  
	bfh.bfType = 0x4D42;
	//bfh.bfType = 'B'+('M' << 8); 

	// <<8 used to shift ‘M’ to end  */  

	// Offset to the RGBQUAD  
	bfh.bfOffBits = headers_size;

	// Total size of image including size of headers  
	bfh.bfSize = headers_size + pixel_data_size;

	// Create the file in disk to write  
	HANDLE hFile = CreateFile(lpszFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// Return if error opening file  
	if (!hFile) return;

	DWORD dwWritten = 0;

	// Write the File header  
	WriteFile(hFile,
		&bfh,
		sizeof(bfh),
		&dwWritten,
		NULL);

	// Write the bitmap info header  
	WriteFile(hFile,
		&bmpInfoHeader,
		sizeof(bmpInfoHeader),
		&dwWritten,
		NULL);

	// Write the RGB Data  
	WriteFile(hFile,
		pBitmapBits,
		bmpInfoHeader.biSizeImage,
		&dwWritten,
		NULL);

	// Close the file handle  
	CloseHandle(hFile);
}

BYTE* ImageDataHandler::LoadBMP(int* width, int* height, unsigned long* size, LPCTSTR bmpfile)
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(bmpfile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (NULL == file)
		return NULL;

	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	// Read bitmap info
	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB')
	{
		CloseHandle(file);
		return NULL;
	}

	// get image measurements
	*width = bmpinfo.biWidth;
	*height = abs(bmpinfo.biHeight);

	// Check if bmp iuncompressed
	if (bmpinfo.biCompression != BI_RGB)
	{
		CloseHandle(file);
		return NULL;
	}

	// Check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24)
	{
		CloseHandle(file);
		return NULL;
	}

	// create buffer to hold the data
	*size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[*size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	if (ReadFile(file, Buffer, *size, &bytesread, NULL) == false)
	{
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}

	// everything successful here: close file and return buffer

	CloseHandle(file);

	return Buffer;
}

std::unique_ptr<BYTE[]> ImageDataHandler::CreateNewBuffer(unsigned long& padding,
	BYTE* pmatrix,
	const int& width,
	const int& height)
{
	padding = (4 - ((width * 3) % 4)) % 4;
	int scanlinebytes = width * 3;
	int total_scanlinebytes = scanlinebytes + padding;
	long newsize = height * total_scanlinebytes;
	std::unique_ptr<BYTE[]> newbuf(new BYTE[newsize]);

	// Fill new array with original buffer, pad remaining with zeros
	std::fill(&newbuf[0], &newbuf[newsize], 0);
	long bufpos = 0;
	long newpos = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < 3 * width; x += 3)
		{
			// Determine positions in original and padded buffers
			bufpos = y * 3 * width + (3 * width - x);
			newpos = (height - y - 1) * total_scanlinebytes + x;

			// Swap R&B, G remains, swap B&R
			newbuf[newpos] = pmatrix[bufpos + 2];
			newbuf[newpos + 1] = pmatrix[bufpos + 1];
			newbuf[newpos + 2] = pmatrix[bufpos];
		}
	}

	return newbuf;
}

void ImageDataHandler::generateBitmap()
{

	const int dataSize = 1920 * 1061 * 3;
	BYTE* buf = new BYTE[dataSize];

	for (int y = 0; y < 1061; ++y)
	{
		for (int x = 0; x < 1920; ++x)
		{
			unsigned char abgr[4];
			UINT32 temp = pPixelData[y * 1920 + x];

			abgr[0] = (temp >> 24) & 0xFF;
			abgr[1] = (temp >> 16) & 0xFF;
			abgr[2] = (temp >> 8) & 0xFF;
			abgr[3] = temp & 0xFF;

			buf[(y * 1920 + x) * 3] = abgr[1];
			buf[(y * 1920 + x) * 3+1] = abgr[2];
			buf[(y * 1920 + x) * 3+2] = abgr[3];
		}
	}

	buf[0];
	buf[1];
	buf[2];
	int c = 0;

	for (int y = 0; y < 1061; y++)
	{
		BYTE temp[1920];
		for (int i = 0; i < 1920; i++)
		{
			temp[i] = buf[y * 1920 + i];
		}

		for (int i = 0; i < 1080; i++)
		{
			buf[y * 1920 + i] = buf[(1060 - y) * 1920 + i];
		}

		for (int i = 0; i < 1920; i++)
		{
			buf[(1060 - y) * 1920 + i] = temp[i];
		}
	}

	saveScreenshot((BYTE*)buf,
		1920,
		1061,
		24,
		0,
		L"file2.bmp");

	delete[] buf;
}