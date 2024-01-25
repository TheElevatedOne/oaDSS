/*
 * This file should eventually go away when all the workarounds in it
 * are resolved
 */

#pragma once

#include <stdlib.h>
#include <cstdint>

#define	_T(x)			x

#define	__int8		int8_t
#define	__int64		int64_t

#define	MAXWORD				655535

typedef	bool					BOOL;
typedef	char					CHAR;
typedef	int16_t				DWORD;
typedef	int64_t				LONG;
typedef	uint64_t*			ULONG_PTR;
typedef	char					TCHAR;
typedef const char*		LPCSTR;
typedef const char*		LPCTSTR;

typedef struct _rect {
	long	left;
	long	top;
	long	right;
	long	bottom;
} RECT;

typedef struct _RGBQUAD {
  uint8_t rgbBlue;
  uint8_t rgbGreen;
  uint8_t rgbRed;
  uint8_t rgbReserved;
} RGBQUAD;

typedef struct _BITMAP {
	int			bmWidth;
	int			bmHeight;
} bitmap;

typedef struct _BITMAPHEADER {
	int			biSize;
	int			biWidth;
	int			biHeight;
	int			biPlanes;
	int			biBitCount;
	int			biCompression;
	int			biSizeImage;
	int			biXPelsPerMeter;
	int			biYPelsPerMeter;
	int			biClrUsed;
	int			biClrImportant;
} bitmapHeader;

typedef struct _BITMAPINFO {
	int						bmWidth;
	int						bmHeight;
	bitmapHeader	bmiHeader;
} bitmapInfo;

class	CWnd {
};


class CWinApp {
};

typedef void*						HWND;
typedef void*						HDC;
typedef void*						HGLOBAL;

typedef uint64_t				COLORREF;
typedef void*						HBITMAP;
typedef RECT*						LPRECT;
typedef RECT						tagRECT;
typedef RGBQUAD*				LPRGBQUAD;

extern COLORREF					RGB ( double, double, double );
extern uint8_t					GetRValue ( const COLORREF );
extern uint8_t					GetBValue ( const COLORREF );
extern uint8_t					GetGValue ( const COLORREF );
extern uint8_t					GetObject ( HBITMAP, size_t, bitmap* );
extern HBITMAP					SelectObject ( void*, HBITMAP );
extern HBITMAP					CreateCompatibleBitmap ( void*, int32_t, int32_t );
extern HDC							GetDC ( void* );
extern HDC							DeleteDC ( void* );
extern HDC							ReleaseDC ( void*, void* );
extern HDC							CreateCompatibleDC ( HDC );
extern int							OpenClipboard ( void* );
extern void							CloseClipboard ( void );
extern void							EmptyClipboard ( void );
extern int							SetClipboardData ( uint8_t, void* );

extern HGLOBAL					GlobalAlloc ( int, size_t );
extern void							GlobalFree ( HGLOBAL );
extern void*						GlobalLock ( HGLOBAL );
extern void							GlobalUnlock ( HGLOBAL );

#define	CF_BITMAP			1
#define	CF_DIB				2

#define BI_RGB				1

#define GMEM_MOVEABLE		1
