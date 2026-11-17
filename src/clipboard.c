/******************************************************************************\
# C - clipboard                                  #       Maximum Tension       #
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
# CLIPBOARD SYNC. A HIDDEN WINDOW IN ITS OWN THREAD WATCHES THE CLIPBOARD AND
# ALSO PUTS THINGS ON IT (THE CLIPBOARD WANTS A WINDOW). FILES ARE SENT BY ONE
# MORE THREAD, SO A BIG COPY NEVER STOPS THE SCREEN OR THE MOUSE.
# WHEN THIS SIDE PUTS SOMETHING ON THE CLIPBOARD, IT REMEMBERS THE CLIPBOARD'S
# SEQUENCE NUMBER, SO THAT CHANGE IS NOT SENT BACK AGAIN.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define COBJMACROS
 * #define MSG_CLIP_ABORT
 * #define MSG_CLIP_BEGIN
 * #define MSG_CLIP_DATA
 * #define MSG_CLIP_END
 * #define MSG_CLIP_FILE
 * #define MSG_CLIP_TEXT
 * #define WIN32_LEAN_AND_MEAN
 * struct S_CLIP_FILE;
 */
#include <windows.h>
/******* """""""""""
 * #define CALLBACK
 * #define CF_HDROP
 * #define CF_UNICODETEXT
 * #define CREATE_ALWAYS
 * #define FALSE
 * #define FILE_ATTRIBUTE_DIRECTORY
 * #define FILE_ATTRIBUTE_NORMAL
 * #define FILE_FLAG_SEQUENTIAL_SCAN
 * #define FILE_SHARE_DELETE
 * #define FILE_SHARE_READ
 * #define FILE_SHARE_WRITE
 * #define GENERIC_READ
 * #define GENERIC_WRITE
 * #define GHND
 * #define GMEM_MOVEABLE
 * #define GWLP_USERDATA
 * #define HWND_MESSAGE
 * #define InterlockedCompareExchange
 * #define InterlockedExchange
 * #define InterlockedIncrement
 * #define INVALID_FILE_ATTRIBUTES
 * #define INVALID_HANDLE_VALUE
 * #define MAX_PATH
 * #define OPEN_EXISTING
 * #define S_OK
 * #define SMTO_ABORTIFHUNG
 * #define TRUE
 * #define WINAPI
 * #define WM_CLIPBOARDUPDATE
 * #define WM_CLOSE
 * #define WM_DESTROY
 * #define WM_NULL
 * #define WM_TIMER
 * typedef	DWORD;
 * typedef	FILETIME;
 * typedef	HANDLE;
 * typedef	HGLOBAL;
 * typedef	HWND;
 * typedef	LARGE_INTEGER;
 * typedef	LONG;
 * typedef	LONG_PTR;
 * typedef	LPARAM;
 * typedef	LPVOID;
 * typedef	LRESULT;
 * typedef	MSG;
 * typedef	UINT;
 * typedef	ULARGE_INTEGER;
 * typedef	WCHAR;
 * typedef	WIN32_FIND_DATAW;
 * typedef	WNDCLASSW;
 * typedef	WPARAM;
 * BOOL		AddClipboardFormatListener(HWND);
 * BOOL		CloseClipboard(void);
 * BOOL		CloseHandle(HANDLE);
 * BOOL		CreateDirectoryW(LPCWSTR, LPSECURITY_ATTRIBUTES);
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
 * BOOL		DeleteFileW(LPCWSTR);
 * BOOL		DestroyWindow(HWND);
 * LRESULT	DispatchMessageW(const MSG *);
 * BOOL		EmptyClipboard(void);
 * BOOL		FindClose(HANDLE);
 * HANDLE	FindFirstFileW(LPCWSTR, LPWIN32_FIND_DATAW);
 * BOOL		FindNextFileW(HANDLE, LPWIN32_FIND_DATAW);
 * HANDLE	GetClipboardData(UINT);
 * HWND		GetClipboardOwner(void);
 * DWORD	GetClipboardSequenceNumber(void);
 * DWORD	GetCurrentProcessId(void);
 * DWORD	GetEnvironmentVariableW(LPCWSTR, LPWSTR, DWORD);
 * DWORD	GetFileAttributesW(LPCWSTR);
 * BOOL		GetFileSizeEx(HANDLE, PLARGE_INTEGER);
 * BOOL		GetMessageW(LPMSG, HWND, UINT, UINT);
 * HMODULE	GetModuleHandleW(LPCWSTR);
 * void		GetSystemTimeAsFileTime(LPFILETIME);
 * DWORD	GetTempPathW(DWORD, LPWSTR);
 * LONG_PTR	GetWindowLongPtrW(HWND, int);
 * HGLOBAL	GlobalAlloc(UINT, SIZE_T);
 * HGLOBAL	GlobalFree(HGLOBAL);
 * LPVOID	GlobalLock(HGLOBAL);
 * BOOL		GlobalUnlock(HGLOBAL);
 * BOOL		IsClipboardFormatAvailable(UINT);
 * BOOL		KillTimer(HWND, UINT_PTR);
 * int		lstrcmpW(LPCWSTR, LPCWSTR);
 * int		lstrlenW(LPCWSTR);
 * BOOL		OpenClipboard(HWND);
 * BOOL		PostMessageW(HWND, UINT, WPARAM, LPARAM);
 * void		PostQuitMessage(int);
 * BOOL		ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
 * ATOM		RegisterClassW(const WNDCLASSW *);
 * UINT		RegisterClipboardFormatW(LPCWSTR);
 * BOOL		RemoveClipboardFormatListener(HWND);
 * BOOL		RemoveDirectoryW(LPCWSTR);
 * LRESULT	SendMessageTimeoutW(
 * 	HWND,
 * 	UINT,
 * 	WPARAM,
 * 	LPARAM,
 * 	UINT,
 * 	UINT,
 * 	PDWORD_PTR
 * );
 * HANDLE	SetClipboardData(UINT, HANDLE);
 * BOOL		SetEvent(HANDLE);
 * BOOL		SetFileAttributesW(LPCWSTR, DWORD);
 * UINT_PTR	SetTimer(HWND, UINT_PTR, UINT, TIMERPROC);
 * LONG_PTR	SetWindowLongPtrW(HWND, int, LONG_PTR);
 * void		Sleep(DWORD);
 * DWORD	WaitForSingleObject(HANDLE, DWORD);
 * BOOL		WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
 */
#include <shellapi.h>
/******* """"""""""""
 * typedef	HDROP;
 * UINT		DragQueryFileW(HDROP, UINT, LPWSTR, UINT);
 */
#include <shlobj.h>
/******* """"""""""
 * typedef	DROPFILES;
 */
#include <ole2.h>
/******* """"""""
 * #define IDataObject_GetData(This, pformatetcIn, pmedium)
 * #define IDataObject_Release(This)
 * enum tagDVASPECT;
 * enum tagTYMED;
 * typedef	FORMATETC;
 * typedef	IDataObject;
 * typedef	STGMEDIUM;
 * HRESULT	OleGetClipboard(LPDATAOBJECT *);
 * HRESULT	OleInitialize(LPVOID);
 * void		OleUninitialize(void);
 * void		ReleaseStgMedium(LPSTGMEDIUM);
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
 * int		_wcsnicmp(const wchar_t *, const wchar_t *, size_t);
 * void		*memcpy(void *, const void *, size_t);
 * void		*memset(void *, int, size_t);
 * wchar_t	*wcschr(const wchar_t *, wchar_t);
 * wchar_t	*wcsrchr(const wchar_t *, wchar_t);
 */
#include <stdint.h>
/******* """"""""""
 * typedef	uint32_t;
 * typedef	uint64_t;
 * typedef	uint8_t;
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
 * void			NOTIFY(const char *, ...);
 * const char	*SYM(const char *, const char *);
 */
#include "net.h"
/******* """""""
 * struct S_LINK;
 * int	LINK_SEND(
 * 	struct S_LINK *,
 * 	uint32_t,
 * 	const void *,
 * 	uint32_t,
 * 	const void *,
 * 	uint32_t
 * );
 */
#include "clipboard.h"
/******* """""""""""""
 * #define CLIP_CHUNK
 * #define CLIP_MAX_FILES
 * #define CLIP_MAX_TEXT
 * struct S_CLIP;
 */
#include "host.h"
/******* """"""""
 * int	G_WORKER;
 */
/* **************************** [^] INCLUDES [^] **************************** */

#ifndef WM_CLIPBOARDUPDATE
#define WM_CLIPBOARDUPDATE 0X031D /* OLD MINGW HEADERS */
#endif /* !WM_CLIPBOARDUPDATE */

#define WM_CLIP_SET_TEXT (WM_APP + 1) /* LPARAM = WCHAR *, FREED */
#define WM_CLIP_SET_FILES (WM_APP + 2) /* LPARAM = LIST, FREED */
#define CLIP_TIMER 1 /* READS THE CLIPBOARD A MOMENT AFTER IT CHANGED */
#define CLIP_SETTLE_MS 150
#define CLIP_CLASS L"INWAY_CLIP"

struct	S_CLIP
{
	struct S_LINK	*LINK;
	char			WHO[24];
	HANDLE			THREAD; /* THE HIDDEN WINDOW */
	HANDLE			READY;
	HWND			WINDOW;
	volatile LONG	IGNORE_SEQ; /* OUR OWN LAST CHANGE */
	/* OUTGOING FILES */
	HANDLE			SENDER;
	volatile LONG	CANCEL;
	uint32_t		NEXT_ID;
	/* INCOMING FILES, ONLY TOUCHED BY THE NETWORK THREAD */
	WCHAR			BASE[MAX_PATH]; /* ...\INWAY-CLIPBOARD */
	WCHAR			IN_DIR[MAX_PATH]; /* FOLDER OF THE CURRENT COPY */
	uint32_t		IN_ID;
	int				IN_ACTIVE;
	HANDLE			IN_FILE;
	struct S_TEXT	IN_TOP; /* TOP ITEMS, "PATH\0PATH\0" */
	int				NUMBER; /* KEEPS FOLDERS OF TWO SESSIONS APART */
};

struct	S_SEND_JOB
{
	struct S_CLIP	*C;
	WCHAR			*LIST; /* "PATH\0PATH\0\0" */
	uint32_t		ID;
};

static volatile LONG	G__CLASS_DONE__ = 0;
static volatile LONG	G__NUMBER__ = 0;

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      SMALL HELPERS                       |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	TELL(enum E_STR ID, const char *WHO)
{
	struct S_TEXT	T = {0};

	TEXT_ADDF(&T, "  {d}%s ", SYM("●", "*"));
	TEXT_ADDF(&T, LANG(ID), WHO);
	TEXT_ADD(&T, "{0}");
	NOTIFY("%s", T.DATA);
	TEXT_FREE(&T);
}

/* "A\B" + "C" -> NEW MEMORY "A\B\C" */
static WCHAR
	*JOIN_PATH(const WCHAR *A, const WCHAR *B)
{
	size_t	LA;
	size_t	LB;
	WCHAR	*OUTPUT;

	LA = lstrlenW(A);
	LB = lstrlenW(B);
	OUTPUT = (WCHAR *)malloc((LA + LB + 2) * sizeof(WCHAR));
	if (!OUTPUT)
	{
		return (NULL);
	}
	memcpy(OUTPUT, A, LA * sizeof(WCHAR));
	OUTPUT[LA] = L'\\';
	memcpy(OUTPUT + LA + 1, B, (LB + 1) * sizeof(WCHAR));
	return (OUTPUT);
}

static void
	DELETE_TREE(const WCHAR *PATH)
{
	WIN32_FIND_DATAW	FD;
	HANDLE				FIND;
	WCHAR				*MASK;
	WCHAR				*CHILD;

	MASK = JOIN_PATH(PATH, L"*");
	if (!MASK)
	{
		return ;
	}
	FIND = FindFirstFileW(MASK, &FD);
	free(MASK);
	if (FIND != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!lstrcmpW(FD.cFileName, L".") || !lstrcmpW(FD.cFileName, L".."))
			{
				continue ;
			}
			CHILD = JOIN_PATH(PATH, FD.cFileName);
			if (!CHILD)
			{
				continue ;
			}
			if (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				DELETE_TREE(CHILD);
			}
			else
			{
				SetFileAttributesW(CHILD, FILE_ATTRIBUTE_NORMAL);
				DeleteFileW(CHILD);
			}
			free(CHILD);
		}
		while (FindNextFileW(FIND, &FD));
		FindClose(FIND);
	}
	RemoveDirectoryW(PATH);
}

/* COPIES FROM AN EARLIER DAY ARE NOT NEEDED ANY MORE */
static void
	CLEAN_OLD(const WCHAR *BASE)
{
	WIN32_FIND_DATAW	FD;
	HANDLE				FIND;
	WCHAR				*MASK;
	WCHAR				*CHILD;
	FILETIME			NOW;
	ULARGE_INTEGER		A;
	ULARGE_INTEGER		B;

	GetSystemTimeAsFileTime(&NOW);
	A.LowPart = NOW.dwLowDateTime;
	A.HighPart = NOW.dwHighDateTime;
	MASK = JOIN_PATH(BASE, L"*");
	if (!MASK)
	{
		return ;
	}
	FIND = FindFirstFileW(MASK, &FD);
	free(MASK);
	if (FIND == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	do
	{
		B.LowPart = FD.ftLastWriteTime.dwLowDateTime;
		B.HighPart = FD.ftLastWriteTime.dwHighDateTime;
		if ((FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			FD.cFileName[0] != L'.' &&
			A.QuadPart - B.QuadPart > 864000000000ULL) /* 24 HOURS */
		{
			CHILD = JOIN_PATH(BASE, FD.cFileName);
			if (CHILD)
			{
				DELETE_TREE(CHILD);
				free(CHILD);
			}
		}
	}
	while (FindNextFileW(FIND, &FD));
	FindClose(FIND);
}

/*
# A NAME FROM THE OTHER PC MUST STAY INSIDE OUR FOLDER: NO "..", NO DRIVE,
# NO LEADING SLASH. "/" IS TURNED INTO "\".
*/
static int
	SAFE_NAME(WCHAR *NAME)
{
	WCHAR	*P;
	WCHAR	*START;

	if (!NAME[0] || NAME[0] == L'\\' || NAME[0] == L'/' ||
		lstrlenW(NAME) > 1024)
	{
		return (0);
	}
	for (P = NAME; *P; ++P)
	{
		if (*P == L'/')
		{
			*P = L'\\';
		}
		if (*P == L':' || *P < 32)
		{
			return (0);
		}
	}
	START = NAME;
	for (P = NAME; ; ++P)
	{
		if (*P == L'\\' || *P == L'\0')
		{
			if (P == START || (P - START == 2 && START[0] == L'.' &&
				START[1] == L'.') || (P - START == 1 && START[0] == L'.'))
			{
				return (0);
			}
			if (*P == L'\0')
			{
				break ;
			}
			START = P + 1;
		}
	}
	return (1);
}

/* MAKES EVERY FOLDER ON THE WAY TO PATH (NOT THE LAST PART) */
static void
	MAKE_PARENTS(WCHAR *PATH, size_t SKIP)
{
	WCHAR	*P;

	for (P = PATH + SKIP; *P; ++P)
	{
		if (*P == L'\\')
		{
			*P = L'\0';
			CreateDirectoryW(PATH, NULL);
			*P = L'\\';
		}
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      SMALL HELPERS                       |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |             PUTTING THINGS ON THE CLIPBOARD              |  \\//  */
/*   \/   #==========================================================#   \/   */
static int
	OPEN_CLIPBOARD(HWND WINDOW)
{
	int	I;

	for (I = 0; I < 20; ++I) /* ANOTHER PROGRAM MAY HAVE IT OPEN */
	{
		if (OpenClipboard(WINDOW))
		{
			return (1);
		}
		Sleep(25);
	}
	return (0);
}

static void
	SET_TEXT(struct S_CLIP *C, const WCHAR *TEXT)
{
	HGLOBAL	MEMORY;
	size_t	BYTES;
	void	*P;

	BYTES = ((size_t)lstrlenW(TEXT) + 1) * sizeof(WCHAR);
	MEMORY = GlobalAlloc(GMEM_MOVEABLE, BYTES);
	if (!MEMORY)
	{
		return ;
	}
	P = GlobalLock(MEMORY);
	memcpy(P, TEXT, BYTES);
	GlobalUnlock(MEMORY);
	if (!OPEN_CLIPBOARD(C->WINDOW))
	{
		GlobalFree(MEMORY);
		return ;
	}
	EmptyClipboard();
	if (!SetClipboardData(CF_UNICODETEXT, MEMORY))
	{
		GlobalFree(MEMORY);
	}
	CloseClipboard();
	InterlockedExchange(&C->IGNORE_SEQ, (LONG)GetClipboardSequenceNumber());
}

/* LIST IS "PATH\0PATH\0\0" */
static void
	SET_FILES(struct S_CLIP *C, const WCHAR *LIST)
{
	const WCHAR	*P;
	size_t		BYTES;
	HGLOBAL		DROP;
	HGLOBAL		EFFECT;
	DROPFILES	*D;
	DWORD		*E;
	UINT		FORMAT;

	for (P = LIST; *P; P += lstrlenW(P) + 1)
	{
	}
	BYTES = (size_t)(P - LIST + 1) * sizeof(WCHAR);
	DROP = GlobalAlloc(GHND, sizeof(DROPFILES) + BYTES);
	EFFECT = GlobalAlloc(GMEM_MOVEABLE, sizeof(DWORD));
	if (!DROP || !EFFECT)
	{
		if (DROP)
		{
			GlobalFree(DROP);
		}
		if (EFFECT)
		{
			GlobalFree(EFFECT);
		}
		return ;
	}
	D = (DROPFILES *)GlobalLock(DROP);
	D->pFiles = sizeof(DROPFILES);
	D->fWide = TRUE;
	memcpy((uint8_t *)D + sizeof(DROPFILES), LIST, BYTES);
	GlobalUnlock(DROP);
	E = (DWORD *)GlobalLock(EFFECT);
	*E = 1; /* DROPEFFECT_COPY: PASTE COPIES, IT DOES NOT MOVE */
	GlobalUnlock(EFFECT);
	if (!OPEN_CLIPBOARD(C->WINDOW))
	{
		GlobalFree(DROP);
		GlobalFree(EFFECT);
		return ;
	}
	EmptyClipboard();
	if (!SetClipboardData(CF_HDROP, DROP))
	{
		GlobalFree(DROP);
	}
	FORMAT = RegisterClipboardFormatW(L"Preferred DropEffect");
	if (!SetClipboardData(FORMAT, EFFECT))
	{
		GlobalFree(EFFECT);
	}
	CloseClipboard();
	InterlockedExchange(&C->IGNORE_SEQ, (LONG)GetClipboardSequenceNumber());
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |             PUTTING THINGS ON THE CLIPBOARD              |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      SENDING FILES                       |  \\//  */
/*   \/   #==========================================================#   \/   */
/* SIZE OF EVERYTHING UNDER PATH; STOPS EARLY ONCE IT IS TOO MUCH */
static uint64_t
	MEASURE(const WCHAR *PATH)
{
	WIN32_FIND_DATAW	FD;
	HANDLE				FIND;
	WCHAR				*MASK;
	WCHAR				*CHILD;
	uint64_t			TOTAL;
	DWORD				ATTR;

	ATTR = GetFileAttributesW(PATH);
	if (ATTR == INVALID_FILE_ATTRIBUTES)
	{
		return (0);
	}
	if (!(ATTR & FILE_ATTRIBUTE_DIRECTORY))
	{
		FIND = FindFirstFileW(PATH, &FD);
		if (FIND == INVALID_HANDLE_VALUE)
		{
			return (0);
		}
		FindClose(FIND);
		return (((uint64_t)FD.nFileSizeHigh << 32) | FD.nFileSizeLow);
	}
	TOTAL = 0;
	MASK = JOIN_PATH(PATH, L"*");
	FIND = MASK ? FindFirstFileW(MASK, &FD) : INVALID_HANDLE_VALUE;
	free(MASK);
	if (FIND == INVALID_HANDLE_VALUE)
	{
		return (0);
	}
	do
	{
		if (!lstrcmpW(FD.cFileName, L".") || !lstrcmpW(FD.cFileName, L".."))
		{
			continue ;
		}
		CHILD = JOIN_PATH(PATH, FD.cFileName);
		if (CHILD)
		{
			TOTAL += MEASURE(CHILD);
			free(CHILD);
		}
	}
	while (TOTAL <= CLIP_MAX_FILES && FindNextFileW(FIND, &FD));
	FindClose(FIND);
	return (TOTAL);
}

/* SENDS ONE FILE OR FOLDER (AND WHAT IS INSIDE IT). 0 = STOP EVERYTHING. */
static int
	SEND_ITEM(
		struct S_CLIP *C,
		uint32_t ID,
		const WCHAR *FULL,
		const WCHAR *NAME
	)
{
	struct S_CLIP_FILE	HDR;
	WIN32_FIND_DATAW	FD;
	LARGE_INTEGER		SIZE;
	HANDLE				FIND;
	HANDLE				FILE_HANDLE;
	WCHAR				*MASK;
	WCHAR				*CHILD_FULL;
	WCHAR				*CHILD_NAME;
	uint8_t				*PIECE;
	DWORD				ATTR;
	DWORD				GOT;
	uint32_t			NAME_BYTES;
	int					OK;

	ATTR = GetFileAttributesW(FULL);
	if (ATTR == INVALID_FILE_ATTRIBUTES)
	{
		return (1); /* GONE IN THE MEANTIME: SKIP IT */
	}
	NAME_BYTES = (uint32_t)(lstrlenW(NAME) + 1) * sizeof(WCHAR);
	HDR.ID = ID;
	if (ATTR & FILE_ATTRIBUTE_DIRECTORY)
	{
		HDR.IS_FOLDER = 1;
		HDR.SIZE = 0;
		if (!LINK_SEND(
			C->LINK,
			MSG_CLIP_FILE,
			&HDR,
			sizeof(HDR),
			NAME,
			NAME_BYTES
		))
		{
			return (0);
		}
		MASK = JOIN_PATH(FULL, L"*");
		FIND = MASK ? FindFirstFileW(MASK, &FD) : INVALID_HANDLE_VALUE;
		free(MASK);
		if (FIND == INVALID_HANDLE_VALUE)
		{
			return (1);
		}
		OK = 1;
		do
		{
			if (!lstrcmpW(FD.cFileName, L".") || !lstrcmpW(FD.cFileName, L".."))
			{
				continue ;
			}
			CHILD_FULL = JOIN_PATH(FULL, FD.cFileName);
			CHILD_NAME = JOIN_PATH(NAME, FD.cFileName);
			if (CHILD_FULL && CHILD_NAME)
			{
				OK = SEND_ITEM(C, ID, CHILD_FULL, CHILD_NAME);
			}
			free(CHILD_FULL);
			free(CHILD_NAME);
		}
		while (OK && !C->CANCEL && FindNextFileW(FIND, &FD));
		FindClose(FIND);
		return (OK && !C->CANCEL);
	}
	FILE_HANDLE = CreateFileW(
		FULL,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL
	);
	if (FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		return (1); /* LOCKED OR NO RIGHTS: SKIP IT */
	}
	SIZE.QuadPart = 0;
	GetFileSizeEx(FILE_HANDLE, &SIZE);
	HDR.IS_FOLDER = 0;
	HDR.SIZE = (uint64_t)SIZE.QuadPart;
	PIECE = (uint8_t *)malloc(CLIP_CHUNK);
	OK = PIECE && LINK_SEND(
		C->LINK,
		MSG_CLIP_FILE,
		&HDR,
		sizeof(HDR),
		NAME,
		NAME_BYTES
	);
	while (OK && !C->CANCEL &&
		ReadFile(FILE_HANDLE, PIECE, CLIP_CHUNK, &GOT, NULL) && GOT)
	{
		OK = LINK_SEND(C->LINK, MSG_CLIP_DATA, &ID, 4, PIECE, GOT);
	}
	free(PIECE);
	CloseHandle(FILE_HANDLE);
	return (OK && !C->CANCEL);
}

static DWORD WINAPI
	SENDER_THREAD(LPVOID PARAMETER)
{
	struct S_SEND_JOB	*JOB;
	struct S_CLIP		*C;
	const WCHAR			*P;
	const WCHAR			*NAME;
	uint64_t			TOTAL;
	int					OK;

	JOB = (struct S_SEND_JOB *)PARAMETER;
	C = JOB->C;
	TOTAL = 0;
	for (P = JOB->LIST; *P && TOTAL <= CLIP_MAX_FILES; P += lstrlenW(P) + 1)
	{
		TOTAL += MEASURE(P);
	}
	if (TOTAL > CLIP_MAX_FILES)
	{
		TELL(STR_C_TOO_BIG, C->WHO);
		free(JOB->LIST);
		free(JOB);
		return (0);
	}
	OK = LINK_SEND(C->LINK, MSG_CLIP_BEGIN, &JOB->ID, 4, NULL, 0);
	for (P = JOB->LIST; OK && *P && !C->CANCEL; P += lstrlenW(P) + 1)
	{
		NAME = wcsrchr(P, L'\\');
		NAME = NAME ? NAME + 1 : P;
		if (*NAME) /* "C:\" ALONE HAS NO NAME: SKIP */
		{
			OK = SEND_ITEM(C, JOB->ID, P, NAME);
		}
	}
	if (OK && !C->CANCEL)
	{
		if (LINK_SEND(C->LINK, MSG_CLIP_END, &JOB->ID, 4, NULL, 0))
		{
			TELL(STR_C_SENT, C->WHO);
		}
	}
	else
	{
		LINK_SEND(C->LINK, MSG_CLIP_ABORT, &JOB->ID, 4, NULL, 0);
	}
	free(JOB->LIST);
	free(JOB);
	return (0);
}

static void
	CANCEL_SENDER(struct S_CLIP *C)
{
	if (!C->SENDER)
	{
		return ;
	}
	InterlockedExchange(&C->CANCEL, 1);
	WaitForSingleObject(C->SENDER, 10000);
	CloseHandle(C->SENDER);
	C->SENDER = NULL;
	InterlockedExchange(&C->CANCEL, 0);
}

static void
	START_SENDER(struct S_CLIP *C, WCHAR *LIST)
{
	struct S_SEND_JOB	*JOB;

	CANCEL_SENDER(C);
	JOB = (struct S_SEND_JOB *)malloc(sizeof(*JOB));
	if (!JOB)
	{
		free(LIST);
		return ;
	}
	JOB->C = C;
	JOB->LIST = LIST;
	JOB->ID = ++C->NEXT_ID;
	C->SENDER = CreateThread(NULL, 0, SENDER_THREAD, JOB, 0, NULL);
	if (!C->SENDER)
	{
		free(LIST);
		free(JOB);
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      SENDING FILES                       |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                  WATCHING THE CLIPBOARD                  |  \\//  */
/*   \/   #==========================================================#   \/   */
/* "PATH\0PATH\0\0" FROM A FILE LIST (CF_HDROP) */
static WCHAR
	*DROP_LIST(HDROP DROP)
{
	WCHAR	*LIST;
	UINT	COUNT;
	UINT	I;
	UINT	LEN;
	size_t	CHARS;
	size_t	AT;

	COUNT = DragQueryFileW(DROP, 0XFFFFFFFF, NULL, 0);
	if (!COUNT)
	{
		return (NULL);
	}
	CHARS = 1;
	for (I = 0; I < COUNT; ++I)
	{
		CHARS += DragQueryFileW(DROP, I, NULL, 0) + 1;
	}
	LIST = (WCHAR *)calloc(CHARS, sizeof(WCHAR));
	AT = 0;
	for (I = 0; LIST && I < COUNT; ++I)
	{
		LEN = DragQueryFileW(DROP, I, LIST + AT, (UINT)(CHARS - AT));
		AT += LEN + 1;
	}
	return (LIST);
}

/*
# EXPLORER DOES NOT PUT THE FILE LIST ON THE CLIPBOARD RIGHT AWAY: IT MAKES IT
# WHEN SOMEONE ASKS. WHEN THAT DID NOT WORK, THIS ASKS EXPLORER'S COPIED DATA
# DIRECTLY, THE SAME WAY EXPLORER ITSELF PASTES.
*/
static WCHAR
	*OLE_FILE_LIST(void)
{
	IDataObject	*OBJECT;
	FORMATETC	FORMAT;
	STGMEDIUM	MEDIUM;
	WCHAR		*LIST;

	LIST = NULL;
	if (OleGetClipboard(&OBJECT) != S_OK)
	{
		return (NULL);
	}
	FORMAT.cfFormat = CF_HDROP;
	FORMAT.ptd = NULL;
	FORMAT.dwAspect = DVASPECT_CONTENT;
	FORMAT.lindex = -1;
	FORMAT.tymed = TYMED_HGLOBAL;
	if (IDataObject_GetData(OBJECT, &FORMAT, &MEDIUM) == S_OK)
	{
		if (MEDIUM.tymed == TYMED_HGLOBAL && MEDIUM.hGlobal)
		{
			LIST = DROP_LIST((HDROP)MEDIUM.hGlobal);
		}
		ReleaseStgMedium(&MEDIUM);
	}
	IDataObject_Release(OBJECT);
	return (LIST);
}

/*
# 1 WHEN EVERY PATH IN LIST IS IN OUR OWN FOLDER: FILES THE OTHER PC SENT.
# EXPLORER WRITES "PASTE DONE" INTO THE CLIPBOARD AFTER A PASTE, AND WITHOUT
# THIS THE SAME FILES WOULD GO STRAIGHT BACK TO THE OTHER PC.
*/
static int
	OUR_OWN_COPY(const struct S_CLIP *C, const WCHAR *LIST)
{
	const WCHAR	*P;
	size_t		N;

	N = (size_t)lstrlenW(C->BASE);
	for (P = LIST; *P; P += lstrlenW(P) + 1)
	{
		if ((size_t)lstrlenW(P) <= N || _wcsnicmp(P, C->BASE, N) != 0 ||
			P[N] != L'\\')
		{
			return (0);
		}
	}
	return (P != LIST);
}

/* SOMETHING WAS COPIED ON THIS PC */
static void
	CLIPBOARD_CHANGED(struct S_CLIP *C)
{
	HANDLE		DATA;
	HWND		OWNER;
	const WCHAR	*TEXT;
	WCHAR		*COPY;
	WCHAR		*LIST;
	size_t		CHARS;
	int			FILES;

	if ((LONG)GetClipboardSequenceNumber() == C->IGNORE_SEQ)
	{
		return ; /* WE PUT IT THERE OURSELVES */
	}
	if (!OPEN_CLIPBOARD(C->WINDOW))
	{
		return ;
	}
	COPY = NULL;
	LIST = NULL;
	FILES = IsClipboardFormatAvailable(CF_HDROP);
	if (FILES)
	{
		DATA = GetClipboardData(CF_HDROP);
		OWNER = GetClipboardOwner();
		if (!DATA && OWNER && SendMessageTimeoutW(
			OWNER,
			WM_NULL,
			0,
			0,
			SMTO_ABORTIFHUNG,
			3000,
			NULL
		))
		{
			DATA = GetClipboardData(CF_HDROP); /* IT WAS STILL MAKING IT */
		}
		LIST = DATA ? DROP_LIST((HDROP)DATA) : NULL;
	}
	else if (IsClipboardFormatAvailable(CF_UNICODETEXT) &&
		(DATA = GetClipboardData(CF_UNICODETEXT)) != NULL)
	{
		TEXT = (const WCHAR *)GlobalLock(DATA);
		if (TEXT)
		{
			CHARS = (size_t)lstrlenW(TEXT) + 1;
			if (CHARS * sizeof(WCHAR) <= CLIP_MAX_TEXT)
			{
				COPY = (WCHAR *)malloc(CHARS * sizeof(WCHAR));
				if (COPY)
				{
					memcpy(COPY, TEXT, CHARS * sizeof(WCHAR));
				}
			}
			GlobalUnlock(DATA);
		}
	}
	CloseClipboard();
	if (LIST && OUR_OWN_COPY(C, LIST))
	{
		free(LIST); /* PASTING IT TOUCHED THE CLIPBOARD: DO NOT SEND IT BACK */
		return ;
	}
	if (FILES && !LIST)
	{
		LIST = OLE_FILE_LIST();
		if (!LIST)
		{
			TELL(STR_C_CANT_READ, C->WHO);
			return ;
		}
	}
	if (LIST)
	{
		START_SENDER(C, LIST);
	}
	else if (COPY)
	{
		CANCEL_SENDER(C);
		LINK_SEND(
			C->LINK,
			MSG_CLIP_TEXT,
			COPY,
			(uint32_t)(((size_t)lstrlenW(COPY) + 1) * sizeof(WCHAR)),
			NULL,
			0
		);
		free(COPY);
	}
}

static LRESULT CALLBACK
	WINDOW_PROC(HWND WINDOW, UINT MSG, WPARAM WP, LPARAM LP)
{
	struct S_CLIP	*C;

	C = (struct S_CLIP *)GetWindowLongPtrW(WINDOW, GWLP_USERDATA);
	switch (MSG)
	{
		case (WM_CLIPBOARDUPDATE):
			/* WAIT A MOMENT: A COPY OFTEN CHANGES THE CLIPBOARD A FEW TIMES */
			SetTimer(WINDOW, CLIP_TIMER, CLIP_SETTLE_MS, NULL);
			return (0);
		case (WM_TIMER):
			if (WP == CLIP_TIMER)
			{
				KillTimer(WINDOW, CLIP_TIMER);
				if (C)
				{
					CLIPBOARD_CHANGED(C);
				}
			}
			return (0);
		case (WM_CLIP_SET_TEXT):
			if (C)
			{
				SET_TEXT(C, (const WCHAR *)LP);
			}
			free((void *)LP);
			return (0);
		case (WM_CLIP_SET_FILES):
			if (C)
			{
				SET_FILES(C, (const WCHAR *)LP);
			}
			free((void *)LP);
			return (0);
		case (WM_CLOSE):
			DestroyWindow(WINDOW);
			return (0);
		case (WM_DESTROY):
			RemoveClipboardFormatListener(WINDOW);
			PostQuitMessage(0);
			return (0);
	}
	return (DefWindowProcW(WINDOW, MSG, WP, LP));
}

static DWORD WINAPI
	WINDOW_THREAD(LPVOID PARAMETER)
{
	struct S_CLIP	*C;
	WNDCLASSW		WC;
	MSG				M;

	C = (struct S_CLIP *)PARAMETER;
	OleInitialize(NULL); /* FOR OLE_FILE_LIST() */
	if (InterlockedCompareExchange(&G__CLASS_DONE__, 1, 0) == 0)
	{
		memset(&WC, 0, sizeof(WC));
		WC.lpfnWndProc = WINDOW_PROC;
		WC.hInstance = GetModuleHandleW(NULL);
		WC.lpszClassName = CLIP_CLASS;
		RegisterClassW(&WC);
	}
	C->WINDOW = CreateWindowExW(
		0,
		CLIP_CLASS,
		L"",
		0,
		0,
		0,
		0,
		0,
		HWND_MESSAGE,
		NULL,
		GetModuleHandleW(NULL),
		NULL
	);
	if (C->WINDOW)
	{
		SetWindowLongPtrW(C->WINDOW, GWLP_USERDATA, (LONG_PTR)C);
		AddClipboardFormatListener(C->WINDOW);
	}
	SetEvent(C->READY);
	if (!C->WINDOW)
	{
		OleUninitialize();
		return (0);
	}
	while (GetMessageW(&M, NULL, 0, 0) > 0)
	{
		DispatchMessageW(&M);
	}
	OleUninitialize();
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                  WATCHING THE CLIPBOARD                  |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |               RECEIVING FROM THE OTHER PC                |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	CLOSE_IN_FILE(struct S_CLIP *C)
{
	if (C->IN_FILE != INVALID_HANDLE_VALUE)
	{
		CloseHandle(C->IN_FILE);
		C->IN_FILE = INVALID_HANDLE_VALUE;
	}
}

static void
	GOT_BEGIN(struct S_CLIP *C, uint32_t ID)
{
	CLOSE_IN_FILE(C);
	if (C->IN_DIR[0])
	{
		DELETE_TREE(C->IN_DIR); /* THE COPY BEFORE THIS ONE */
	}
	CreateDirectoryW(C->BASE, NULL);
	swprintf(
		C->IN_DIR,
		MAX_PATH,
		L"%ls\\%lu-%d-%lu",
		C->BASE,
		(unsigned long)GetCurrentProcessId(),
		C->NUMBER,
		(unsigned long)ID
	);
	DELETE_TREE(C->IN_DIR); /* A LEFTOVER WITH THE SAME NAME */
	CreateDirectoryW(C->IN_DIR, NULL);
	C->IN_ID = ID;
	C->IN_ACTIVE = 1;
	C->IN_TOP.LEN = 0;
}

static void
	GOT_FILE(struct S_CLIP *C, const uint8_t *DATA, uint32_t SIZE)
{
	struct S_CLIP_FILE	HDR;
	WCHAR				*NAME;
	WCHAR				*FULL;
	size_t				CHARS;

	if (SIZE < sizeof(HDR) + 2)
	{
		return ;
	}
	memcpy(&HDR, DATA, sizeof(HDR));
	if (!C->IN_ACTIVE || HDR.ID != C->IN_ID)
	{
		return ;
	}
	CLOSE_IN_FILE(C);
	CHARS = (SIZE - sizeof(HDR)) / sizeof(WCHAR);
	NAME = (WCHAR *)calloc(CHARS + 1, sizeof(WCHAR));
	if (!NAME)
	{
		return ;
	}
	memcpy(NAME, DATA + sizeof(HDR), CHARS * sizeof(WCHAR));
	if (!SAFE_NAME(NAME) ||
		(FULL = JOIN_PATH(C->IN_DIR, NAME)) == NULL)
	{
		free(NAME);
		return ;
	}
	MAKE_PARENTS(FULL, (size_t)lstrlenW(C->IN_DIR) + 1);
	if (HDR.IS_FOLDER)
	{
		CreateDirectoryW(FULL, NULL);
	}
	else
	{
		C->IN_FILE = CreateFileW(
			FULL,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
	}
	if (!wcschr(NAME, L'\\')) /* A TOP ITEM: IT GOES ON THE CLIPBOARD */
	{
		TEXT_ADDN(
			&C->IN_TOP,
			(const char *)FULL,
			((size_t)lstrlenW(FULL) + 1) * sizeof(WCHAR)
		);
	}
	free(FULL);
	free(NAME);
}

static void
	GOT_END(struct S_CLIP *C, uint32_t ID)
{
	WCHAR	*LIST;

	CLOSE_IN_FILE(C);
	if (!C->IN_ACTIVE || ID != C->IN_ID)
	{
		return ;
	}
	C->IN_ACTIVE = 0;
	if (!C->IN_TOP.LEN)
	{
		return ;
	}
	LIST = (WCHAR *)calloc(C->IN_TOP.LEN + 2 * sizeof(WCHAR), 1);
	if (!LIST)
	{
		return ;
	}
	memcpy(LIST, C->IN_TOP.DATA, C->IN_TOP.LEN);
	if (!PostMessageW(C->WINDOW, WM_CLIP_SET_FILES, 0, (LPARAM)LIST))
	{
		free(LIST);
		return ;
	}
	TELL(STR_C_GOT, C->WHO);
}

/* CALLED BY THE THREAD THAT READS THE CONNECTION */
void
	CLIP_RECEIVE(
		struct S_CLIP *C,
		uint32_t TYPE,
		const uint8_t *DATA,
		uint32_t SIZE
	)
{
	WCHAR		*TEXT;
	uint32_t	ID;
	DWORD		DONE;

	if (!C)
	{
		return ;
	}
	ID = 0;
	if (SIZE >= 4)
	{
		memcpy(&ID, DATA, 4);
	}
	switch (TYPE)
	{
		case (MSG_CLIP_TEXT):
			TEXT = (WCHAR *)calloc(SIZE / sizeof(WCHAR) + 1, sizeof(WCHAR));
			if (!TEXT)
			{
				return ;
			}
			memcpy(TEXT, DATA, (SIZE / sizeof(WCHAR)) * sizeof(WCHAR));
			if (!PostMessageW(C->WINDOW, WM_CLIP_SET_TEXT, 0, (LPARAM)TEXT))
			{
				free(TEXT);
			}
			return ;
		case (MSG_CLIP_BEGIN):
			GOT_BEGIN(C, ID);
			return ;
		case (MSG_CLIP_FILE):
			GOT_FILE(C, DATA, SIZE);
			return ;
		case (MSG_CLIP_DATA):
			if (C->IN_ACTIVE && ID == C->IN_ID && SIZE > 4 &&
				C->IN_FILE != INVALID_HANDLE_VALUE)
			{
				WriteFile(C->IN_FILE, DATA + 4, SIZE - 4, &DONE, NULL);
			}
			return ;
		case (MSG_CLIP_END):
			GOT_END(C, ID);
			return ;
		case (MSG_CLIP_ABORT):
			CLOSE_IN_FILE(C);
			if (C->IN_ACTIVE && ID == C->IN_ID && C->IN_DIR[0])
			{
				DELETE_TREE(C->IN_DIR);
				C->IN_DIR[0] = L'\0';
			}
			C->IN_ACTIVE = 0;
			return ;
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |               RECEIVING FROM THE OTHER PC                |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      START AND STOP                      |  \\//  */
/*   \/   #==========================================================#   \/   */
struct S_CLIP
	*CLIP_START(struct S_LINK *LINK, const char *WHO)
{
	struct S_CLIP	*C;
	WCHAR			ROOT[MAX_PATH];
	DWORD			LEN;

	C = (struct S_CLIP *)calloc(1, sizeof(*C));
	if (!C)
	{
		return (NULL);
	}
	C->LINK = LINK;
	snprintf(C->WHO, sizeof(C->WHO), "%s", WHO);
	C->IN_FILE = INVALID_HANDLE_VALUE;
	C->IGNORE_SEQ = -1;
	C->NUMBER = (int)InterlockedIncrement(&G__NUMBER__);
	/*
	# THE BACKGROUND SERVICE RUNS AS SYSTEM: ITS TEMP FOLDER IS NOT READABLE
	# FOR THE PERSON AT THE PC, SO IT USES C:\USERS\PUBLIC INSTEAD.
	*/
	LEN = 0;
	if (G_WORKER)
	{
		LEN = GetEnvironmentVariableW(L"PUBLIC", ROOT, MAX_PATH);
	}
	if (!LEN || LEN >= MAX_PATH - 32)
	{
		LEN = GetTempPathW(MAX_PATH, ROOT);
	}
	while (LEN && ROOT[LEN - 1] == L'\\')
	{
		ROOT[--LEN] = L'\0';
	}
	swprintf(C->BASE, MAX_PATH, L"%ls\\INWAY-Clipboard", ROOT);
	CLEAN_OLD(C->BASE);
	C->READY = CreateEventW(NULL, TRUE, FALSE, NULL);
	C->THREAD = CreateThread(NULL, 0, WINDOW_THREAD, C, 0, NULL);
	if (!C->READY || !C->THREAD)
	{
		if (C->READY)
		{
			CloseHandle(C->READY);
		}
		free(C);
		return (NULL);
	}
	WaitForSingleObject(C->READY, 5000);
	if (!C->WINDOW)
	{
		CLIP_STOP(C);
		return (NULL);
	}
	return (C);
}

void
	CLIP_STOP(struct S_CLIP *C)
{
	if (!C)
	{
		return ;
	}
	CANCEL_SENDER(C);
	if (C->WINDOW)
	{
		PostMessageW(C->WINDOW, WM_CLOSE, 0, 0);
	}
	if (C->THREAD)
	{
		WaitForSingleObject(C->THREAD, 5000);
		CloseHandle(C->THREAD);
	}
	if (C->READY)
	{
		CloseHandle(C->READY);
	}
	CLOSE_IN_FILE(C);
	TEXT_FREE(&C->IN_TOP);
	free(C);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      START AND STOP                      |  //\\  */
/* '""""' #==========================================================# '""""' */
