/******************************************************************************\
# C - host                                       #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/23 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# THE SHARING SIDE. ONE THREAD ACCEPTS CONNECTIONS, ONE THREAD PER CONNECTION
# CHECKS THE CODE AND SENDS THE SCREEN, AND ONE MORE THREAD PLAYS BACK THE MOUSE
# AND KEYBOARD.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define IN_BUTTON
 * #define IN_KEY
 * #define IN_MOVE
 * #define IN_SAS
 * #define IN_WHEEL
 * #define INWAY_ADMIN_SECONDS
 * #define INWAY_FRAME_MS
 * #define INWAY_IDLE_MS
 * #define INWAY_MAGIC
 * #define INWAY_PORT
 * #define INWAY_SAS_EVENT
 * #define INWAY_TILE
 * #define MSG_CLIP_ABORT
 * #define MSG_CLIP_TEXT
 * #define MSG_FRAME
 * #define MSG_INPUT
 * #define REPLY_BUSY
 * #define REPLY_DECLINED
 * #define REPLY_NEED_CODE
 * #define REPLY_UPDATING
 * #define REPLY_WRONG_CODE
 * #define TILE_EXACT
 * #define TILE_JPEG
 * #define WIN32_LEAN_AND_MEAN
 * struct S_INPUT_MSG;
 * struct S_TILE_HDR;
 */
#include <winsock2.h>
/******* """"""""""""
 * #define AF_INET
 * #define FD_SET(fd, set)
 * #define FD_ZERO(set)
 * #define INADDR_ANY
 * #define INADDR_LOOPBACK
 * #define INVALID_SOCKET
 * #define IPPROTO_TCP
 * #define SD_BOTH
 * #define SD_SEND
 * #define SO_RCVTIMEO
 * #define SO_SNDBUF
 * #define SOCK_STREAM
 * #define SOL_SOCKET
 * #define TCP_NODELAY
 * typedef	fd_set;
 * typedef	SOCKET;
 * struct sockaddr;
 * struct sockaddr_in;
 * typedef	u_short;
 * SOCKET	accept(SOCKET, struct sockaddr *, int *);
 * int		bind(SOCKET, const struct sockaddr *, int);
 * int		closesocket(SOCKET);
 * int		connect(SOCKET, const struct sockaddr *, int);
 * int		getpeername(SOCKET, struct sockaddr *, int *);
 * int		getsockname(SOCKET, struct sockaddr *, int *);
 * u_long	htonl(u_long);
 * u_short	htons(u_short);
 * int		listen(SOCKET, int);
 * u_long	ntohl(u_long);
 * u_short	ntohs(u_short);
 * int		recv(SOCKET, char *, int, int);
 * int		select(int, fd_set *, fd_set *, fd_set *, const TIMEVAL *);
 * int		setsockopt(SOCKET, int, int, const char *, int);
 * int		shutdown(SOCKET, int);
 * SOCKET	socket(int, int, int);
 */
#include <windows.h>
/******* """""""""""
 * #define BI_RGB
 * #define CURSOR_SHOWING
 * #define DI_NORMAL
 * #define DIB_RGB_COLORS
 * #define DUPLICATE_SAME_ACCESS
 * #define ERROR_IO_PENDING
 * #define ERROR_PIPE_CONNECTED
 * #define EVENT_MODIFY_STATE
 * #define FALSE
 * #define FILE_FLAG_FIRST_PIPE_INSTANCE
 * #define FILE_FLAG_OVERLAPPED
 * #define GENERIC_ALL
 * #define GENERIC_READ
 * #define GENERIC_WRITE
 * #define IDC_ARROW
 * #define INFINITE
 * #define INPUT_KEYBOARD
 * #define INPUT_MOUSE
 * #define InterlockedCompareExchange
 * #define InterlockedDecrement
 * #define InterlockedExchange
 * #define INVALID_HANDLE_VALUE
 * #define KEYEVENTF_EXTENDEDKEY
 * #define KEYEVENTF_KEYUP
 * #define LoadCursor
 * #define MAX_PATH
 * #define MOUSEEVENTF_LEFTDOWN
 * #define MOUSEEVENTF_LEFTUP
 * #define MOUSEEVENTF_MIDDLEDOWN
 * #define MOUSEEVENTF_MIDDLEUP
 * #define MOUSEEVENTF_RIGHTDOWN
 * #define MOUSEEVENTF_RIGHTUP
 * #define MOUSEEVENTF_WHEEL
 * #define OPEN_EXISTING
 * #define PIPE_ACCESS_DUPLEX
 * #define PIPE_REJECT_REMOTE_CLIENTS
 * #define PIPE_TYPE_BYTE
 * #define PIPE_WAIT
 * #define SM_CXSCREEN
 * #define SM_CYSCREEN
 * #define SRCCOPY
 * #define SW_HIDE
 * #define SYNCHRONIZE
 * #define TRUE
 * #define UOI_NAME
 * #define WAIT_OBJECT_0
 * #define WINAPI
 * typedef	BITMAPINFO;
 * typedef	BITMAPINFOHEADER;
 * typedef	BOOL;
 * typedef	CURSORINFO;
 * typedef	DWORD;
 * typedef	HANDLE;
 * typedef	HBITMAP;
 * typedef	HCURSOR;
 * typedef	HDC;
 * typedef	HDESK;
 * typedef	HGDIOBJ;
 * typedef	ICONINFO;
 * typedef	INPUT;
 * typedef	LONG;
 * typedef	LPVOID;
 * typedef	OVERLAPPED;
 * typedef	POINT;
 * typedef	WCHAR;
 * typedef	WORD;
 * BOOL		BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
 * BOOL		CancelIo(HANDLE);
 * BOOL		CloseDesktop(HDESK);
 * BOOL		CloseHandle(HANDLE);
 * BOOL		ConnectNamedPipe(HANDLE, LPOVERLAPPED);
 * HDC		CreateCompatibleDC(HDC);
 * HBITMAP	CreateDIBSection(
 * 	HDC,
 * 	const BITMAPINFO *,
 * 	UINT,
 * 	void **,
 * 	HANDLE,
 * 	DWORD
 * );
 * HANDLE	CreateEventW(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCWSTR);
 * HANDLE	CreateFileW(
 * 	LPCWSTR,
 * 	DWORD,
 * 	DWORD,
 * 	LPSECURITY_ATTRIBUTES,
 * 	DWORD,
 * 	DWORD,
 * 	HANDLE
 * );
 * HANDLE	CreateNamedPipeW(
 * 	LPCWSTR,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	LPSECURITY_ATTRIBUTES
 * );
 * HANDLE	CreateThread(
 * 	LPSECURITY_ATTRIBUTES,
 * 	SIZE_T,
 * 	LPTHREAD_START_ROUTINE,
 * 	LPVOID,
 * 	DWORD,
 * 	LPDWORD
 * );
 * BOOL		DeleteDC(HDC);
 * BOOL		DeleteObject(HGDIOBJ);
 * BOOL		DrawIconEx(HDC, int, int, HICON, int, int, UINT, HBRUSH, UINT);
 * BOOL		DuplicateHandle(
 * 	HANDLE,
 * 	HANDLE,
 * 	HANDLE,
 * 	LPHANDLE,
 * 	DWORD,
 * 	BOOL,
 * 	DWORD
 * );
 * void		EnterCriticalSection(LPCRITICAL_SECTION);
 * BOOL		GdiFlush(void);
 * HANDLE	GetCurrentProcess(void);
 * DWORD	GetCurrentProcessId(void);
 * DWORD	GetCurrentThreadId(void);
 * BOOL		GetCursorInfo(PCURSORINFO);
 * BOOL		GetCursorPos(LPPOINT);
 * HDC		GetDC(HWND);
 * BOOL		GetExitCodeProcess(HANDLE, LPDWORD);
 * BOOL		GetExitCodeThread(HANDLE, LPDWORD);
 * BOOL		GetIconInfo(HICON, PICONINFO);
 * DWORD	GetLastError(void);
 * DWORD	GetModuleFileNameW(HMODULE, LPWSTR, DWORD);
 * BOOL		GetOverlappedResult(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
 * int		GetSystemMetrics(int);
 * HDESK	GetThreadDesktop(DWORD);
 * DWORD	GetTickCount(void);
 * BOOL		GetUserObjectInformationA(HANDLE, int, PVOID, DWORD, LPDWORD);
 * void		LeaveCriticalSection(LPCRITICAL_SECTION);
 * HANDLE	OpenEventW(DWORD, BOOL, LPCWSTR);
 * HDESK	OpenInputDesktop(DWORD, BOOL, ACCESS_MASK);
 * HANDLE	OpenProcess(DWORD, BOOL, DWORD);
 * BOOL		ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
 * int		ReleaseDC(HWND, HDC);
 * HGDIOBJ	SelectObject(HDC, HGDIOBJ);
 * UINT		SendInput(UINT, LPINPUT, int);
 * BOOL		SetCursorPos(int, int);
 * BOOL		SetEvent(HANDLE);
 * BOOL		SetThreadDesktop(HDESK);
 * void		Sleep(DWORD);
 * DWORD	WaitForMultipleObjects(DWORD, const HANDLE *, BOOL, DWORD);
 * DWORD	WaitForSingleObject(HANDLE, DWORD);
 * BOOL		WaitNamedPipeW(LPCWSTR, DWORD);
 * BOOL		WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
 */
#include <shellapi.h>
/******* """"""""""""
 * #define SEE_MASK_NOASYNC
 * #define SEE_MASK_NOCLOSEPROCESS
 * typedef	SHELLEXECUTEINFOW;
 * BOOL		ShellExecuteExW(SHELLEXECUTEINFOW *);
 */
#include <bcrypt.h>
/******* """"""""""
 * #define BCRYPT_USE_SYSTEM_PREFERRED_RNG
 * NTSTATUS	BCryptGenRandom(BCRYPT_ALG_HANDLE, PUCHAR, ULONG, ULONG);
 */
#include <stdio.h>
/******* """""""""
 * int	snprintf(char *, size_t, const char *, ...);
 * int	swprintf(wchar_t *, size_t, const wchar_t *, ...);
 */
#include <stdlib.h>
/******* """"""""""
 * void	*calloc(size_t, size_t);
 * void	free(void *);
 * void	*malloc(size_t);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * typedef	size_t;
 * int		memcmp(const void *, const void *, size_t);
 * void		*memcpy(void *, const void *, size_t);
 * void		*memset(void *, int, size_t);
 * int		strcmp(const char *, const char *);
 */
#include <stdint.h>
/******* """"""""""
 * typedef	int32_t;
 * typedef	uint16_t;
 * typedef	uint32_t;
 * typedef	uint8_t;
 * typedef	uintptr_t;
 */
#include <time.h>
/******* """"""""
 * struct timeval;
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT;
 * void	TEXT_ADD(struct S_TEXT *, const char *);
 * void	TEXT_ADDF(struct S_TEXT *, const char *, ...);
 * void	TEXT_ADDN(struct S_TEXT *, const char *, size_t);
 * void	TEXT_FREE(struct S_TEXT *);
 */
#include "lang.h"
/******* """"""""
 * enum E_STR;
 * const char	*LANG(enum E_STR);
 */
#include "console.h"
/******* """""""""""
 * CRITICAL_SECTION	G_CONSOLE_LOCK;
 * char				G_HOST_STATUS[64];
 * void				NOTIFY(const char *, ...);
 * const char		*SYM(const char *, const char *);
 */
#include "net.h"
/******* """""""
 * struct S_LINK;
 * void	IP_SHORT(uint32_t, char *, size_t);
 * void	NAME_ANSWER_START(void);
 * void	LINK_FREE(struct S_LINK *);
 * void	LINK_INIT(struct S_LINK *, SOCKET);
 * int	LINK_SEND(
 * 	struct S_LINK *,
 * 	uint32_t,
 * 	const void *,
 * 	uint32_t,
 * 	const void *,
 * 	uint32_t
 * );
 * int	RECV_ALL(SOCKET, void *, int);
 * int	RECV_MSG(
 * 	SOCKET,
 * 	uint32_t *,
 * 	uint8_t **,
 * 	uint32_t *,
 * 	uint32_t *,
 * 	uint32_t
 * );
 * int	SEND_ALL(SOCKET, const void *, int);
 */
#include "codec.h"
/******* """""""""
 * struct S_JPEG;
 * size_t	ENCODE_TILE(const uint32_t *, int, int, int, uint8_t *);
 * void		JPEG_CLOSE(struct S_JPEG *);
 * int		JPEG_ENCODE(
 * 	struct S_JPEG *,
 * 	const uint32_t *,
 * 	int,
 * 	int,
 * 	int,
 * 	struct S_TEXT *
 * );
 * int		JPEG_OPEN(struct S_JPEG *);
 */
#include "clipboard.h"
/******* """""""""""""
 * #define CLIP_CHUNK
 * #define CLIP_MAX_TEXT
 * struct S_CLIP;
 * void				CLIP_RECEIVE(
 * 	struct S_CLIP *,
 * 	uint32_t,
 * 	const uint8_t *,
 * 	uint32_t
 * );
 * struct S_CLIP	*CLIP_START(struct S_LINK *, const char *);
 * void				CLIP_STOP(struct S_CLIP *);
 */
#include "htmlui.h"
/******* """"""""""
 * int	HTMLUI_ASK(SOCKET, const char *);
 */
#include "host.h"
/******* """"""""
 * void		HOST_START(void);
 * int		HOST_TAKE_OVER(const char *);
 * DWORD	HOST_THREAD(LPVOID);
 * SOCKET	OPEN_LISTENER(void);
 */
#include "service.h"
/******* """""""""""
 * int		SERVICE_IS_ADMIN(void);
 * HANDLE	SERVICE_NOTICE_START(const char *);
 * void		SERVICE_NOTICE_STOP(HANDLE);
 */
#include "update.h"
/******* """"""""""
 * int	UPDATE_DUE(void);
 * void	UPDATE_NOW(void);
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* **************************** [v] GLOBALS [v] ***************************** */
volatile LONG			G_RUNNING = 0;
int						G_WORKER = 0;
int						G_BACKGROUND = 0;
int						G_CODE = 0;
SOCKET					G_LISTEN = INVALID_SOCKET;
static volatile LONG	G__BUSY__ = 0; /* A REQUEST OR SESSION IS OPEN */
static int				G__WRONG_IN_A_ROW__ = 0; /* GUARDED BY G__BUSY__ */
static volatile LONG	G__UPDATING_SINCE__ = 0; /* TICK, 0 = NOT UPDATING */
/* **************************** [^] GLOBALS [^] ***************************** */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                   FOLLOWING THE SCREEN                   |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# THE LOCK SCREEN, THE LOGIN SCREEN AND "DO YOU WANT TO ALLOW THIS APP?"
# POPUPS LIVE ON A SEPARATE, PROTECTED DESKTOP. WITH SYSTEM RIGHTS (THE
# BACKGROUND SERVICE) THIS THREAD FOLLOWS WHICHEVER DESKTOP IS SHOWING.
# FOR A NORMAL PROGRAM OPENINPUTDESKTOP() FAILS THERE AND NOTHING HAPPENS.
*/
static int
	FOLLOW_DESKTOP(HDESK *OWNED)
{
	HDESK	INPUT;
	char	A[256];
	char	B[256];
	DWORD	N;

	INPUT = OpenInputDesktop(0, FALSE, GENERIC_ALL);
	if (!INPUT)
	{
		return (0);
	}
	memset(A, 0, sizeof(A));
	memset(B, 0, sizeof(B));
	GetUserObjectInformationA(INPUT, UOI_NAME, A, sizeof(A), &N);
	GetUserObjectInformationA(
		GetThreadDesktop(GetCurrentThreadId()),
		UOI_NAME,
		B,
		sizeof(B),
		&N
	);
	if (strcmp(A, B) == 0 || !SetThreadDesktop(INPUT))
	{
		CloseDesktop(INPUT);
		return (0);
	}
	if (*OWNED)
	{
		CloseDesktop(*OWNED);
	}
	*OWNED = INPUT;
	return (1);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                   FOLLOWING THE SCREEN                   |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                    MOUSE AND KEYBOARD                    |  \\//  */
/*   \/   #==========================================================#   \/   */
struct	S_SESSION
{
	struct S_LINK	*LINK;
	struct S_CLIP	*CLIP;
};

static void
	PLAY_INPUT(const struct S_INPUT_MSG *M)
{
	static const DWORD	DOWN[3] = {
		MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_MIDDLEDOWN
	};
	static const DWORD	UP[3] = {
		MOUSEEVENTF_LEFTUP, MOUSEEVENTF_RIGHTUP, MOUSEEVENTF_MIDDLEUP
	};
	HANDLE	EVENT;
	INPUT	ACTION;

	memset(&ACTION, 0, sizeof(ACTION));
	switch (M->TYPE)
	{
		case (IN_MOVE):
			SetCursorPos(M->A, M->B); /* EXACT PIXEL POSITION */
			return ;
		case (IN_BUTTON):
			if (M->A < 0 || M->A > 2)
			{
				return ;
			}
			ACTION.type = INPUT_MOUSE;
			ACTION.mi.dwFlags = M->B ? DOWN[M->A] : UP[M->A];
			break ;
		case (IN_WHEEL):
			ACTION.type = INPUT_MOUSE;
			ACTION.mi.mouseData = (DWORD)M->A;
			ACTION.mi.dwFlags = MOUSEEVENTF_WHEEL;
			break ;
		case (IN_KEY):
			ACTION.type = INPUT_KEYBOARD;
			ACTION.ki.wVk = (WORD)M->A;
			ACTION.ki.dwFlags = (M->B ? 0 : KEYEVENTF_KEYUP) |
				(M->C ? KEYEVENTF_EXTENDEDKEY : 0);
			break ;
		case (IN_SAS):
			/* ONLY THE WINDOWS SERVICE MAY PRESS CTRL + ALT + DEL */
			if (G_WORKER)
			{
				EVENT = OpenEventW(EVENT_MODIFY_STATE, FALSE, INWAY_SAS_EVENT);
				if (EVENT)
				{
					SetEvent(EVENT);
					CloseHandle(EVENT);
				}
			}
			return ;
		default:
			return ;
	}
	SendInput(1, &ACTION, sizeof(ACTION));
}

/* READS EVERYTHING THE VIEWER SENDS: MOUSE, KEYS AND CLIPBOARD */
static DWORD WINAPI
	INPUT_THREAD(LPVOID PARAMETER)
{
	struct S_SESSION	*SESSION;
	struct S_INPUT_MSG	M;
	HDESK				DESK;
	uint8_t				*BUFFER;
	uint32_t			CAP;
	uint32_t			SIZE;
	uint32_t			TYPE;

	SESSION = (struct S_SESSION *)PARAMETER;
	DESK = NULL;
	BUFFER = NULL;
	CAP = 0;
	while (G_RUNNING && RECV_MSG(
		SESSION->LINK->S,
		&TYPE,
		&BUFFER,
		&SIZE,
		&CAP,
		CLIP_MAX_TEXT + CLIP_CHUNK + 64
	))
	{
		if (TYPE == MSG_INPUT && SIZE >= sizeof(M))
		{
			memcpy(&M, BUFFER, sizeof(M));
			FOLLOW_DESKTOP(&DESK);
			PLAY_INPUT(&M);
		}
		else if (TYPE >= MSG_CLIP_TEXT && TYPE <= MSG_CLIP_ABORT)
		{
			CLIP_RECEIVE(SESSION->CLIP, TYPE, BUFFER, SIZE);
		}
	}
	free(BUFFER);
	InterlockedExchange(&G_RUNNING, 0);
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                    MOUSE AND KEYBOARD                    |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                    SENDING THE SCREEN                    |  \\//  */
/*   \/   #==========================================================#   \/   */
struct	S_CAPTURE
{
	HDC			SCREEN;
	HDC			MEMORY;
	HBITMAP		DIB;
	HGDIOBJ		OLD;
	uint32_t	*PIXELS;
};

/* MADE AGAIN EVERY TIME THE DESKTOP CHANGES */
static void
	CAPTURE_OPEN(struct S_CAPTURE *C, int W, int H)
{
	BITMAPINFO	BI;
	void		*BITS;

	memset(&BI, 0, sizeof(BI));
	BI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BI.bmiHeader.biWidth = W;
	BI.bmiHeader.biHeight = -H;
	BI.bmiHeader.biPlanes = 1;
	BI.bmiHeader.biBitCount = 32;
	BI.bmiHeader.biCompression = BI_RGB;
	BITS = NULL;
	C->SCREEN = GetDC(NULL);
	C->MEMORY = CreateCompatibleDC(C->SCREEN);
	C->DIB = CreateDIBSection(C->SCREEN, &BI, DIB_RGB_COLORS, &BITS, NULL, 0);
	C->OLD = C->DIB ? SelectObject(C->MEMORY, C->DIB) : NULL;
	C->PIXELS = (uint32_t *)BITS;
}

static void
	CAPTURE_CLOSE(struct S_CAPTURE *C)
{
	if (C->MEMORY && C->OLD)
	{
		SelectObject(C->MEMORY, C->OLD);
	}
	if (C->DIB)
	{
		DeleteObject(C->DIB);
	}
	if (C->MEMORY)
	{
		DeleteDC(C->MEMORY);
	}
	if (C->SCREEN)
	{
		ReleaseDC(NULL, C->SCREEN);
	}
	memset(C, 0, sizeof(*C));
}

static void
	DRAW_CURSOR(HDC MEMORY)
{
	CURSORINFO	CI;
	ICONINFO	II;
	HCURSOR		CURSOR;
	POINT		AT;

	memset(&CI, 0, sizeof(CI));
	CI.cbSize = sizeof(CI);
	CURSOR = NULL;
	if (GetCursorInfo(&CI))
	{
		AT = CI.ptScreenPos;
		if ((CI.flags & CURSOR_SHOWING) && CI.hCursor)
		{
			CURSOR = CI.hCursor;
		}
	}
	else if (!GetCursorPos(&AT))
	{
		return ;
	}
	/*
	# WINDOWS HIDES THE POINTER WHEN IT THINKS NO MOUSE IS PLUGGED IN, OR AFTER
	# A TOUCH. THE OTHER PC STILL HAS TO SEE WHERE IT IS: THEN DRAW AN ARROW.
	*/
	if (!CURSOR || !GetIconInfo(CURSOR, &II))
	{
		CURSOR = LoadCursor(NULL, IDC_ARROW);
		if (!GetIconInfo(CURSOR, &II))
		{
			return ;
		}
	}
	DrawIconEx(
		MEMORY,
		AT.x - (int)II.xHotspot,
		AT.y - (int)II.yHotspot,
		CURSOR,
		0,
		0,
		0,
		NULL,
		DI_NORMAL
	);
	if (II.hbmMask)
	{
		DeleteObject(II.hbmMask);
	}
	if (II.hbmColor)
	{
		DeleteObject(II.hbmColor);
	}
}

/* ONE LOSSLESS 64 X 64 TILE INTO THE FRAME */
static void
	ADD_EXACT(
		struct S_TEXT *FRAME,
		const uint32_t *PIXELS,
		uint8_t *PACKED,
		int SCREEN_W,
		int SCREEN_H,
		int TX,
		int TY
	)
{
	struct S_TILE_HDR	TH;

	TH.X = (uint16_t)(TX * INWAY_TILE);
	TH.Y = (uint16_t)(TY * INWAY_TILE);
	TH.W = (uint16_t)(SCREEN_W - TH.X < INWAY_TILE ?
		SCREEN_W - TH.X : INWAY_TILE);
	TH.H = (uint16_t)(SCREEN_H - TH.Y < INWAY_TILE ?
		SCREEN_H - TH.Y : INWAY_TILE);
	TH.KIND = TILE_EXACT;
	TH.LEN = (uint32_t)ENCODE_TILE(
		PIXELS + (size_t)TH.Y * (size_t)SCREEN_W + TH.X,
		SCREEN_W,
		TH.W,
		TH.H,
		PACKED
	);
	TEXT_ADDN(FRAME, (const char *)&TH, sizeof(TH));
	TEXT_ADDN(FRAME, (const char *)PACKED, TH.LEN);
}

/*
# THE SCREEN IS A GRID OF 64 X 64 TILES. EACH FRAME:
# - ONLY TILES THAT CHANGED ARE SENT.
# - WHEN A LOT CHANGED AT ONCE (A WINDOW OPENS, A VIDEO PLAYS), THE CHANGED
#   ROWS ARE SENT AS A FEW JPEG PICTURES: MANY TIMES SMALLER, SO IT STAYS
#   SMOOTH. THOSE TILES ARE MARKED "LOSSY".
# - A LOSSY TILE THAT HAS NOT CHANGED FOR A FEW FRAMES IS SENT AGAIN
#   LOSSLESS, A QUARTER OF THE SCREEN PER FRAME, SO TEXT BECOMES SHARP.
*/
struct	S_GRID
{
	int		COLS;
	int		ROWS;
	uint8_t	*CHANGED;
	uint8_t	*LOSSY;
	uint8_t	*QUIET; /* FRAMES SINCE THE LAST CHANGE, UP TO 255 */
};

static int
	ADD_JPEG(
		struct S_TEXT *FRAME,
		struct S_JPEG *JPEG,
		struct S_TEXT *SCRATCH,
		const uint32_t *PIXELS,
		int SCREEN_W,
		int X,
		int Y,
		int W,
		int H
	)
{
	struct S_TILE_HDR	TH;

	SCRATCH->LEN = 0;
	if (!JPEG_ENCODE(
		JPEG,
		PIXELS + (size_t)Y * (size_t)SCREEN_W + X,
		SCREEN_W,
		W,
		H,
		SCRATCH
	))
	{
		return (0);
	}
	TH.X = (uint16_t)X;
	TH.Y = (uint16_t)Y;
	TH.W = (uint16_t)W;
	TH.H = (uint16_t)H;
	TH.LEN = (uint32_t)SCRATCH->LEN;
	TH.KIND = TILE_JPEG;
	TEXT_ADDN(FRAME, (const char *)&TH, sizeof(TH));
	TEXT_ADDN(FRAME, SCRATCH->DATA, SCRATCH->LEN);
	return (1);
}

/* THE CHANGED SPAN OF ONE TILE ROW, OR 0 WHEN NOTHING CHANGED IN IT */
static int
	ROW_SPAN(const struct S_GRID *G, int ROW, int *FROM, int *TO)
{
	int	X;

	*FROM = -1;
	*TO = -1;
	for (X = 0; X < G->COLS; ++X)
	{
		if (G->CHANGED[ROW * G->COLS + X])
		{
			if (*FROM < 0)
			{
				*FROM = X;
			}
			*TO = X;
		}
	}
	return (*FROM >= 0);
}

/* BIG CHANGE: ROWS WITH THE SAME SPAN ARE JOINED INTO ONE JPEG */
static uint32_t
	SEND_MOVING(
		struct S_TEXT *FRAME,
		struct S_JPEG *JPEG,
		struct S_TEXT *SCRATCH,
		struct S_GRID *G,
		const uint32_t *PIXELS,
		uint8_t *PACKED,
		int SCREEN_W,
		int SCREEN_H
	)
{
	uint32_t	TILES;
	int			ROW;
	int			END;
	int			FROM;
	int			TO;
	int			F2;
	int			T2;
	int			X;
	int			Y;
	int			W;
	int			H;
	int			TX;
	int			TY;

	TILES = 0;
	for (ROW = 0; ROW < G->ROWS; ROW = END)
	{
		END = ROW + 1;
		if (!ROW_SPAN(G, ROW, &FROM, &TO))
		{
			continue ;
		}
		while (END < G->ROWS && ROW_SPAN(G, END, &F2, &T2) &&
			F2 == FROM && T2 == TO)
		{
			++END;
		}
		X = FROM * INWAY_TILE;
		Y = ROW * INWAY_TILE;
		W = (TO + 1) * INWAY_TILE;
		W = (W > SCREEN_W ? SCREEN_W : W) - X;
		H = (END * INWAY_TILE > SCREEN_H ? SCREEN_H : END * INWAY_TILE) - Y;
		if (ADD_JPEG(FRAME, JPEG, SCRATCH, PIXELS, SCREEN_W, X, Y, W, H))
		{
			++TILES;
			for (TY = ROW; TY < END; ++TY)
			{
				for (TX = FROM; TX <= TO; ++TX)
				{
					G->LOSSY[TY * G->COLS + TX] = 1;
				}
			}
			continue ;
		}
		for (TY = ROW; TY < END; ++TY) /* NO JPEG: SEND THEM LOSSLESS */
		{
			for (TX = FROM; TX <= TO; ++TX)
			{
				if (G->CHANGED[TY * G->COLS + TX])
				{
					ADD_EXACT(
						FRAME,
						PIXELS,
						PACKED,
						SCREEN_W,
						SCREEN_H,
						TX,
						TY
					);
					G->LOSSY[TY * G->COLS + TX] = 0;
					++TILES;
				}
			}
		}
	}
	return (TILES);
}

/* SMALL CHANGE: LOSSLESS TILES, PLUS SHARPENING OF OLD LOSSY TILES */
static uint32_t
	SEND_STILL(
		struct S_TEXT *FRAME,
		struct S_GRID *G,
		const uint32_t *PIXELS,
		uint8_t *PACKED,
		int SCREEN_W,
		int SCREEN_H
	)
{
	uint32_t	TILES;
	int			BUDGET;
	int			T;

	TILES = 0;
	BUDGET = (G->COLS * G->ROWS) / 4 + 16;
	for (T = 0; T < G->COLS * G->ROWS; ++T)
	{
		if (G->CHANGED[T] || (G->LOSSY[T] && G->QUIET[T] >= 6 && BUDGET-- > 0))
		{
			ADD_EXACT(
				FRAME,
				PIXELS,
				PACKED,
				SCREEN_W,
				SCREEN_H,
				T % G->COLS,
				T / G->COLS
			);
			G->LOSSY[T] = 0;
			++TILES;
		}
	}
	return (TILES);
}

/* MARKS CHANGED TILES AND KEEPS THE LAST PICTURE IN PREV. RETURNS COUNT. */
static int
	FIND_CHANGES(
		struct S_GRID *G,
		const uint32_t *PIXELS,
		uint32_t *PREV,
		int SCREEN_W,
		int SCREEN_H,
		int ALL
	)
{
	size_t	OFF;
	int		COUNT;
	int		TX;
	int		TY;
	int		TW;
	int		TH;
	int		Y;
	int		T;
	int		CHANGED;

	COUNT = 0;
	for (TY = 0; TY < G->ROWS; ++TY)
	{
		for (TX = 0; TX < G->COLS; ++TX)
		{
			T = TY * G->COLS + TX;
			TW = (SCREEN_W - TX * INWAY_TILE < INWAY_TILE) ?
				SCREEN_W - TX * INWAY_TILE : INWAY_TILE;
			TH = (SCREEN_H - TY * INWAY_TILE < INWAY_TILE) ?
				SCREEN_H - TY * INWAY_TILE : INWAY_TILE;
			CHANGED = ALL;
			for (Y = 0; Y < TH && !CHANGED; ++Y)
			{
				OFF = (size_t)(TY * INWAY_TILE + Y) * (size_t)SCREEN_W +
					(size_t)(TX * INWAY_TILE);
				CHANGED = !!memcmp(PIXELS + OFF, PREV + OFF, (size_t)TW * 4);
			}
			G->CHANGED[T] = (uint8_t)CHANGED;
			if (!CHANGED)
			{
				if (G->QUIET[T] < 255)
				{
					++G->QUIET[T];
				}
				continue ;
			}
			G->QUIET[T] = 0;
			++COUNT;
			for (Y = 0; Y < TH; ++Y)
			{
				OFF = (size_t)(TY * INWAY_TILE + Y) * (size_t)SCREEN_W +
					(size_t)(TX * INWAY_TILE);
				memcpy(PREV + OFF, PIXELS + OFF, (size_t)TW * 4);
			}
		}
	}
	return (COUNT);
}

static void
	SERVE(SOCKET S, const char *WHO)
{
	int32_t				HELLO[3];
	struct S_CAPTURE	CAP;
	struct S_LINK		LINK;
	struct S_SESSION	SESSION;
	struct S_JPEG		JPEG;
	struct S_GRID		GRID;
	struct S_TEXT		FRAME = {0};
	struct S_TEXT		SCRATCH = {0};
	HDESK				DESK;
	HANDLE				INPUT;
	BOOL				NO_DELAY;
	int					BUFFER_SIZE;
	uint32_t			*PREV;
	uint8_t				*PACKED;
	uint32_t			TILES;
	DWORD				T0;
	DWORD				DT;
	DWORD				WAIT;
	int					W;
	int					H;
	int					COUNT;
	int					FIRST;
	int					HAVE_JPEG;

	W = GetSystemMetrics(SM_CXSCREEN);
	H = GetSystemMetrics(SM_CYSCREEN);
	HELLO[0] = INWAY_MAGIC;
	HELLO[1] = W;
	HELLO[2] = H;
	if (!SEND_ALL(S, HELLO, sizeof(HELLO)))
	{
		return ;
	}
	NO_DELAY = TRUE;
	setsockopt(
		S,
		IPPROTO_TCP,
		TCP_NODELAY,
		(char *)&NO_DELAY,
		sizeof(NO_DELAY)
	);
	BUFFER_SIZE = 4 << 20; /* ROOM FOR A FEW FULL FRAMES ON A FAST LAN */
	setsockopt(
		S,
		SOL_SOCKET,
		SO_SNDBUF,
		(char *)&BUFFER_SIZE,
		sizeof(BUFFER_SIZE)
	);
	memset(&GRID, 0, sizeof(GRID));
	GRID.COLS = (W + INWAY_TILE - 1) / INWAY_TILE;
	GRID.ROWS = (H + INWAY_TILE - 1) / INWAY_TILE;
	GRID.CHANGED = (uint8_t *)calloc((size_t)(GRID.COLS * GRID.ROWS), 1);
	GRID.LOSSY = (uint8_t *)calloc((size_t)(GRID.COLS * GRID.ROWS), 1);
	GRID.QUIET = (uint8_t *)calloc((size_t)(GRID.COLS * GRID.ROWS), 1);
	PREV = (uint32_t *)calloc((size_t)W * (size_t)H, sizeof(uint32_t));
	PACKED = (uint8_t *)malloc(INWAY_TILE * INWAY_TILE * 4 + 16);
	if (!PREV || !PACKED || !GRID.CHANGED || !GRID.LOSSY || !GRID.QUIET)
	{
		free(PREV);
		free(PACKED);
		free(GRID.CHANGED);
		free(GRID.LOSSY);
		free(GRID.QUIET);
		return ;
	}
	LINK_INIT(&LINK, S);
	SESSION.LINK = &LINK;
	SESSION.CLIP = CLIP_START(&LINK, WHO);
	HAVE_JPEG = JPEG_OPEN(&JPEG);
	DESK = NULL;
	FOLLOW_DESKTOP(&DESK);
	memset(&CAP, 0, sizeof(CAP));
	CAPTURE_OPEN(&CAP, W, H);
	FIRST = 1;
	InterlockedExchange(&G_RUNNING, 1);
	INPUT = CreateThread(NULL, 0, INPUT_THREAD, &SESSION, 0, NULL);
	while (G_RUNNING)
	{
		T0 = GetTickCount();
		if (FOLLOW_DESKTOP(&DESK))
		{
			CAPTURE_CLOSE(&CAP);
			CAPTURE_OPEN(&CAP, W, H);
		}
		if (!CAP.PIXELS)
		{
			Sleep(INWAY_IDLE_MS);
			continue ;
		}
		BitBlt(CAP.MEMORY, 0, 0, W, H, CAP.SCREEN, 0, 0, SRCCOPY);
		DRAW_CURSOR(CAP.MEMORY);
		GdiFlush();
		COUNT = FIND_CHANGES(&GRID, CAP.PIXELS, PREV, W, H, FIRST);
		FRAME.LEN = 0;
		TEXT_ADDN(&FRAME, "\0\0\0\0", 4); /* TILE COUNT, FILLED IN BELOW */
		if (HAVE_JPEG && COUNT * 100 >= GRID.COLS * GRID.ROWS * 8)
		{
			TILES = SEND_MOVING(
				&FRAME,
				&JPEG,
				&SCRATCH,
				&GRID,
				CAP.PIXELS,
				PACKED,
				W,
				H
			);
		}
		else
		{
			TILES = SEND_STILL(&FRAME, &GRID, CAP.PIXELS, PACKED, W, H);
		}
		if (TILES && FRAME.DATA)
		{
			memcpy(FRAME.DATA, &TILES, 4);
			if (!LINK_SEND(
				&LINK,
				MSG_FRAME,
				FRAME.DATA,
				(uint32_t)FRAME.LEN,
				NULL,
				0
			))
			{
				break ;
			}
		}
		FIRST = 0;
		/* UP TO 60 FRAMES A SECOND WHILE THINGS MOVE, FEWER WHEN STILL */
		WAIT = TILES ? INWAY_FRAME_MS : INWAY_IDLE_MS;
		DT = GetTickCount() - T0;
		if (DT < WAIT)
		{
			Sleep(WAIT - DT);
		}
	}
	InterlockedExchange(&G_RUNNING, 0);
	shutdown(S, SD_BOTH);
	if (INPUT)
	{
		WaitForSingleObject(INPUT, 3000);
		CloseHandle(INPUT);
	}
	CLIP_STOP(SESSION.CLIP);
	if (HAVE_JPEG)
	{
		JPEG_CLOSE(&JPEG);
	}
	CAPTURE_CLOSE(&CAP);
	LINK_FREE(&LINK);
	TEXT_FREE(&FRAME);
	TEXT_FREE(&SCRATCH);
	free(PREV);
	free(PACKED);
	free(GRID.CHANGED);
	free(GRID.LOSSY);
	free(GRID.QUIET);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                    SENDING THE SCREEN                    |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |               ADMIN RIGHTS FOR ONE SESSION               |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# WINDOWS DOES NOT LET A NORMAL PROGRAM CLICK OR TYPE INTO ADMIN WINDOWS, SO A
# NORMAL INWAY ASKS FOR ADMIN RIGHTS WHEN A SESSION STARTS:
# 1. IT OPENS A PIPE AND STARTS "INWAY.EXE -SERVE <PIPE>" AS ADMIN, HIDDEN.
# 2. YES: THE ADMIN COPY GETS A LOCAL PORT (127.0.0.1) AND A SECRET THROUGH
#    THE PIPE, CONNECTS THERE AND SENDS THE SECRET FIRST. THIS COPY THEN JUST
#    PASSES THE BYTES BETWEEN THE OTHER PC AND THE ADMIN COPY.
# 3. NO, OR NO ANSWER IN INWAY_ADMIN_SECONDS: THE PIPE IS CLOSED AND THIS COPY
#    SERVES THE SESSION ITSELF. AN ADMIN COPY THAT STARTS TOO LATE FINDS NO
#    PIPE AND ENDS.
# A SOCKET CAN NOT SIMPLY BE HANDED TO THE ADMIN COPY: A NORMAL PROGRAM MAY
# NOT GIVE HANDLES TO AN ADMIN ONE, AND A TAKEN ONE IS NOT RELIABLE.
*/
#define SECRET_BYTES 16

struct	S_HANDOVER
{
	uint32_t	PORT; /* WHERE THE ADMIN COPY CONNECTS, ON 127.0.0.1 */
	uint8_t		SECRET[SECRET_BYTES]; /* IT SENDS THIS FIRST */
	char		WHO[24];
};

struct	S_RUNAS
{
	volatile LONG	REFS; /* THE THREAD AND ADMIN_HANDOVER: LAST ONE FREES */
	WCHAR			PARAMETERS[96];
	HANDLE			PROCESS;
};

struct	S_PUMP
{
	SOCKET	FROM;
	SOCKET	TO;
};

static void
	RUNAS_RELEASE(struct S_RUNAS *R)
{
	if (InterlockedDecrement(&R->REFS) == 0)
	{
		if (R->PROCESS)
		{
			CloseHandle(R->PROCESS);
		}
		free(R);
	}
}

/* THE YES / NO QUESTION BLOCKS UNTIL IT IS ANSWERED: IT GETS ITS OWN THREAD */
static DWORD WINAPI
	RUNAS_THREAD(LPVOID PARAMETER)
{
	struct S_RUNAS		*R;
	SHELLEXECUTEINFOW	SEI;
	WCHAR				EXE[MAX_PATH];
	DWORD				RESULT;

	R = (struct S_RUNAS *)PARAMETER;
	EXE[0] = L'\0';
	GetModuleFileNameW(NULL, EXE, MAX_PATH);
	memset(&SEI, 0, sizeof(SEI));
	SEI.cbSize = sizeof(SEI);
	SEI.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC;
	SEI.lpVerb = L"runas";
	SEI.lpFile = EXE;
	SEI.lpParameters = R->PARAMETERS;
	SEI.nShow = SW_HIDE;
	RESULT = ShellExecuteExW(&SEI) ? 1 : 0;
	R->PROCESS = SEI.hProcess;
	RUNAS_RELEASE(R);
	return (RESULT); /* 0 = NO, OR IT COULD NOT START */
}

/* ONE READ OR WRITE ON THE PIPE, WITH A TIME LIMIT */
static int
	PIPE_IO(HANDLE PIPE, void *BUFFER, DWORD SIZE, int WRITE, DWORD MS)
{
	OVERLAPPED	OV;
	DWORD		DONE;
	BOOL		OK;

	memset(&OV, 0, sizeof(OV));
	OV.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	if (!OV.hEvent)
	{
		return (0);
	}
	if (WRITE)
	{
		OK = WriteFile(PIPE, BUFFER, SIZE, NULL, &OV);
	}
	else
	{
		OK = ReadFile(PIPE, BUFFER, SIZE, NULL, &OV);
	}
	if (!OK && GetLastError() == ERROR_IO_PENDING &&
		WaitForSingleObject(OV.hEvent, MS) != WAIT_OBJECT_0)
	{
		CancelIo(PIPE);
	}
	DONE = 0;
	OK = GetOverlappedResult(PIPE, &OV, &DONE, TRUE);
	CloseHandle(OV.hEvent);
	return (OK && DONE == SIZE);
}

/* A PORT ON 127.0.0.1 THAT ONLY THIS PC CAN REACH */
static SOCKET
	LOCAL_LISTENER(uint32_t *PORT)
{
	SOCKET				LS;
	struct sockaddr_in	A;
	int					SIZE;

	LS = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (LS == INVALID_SOCKET)
	{
		return (INVALID_SOCKET);
	}
	memset(&A, 0, sizeof(A));
	A.sin_family = AF_INET;
	A.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	SIZE = sizeof(A);
	if (bind(LS, (struct sockaddr *)&A, sizeof(A)) != 0 ||
		listen(LS, 1) != 0 ||
		getsockname(LS, (struct sockaddr *)&A, &SIZE) != 0)
	{
		closesocket(LS);
		return (INVALID_SOCKET);
	}
	*PORT = ntohs(A.sin_port);
	return (LS);
}

/* THE ADMIN COPY'S CONNECTION, ONCE IT SENT THE RIGHT SECRET */
static SOCKET
	ACCEPT_ADMIN(SOCKET LS, const uint8_t *SECRET)
{
	fd_set			READY;
	struct timeval	TV;
	SOCKET			L;
	DWORD			TIMEOUT;
	uint8_t			GOT[SECRET_BYTES];

	FD_ZERO(&READY);
	FD_SET(LS, &READY);
	TV.tv_sec = 10;
	TV.tv_usec = 0;
	if (select(0, &READY, NULL, NULL, &TV) != 1)
	{
		return (INVALID_SOCKET);
	}
	L = accept(LS, NULL, NULL);
	if (L == INVALID_SOCKET)
	{
		return (INVALID_SOCKET);
	}
	TIMEOUT = 5000;
	setsockopt(L, SOL_SOCKET, SO_RCVTIMEO, (char *)&TIMEOUT, sizeof(TIMEOUT));
	if (!RECV_ALL(L, GOT, SECRET_BYTES) || memcmp(GOT, SECRET, SECRET_BYTES))
	{
		closesocket(L);
		return (INVALID_SOCKET);
	}
	TIMEOUT = 0;
	setsockopt(L, SOL_SOCKET, SO_RCVTIMEO, (char *)&TIMEOUT, sizeof(TIMEOUT));
	return (L);
}

/*
# ASKS FOR ADMIN RIGHTS. YES: RETURNS THE ADMIN COPY'S PROCESS AND ITS LOCAL
# CONNECTION IN *LOCAL. NO: NULL, THEN SERVE IT WITH NORMAL RIGHTS.
*/
static HANDLE
	ADMIN_HANDOVER(const char *WHO, SOCKET *LOCAL)
{
	struct S_HANDOVER	H;
	struct S_RUNAS		*R;
	OVERLAPPED			OV;
	SOCKET				LS;
	HANDLE				PIPE;
	HANDLE				THREAD;
	HANDLE				WAIT[2];
	HANDLE				CHILD;
	WCHAR				NAME[48];
	WCHAR				PATH[64];
	DWORD				HOW;
	DWORD				DONE;
	DWORD				STARTED;
	uint32_t			REPLY[2];

	*LOCAL = INVALID_SOCKET;
	memset(&H, 0, sizeof(H));
	snprintf(H.WHO, sizeof(H.WHO), "%s", WHO);
	if (BCryptGenRandom(NULL, H.SECRET, SECRET_BYTES,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0)
	{
		return (NULL);
	}
	LS = LOCAL_LISTENER(&H.PORT);
	if (LS == INVALID_SOCKET)
	{
		return (NULL);
	}
	swprintf(
		NAME,
		48,
		L"INWAY-%lu-%lu",
		(unsigned long)GetCurrentProcessId(),
		(unsigned long)GetTickCount()
	);
	swprintf(PATH, 64, L"\\\\.\\pipe\\%ls", NAME);
	PIPE = CreateNamedPipeW(
		PATH,
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED |
		FILE_FLAG_FIRST_PIPE_INSTANCE,
		PIPE_TYPE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
		1,
		256,
		256,
		0,
		NULL
	);
	R = (struct S_RUNAS *)calloc(1, sizeof(*R));
	memset(&OV, 0, sizeof(OV));
	OV.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	THREAD = NULL;
	if (PIPE != INVALID_HANDLE_VALUE && R && OV.hEvent)
	{
		R->REFS = 2;
		swprintf(R->PARAMETERS, 96, L"-serve %ls", NAME);
		THREAD = CreateThread(NULL, 0, RUNAS_THREAD, R, 0, NULL);
	}
	if (!THREAD)
	{
		free(R);
		if (OV.hEvent)
		{
			CloseHandle(OV.hEvent);
		}
		if (PIPE != INVALID_HANDLE_VALUE)
		{
			CloseHandle(PIPE);
		}
		closesocket(LS);
		return (NULL);
	}
	if (!ConnectNamedPipe(PIPE, &OV) && GetLastError() == ERROR_PIPE_CONNECTED)
	{
		SetEvent(OV.hEvent);
	}
	WAIT[0] = OV.hEvent;
	WAIT[1] = THREAD;
	HOW = WaitForMultipleObjects(2, WAIT, FALSE, INWAY_ADMIN_SECONDS * 1000);
	STARTED = 0;
	if (HOW == WAIT_OBJECT_0 + 1 && GetExitCodeThread(THREAD, &STARTED) &&
		STARTED)
	{
		/* YES WAS CLICKED: GIVE THE ADMIN COPY A MOMENT TO CONNECT */
		HOW = WaitForSingleObject(OV.hEvent, 15000);
	}
	CHILD = NULL;
	if (HOW == WAIT_OBJECT_0)
	{
		REPLY[0] = 0;
		if (PIPE_IO(PIPE, &H, sizeof(H), 1, 5000) &&
			(*LOCAL = ACCEPT_ADMIN(LS, H.SECRET)) != INVALID_SOCKET &&
			PIPE_IO(PIPE, REPLY, sizeof(REPLY), 0, 10000) && REPLY[0] == 1)
		{
			CHILD = OpenProcess(SYNCHRONIZE, FALSE, REPLY[1]);
		}
	}
	else
	{
		CancelIo(PIPE); /* NOBODY CAME: STOP WAITING FOR A CONNECTION */
		GetOverlappedResult(PIPE, &OV, &DONE, TRUE);
	}
	if (!CHILD && *LOCAL != INVALID_SOCKET)
	{
		closesocket(*LOCAL);
		*LOCAL = INVALID_SOCKET;
	}
	closesocket(LS);
	CloseHandle(PIPE);
	CloseHandle(OV.hEvent);
	CloseHandle(THREAD);
	RUNAS_RELEASE(R);
	return (CHILD);
}

static DWORD WINAPI
	PUMP_THREAD(LPVOID PARAMETER)
{
	struct S_PUMP	*P;
	char			*BUFFER;
	int				GOT;

	P = (struct S_PUMP *)PARAMETER;
	BUFFER = (char *)malloc(1 << 16);
	while (BUFFER && (GOT = recv(P->FROM, BUFFER, 1 << 16, 0)) > 0)
	{
		if (!SEND_ALL(P->TO, BUFFER, GOT))
		{
			break ;
		}
	}
	free(BUFFER);
	shutdown(P->FROM, SD_BOTH); /* ONE SIDE ENDED: END BOTH */
	shutdown(P->TO, SD_BOTH);
	return (0);
}

/* PASSES BYTES BOTH WAYS UNTIL ONE SIDE HANGS UP */
static void
	RELAY(SOCKET S, SOCKET L)
{
	struct S_PUMP	UP;
	struct S_PUMP	DOWN;
	HANDLE			T;
	BOOL			NO_DELAY;
	int				BUFFER_SIZE;

	NO_DELAY = TRUE;
	BUFFER_SIZE = 4 << 20;
	setsockopt(S, IPPROTO_TCP, TCP_NODELAY, (char *)&NO_DELAY, sizeof(BOOL));
	setsockopt(L, IPPROTO_TCP, TCP_NODELAY, (char *)&NO_DELAY, sizeof(BOOL));
	setsockopt(
		S,
		SOL_SOCKET,
		SO_SNDBUF,
		(char *)&BUFFER_SIZE,
		sizeof(BUFFER_SIZE)
	);
	UP.FROM = S; /* MOUSE, KEYS AND CLIPBOARD TO THE ADMIN COPY */
	UP.TO = L;
	DOWN.FROM = L; /* THE SCREEN TO THE OTHER PC */
	DOWN.TO = S;
	T = CreateThread(NULL, 0, PUMP_THREAD, &UP, 0, NULL);
	PUMP_THREAD(&DOWN);
	if (T)
	{
		WaitForSingleObject(T, INFINITE);
		CloseHandle(T);
	}
}

/* "INWAY.EXE -SERVE <PIPE>": THE ADMIN COPY. 0 = IT SERVED A SESSION. */
int
	HOST_TAKE_OVER(const char *PIPE_NAME)
{
	struct S_HANDOVER	H;
	struct sockaddr_in	A;
	HANDLE				PIPE;
	SOCKET				L;
	WCHAR				PATH[64];
	DWORD				DONE;
	uint32_t			REPLY[2];
	size_t				I;
	char				CH;

	for (I = 0; PIPE_NAME[I]; ++I)
	{
		CH = PIPE_NAME[I];
		if (!((CH >= '0' && CH <= '9') || (CH >= 'A' && CH <= 'Z') ||
			CH == '-') || I >= 40)
		{
			return (1);
		}
	}
	swprintf(PATH, 64, L"\\\\.\\pipe\\%hs", PIPE_NAME);
	if (!I || !WaitNamedPipeW(PATH, 5000))
	{
		return (1);
	}
	PIPE = CreateFileW(
		PATH,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (PIPE == INVALID_HANDLE_VALUE)
	{
		return (1);
	}
	L = INVALID_SOCKET;
	if (ReadFile(PIPE, &H, sizeof(H), &DONE, NULL) && DONE == sizeof(H) &&
		H.PORT > 0 && H.PORT < 65536)
	{
		L = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		memset(&A, 0, sizeof(A));
		A.sin_family = AF_INET;
		A.sin_port = htons((u_short)H.PORT);
		A.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		if (L != INVALID_SOCKET &&
			(connect(L, (struct sockaddr *)&A, sizeof(A)) != 0 ||
			!SEND_ALL(L, H.SECRET, SECRET_BYTES)))
		{
			closesocket(L);
			L = INVALID_SOCKET;
		}
	}
	REPLY[0] = (L != INVALID_SOCKET);
	REPLY[1] = GetCurrentProcessId();
	WriteFile(PIPE, REPLY, sizeof(REPLY), &DONE, NULL);
	CloseHandle(PIPE);
	if (L == INVALID_SOCKET)
	{
		return (1);
	}
	H.WHO[sizeof(H.WHO) - 1] = '\0';
	SERVE(L, H.WHO);
	closesocket(L);
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |               ADMIN RIGHTS FOR ONE SESSION               |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      ONE CONNECTION                      |  \\//  */
/*   \/   #==========================================================#   \/   */
/* A SECOND HANDLE TO THE SAME PROCESS */
static HANDLE
	DUPLICATE(HANDLE H)
{
	HANDLE	COPY;

	COPY = NULL;
	DuplicateHandle(
		GetCurrentProcess(),
		H,
		GetCurrentProcess(),
		&COPY,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS
	);
	return (COPY);
}

struct	S_NOTICE
{
	HANDLE			PROCESS; /* "INWAY.EXE -NOTICE", AS THE PERSON SIGNED IN */
	SOCKET			S;
	volatile LONG	ENDED; /* THE SESSION IS OVER: DISCONNECT MEANS NOTHING */
};

/* THE FULL SERVER'S BANNER CLOSED: WAS "DISCONNECT" CLICKED? */
static DWORD WINAPI
	NOTICE_THREAD(LPVOID PARAMETER)
{
	struct S_NOTICE	*N;
	DWORD			CODE;

	N = (struct S_NOTICE *)PARAMETER;
	WaitForSingleObject(N->PROCESS, INFINITE);
	if (!N->ENDED && GetExitCodeProcess(N->PROCESS, &CODE) && CODE == 1)
	{
		InterlockedExchange(&G_RUNNING, 0);
		shutdown(N->S, SD_BOTH);
	}
	return (0);
}

static DWORD WINAPI
	SERVER_UPDATE_THREAD(LPVOID PARAMETER)
{
	(void)PARAMETER;
	Sleep(500); /* LET THE "UPDATING" ANSWER GO OUT FIRST */
	UPDATE_NOW();
	InterlockedExchange(&G__UPDATING_SINCE__, 0); /* IT DID NOT WORK OUT */
	return (0);
}

/*
# A SERVER LOOKS FOR A NEW INWAY JUST BEFORE IT LETS SOMEONE IN. IS ONE OUT,
# IT UPDATES FIRST AND ANSWERS "UPDATING": THEIR INWAY CONNECTS AGAIN BY
# ITSELF. FOR UP TO 2 MINUTES, EVERYONE ELSE GETS THE SAME ANSWER.
*/
static int
	SERVER_UPDATING(void)
{
	DWORD	SINCE;
	HANDLE	T;

	SINCE = (DWORD)G__UPDATING_SINCE__;
	if (SINCE && GetTickCount() - SINCE < 120000)
	{
		return (1);
	}
	if (!UPDATE_DUE())
	{
		return (0);
	}
	InterlockedExchange(&G__UPDATING_SINCE__, (LONG)(GetTickCount() | 1));
	T = CreateThread(NULL, 0, SERVER_UPDATE_THREAD, NULL, 0, NULL);
	if (T)
	{
		CloseHandle(T);
	}
	return (1);
}

/* {MAGIC, VALUE, VALUE} INSTEAD OF A SCREEN SIZE, THEN HANG UP */
static void
	REPLY(SOCKET S, int32_t VALUE)
{
	int32_t	MESSAGE[3];

	MESSAGE[0] = INWAY_MAGIC;
	MESSAGE[1] = VALUE;
	MESSAGE[2] = VALUE;
	SEND_ALL(S, MESSAGE, sizeof(MESSAGE));
	shutdown(S, SD_SEND);
	Sleep(200);
}

/* "  {Y}● 2.253 IS NOW CONTROLLING THIS PC.{0}" UNDER THE MAIN SCREEN */
static void
	TELL(const char *COLOUR, enum E_STR ID, const char *WHO)
{
	struct S_TEXT	T = {0};

	TEXT_ADDF(&T, "  %s%s ", COLOUR, SYM("●", "*"));
	TEXT_ADDF(&T, LANG(ID), WHO);
	TEXT_ADD(&T, "{0}");
	NOTIFY("%s", T.DATA);
	TEXT_FREE(&T);
}

static DWORD WINAPI
	CONNECTION_THREAD(LPVOID PARAMETER)
{
	SOCKET				S;
	struct sockaddr_in	PEER;
	int					SIZE;
	char				WHO[24];
	DWORD				TIMEOUT;
	int32_t				GOT;
	int					ANSWER;
	HANDLE				ADMIN;
	SOCKET				LOCAL;
	struct S_NOTICE		NOTICE;
	HANDLE				NOTICE_WATCH;

	S = (SOCKET)(uintptr_t)PARAMETER;
	memset(&PEER, 0, sizeof(PEER));
	SIZE = sizeof(PEER);
	getpeername(S, (struct sockaddr *)&PEER, &SIZE);
	IP_SHORT(ntohl(PEER.sin_addr.s_addr), WHO, sizeof(WHO));
	if (InterlockedCompareExchange(&G__BUSY__, 1, 0) != 0)
	{
		REPLY(S, REPLY_BUSY);
		closesocket(S);
		return (0);
	}
	/* FIRST THE VIEWER SENDS ITS CODE; 0 MEANS "PLEASE ASK THEM" */
	TIMEOUT = 10000;
	setsockopt(S, SOL_SOCKET, SO_RCVTIMEO, (char *)&TIMEOUT, sizeof(TIMEOUT));
	GOT = -1;
	if (!RECV_ALL(S, &GOT, 4))
	{
		closesocket(S);
		InterlockedExchange(&G__BUSY__, 0);
		return (0);
	}
	TIMEOUT = 0;
	setsockopt(S, SOL_SOCKET, SO_RCVTIMEO, (char *)&TIMEOUT, sizeof(TIMEOUT));
	if (GOT == 0 && (G_WORKER || G_BACKGROUND))
	{
		InterlockedExchange(&G__BUSY__, 0);
		REPLY(S, REPLY_NEED_CODE); /* A SERVER: ONLY ITS CODE WORKS */
		closesocket(S);
		return (0);
	}
	if (GOT == 0)
	{
		TELL("{c}", STR_N_ASKING, WHO);
		ANSWER = HTMLUI_ASK(S, WHO);
		if (ANSWER != 1)
		{
			InterlockedExchange(&G__BUSY__, 0);
			if (ANSWER == 0)
			{
				REPLY(S, REPLY_DECLINED);
				TELL("{r}", STR_N_REFUSED, WHO);
			}
			else
			{
				TELL("{d}", STR_N_GAVE_UP, WHO);
			}
			closesocket(S);
			return (0);
		}
	}
	else if (GOT != G_CODE)
	{
		TELL("{r}", STR_N_WRONG, WHO);
		/* SLOW DOWN GUESSING, A LOT AFTER 5 WRONG CODES IN A ROW */
		Sleep(++G__WRONG_IN_A_ROW__ >= 5 ? 30000 : 2000);
		InterlockedExchange(&G__BUSY__, 0);
		REPLY(S, REPLY_WRONG_CODE);
		closesocket(S);
		return (0);
	}
	G__WRONG_IN_A_ROW__ = 0;
	if ((G_WORKER || G_BACKGROUND) && SERVER_UPDATING())
	{
		InterlockedExchange(&G__BUSY__, 0);
		REPLY(S, REPLY_UPDATING); /* THEIR INWAY TRIES AGAIN BY ITSELF */
		closesocket(S);
		return (0);
	}
	ADMIN = NULL;
	LOCAL = INVALID_SOCKET;
	if (!G_WORKER && !G_BACKGROUND && !SERVICE_IS_ADMIN())
	{
		TELL("{c}", STR_N_ADMIN_ASK, WHO);
		ADMIN = ADMIN_HANDOVER(WHO, &LOCAL);
		if (!ADMIN)
		{
			TELL("{d}", STR_N_ADMIN_NO, WHO);
		}
	}
	EnterCriticalSection(&G_CONSOLE_LOCK);
	snprintf(G_HOST_STATUS, sizeof(G_HOST_STATUS), "%s", WHO);
	LeaveCriticalSection(&G_CONSOLE_LOCK);
	TELL("{y}", STR_N_CONTROLLING, WHO);
	/*
	# "2.253 IS CONTROLLING THIS PC" COMES FROM ITS OWN SMALL PROCESS: WHEN
	# THE WINDOW EVER HANGS, IT CAN STILL BE ENDED WITH THE SESSION.
	*/
	memset(&NOTICE, 0, sizeof(NOTICE));
	NOTICE_WATCH = NULL;
	NOTICE.PROCESS = SERVICE_NOTICE_START(WHO);
	if (NOTICE.PROCESS)
	{
		NOTICE.S = S;
		NOTICE_WATCH = CreateThread(NULL, 0, NOTICE_THREAD, &NOTICE, 0, NULL);
	}
	if (ADMIN)
	{
		/* THE ADMIN COPY SERVES; "DISCONNECT" STILL WORKS FROM HERE */
		InterlockedExchange(&G_RUNNING, 1);
		RELAY(S, LOCAL);
		closesocket(LOCAL);
		WaitForSingleObject(ADMIN, 5000);
		CloseHandle(ADMIN);
		InterlockedExchange(&G_RUNNING, 0);
	}
	else
	{
		SERVE(S, WHO);
	}
	if (NOTICE.PROCESS)
	{
		InterlockedExchange(&NOTICE.ENDED, 1);
		if (NOTICE_WATCH)
		{
			/* SERVICE_NOTICE_STOP() CLOSES THE HANDLE: KEEP OUR OWN COPY */
			SERVICE_NOTICE_STOP(DUPLICATE(NOTICE.PROCESS));
			WaitForSingleObject(NOTICE_WATCH, 5000);
			CloseHandle(NOTICE_WATCH);
		}
		else
		{
			SERVICE_NOTICE_STOP(DUPLICATE(NOTICE.PROCESS));
		}
		CloseHandle(NOTICE.PROCESS);
	}
	closesocket(S);
	EnterCriticalSection(&G_CONSOLE_LOCK);
	G_HOST_STATUS[0] = '\0';
	LeaveCriticalSection(&G_CONSOLE_LOCK);
	TELL("{g}", STR_N_STOPPED, WHO);
	InterlockedExchange(&G__BUSY__, 0);
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      ONE CONNECTION                      |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                        LISTENING                         |  \\//  */
/*   \/   #==========================================================#   \/   */
SOCKET
	OPEN_LISTENER(void)
{
	SOCKET				LS;
	struct sockaddr_in	A;

	LS = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (LS == INVALID_SOCKET)
	{
		return (INVALID_SOCKET);
	}
	memset(&A, 0, sizeof(A));
	A.sin_family = AF_INET;
	A.sin_port = htons(INWAY_PORT);
	A.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(LS, (struct sockaddr *)&A, sizeof(A)) != 0 || listen(LS, 4) != 0)
	{
		closesocket(LS);
		return (INVALID_SOCKET);
	}
	NAME_ANSWER_START(); /* "WHICH PC IS CALLED ...?" FROM OTHER INWAYS */
	return (LS);
}

DWORD WINAPI
	HOST_THREAD(LPVOID PARAMETER)
{
	SOCKET	S;
	HANDLE	T;

	(void)PARAMETER;
	for (;;)
	{
		S = accept(G_LISTEN, NULL, NULL);
		if (S == INVALID_SOCKET)
		{
			Sleep(200);
			continue ;
		}
		T = CreateThread(
			NULL,
			0,
			CONNECTION_THREAD,
			(LPVOID)(uintptr_t)S,
			0,
			NULL
		);
		if (T)
		{
			CloseHandle(T);
		}
		else
		{
			closesocket(S);
		}
	}
	return (0);
}

/* OPENS THE PORT AND STARTS LISTENING IN THE BACKGROUND */
void
	HOST_START(void)
{
	HANDLE	T;

	G_LISTEN = OPEN_LISTENER();
	if (G_LISTEN == INVALID_SOCKET)
	{
		return ;
	}
	T = CreateThread(NULL, 0, HOST_THREAD, NULL, 0, NULL);
	if (T)
	{
		CloseHandle(T);
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                        LISTENING                         |  //\\  */
/* '""""' #==========================================================# '""""' */
