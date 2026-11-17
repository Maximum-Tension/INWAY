/******************************************************************************\
# H - INWAY                                      #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# :  License - GPL-3.0  :: Update - 2026/11/17 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/
/******************************************************************************\
|* __________________________________________________________________________ *|
|*|                                  INWAY                                   |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE ONE HEADER EVERY INWAY SOURCE FILE INCLUDES. IT PULLS IN WINDOWS,    |*|
|*| THE C LIBRARY AND ALL INWAY MODULES, AND HOLDS WHAT BOTH SIDES OF A      |*|
|*| CONNECTION MUST AGREE ON: THE PORT, THE PROTOCOL NUMBER AND THE MESSAGES |*|
|*| ON THE WIRE.                                                             |*|
|*| THE PORT IS NOT THE ONE THE OLD CONNECT TOOL USED (50505), SO BOTH CAN   |*|
|*| RUN ON ONE PC WHILE A PC IS MOVED FROM CONNECT TO INWAY.                 |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| #define INWAY_PORT                                                  : *|
|* : 3| #define INWAY_MAGIC                                                 : *|
|* : 4| struct S_MSG_HDR;                                                   : *|
|* : 5| struct S_TILE_HDR;                                                  : *|
|* : 6| struct S_INPUT_MSG;                                                 : *|
|* : 7| enum E_RESULT;                                                      : *|
|* : 8|                                                                     : *|
|* :                                                                        : *|
|* : EXAMPLES:                                                              : *|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| #include "inway.h"                                                 : *|
|* :  3|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef INWAY_H
#define INWAY_H

/* ************************ [v] WINDOWS VERSION [v] ************************* */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0X0601 /* WINDOWS 7 AND NEWER API */
#endif /* !_WIN32_WINNT */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif /* !WIN32_LEAN_AND_MEAN */
#ifndef COBJMACROS
#define COBJMACROS /* IMONIKER_RELEASE() STYLE COM CALLS IN C */
#endif /* !COBJMACROS */
/* ************************ [^] WINDOWS VERSION [^] ************************* */

/* **************************** [v] INCLUDES [v] **************************** */
#include <winsock2.h>
/******* """"""""""""
 * SOCKET, send(), recv(), select(), WSAStartup()
 */
#include <ws2tcpip.h>
/******* """"""""""""
 * getaddrinfo(), TCP_NODELAY
 */
#include <windows.h>
/******* """""""""""
 * HANDLE, CreateThread(), SendInput(), BitBlt()
 */
#include <windowsx.h>
/******* """"""""""""
 * GET_X_LPARAM(), GET_Y_LPARAM()
 */
#include <shellapi.h>
/******* """"""""""""
 * ShellExecuteExW()
 */
#include <stdio.h>
/******* """""""""
 * fgets(), snprintf(), vsnprintf()
 */
#include <stdlib.h>
/******* """"""""""
 * malloc(), realloc(), free(), atoi()
 */
#include <string.h>
/******* """"""""""
 * memcpy(), memcmp(), strlen(), strcmp()
 */
#include <stdint.h>
/******* """"""""""
 * uint8_t, uint16_t, uint32_t, int32_t
 */
#include <stdarg.h>
/******* """"""""""
 * va_list, va_start(), va_end()
 */
#include <time.h>
/******* """"""""
 * time()
 */
/* **************************** [^] INCLUDES [^] **************************** */

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0X0004 /* OLD MINGW HEADERS */
#endif /* !ENABLE_VIRTUAL_TERMINAL_PROCESSING */

/* *************************** [v] CONNECTION [v] *************************** */
#define INWAY_PORT			50506			/* TCP PORT OF THE SHARING SIDE */
#define INWAY_MAGIC			0X494E5731	/* "INW1": PROTOCOL NUMBER */
#define INWAY_TILE			64				/* SCREEN IS SENT IN 64X64 TILES */
#define INWAY_FRAME_MS		16				/* UP TO 60 FRAMES A SECOND */
#define INWAY_IDLE_MS		40				/* WHEN NOTHING MOVES */
#define INWAY_ASK_SECONDS	60				/* ACCEPT WINDOW GIVES UP AFTER */
#define INWAY_SAS_EVENT		L"Global\\INWAY_SAS"	/* WORKER -> SERVICE */

/*
# REPLIES THE SHARING SIDE SENDS INSTEAD OF A SCREEN SIZE:
# - {MAGIC,  0,  0} THE PERSON CLICKED CANCEL
# - {MAGIC, -1, -1} SOMEONE ELSE IS ALREADY CONNECTED
# - {MAGIC, -2, -2} WRONG CODE
# - {MAGIC, -3, -3} THIS PC IS A SERVER AND ONLY TAKES ITS CODE
*/
#define REPLY_DECLINED		0
#define REPLY_BUSY			-1
#define REPLY_WRONG_CODE	-2
#define REPLY_NEED_CODE		-3
/* *************************** [^] CONNECTION [^] *************************** */

/* ********************** [v] MESSAGES ON THE WIRE [v] ********************** */
/*
# AFTER THE CODE AND THE {MAGIC, W, H} HELLO, BOTH SIDES ONLY SEND MESSAGES:
# A S_MSG_HDR, THEN "BYTES" BYTES OF DATA. THE SAME FORMAT GOES BOTH WAYS.
*/
#pragma pack(push, 1)
struct	S_MSG_HDR
{
	uint32_t	TYPE;	/* MSG_FRAME, MSG_INPUT, MSG_CLIP_... */
	uint32_t	BYTES;	/* SIZE OF THE DATA THAT FOLLOWS */
};

struct	S_TILE_HDR
{
	uint16_t	X;
	uint16_t	Y;
	uint16_t	W;
	uint16_t	H;
	uint32_t	LEN;	/* SIZE OF THE PACKED PIXELS */
	uint8_t	KIND;	/* TILE_EXACT OR TILE_JPEG */
};

struct	S_INPUT_MSG
{
	int32_t		TYPE;	/* IN_MOVE, IN_BUTTON, IN_WHEEL, IN_KEY, IN_SAS */
	int32_t		A;
	int32_t		B;
	int32_t		C;
};

struct	S_CLIP_FILE
{
	uint32_t	ID;	/* WHICH COPY THIS BELONGS TO */
	uint32_t	IS_FOLDER;
	uint64_t	SIZE;
	/* THEN THE PATH INSIDE THE COPY, UTF-16, ENDING WITH 0 */
};
#pragma pack(pop)

#define MSG_FRAME		1	/* HOST > VIEWER: UINT32 TILE COUNT, THEN TILES */
#define MSG_INPUT		2	/* VIEWER > HOST: STRUCT S_INPUT_MSG */
#define MSG_CLIP_TEXT	3	/* BOTH WAYS: COPIED TEXT, UTF-16 */
#define MSG_CLIP_BEGIN	4	/* BOTH WAYS: UINT32 ID, FILES ARE COMING */
#define MSG_CLIP_FILE	5	/* BOTH WAYS: STRUCT S_CLIP_FILE + PATH */
#define MSG_CLIP_DATA	6	/* BOTH WAYS: UINT32 ID + A PIECE OF THE FILE */
#define MSG_CLIP_END	7	/* BOTH WAYS: UINT32 ID, ALL FILES ARRIVED */
#define MSG_CLIP_ABORT	8	/* BOTH WAYS: UINT32 ID, FORGET THAT COPY */

#define TILE_EXACT		0	/* LOSSLESS, SEE CODEC.C */
#define TILE_JPEG		1	/* JPEG, USED WHILE THE SCREEN MOVES */

#define IN_MOVE			1	/* A = X, B = Y */
#define IN_BUTTON		2	/* A = BUTTON 0-2, B = DOWN */
#define IN_WHEEL		3	/* A = WHEEL DELTA */
#define IN_KEY			4	/* A = VIRTUAL KEY, B = DOWN, C = EXTENDED */
#define IN_SAS			5	/* CTRL + ALT + DEL */
/* ********************** [^] MESSAGES ON THE WIRE [^] ********************** */

/* *********************** [v] CONNECTION RESULTS [v] *********************** */
enum	E_RESULT
{
	CR_ENDED,		/* CONNECTED, THEN THE WINDOW WAS CLOSED */
	CR_NO_PC,		/* NOBODY ANSWERED ON THAT ADDRESS */
	CR_WRONG_CODE,
	CR_NEED_CODE,	/* A SERVER: ONLY ITS CODE WORKS */
	CR_DECLINED,	/* THEY CLICKED CANCEL */
	CR_BUSY,
	CR_NO_ANSWER,	/* NOBODY CLICKED ACCEPT IN TIME */
	CR_DROPPED,
	CR_VERSION,
	CR_ERROR
};
/* *********************** [^] CONNECTION RESULTS [^] *********************** */

/* ************************* [v] INWAY MODULES [v] ************************** */
#include "version.h"
/******* """""""""""
 * #define INWAY_VERSION_TEXT
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT, TEXT_ADD(), UTF8_TO_WIDE()
 */
#include "lang.h"
/******* """"""""
 * LANG(), LANG_INIT()
 */
#include "console.h"
/******* """""""""""
 * SAY(), ASK(), NOTIFY(), BOX()
 */
#include "net.h"
/******* """""""
 * SEND_ALL(), RECV_ALL(), PARSE_ADDRESS()
 */
#include "codec.h"
/******* """""""""
 * ENCODE_TILE(), JPEG_ENCODE(), JPEG_DECODE()
 */
#include "clipboard.h"
/******* """""""""""""
 * CLIP_START(), CLIP_RECEIVE(), CLIP_STOP()
 */
#include "htmlui.h"
/******* """"""""""
 * HTMLUI_ASK(), HTMLUI_BANNER_SHOW()
 */
#include "host.h"
/******* """"""""
 * HOST_START(), G_CODE, G_WORKER
 */
#include "viewer.h"
/******* """"""""""
 * VIEWER_CONNECT()
 */
#include "service.h"
/******* """""""""""
 * SERVICE_RUN(), SERVICE_INSTALL()
 */
#include "update.h"
/******* """"""""""
 * UPDATE_CHECK(), UPDATE_FINISH()
 */
/* ************************* [^] INWAY MODULES [^] ************************** */

#endif /* !INWAY_H */
