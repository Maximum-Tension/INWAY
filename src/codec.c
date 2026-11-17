/******************************************************************************\
# C - codec                                      #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/17 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# THE TILE PACKER. ONE BYTE OPERATIONS:
# 00XXXXXX = COLOUR FROM THE CACHE, 01RRGGBB = SMALL DIFFERENCE, 10GGGGGG + 1
# BYTE = LUMA DIFFERENCE, 11XXXXXX = RUN OF 1 TO 62 PIXELS, 0XFE + 3 BYTES =
# FULL RGB COLOUR.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define COBJMACROS
 * #define WIN32_LEAN_AND_MEAN
 */
#include <windows.h>
/******* """""""""""
 * #define FAILED(hr)
 * #define IsEqualGUID(rguid1, rguid2)
 * #define SUCCEEDED(hr)
 * #define TRUE
 * typedef	HGLOBAL;
 * typedef	HRESULT;
 * typedef	LARGE_INTEGER;
 * typedef	UINT;
 * typedef	ULARGE_INTEGER;
 * LPVOID	GlobalLock(HGLOBAL);
 * BOOL		GlobalUnlock(HGLOBAL);
 */
#include <ole2.h>
/******* """"""""
 * #define IStream_Release(This)
 * #define IStream_Seek(This, dlibMove, dwOrigin, plibNewPosition)
 * enum tagCLSCTX;
 * enum tagCOINIT;
 * enum VARENUM;
 * typedef	IStream;
 * typedef	VARIANT;
 * HRESULT	CoCreateInstance(
 * 	const IID *const,
 * 	LPUNKNOWN,
 * 	DWORD,
 * 	const IID *const,
 * 	LPVOID *
 * );
 * HRESULT	CoInitializeEx(LPVOID, DWORD);
 * void		CoUninitialize(void);
 * void		VariantInit(VARIANTARG *);
 */
#include <mshtmhst.h>
/******* """"""""""""
 * #define IPropertyBag2_Release(This)
 * #define IPropertyBag2_Write(This, cProperties, pPropBag, pvarValue)
 * typedef	IPropertyBag2;
 * typedef	PROPBAG2;
 */
#include <wincodec.h>
/******* """"""""""""
 * #define IWICBitmapDecoder_GetFrame(This, index, ppIBitmapFrame)
 * #define IWICBitmapDecoder_Release(This)
 * #define IWICBitmapEncoder_Commit(This)
 * #define IWICBitmapEncoder_CreateNewFrame(
 * 	This,
 * 	ppIFrameEncode,
 * 	ppIEncoderOptions
 * )
 * #define IWICBitmapEncoder_Initialize(This, pIStream, cacheOption)
 * #define IWICBitmapEncoder_Release(This)
 * #define IWICBitmapFrameDecode_GetSize(This, puiWidth, puiHeight)
 * #define IWICBitmapFrameDecode_Release(This)
 * #define IWICBitmapFrameEncode_Commit(This)
 * #define IWICBitmapFrameEncode_Initialize(This, pIEncoderOptions)
 * #define IWICBitmapFrameEncode_Release(This)
 * #define IWICBitmapFrameEncode_SetPixelFormat(This, pPixelFormat)
 * #define IWICBitmapFrameEncode_SetSize(This, uiWidth, uiHeight)
 * #define IWICBitmapFrameEncode_WritePixels(
 * 	This,
 * 	lineCount,
 * 	cbStride,
 * 	cbBufferSize,
 * 	pbPixels
 * )
 * #define IWICBitmapSource_CopyPixels(
 * 	This,
 * 	prc,
 * 	cbStride,
 * 	cbBufferSize,
 * 	pbBuffer
 * )
 * #define IWICBitmapSource_Release(This)
 * #define IWICImagingFactory_CreateDecoderFromStream(
 * 	This,
 * 	pIStream,
 * 	pguidVendor,
 * 	metadataOptions,
 * 	ppIDecoder
 * )
 * #define IWICImagingFactory_CreateEncoder(
 * 	This,
 * 	guidContainerFormat,
 * 	pguidVendor,
 * 	ppIEncoder
 * )
 * #define IWICImagingFactory_CreateStream(This, ppIWICStream)
 * #define IWICImagingFactory_Release(This)
 * #define IWICStream_InitializeFromMemory(This, pbBuffer, cbBufferSize)
 * #define IWICStream_Release(This)
 * typedef		IWICBitmapDecoder;
 * typedef		IWICBitmapEncoder;
 * typedef		IWICBitmapFrameDecode;
 * typedef		IWICBitmapFrameEncode;
 * typedef		IWICBitmapSource;
 * typedef		IWICImagingFactory;
 * typedef		IWICStream;
 * typedef		WICPixelFormatGUID;
 * const GUID	CLSID_WICImagingFactory;
 * const GUID	GUID_WICPixelFormat24bppBGR;
 * const GUID	IID_IWICImagingFactory;
 */
#include <stdlib.h>
/******* """"""""""
 * void	free(void *);
 * void	*realloc(void *, size_t);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * typedef	size_t;
 * void		*memset(void *, int, size_t);
 */
#include <stdint.h>
/******* """"""""""
 * typedef	int8_t;
 * typedef	uint32_t;
 * typedef	uint8_t;
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT;
 * void	TEXT_ADDN(struct S_TEXT *, const char *, size_t);
 */
#include "codec.h"
/******* """""""""
 * #define JPEG_QUALITY
 * struct S_JPEG;
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define OP_RGB 0XFE
#define OP_RUN 0XC0
#define OP_DIFF 0X40
#define OP_LUMA 0X80

static int
	PIXEL_HASH(uint32_t PIXEL)
{
	return ((int)(
		(((PIXEL >> 16) & 255) * 3 + ((PIXEL >> 8) & 255) * 5 +
		(PIXEL & 255) * 7) & 63
	));
}

size_t
	ENCODE_TILE(
		const uint32_t *SOURCE,
		int STRIDE,
		int W,
		int H,
		uint8_t *OUTPUT
	)
{
	uint32_t		INDEX[64];
	uint32_t		PREV;
	uint32_t		PIXEL;
	const uint32_t	*ROW;
	size_t			N;
	int				RUN;
	int				X;
	int				Y;
	int				SLOT;
	int				DR;
	int				DG;
	int				DB;

	memset(INDEX, 0, sizeof(INDEX));
	PREV = 0;
	RUN = 0;
	N = 0;
	for (Y = 0; Y < H; ++Y)
	{
		ROW = SOURCE + (size_t)Y * (size_t)STRIDE;
		for (X = 0; X < W; ++X)
		{
			PIXEL = ROW[X] & 0XFFFFFF;
			if (PIXEL == PREV)
			{
				if (++RUN == 62)
				{
					OUTPUT[N++] = OP_RUN | 61;
					RUN = 0;
				}
				continue ;
			}
			if (RUN)
			{
				OUTPUT[N++] = (uint8_t)(OP_RUN | (RUN - 1));
				RUN = 0;
			}
			SLOT = PIXEL_HASH(PIXEL);
			if (INDEX[SLOT] == PIXEL)
			{
				OUTPUT[N++] = (uint8_t)SLOT;
			}
			else
			{
				INDEX[SLOT] = PIXEL;
				DR = (int8_t)(uint8_t)((PIXEL >> 16) - (PREV >> 16));
				DG = (int8_t)(uint8_t)((PIXEL >> 8) - (PREV >> 8));
				DB = (int8_t)(uint8_t)(PIXEL - PREV);
				if (DR >= -2 && DR <= 1 && DG >= -2 && DG <= 1 &&
					DB >= -2 && DB <= 1)
				{
					OUTPUT[N++] = (uint8_t)(OP_DIFF |
						((DR + 2) << 4) | ((DG + 2) << 2) | (DB + 2));
				}
				else if (DG >= -32 && DG <= 31 && DR - DG >= -8 &&
					DR - DG <= 7 && DB - DG >= -8 && DB - DG <= 7)
				{
					OUTPUT[N++] = (uint8_t)(OP_LUMA | (DG + 32));
					OUTPUT[N++] = (uint8_t)(
						((DR - DG + 8) << 4) | (DB - DG + 8)
					);
				}
				else
				{
					OUTPUT[N++] = OP_RGB;
					OUTPUT[N++] = (uint8_t)(PIXEL >> 16);
					OUTPUT[N++] = (uint8_t)(PIXEL >> 8);
					OUTPUT[N++] = (uint8_t)PIXEL;
				}
			}
			PREV = PIXEL;
		}
	}
	if (RUN)
	{
		OUTPUT[N++] = (uint8_t)(OP_RUN | (RUN - 1));
	}
	return (N);
}

int
	DECODE_TILE(
		const uint8_t *BYTES,
		size_t SIZE,
		uint32_t *TARGET,
		int STRIDE,
		int W,
		int H
	)
{
	uint32_t	INDEX[64];
	uint32_t	PREV;
	uint32_t	*ROW;
	size_t		I;
	int			RUN;
	int			X;
	int			Y;
	int			R;
	int			G;
	int			B;
	int			DG;
	uint8_t		OP;
	uint8_t		B2;

	memset(INDEX, 0, sizeof(INDEX));
	PREV = 0;
	RUN = 0;
	I = 0;
	for (Y = 0; Y < H; ++Y)
	{
		ROW = TARGET + (size_t)Y * (size_t)STRIDE;
		for (X = 0; X < W; ++X)
		{
			if (RUN > 0)
			{
				--RUN;
				ROW[X] = PREV;
				continue ;
			}
			if (I >= SIZE)
			{
				return (0);
			}
			OP = BYTES[I++];
			if (OP == OP_RGB)
			{
				if (I + 3 > SIZE)
				{
					return (0);
				}
				PREV = ((uint32_t)BYTES[I] << 16) |
					((uint32_t)BYTES[I + 1] << 8) | BYTES[I + 2];
				I += 3;
				INDEX[PIXEL_HASH(PREV)] = PREV;
			}
			else if (OP == 0XFF)
			{
				return (0);
			}
			else
			{
				R = (PREV >> 16) & 255;
				G = (PREV >> 8) & 255;
				B = PREV & 255;
				switch (OP & 0XC0)
				{
					case (0X00):
						PREV = INDEX[OP & 63];
						break ;
					case (OP_DIFF):
						R = (R + ((OP >> 4) & 3) - 2) & 255;
						G = (G + ((OP >> 2) & 3) - 2) & 255;
						B = (B + (OP & 3) - 2) & 255;
						PREV = ((uint32_t)R << 16) | ((uint32_t)G << 8) |
							(uint32_t)B;
						INDEX[PIXEL_HASH(PREV)] = PREV;
						break ;
					case (OP_LUMA):
						if (I >= SIZE)
						{
							return (0);
						}
						B2 = BYTES[I++];
						DG = (OP & 63) - 32;
						R = (R + DG + ((B2 >> 4) & 15) - 8) & 255;
						G = (G + DG) & 255;
						B = (B + DG + (B2 & 15) - 8) & 255;
						PREV = ((uint32_t)R << 16) | ((uint32_t)G << 8) |
							(uint32_t)B;
						INDEX[PIXEL_HASH(PREV)] = PREV;
						break ;
					default:
						RUN = OP & 63; /* THIS PIXEL + RUN MORE */
						break ;
				}
			}
			ROW[X] = PREV;
		}
	}
	return (I == SIZE && RUN == 0);
}

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |           JPEG WITH WINDOWS IMAGING COMPONENT            |  \\//  */
/*   \/   #==========================================================#   \/   */
int
	JPEG_OPEN(struct S_JPEG *J)
{
	HRESULT	HR;

	memset(J, 0, sizeof(*J));
	HR = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	J->COM = SUCCEEDED(HR);
	HR = CoCreateInstance(
		&CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		&IID_IWICImagingFactory,
		&J->FACTORY
	);
	if (FAILED(HR))
	{
		J->FACTORY = NULL;
	}
	return (J->FACTORY != NULL);
}

void
	JPEG_CLOSE(struct S_JPEG *J)
{
	if (J->FACTORY)
	{
		IWICImagingFactory_Release((IWICImagingFactory *)J->FACTORY);
	}
	free(J->TEMP);
	if (J->COM)
	{
		CoUninitialize();
	}
	memset(J, 0, sizeof(*J));
}

/* 32 BIT BGRX -> 24 BIT BGR, THE FORMAT THE JPEG CODER TAKES */
static int
	TO_24_BIT(
		struct S_JPEG *J,
		const uint32_t *SOURCE,
		int STRIDE,
		int W,
		int H,
		UINT *ROW_BYTES
	)
{
	size_t			NEED;
	uint8_t			*BIGGER;
	uint8_t			*D;
	const uint32_t	*ROW;
	int				X;
	int				Y;

	*ROW_BYTES = (UINT)((W * 3 + 3) & ~3);
	NEED = (size_t)*ROW_BYTES * (size_t)H;
	if (NEED > J->TEMP_CAP)
	{
		BIGGER = (uint8_t *)realloc(J->TEMP, NEED);
		if (!BIGGER)
		{
			return (0);
		}
		J->TEMP = BIGGER;
		J->TEMP_CAP = NEED;
	}
	for (Y = 0; Y < H; ++Y)
	{
		ROW = SOURCE + (size_t)Y * (size_t)STRIDE;
		D = J->TEMP + (size_t)Y * *ROW_BYTES;
		for (X = 0; X < W; ++X)
		{
			D[0] = (uint8_t)ROW[X];
			D[1] = (uint8_t)(ROW[X] >> 8);
			D[2] = (uint8_t)(ROW[X] >> 16);
			D += 3;
		}
	}
	return (1);
}

/* APPENDS THE JPEG OF A W X H AREA TO OUT. 0 WHEN IT DID NOT WORK. */
int
	JPEG_ENCODE(
		struct S_JPEG *J,
		const uint32_t *SOURCE,
		int STRIDE,
		int W,
		int H,
		struct S_TEXT *OUTPUT
	)
{
	IWICImagingFactory		*F;
	IStream					*STREAM;
	IWICBitmapEncoder		*ENCODER;
	IWICBitmapFrameEncode	*FRAME;
	IPropertyBag2			*BAG;
	PROPBAG2				OPTION;
	VARIANT					VALUE;
	WICPixelFormatGUID		FORMAT;
	LARGE_INTEGER			ZERO;
	ULARGE_INTEGER			END;
	HGLOBAL					MEMORY;
	void					*DATA;
	UINT					ROW_BYTES;
	int						OK;

	F = (IWICImagingFactory *)J->FACTORY;
	if (!F || !TO_24_BIT(J, SOURCE, STRIDE, W, H, &ROW_BYTES))
	{
		return (0);
	}
	OK = 0;
	STREAM = NULL;
	ENCODER = NULL;
	FRAME = NULL;
	BAG = NULL;
	if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &STREAM)))
	{
		return (0);
	}
	if (SUCCEEDED(IWICImagingFactory_CreateEncoder(
		F,
		&GUID_ContainerFormatJpeg,
		NULL,
		&ENCODER
	)) &&
		SUCCEEDED(IWICBitmapEncoder_Initialize(
			ENCODER,
			STREAM,
			WICBitmapEncoderNoCache
		)) &&
		SUCCEEDED(IWICBitmapEncoder_CreateNewFrame(ENCODER, &FRAME, &BAG)))
	{
		memset(&OPTION, 0, sizeof(OPTION));
		OPTION.pstrName = L"ImageQuality";
		VariantInit(&VALUE);
		VALUE.vt = VT_R4;
		VALUE.fltVal = JPEG_QUALITY;
		IPropertyBag2_Write(BAG, 1, &OPTION, &VALUE);
		FORMAT = GUID_WICPixelFormat24bppBGR;
		if (SUCCEEDED(IWICBitmapFrameEncode_Initialize(FRAME, BAG)) &&
			SUCCEEDED(IWICBitmapFrameEncode_SetSize(FRAME, (UINT)W, (UINT)H)) &&
			SUCCEEDED(IWICBitmapFrameEncode_SetPixelFormat(FRAME, &FORMAT)) &&
			IsEqualGUID(&FORMAT, &GUID_WICPixelFormat24bppBGR) &&
			SUCCEEDED(IWICBitmapFrameEncode_WritePixels(
				FRAME,
				(UINT)H,
				ROW_BYTES,
				ROW_BYTES * (UINT)H,
				J->TEMP
			)) &&
			SUCCEEDED(IWICBitmapFrameEncode_Commit(FRAME)) &&
			SUCCEEDED(IWICBitmapEncoder_Commit(ENCODER)))
		{
			ZERO.QuadPart = 0;
			if (SUCCEEDED(IStream_Seek(STREAM, ZERO, STREAM_SEEK_CUR, &END)) &&
				SUCCEEDED(GetHGlobalFromStream(STREAM, &MEMORY)))
			{
				DATA = GlobalLock(MEMORY);
				if (DATA)
				{
					TEXT_ADDN(OUTPUT, (const char *)DATA, (size_t)END.QuadPart);
					GlobalUnlock(MEMORY);
					OK = 1;
				}
			}
		}
	}
	if (BAG)
	{
		IPropertyBag2_Release(BAG);
	}
	if (FRAME)
	{
		IWICBitmapFrameEncode_Release(FRAME);
	}
	if (ENCODER)
	{
		IWICBitmapEncoder_Release(ENCODER);
	}
	IStream_Release(STREAM);
	return (OK);
}

/* UNPACKS A JPEG STRAIGHT INTO THE SCREEN PICTURE */
int
	JPEG_DECODE(
		struct S_JPEG *J,
		const uint8_t *BYTES,
		size_t SIZE,
		uint32_t *TARGET,
		int STRIDE,
		int W,
		int H
	)
{
	IWICImagingFactory		*F;
	IWICStream				*STREAM;
	IWICBitmapDecoder		*DECODER;
	IWICBitmapFrameDecode	*FRAME;
	IWICBitmapSource		*CONVERTED;
	UINT					FW;
	UINT					FH;
	int						OK;

	F = (IWICImagingFactory *)J->FACTORY;
	if (!F || !SIZE)
	{
		return (0);
	}
	OK = 0;
	STREAM = NULL;
	DECODER = NULL;
	FRAME = NULL;
	CONVERTED = NULL;
	if (SUCCEEDED(IWICImagingFactory_CreateStream(F, &STREAM)) &&
		SUCCEEDED(IWICStream_InitializeFromMemory(
			STREAM,
			(BYTE *)BYTES,
			(DWORD)SIZE
		)) &&
		SUCCEEDED(IWICImagingFactory_CreateDecoderFromStream(
			F,
			(IStream *)STREAM,
			NULL,
			WICDecodeMetadataCacheOnDemand,
			&DECODER
		)) &&
		SUCCEEDED(IWICBitmapDecoder_GetFrame(DECODER, 0, &FRAME)) &&
		SUCCEEDED(IWICBitmapFrameDecode_GetSize(FRAME, &FW, &FH)) &&
		FW == (UINT)W && FH == (UINT)H &&
		SUCCEEDED(WICConvertBitmapSource(
			&GUID_WICPixelFormat32bppBGR,
			(IWICBitmapSource *)FRAME,
			&CONVERTED
		)))
	{
		OK = SUCCEEDED(IWICBitmapSource_CopyPixels(
			CONVERTED,
			NULL,
			(UINT)STRIDE * 4,
			(UINT)((size_t)(H - 1) * (size_t)STRIDE * 4 + (size_t)W * 4),
			(BYTE *)TARGET
		));
	}
	if (CONVERTED)
	{
		IWICBitmapSource_Release(CONVERTED);
	}
	if (FRAME)
	{
		IWICBitmapFrameDecode_Release(FRAME);
	}
	if (DECODER)
	{
		IWICBitmapDecoder_Release(DECODER);
	}
	if (STREAM)
	{
		IWICStream_Release(STREAM);
	}
	return (OK);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |           JPEG WITH WINDOWS IMAGING COMPONENT            |  //\\  */
/* '""""' #==========================================================# '""""' */
