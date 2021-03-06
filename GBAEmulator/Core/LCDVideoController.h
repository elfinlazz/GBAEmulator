#pragma once
#include "ClockBlock.h"

class ARM7TDMI;
class IORegisters;
class PalRam;
class VRAM;
class OAM;

typedef struct
{
	union
	{
		uint32_t     attr01;
		struct
		{
			uint16_t     attr0;
			uint16_t     attr1;
		};
		struct
		{
			uint32_t     y : 8;
			uint32_t     rsMode : 2;
			uint32_t     objMode : 2;
			uint32_t     mosaic : 1;
			uint32_t     colorMode : 1;
			uint32_t     shape : 2;

			uint32_t     x : 9;
			uint32_t     rsParam : 5;
			uint32_t     size : 2;
		};
		struct
		{
			uint32_t     _0 : 28;
			uint32_t     flipH : 1;
			uint32_t     flipV : 1;
			uint32_t     _1 : 2;
		};
	};
	union
	{
		struct
		{
			uint16_t     attr2;
			uint16_t     _3;
		};
		uint32_t     attr23;
		struct
		{
			uint32_t     charNo : 10;
			uint32_t     priority : 2;
			uint32_t     cParam : 4;
			uint32_t     _2 : 16;
		};
	};
}
GXOamAttr;

typedef struct
{
	uint16_t     _0;
	uint16_t     _1;
	uint16_t     _2;
	int16_t     PA;
	uint16_t     _4;
	uint16_t     _5;
	uint16_t     _6;
	int16_t     PB;
	uint16_t     _8;
	uint16_t     _9;
	uint16_t     _10;
	int16_t     PC;
	uint16_t     _12;
	uint16_t     _13;
	uint16_t     _14;
	int16_t     PD;
}
GXOamAffine;

typedef union
{
	uint16_t color;
	struct
	{
		uint16_t r : 5;
		uint16_t g : 5;
		uint16_t b : 5;
		uint16_t x : 1;
	};
} GXRgb;

class LCDVideoController : ClockBlock
{
private:
	ARM7TDMI* mProcessor;
	IORegisters* mIORegisters;
	PalRam* mPalRam;
	VRAM* mVRAM;
	OAM* mOAM;
	int mClockCounter;
	int mX;
	int mY;

	HDC mHDC;
	COLORREF mFrameBuffer[160 * 240];
	BITMAPINFO* mDIB;
public:
	LCDVideoController(ARM7TDMI* processor, IORegisters* ioRegisters, PalRam* palRam, VRAM* vram, OAM* oam, HDC hdc)
		: mProcessor(processor), mIORegisters(ioRegisters), mPalRam(palRam), mVRAM(vram), mOAM(oam), mClockCounter(0), mX(0), mY(0), mHDC(hdc)
	{ 
		mDIB = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 256);
		memset(mDIB, 0, sizeof(BITMAPINFO) + sizeof(RGBQUAD) * 256);
		BITMAPINFOHEADER* bmInfohdr = &mDIB->bmiHeader;
		bmInfohdr->biSize = sizeof(BITMAPINFO);// + sizeof(RGBQUAD) * 256; //I think it's not of use
		bmInfohdr->biWidth = 240;
		bmInfohdr->biHeight = -160;
		bmInfohdr->biPlanes = 1;
		bmInfohdr->biBitCount = 32;
		bmInfohdr->biCompression = BI_RGB;

	}
	~LCDVideoController()
	{
		free(mDIB);
	}
	void RunCycle();
};