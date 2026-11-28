/******************************************************************************\
# C - viewer                                     #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/28 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# THE REMOTE SCREEN WINDOW. ONE THREAD RECEIVES AND UNPACKS TILES, THE MAIN
# THREAD RUNS THE WINDOW AND SENDS MOUSE AND KEYS.
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
 * #define INWAY_ASK_SECONDS
 * #define INWAY_MAGIC
 * #define INWAY_RELAY
 * #define INWAY_TILE
 * #define MSG_CLIP_ABORT
 * #define MSG_CLIP_TEXT
 * #define MSG_FRAME
 * #define MSG_INPUT
 * #define REPLY_BUSY
 * #define REPLY_DECLINED
 * #define REPLY_NEED_CODE
 * #define REPLY_NO_PC
 * #define REPLY_RELAY_OK
 * #define REPLY_UPDATING
 * #define REPLY_WRONG_CODE
 * #define TILE_JPEG
 * #define WIN32_LEAN_AND_MEAN
 * enum E_RESULT;
 * struct S_INPUT_MSG;
 * struct S_TILE_HDR;
 */
#include <winsock2.h>
/******* """"""""""""
 * #define AF_INET
 * #define INVALID_SOCKET
 * #define IPPROTO_TCP
 * #define SD_BOTH
 * #define SO_RCVBUF
 * #define SO_RCVTIMEO
 * #define SOCK_STREAM
 * #define SOL_SOCKET
 * #define TCP_NODELAY
 * typedef	SOCKET;
 * int		closesocket(SOCKET);
 * int		recv(SOCKET, char *, int, int);
 * int		setsockopt(SOCKET, int, int, const char *, int);
 * int		shutdown(SOCKET, int);
 * SOCKET	socket(int, int, int);
 * int		WSAGetLastError(void);
 */
#include <windows.h>
/******* """""""""""
 * #define BI_RGB
 * #define BLACK_BRUSH
 * #define CALLBACK
 * #define COLORONCOLOR
 * #define CS_HREDRAW
 * #define CS_VREDRAW
 * #define CTRL_BREAK_EVENT
 * #define CTRL_C_EVENT
 * #define CW_USEDEFAULT
 * #define DIB_RGB_COLORS
 * #define FALSE
 * #define GET_WHEEL_DELTA_WPARAM(wParam)
 * #define HALFTONE
 * #define HC_ACTION
 * #define IDC_ARROW
 * #define InterlockedExchange
 * #define LLKHF_INJECTED
 * #define LoadCursor
 * #define LOWORD(l)
 * #define MAKEINTRESOURCEW(i)
 * #define MB_ICONINFORMATION
 * #define MB_OK
 * #define MF_STRING
 * #define SPI_GETWORKAREA
 * #define SRCCOPY
 * #define SW_HIDE
 * #define TRUE
 * #define VK_CONTROL
 * #define VK_END
 * #define VK_INSERT
 * #define VK_LWIN
 * #define VK_MENU
 * #define VK_RWIN
 * #define VK_SHIFT
 * #define WH_KEYBOARD_LL
 * #define WINAPI
 * #define WM_APP
 * #define WM_CLOSE
 * #define WM_COMMAND
 * #define WM_DESTROY
 * #define WM_ERASEBKGND
 * #define WM_KEYDOWN
 * #define WM_KEYUP
 * #define WM_LBUTTONDOWN
 * #define WM_LBUTTONUP
 * #define WM_MBUTTONDOWN
 * #define WM_MBUTTONUP
 * #define WM_MOUSEMOVE
 * #define WM_MOUSEWHEEL
 * #define WM_PAINT
 * #define WM_RBUTTONDOWN
 * #define WM_RBUTTONUP
 * #define WM_SYSCHAR
 * #define WM_SYSKEYDOWN
 * #define WM_SYSKEYUP
 * #define WM_TIMER
 * #define WS_OVERLAPPEDWINDOW
 * #define WS_VISIBLE
 * #define WSAETIMEDOUT
 * #define WSAEWOULDBLOCK
 * typedef	BITMAPINFO;
 * typedef	BITMAPINFOHEADER;
 * typedef	BOOL;
 * typedef	CRITICAL_SECTION;
 * typedef	DWORD;
 * typedef	HANDLE;
 * typedef	HBRUSH;
 * typedef	HDC;
 * typedef	HHOOK;
 * typedef	HMENU;
 * typedef	HWND;
 * typedef	KBDLLHOOKSTRUCT;
 * typedef	LONG;
 * typedef	LPARAM;
 * typedef	LPVOID;
 * typedef	LRESULT;
 * typedef	MSG;
 * typedef	PAINTSTRUCT;
 * typedef	RECT;
 * typedef	UINT;
 * typedef	WCHAR;
 * typedef	WNDCLASSW;
 * typedef	WPARAM;
 * BOOL		AdjustWindowRect(LPRECT, DWORD, BOOL);
 * BOOL		AppendMenuW(HMENU, UINT, UINT_PTR, LPCWSTR);
 * HDC		BeginPaint(HWND, LPPAINTSTRUCT);
 * LRESULT	CallNextHookEx(HHOOK, int, WPARAM, LPARAM);
 * BOOL		CloseHandle(HANDLE);
 * HMENU	CreateMenu(void);
 * HANDLE	CreateThread(
 * 	LPSECURITY_ATTRIBUTES,
 * 	SIZE_T,
 * 	LPTHREAD_START_ROUTINE,
 * 	LPVOID,
 * 	DWORD,
 * 	LPDWORD
 * );
 * HWND		CreateWindowExW(
 * 	DWORD,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	DWORD,
 * 	int,
 * 	int,
 * 	int,
 * 	int,
 * 	HWND,
 * 	HMENU,
 * 	HINSTANCE,
 * 	LPVOID
 * );
 * LRESULT	DefWindowProcW(HWND, UINT, WPARAM, LPARAM);
 * BOOL		DestroyWindow(HWND);
 * LRESULT	DispatchMessageW(const MSG *);
 * BOOL		EndPaint(HWND, const PAINTSTRUCT *);
 * void		EnterCriticalSection(LPCRITICAL_SECTION);
 * int		ExcludeClipRect(HDC, int, int, int, int);
 * int		FillRect(HDC, const RECT *, HBRUSH);
 * BOOL		GetClientRect(HWND, LPRECT);
 * HWND		GetConsoleWindow(void);
 * HWND		GetForegroundWindow(void);
 * SHORT	GetKeyState(int);
 * BOOL		GetMessageW(LPMSG, HWND, UINT, UINT);
 * HMODULE	GetModuleHandleW(LPCWSTR);
 * HGDIOBJ	GetStockObject(int);
 * DWORD	GetTickCount(void);
 * void		InitializeCriticalSection(LPCRITICAL_SECTION);
 * BOOL		InvalidateRect(HWND, const RECT *, BOOL);
 * void		LeaveCriticalSection(LPCRITICAL_SECTION);
 * HICON	LoadIconW(HINSTANCE, LPCWSTR);
 * LPSTR	lstrcpynA(LPSTR, LPCSTR, int);
 * int		MessageBoxW(HWND, LPCWSTR, LPCWSTR, UINT);
 * BOOL		PostMessageW(HWND, UINT, WPARAM, LPARAM);
 * void		PostQuitMessage(int);
 * ATOM		RegisterClassW(const WNDCLASSW *);
 * BOOL		ReleaseCapture(void);
 * BOOL		SetBrushOrgEx(HDC, int, int, LPPOINT);
 * HWND		SetCapture(HWND);
 * BOOL		SetConsoleCtrlHandler(PHANDLER_ROUTINE, BOOL);
 * BOOL		SetForegroundWindow(HWND);
 * int		SetStretchBltMode(HDC, int);
 * UINT_PTR	SetTimer(HWND, UINT_PTR, UINT, TIMERPROC);
 * HHOOK	SetWindowsHookExW(int, HOOKPROC, HINSTANCE, DWORD);
 * BOOL		ShowWindow(HWND, int);
 * void		Sleep(DWORD);
 * int		StretchDIBits(
 * 	HDC,
 * 	int,
 * 	int,
 * 	int,
 * 	int,
 * 	int,
 * 	int,
 * 	int,
 * 	int,
 * 	const void *,
 * 	const BITMAPINFO *,
 * 	UINT,
 * 	DWORD
 * );
 * BOOL		SystemParametersInfoW(UINT, UINT, PVOID, UINT);
 * BOOL		TranslateMessage(const MSG *);
 * BOOL		UnhookWindowsHookEx(HHOOK);
 * DWORD	WaitForSingleObject(HANDLE, DWORD);
 */
#include <windowsx.h>
/******* """"""""""""
 * #define GET_X_LPARAM(lp)
 * #define GET_Y_LPARAM(lp)
 */
#include <stdlib.h>
/******* """"""""""
 * void	*calloc(size_t, size_t);
 * void	free(void *);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * typedef	size_t;
 * void		*memcpy(void *, const void *, size_t);
 * void		*memset(void *, int, size_t);
 * size_t	strlen(const char *);
 */
#include <stdint.h>
/******* """"""""""
 * typedef	int32_t;
 * typedef	uint32_t;
 * typedef	uint8_t;
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT;
 * void		TEXT_ADDF(struct S_TEXT *, const char *, ...);
 * void		TEXT_FREE(struct S_TEXT *);
 * WCHAR	*UTF8_TO_WIDE(const char *);
 */
#include "lang.h"
/******* """"""""
 * enum E_STR;
 * const char	*LANG(enum E_STR);
 */
#include "console.h"
/******* """""""""""
 * void			SAY(const char *);
 * void			SAYF(const char *, ...);
 * const char	*SYM(const char *, const char *);
 */
#include "net.h"
/******* """""""
 * struct S_LINK;
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
 * int	NET_CONNECT(SOCKET, uint32_t);
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
 * int	DECODE_TILE(const uint8_t *, size_t, uint32_t *, int, int, int);
 * void	JPEG_CLOSE(struct S_JPEG *);
 * int	JPEG_DECODE(
 * 	struct S_JPEG *,
 * 	const uint8_t *,
 * 	size_t,
 * 	uint32_t *,
 * 	int,
 * 	int,
 * 	int
 * );
 * int	JPEG_OPEN(struct S_JPEG *);
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
 * void				CLIP_FLUSH(struct S_CLIP *);
 * struct S_CLIP	*CLIP_START(struct S_LINK *, const char *);
 * void				CLIP_STOP(struct S_CLIP *);
 */
#include "viewer.h"
/******* """"""""""
 * enum E_RESULT	VIEWER_CONNECT(uint32_t, int32_t, const char *);
 * void				VIEWER_INIT(void);
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define ID_SEND_CAD 100
#define VIEW_CLASS L"INWAY_VIEW"

/* **************************** [v] GLOBALS [v] ***************************** */
volatile LONG			G_QUIT = 0;
int						G_HIDE_CONSOLE = 0;
static struct S_LINK	G__LINK__;
static struct S_CLIP	*G__CLIP__ = NULL;
static int				G__W__ = 0;
static int				G__H__ = 0;
static uint32_t			*G__FB__ = NULL; /* THE OTHER SCREEN, PIXELS */
static BITMAPINFO		G__BI__;
static CRITICAL_SECTION	G__FB_LOCK__;
static HWND				G__HWND__ = NULL;
static HHOOK			G__HOOK__ = NULL;
static volatile LONG	G__LAST_MOVING__ = 0; /* WHEN THE LAST JPEG CAME */
static volatile LONG	G__NEED_SHARP__ = 0; /* REDRAW SMOOTH ONCE STILL */
/* **************************** [^] GLOBALS [^] ***************************** */

static void
	SEND_INPUT_MSG(int32_t TYPE, int32_t A, int32_t B, int32_t C)
{
	struct S_INPUT_MSG	M;

	M.TYPE = TYPE;
	M.A = A;
	M.B = B;
	M.C = C;
	LINK_SEND(&G__LINK__, MSG_INPUT, &M, sizeof(M), NULL, 0);
}

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                          WINDOW                          |  \\//  */
/*   \/   #==========================================================#   \/   */
/* WHERE THE OTHER SCREEN SITS INSIDE THE WINDOW, KEEPING ITS SHAPE */
static RECT
	DEST_RECT(void)
{
	RECT	RC;
	RECT	D;
	int		CW;
	int		CH;
	int		DW;
	int		DH;

	GetClientRect(G__HWND__, &RC);
	CW = RC.right > 1 ? RC.right : 1;
	CH = RC.bottom > 1 ? RC.bottom : 1;
	DW = CW;
	DH = (int)((long long)CW * G__H__ / G__W__);
	if (DH > CH)
	{
		DH = CH;
		DW = (int)((long long)CH * G__W__ / G__H__);
	}
	D.left = (CW - DW) / 2;
	D.top = (CH - DH) / 2;
	D.right = D.left + DW;
	D.bottom = D.top + DH;
	return (D);
}

static void
	TO_HOST(LPARAM LP, int *X, int *Y)
{
	RECT	D;
	int		DW;
	int		DH;

	D = DEST_RECT();
	DW = (D.right - D.left) > 1 ? D.right - D.left : 1;
	DH = (D.bottom - D.top) > 1 ? D.bottom - D.top : 1;
	*X = (int)((long long)(GET_X_LPARAM(LP) - D.left) * G__W__ / DW);
	*Y = (int)((long long)(GET_Y_LPARAM(LP) - D.top) * G__H__ / DH);
	*X = *X < 0 ? 0 : (*X >= G__W__ ? G__W__ - 1 : *X);
	*Y = *Y < 0 ? 0 : (*Y >= G__H__ ? G__H__ - 1 : *Y);
}

static void
	PAINT(HWND HWND_)
{
	PAINTSTRUCT	PS;
	HDC			DC;
	RECT		RC;
	RECT		D;
	int			DW;
	int			DH;

	DC = BeginPaint(HWND_, &PS);
	GetClientRect(HWND_, &RC);
	D = DEST_RECT();
	DW = D.right - D.left;
	DH = D.bottom - D.top;
	/*
	# A SMOOTH (HALFTONE) SHRINK COSTS TIME. WHILE THE OTHER SCREEN MOVES IT
	# IS DRAWN THE FAST WAY; A MOMENT LATER THE TIMER REDRAWS IT SMOOTH.
	*/
	SetStretchBltMode(
		DC,
		((DW == G__W__ && DH == G__H__) ||
		GetTickCount() - (DWORD)G__LAST_MOVING__ < 250) ?
		COLORONCOLOR : HALFTONE
	);
	SetBrushOrgEx(DC, 0, 0, NULL);
	EnterCriticalSection(&G__FB_LOCK__);
	StretchDIBits(
		DC,
		D.left,
		D.top,
		DW,
		DH,
		0,
		0,
		G__W__,
		G__H__,
		G__FB__,
		&G__BI__,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	LeaveCriticalSection(&G__FB_LOCK__);
	ExcludeClipRect(DC, D.left, D.top, D.right, D.bottom);
	FillRect(DC, &RC, (HBRUSH)GetStockObject(BLACK_BRUSH));
	EndPaint(HWND_, &PS);
}

static void
	MESSAGE_BOX(HWND OWNER, enum E_STR ID)
{
	WCHAR	*TEXT;

	TEXT = UTF8_TO_WIDE(LANG(ID));
	MessageBoxW(OWNER, TEXT, L"INWAY", MB_OK | MB_ICONINFORMATION);
	free(TEXT);
}

static LRESULT CALLBACK
	WINDOW_PROC(HWND HWND_, UINT MSG, WPARAM WP, LPARAM LP)
{
	int	X;
	int	Y;
	int	BUTTON;
	int	DOWN;

	switch (MSG)
	{
		case (WM_PAINT):
			PAINT(HWND_);
			return (0);
		case (WM_ERASEBKGND):
			return (1);
		case (WM_TIMER):
			if (G__NEED_SHARP__ &&
				GetTickCount() - (DWORD)G__LAST_MOVING__ >= 250)
			{
				InterlockedExchange(&G__NEED_SHARP__, 0);
				InvalidateRect(HWND_, NULL, FALSE);
			}
			return (0);
		case (WM_MOUSEMOVE):
			TO_HOST(LP, &X, &Y);
			SEND_INPUT_MSG(IN_MOVE, X, Y, 0);
			return (0);
		case (WM_LBUTTONDOWN):
		case (WM_LBUTTONUP):
		case (WM_RBUTTONDOWN):
		case (WM_RBUTTONUP):
		case (WM_MBUTTONDOWN):
		case (WM_MBUTTONUP):
			BUTTON = (MSG == WM_LBUTTONDOWN || MSG == WM_LBUTTONUP) ? 0 :
				(MSG == WM_RBUTTONDOWN || MSG == WM_RBUTTONUP) ? 1 : 2;
			DOWN = (MSG == WM_LBUTTONDOWN || MSG == WM_RBUTTONDOWN ||
				MSG == WM_MBUTTONDOWN);
			TO_HOST(LP, &X, &Y);
			SEND_INPUT_MSG(IN_MOVE, X, Y, 0);
			SEND_INPUT_MSG(IN_BUTTON, BUTTON, DOWN, 0);
			if (DOWN)
			{
				SetCapture(HWND_);
			}
			else
			{
				ReleaseCapture();
			}
			return (0);
		case (WM_MOUSEWHEEL):
			SEND_INPUT_MSG(IN_WHEEL, GET_WHEEL_DELTA_WPARAM(WP), 0, 0);
			return (0);
		case (WM_COMMAND):
			if (LOWORD(WP) == ID_SEND_CAD)
			{
				SEND_INPUT_MSG(IN_SAS, 0, 0, 0);
			}
			return (0);
		case (WM_SYSCHAR): /* NO BEEP, NO MENU ON ALT + LETTER */
			return (0);
		case (WM_KEYDOWN):
		case (WM_SYSKEYDOWN):
		case (WM_KEYUP):
		case (WM_SYSKEYUP):
			DOWN = (MSG == WM_KEYDOWN || MSG == WM_SYSKEYDOWN);
			/* CTRL + ALT + END = CTRL + ALT + DEL ON THE OTHER PC */
			if (WP == VK_END && GetKeyState(VK_CONTROL) < 0 &&
				GetKeyState(VK_MENU) < 0)
			{
				if (DOWN)
				{
					SEND_INPUT_MSG(IN_SAS, 0, 0, 0);
				}
				return (0);
			}
			/* CTRL + V, SHIFT + INSERT: A COPY MADE A MOMENT AGO GOES FIRST */
			if (DOWN && ((WP == 'V' && GetKeyState(VK_CONTROL) < 0) ||
				(WP == VK_INSERT && GetKeyState(VK_SHIFT) < 0)))
			{
				CLIP_FLUSH(G__CLIP__);
			}
			SEND_INPUT_MSG(IN_KEY, (int)WP, DOWN, (int)((LP >> 24) & 1));
			return (0);
		case (WM_APP): /* THE OTHER SIDE HUNG UP */
			if (!G_QUIT)
			{
				MESSAGE_BOX(HWND_, STR_V_REMOTE_CLOSED);
			}
			DestroyWindow(HWND_);
			return (0);
		case (WM_DESTROY):
			PostQuitMessage(0);
			return (0);
	}
	return (DefWindowProcW(HWND_, MSG, WP, LP));
}

/*
# THE WINDOWS KEY NEVER REACHES A NORMAL WINDOW: WINDOWS OPENS ITS OWN START
# MENU FIRST. WHILE THE REMOTE WINDOW IS IN FRONT, THIS HOOK TAKES BOTH
# WINDOWS KEYS, SENDS THEM TO THE OTHER PC AND HIDES THEM FROM THIS PC.
# WIN + R, WIN + E AND SO ON WORK THE SAME WAY: THE SECOND KEY ARRIVES AS A
# NORMAL KEY IN THE WINDOW. (WIN + L ALWAYS LOCKS THIS PC: WINDOWS RULE.)
*/
static LRESULT CALLBACK
	KEYBOARD_HOOK(int CODE, WPARAM WP, LPARAM LP)
{
	KBDLLHOOKSTRUCT	*K;
	int				DOWN;

	if (CODE == HC_ACTION && G__HWND__ && GetForegroundWindow() == G__HWND__)
	{
		K = (KBDLLHOOKSTRUCT *)LP;
		if ((K->vkCode == VK_LWIN || K->vkCode == VK_RWIN) &&
			!(K->flags & LLKHF_INJECTED))
		{
			DOWN = (WP == WM_KEYDOWN || WP == WM_SYSKEYDOWN);
			SEND_INPUT_MSG(IN_KEY, (int)K->vkCode, DOWN, 1);
			return (1);
		}
	}
	return (CallNextHookEx(G__HOOK__, CODE, WP, LP));
}

/* CTRL + C IN THE BLACK WINDOW DURING A SESSION: CLOSE IT AND QUIT */
static BOOL WINAPI
	CTRL_HANDLER(DWORD TYPE)
{
	if ((TYPE == CTRL_C_EVENT || TYPE == CTRL_BREAK_EVENT) && G__HWND__)
	{
		InterlockedExchange(&G_QUIT, 1);
		PostMessageW(G__HWND__, WM_CLOSE, 0, 0);
		return (TRUE);
	}
	return (FALSE);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                          WINDOW                          |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                   RECEIVING THE SCREEN                   |  \\//  */
/*   \/   #==========================================================#   \/   */
/* REDRAWS ONLY THE PART OF THE WINDOW THAT SHOWS THE CHANGED AREA */
static void
	INVALIDATE(int X0, int Y0, int X1, int Y1)
{
	RECT	D;
	RECT	R;
	int		DW;
	int		DH;

	D = DEST_RECT();
	DW = D.right - D.left;
	DH = D.bottom - D.top;
	R.left = D.left + (int)((long long)X0 * DW / G__W__) - 1;
	R.top = D.top + (int)((long long)Y0 * DH / G__H__) - 1;
	R.right = D.left + (int)(((long long)X1 * DW + G__W__ - 1) / G__W__) + 1;
	R.bottom = D.top + (int)(((long long)Y1 * DH + G__H__ - 1) / G__H__) + 1;
	InvalidateRect(G__HWND__, &R, FALSE);
}

/* UNPACKS ONE MSG_FRAME. 0 WHEN THE DATA IS BROKEN. */
static int
	SHOW_FRAME(struct S_JPEG *JPEG, const uint8_t *DATA, uint32_t SIZE)
{
	struct S_TILE_HDR	TH;
	uint32_t			COUNT;
	uint32_t			T;
	size_t				POS;
	int					OK;
	int					MOVING;
	int					X0;
	int					Y0;
	int					X1;
	int					Y1;

	if (SIZE < 4)
	{
		return (0);
	}
	memcpy(&COUNT, DATA, 4);
	POS = 4;
	OK = 1;
	MOVING = 0;
	X0 = G__W__;
	Y0 = G__H__;
	X1 = 0;
	Y1 = 0;
	EnterCriticalSection(&G__FB_LOCK__);
	for (T = 0; T < COUNT && OK; ++T)
	{
		if (POS + sizeof(TH) > SIZE)
		{
			OK = 0;
			break ;
		}
		memcpy(&TH, DATA + POS, sizeof(TH));
		POS += sizeof(TH);
		if (!TH.W || !TH.H || TH.X + TH.W > G__W__ || TH.Y + TH.H > G__H__ ||
			POS + TH.LEN > SIZE)
		{
			OK = 0;
			break ;
		}
		if (TH.KIND == TILE_JPEG)
		{
			/* A BROKEN JPEG ONLY LEAVES AN OLD PATCH: THE SHARP COPY FOLLOWS */
			JPEG_DECODE(
				JPEG,
				DATA + POS,
				TH.LEN,
				G__FB__ + (size_t)TH.Y * (size_t)G__W__ + TH.X,
				G__W__,
				TH.W,
				TH.H
			);
			MOVING = 1;
		}
		else if (TH.W > INWAY_TILE || TH.H > INWAY_TILE)
		{
			OK = 0;
		}
		else
		{
			OK = DECODE_TILE(
				DATA + POS,
				TH.LEN,
				G__FB__ + (size_t)TH.Y * (size_t)G__W__ + TH.X,
				G__W__,
				TH.W,
				TH.H
			);
		}
		POS += TH.LEN;
		X0 = TH.X < X0 ? TH.X : X0;
		Y0 = TH.Y < Y0 ? TH.Y : Y0;
		X1 = TH.X + TH.W > X1 ? TH.X + TH.W : X1;
		Y1 = TH.Y + TH.H > Y1 ? TH.Y + TH.H : Y1;
	}
	LeaveCriticalSection(&G__FB_LOCK__);
	if (MOVING)
	{
		InterlockedExchange(&G__LAST_MOVING__, (LONG)GetTickCount());
		InterlockedExchange(&G__NEED_SHARP__, 1);
	}
	if (OK && X1 > X0 && Y1 > Y0)
	{
		INVALIDATE(X0, Y0, X1, Y1);
	}
	return (OK);
}

static DWORD WINAPI
	RECV_THREAD(LPVOID PARAMETER)
{
	struct S_JPEG	JPEG;
	uint8_t			*BUFFER;
	uint32_t		CAP;
	uint32_t		SIZE;
	uint32_t		TYPE;
	uint32_t		MAX;
	int				HAVE_JPEG;

	(void)PARAMETER;
	HAVE_JPEG = JPEG_OPEN(&JPEG);
	MAX = (uint32_t)((size_t)G__W__ * (size_t)G__H__ * 5 + (1 << 20));
	if (MAX < CLIP_MAX_TEXT + CLIP_CHUNK + 64)
	{
		MAX = CLIP_MAX_TEXT + CLIP_CHUNK + 64;
	}
	BUFFER = NULL;
	CAP = 0;
	while (RECV_MSG(G__LINK__.S, &TYPE, &BUFFER, &SIZE, &CAP, MAX))
	{
		if (TYPE == MSG_FRAME)
		{
			if (!SHOW_FRAME(&JPEG, BUFFER, SIZE))
			{
				break ;
			}
		}
		else if (TYPE >= MSG_CLIP_TEXT && TYPE <= MSG_CLIP_ABORT)
		{
			CLIP_RECEIVE(G__CLIP__, TYPE, BUFFER, SIZE);
		}
	}
	free(BUFFER);
	if (HAVE_JPEG)
	{
		JPEG_CLOSE(&JPEG);
	}
	PostMessageW(G__HWND__, WM_APP, 0, 0);
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                   RECEIVING THE SCREEN                   |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                        CONNECTING                        |  \\//  */
/*   \/   #==========================================================#   \/   */
void
	VIEWER_INIT(void)
{
	WNDCLASSW	WC;

	InitializeCriticalSection(&G__FB_LOCK__);
	memset(&WC, 0, sizeof(WC));
	WC.style = CS_HREDRAW | CS_VREDRAW;
	WC.lpfnWndProc = WINDOW_PROC;
	WC.hInstance = GetModuleHandleW(NULL);
	WC.hCursor = LoadCursor(NULL, IDC_ARROW);
	WC.hIcon = LoadIconW(WC.hInstance, MAKEINTRESOURCEW(1));
	WC.lpszClassName = VIEW_CLASS;
	RegisterClassW(&WC);
	SetConsoleCtrlHandler(CTRL_HANDLER, TRUE);
}

/* THE {MAGIC, W, H} ANSWER, OR WHY THERE IS NONE */
static enum E_RESULT
	READ_HELLO(SOCKET S, int32_t CODE, int32_t *HELLO)
{
	DWORD	TIMEOUT;
	int		GOT;
	int		N;

	/* THE OTHER PC MAY FIRST ASK FOR ACCEPT, THEN FOR ADMIN RIGHTS */
	TIMEOUT = CODE ? (INWAY_ADMIN_SECONDS + 30) * 1000 :
		(INWAY_ASK_SECONDS + INWAY_ADMIN_SECONDS + 30) * 1000;
	setsockopt(S, SOL_SOCKET, SO_RCVTIMEO, (char *)&TIMEOUT, sizeof(TIMEOUT));
	GOT = 0;
	N = 0;
	while (GOT < 12)
	{
		N = recv(S, (char *)HELLO + GOT, 12 - GOT, 0);
		if (N <= 0)
		{
			break ;
		}
		GOT += N;
	}
	if (GOT < 12)
	{
		return ((N < 0 && WSAGetLastError() == WSAETIMEDOUT) ?
			CR_NO_ANSWER : CR_DROPPED);
	}
	if (HELLO[0] != INWAY_MAGIC)
	{
		return (CR_VERSION);
	}
	switch (HELLO[1])
	{
		case (REPLY_DECLINED): return (CR_DECLINED);
		case (REPLY_BUSY): return (CR_BUSY);
		case (REPLY_WRONG_CODE): return (CR_WRONG_CODE);
		case (REPLY_NEED_CODE): return (CR_NEED_CODE);
		case (REPLY_UPDATING): return (CR_UPDATING);
		case (REPLY_NO_PC): return (CR_NO_PC);
	}
	if (HELLO[1] == REPLY_RELAY_OK)
	{
		return (CR_ENDED); /* A PC IN BETWEEN: GO_THROUGH() LOOKS AT IT */
	}
	if (HELLO[1] < 0 || HELLO[2] <= 0 || HELLO[1] > 16384 || HELLO[2] > 16384)
	{
		return (CR_ERROR);
	}
	TIMEOUT = 0;
	setsockopt(S, SOL_SOCKET, SO_RCVTIMEO, (char *)&TIMEOUT, sizeof(TIMEOUT));
	return (CR_ENDED);
}

static HWND
	OPEN_WINDOW(const char *LABEL)
{
	struct S_TEXT	TITLE = {0};
	WCHAR			*WIDE_TITLE;
	WCHAR			*WIDE_MENU;
	HMENU			MENU;
	HWND			WINDOW;
	RECT			WA;
	RECT			R;
	double			SCALE;
	double			SX;
	double			SY;

	SystemParametersInfoW(SPI_GETWORKAREA, 0, &WA, 0);
	SX = 0.9 * (WA.right - WA.left) / G__W__;
	SY = 0.9 * (WA.bottom - WA.top) / G__H__;
	SCALE = SX < SY ? SX : SY;
	SCALE = SCALE > 1.0 ? 1.0 : SCALE;
	R.left = 0;
	R.top = 0;
	R.right = (LONG)(G__W__ * SCALE);
	R.bottom = (LONG)(G__H__ * SCALE);
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, TRUE); /* TRUE: HAS A MENU */
	TEXT_ADDF(&TITLE, LANG(STR_V_TITLE), LABEL);
	WIDE_TITLE = UTF8_TO_WIDE(TITLE.DATA);
	WIDE_MENU = UTF8_TO_WIDE(LANG(STR_V_MENU_CAD));
	MENU = CreateMenu();
	AppendMenuW(MENU, MF_STRING, ID_SEND_CAD, WIDE_MENU);
	WINDOW = CreateWindowExW(
		0,
		VIEW_CLASS,
		WIDE_TITLE,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		R.right - R.left,
		R.bottom - R.top,
		NULL,
		MENU,
		GetModuleHandleW(NULL),
		NULL
	);
	if (WINDOW)
	{
		SetTimer(WINDOW, 1, 100, NULL); /* SMOOTH REDRAW WHEN STILL */
	}
	free(WIDE_TITLE);
	free(WIDE_MENU);
	TEXT_FREE(&TITLE);
	return (WINDOW);
}

/*
# -C A B: EVERY PC BEFORE THE LAST ONE IS ASKED TO PASS THE CONNECTION ON TO
# THE NEXT ONE (SEE INWAY.H). CR_ENDED: THE SOCKET NOW GOES TO THE LAST PC.
*/
static enum E_RESULT
	GO_THROUGH(SOCKET S, const struct S_HOP *PCS, int COUNT, int *FAILED)
{
	int32_t			REQUEST[3];
	int32_t			HELLO[3];
	enum E_RESULT	RESULT;
	int				I;

	for (I = 0; I + 1 < COUNT; ++I)
	{
		*FAILED = I;
		if (!PCS[I].CODE)
		{
			SAY("  {c}");
			SAYF(LANG(STR_WAITING), PCS[I].LABEL);
			SAY("{0}\n");
		}
		REQUEST[0] = INWAY_RELAY;
		REQUEST[1] = PCS[I].CODE;
		REQUEST[2] = (int32_t)strlen(PCS[I + 1].ADDRESS);
		if (!SEND_ALL(S, REQUEST, sizeof(REQUEST)) ||
			!SEND_ALL(S, PCS[I + 1].ADDRESS, REQUEST[2]))
		{
			return (CR_DROPPED);
		}
		RESULT = READ_HELLO(S, PCS[I].CODE, HELLO);
		if (RESULT == CR_NO_PC)
		{
			*FAILED = I + 1; /* THIS PC COULD NOT FIND OR REACH THE NEXT ONE */
		}
		if (RESULT != CR_ENDED)
		{
			return (RESULT);
		}
		if (HELLO[1] != REPLY_RELAY_OK)
		{
			return (CR_VERSION);
		}
	}
	*FAILED = COUNT - 1;
	if (COUNT > 1 && !PCS[COUNT - 1].CODE)
	{
		SAY("  {c}");
		SAYF(LANG(STR_WAITING), PCS[COUNT - 1].LABEL);
		SAY("{0}\n");
	}
	return (CR_ENDED);
}

enum E_RESULT
	VIEWER_CONNECT(uint32_t IP, int32_t CODE, const char *LABEL)
{
	struct S_HOP	PC;
	int				FAILED;

	memset(&PC, 0, sizeof(PC));
	lstrcpynA(PC.LABEL, LABEL, sizeof(PC.LABEL));
	PC.CODE = CODE;
	return (VIEWER_CONNECT_VIA(IP, &PC, 1, &FAILED));
}

enum E_RESULT
	VIEWER_CONNECT_VIA(
		uint32_t IP,
		const struct S_HOP *PCS,
		int COUNT,
		int *FAILED
	)
{
	SOCKET			S;
	int32_t			HELLO[3];
	int32_t			CODE;
	enum E_RESULT	RESULT;
	BOOL			NO_DELAY;
	int				BUFFER_SIZE;
	HANDLE			T;
	MSG				MSG_;
	int				UPDATING;
	int				TRY;
	int				AGAIN;
	int				I;
	const char		*LABEL;

	/*
	# A SERVER MAY ANSWER "UPDATING" AND RESTART WITH THE NEW INWAY: THEN TRY
	# AGAIN EVERY 3 SECONDS, FOR UP TO 90 SECONDS. THROUGH OTHER PCS ONLY WHEN
	# THEY ALL GOT THEIR CODE: NOBODY THERE IS ASKED FOR ACCEPT AGAIN AND AGAIN.
	*/
	CODE = PCS[COUNT - 1].CODE;
	LABEL = PCS[COUNT - 1].LABEL;
	AGAIN = 1;
	for (I = 0; I + 1 < COUNT; ++I)
	{
		AGAIN = AGAIN && PCS[I].CODE;
	}
	UPDATING = 0;
	*FAILED = 0;
	for (TRY = 0; ; ++TRY)
	{
		if (UPDATING)
		{
			Sleep(3000);
		}
		S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (S == INVALID_SOCKET)
		{
			return (CR_ERROR);
		}
		*FAILED = 0;
		if (!NET_CONNECT(S, IP))
		{
			closesocket(S);
			if (UPDATING && TRY < 30)
			{
				continue ; /* IT IS STILL RESTARTING */
			}
			return (UPDATING ? CR_UPDATING : CR_NO_PC);
		}
		RESULT = GO_THROUGH(S, PCS, COUNT, FAILED);
		if (RESULT == CR_ENDED)
		{
			RESULT = SEND_ALL(S, &CODE, 4) ? READ_HELLO(S, CODE, HELLO) :
				CR_DROPPED;
		}
		if ((RESULT == CR_UPDATING || (UPDATING && RESULT == CR_DROPPED)) &&
			TRY < 30 && AGAIN)
		{
			closesocket(S);
			if (!UPDATING)
			{
				SAY("  {c}");
				SAYF(LANG(STR_UPDATING_WAIT), PCS[*FAILED].LABEL);
				SAY("{0}\n");
			}
			UPDATING = 1;
			continue ;
		}
		break ;
	}
	if (RESULT != CR_ENDED)
	{
		closesocket(S);
		return (RESULT);
	}
	G__W__ = HELLO[1];
	G__H__ = HELLO[2];
	NO_DELAY = TRUE;
	setsockopt(
		S,
		IPPROTO_TCP,
		TCP_NODELAY,
		(char *)&NO_DELAY,
		sizeof(NO_DELAY)
	);
	BUFFER_SIZE = 4 << 20;
	setsockopt(
		S,
		SOL_SOCKET,
		SO_RCVBUF,
		(char *)&BUFFER_SIZE,
		sizeof(BUFFER_SIZE)
	);
	free(G__FB__);
	G__FB__ = (uint32_t *)calloc((size_t)G__W__ * (size_t)G__H__, 4);
	if (!G__FB__)
	{
		closesocket(S);
		return (CR_ERROR);
	}
	memset(&G__BI__, 0, sizeof(G__BI__));
	G__BI__.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	G__BI__.bmiHeader.biWidth = G__W__;
	G__BI__.bmiHeader.biHeight = -G__H__;
	G__BI__.bmiHeader.biPlanes = 1;
	G__BI__.bmiHeader.biBitCount = 32;
	G__BI__.bmiHeader.biCompression = BI_RGB;
	LINK_INIT(&G__LINK__, S);
	G__CLIP__ = CLIP_START(&G__LINK__, LABEL);
	G__HWND__ = OPEN_WINDOW(LABEL);
	SetForegroundWindow(G__HWND__);
	G__HOOK__ = SetWindowsHookExW(
		WH_KEYBOARD_LL,
		KEYBOARD_HOOK,
		GetModuleHandleW(NULL),
		0
	);
	SAYF("  {g}%s %s{0}\n", SYM("●", "*"), LANG(STR_CONNECTED));
	SAYF("  {d}%s{0}\n", LANG(STR_CONNECTED_HINT));
	if (G_HIDE_CONSOLE && GetConsoleWindow())
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE); /* ONLY THE SCREEN STAYS */
	}
	T = CreateThread(NULL, 0, RECV_THREAD, NULL, 0, NULL);
	while (GetMessageW(&MSG_, NULL, 0, 0) > 0)
	{
		TranslateMessage(&MSG_);
		DispatchMessageW(&MSG_);
	}
	if (G__HOOK__)
	{
		UnhookWindowsHookEx(G__HOOK__);
		G__HOOK__ = NULL;
	}
	shutdown(S, SD_BOTH);
	closesocket(S);
	if (T)
	{
		WaitForSingleObject(T, 3000);
		CloseHandle(T);
	}
	CLIP_STOP(G__CLIP__);
	G__CLIP__ = NULL;
	LINK_FREE(&G__LINK__);
	G__HWND__ = NULL;
	return (CR_ENDED);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                        CONNECTING                        |  //\\  */
/* '""""' #==========================================================# '""""' */
