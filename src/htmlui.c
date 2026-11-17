/******************************************************************************\
# C - HTMLUI                                     #       Maximum Tension       #
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
|*|                                  HTMLUI                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| SHOWHTMLDIALOG() WRAPPERS FOR THE ACCEPT WINDOW AND THE SESSION BANNER.  |*|
|*| EACH DIALOG RUNS IN ITS OWN THREAD, SO THE CALLER CAN KEEP WATCHING THE  |*|
|*| SOCKET AND CLOSE THE DIALOG WHEN THE OTHER SIDE GIVES UP.                |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| int HTMLUI_ASK(SOCKET S, const char *WHO);                          : *|
|* : 3|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * LANG(), UTF8_TO_WIDE(), G_RUNNING
 */
#include <ole2.h>
/******* """"""""
 * OleInitialize(), VARIANT, SysAllocString()
 */
#include <urlmon.h>
/******* """"""""""
 * CreateURLMoniker()
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* SHOWHTMLDIALOG() FROM MSHTML.DLL, LOADED WHEN NEEDED */
typedef HRESULT	(STDAPICALLTYPE *T_SHOW_HTML_DIALOG)(
	HWND PARENT,
	IMoniker *MONIKER,
	VARIANT *ARGUMENTS,
	WCHAR *OPTIONS,
	VARIANT *RESULT
);

struct	S_DIALOG
{
	const WCHAR	*PAGE;		/* RESOURCE NAME, L"ACCEPT.HTM" */
	WCHAR		*ARGS;		/* WINDOW.DIALOGARGUMENTS */
	const WCHAR	*OPTIONS;	/* SIZE AND LOOK OF THE WINDOW */
	WCHAR		RESULT[32];	/* WINDOW.RETURNVALUE */
	HRESULT		HR;
	HANDLE		THREAD;
	DWORD		THREAD_ID;
};

static struct S_DIALOG	G__BANNER__;
static SOCKET		G__BANNER_SOCKET__ = INVALID_SOCKET;
static volatile LONG	G__BANNER_CLOSING__ = 0;

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                     RUNNING A DIALOG                     |  \\//  */
/*   \/   #==========================================================#   \/   */
/* "res://C:\Program%20Files\INWAY\INWAY.exe/ACCEPT.HTM" */
static void
	PAGE_URL(const WCHAR *PAGE, WCHAR *V_OUT, size_t CAP)
{
	WCHAR	EXE[MAX_PATH];
	WCHAR	*P;
	size_t	N;

	GetModuleFileNameW(NULL, EXE, MAX_PATH);
	N = (size_t)swprintf(V_OUT, CAP, L"res://");
	for (P = EXE; *P && N + 4 < CAP; ++P)
	{
		if (*P == L' ' || *P == L'#' || *P == L'%')
		{
			N += (size_t)swprintf(V_OUT + N, CAP - N, L"%%%02X", (unsigned)*P);
		}
		else
		{
			V_OUT[N++] = *P;
		}
	}
	swprintf(V_OUT + N, CAP - N, L"/%ls", PAGE);
}

static DWORD WINAPI
	DIALOG_THREAD(LPVOID PARAMETER)
{
	struct S_DIALOG		*D;
	HMODULE			MSHTML;
	T_SHOW_HTML_DIALOG	SHOW;
	IMoniker		*MONIKER;
	VARIANT			V_IN;
	VARIANT			V_OUT;
	WCHAR			URL[MAX_PATH * 3 + 64];

	D = (struct S_DIALOG *)PARAMETER;
	D->HR = E_FAIL;
	MONIKER = NULL;
	OleInitialize(NULL);
	MSHTML = LoadLibraryW(L"mshtml.dll");
	SHOW = NULL;
	if (MSHTML)
	{
		SHOW = (T_SHOW_HTML_DIALOG)(void *)GetProcAddress(
			MSHTML,
			"ShowHTMLDialog"
		);
	}
	PAGE_URL(D->PAGE, URL, sizeof(URL) / sizeof(URL[0]));
	if (SHOW && SUCCEEDED(CreateURLMoniker(NULL, URL, &MONIKER)))
	{
		VariantInit(&V_IN);
		VariantInit(&V_OUT);
		V_IN.vt = VT_BSTR;
		V_IN.bstrVal = SysAllocString(D->ARGS);
		D->HR = SHOW(NULL, MONIKER, &V_IN, (WCHAR *)D->OPTIONS, &V_OUT);
		if (SUCCEEDED(D->HR) && V_OUT.vt == VT_BSTR && V_OUT.bstrVal)
		{
			lstrcpynW(D->RESULT, V_OUT.bstrVal, 32);
		}
		VariantClear(&V_IN);
		VariantClear(&V_OUT);
		IMoniker_Release(MONIKER);
	}
	if (MSHTML)
	{
		FreeLibrary(MSHTML);
	}
	OleUninitialize();
	return (0);
}

static BOOL CALLBACK
	FIND_PROC(HWND WINDOW, LPARAM PARAMETER)
{
	WCHAR	CLASS[64];

	if (GetClassNameW(WINDOW, CLASS, 64) &&
		lstrcmpW(CLASS, L"Internet Explorer_TridentDlgFrame") == 0)
	{
		*(HWND *)PARAMETER = WINDOW;
		return (FALSE);
	}
	return (TRUE);
}

/* THE DIALOG'S WINDOW, OR NULL WHEN IT IS NOT ON THE SCREEN (YET) */
static HWND
	DIALOG_WINDOW(const struct S_DIALOG *D)
{
	HWND	FOUND;

	FOUND = NULL;
	if (D->THREAD_ID)
	{
		EnumThreadWindows(D->THREAD_ID, FIND_PROC, (LPARAM)&FOUND);
	}
	return (FOUND);
}

/* 1 WHEN THE OTHER SIDE CLOSED THE CONNECTION WHILE WE WAIT */
static int
	PEER_GONE(SOCKET S)
{
	fd_set		READ;
	struct timeval	TV;
	char		PEEK;

	FD_ZERO(&READ);
	FD_SET(S, &READ);
	TV.tv_sec = 0;
	TV.tv_usec = 0;
	if (select(0, &READ, NULL, NULL, &TV) == 1)
	{
		return (recv(S, &PEEK, 1, MSG_PEEK) <= 0);
	}
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                     RUNNING A DIALOG                     |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      ACCEPT WINDOW                       |  \\//  */
/*   \/   #==========================================================#   \/   */
static int
	ASK_FALLBACK(const char *WHO)
{
	struct S_TEXT	T = {0};
	WCHAR		*TEXT;
	int		ANSWER;

	TEXT_ADDF(&T, LANG(STR_H_TITLE), WHO);
	TEXT_ADD(&T, "\n\n");
	TEXT_ADD(&T, LANG(STR_H_TEXT));
	TEXT = UTF8_TO_WIDE(T.DATA);
	ANSWER = MessageBoxW(
		NULL,
		TEXT,
		L"INWAY",
		MB_YESNO | MB_ICONQUESTION | MB_TOPMOST | MB_SETFOREGROUND
	);
	free(TEXT);
	TEXT_FREE(&T);
	return (ANSWER == IDYES);
}

int
	HTMLUI_ASK(SOCKET S, const char *WHO)
{
	struct S_DIALOG	D;
	struct S_TEXT	ARGS = {0};
	FLASHWINFO		FLASH;
	DWORD		START;
	HWND		WINDOW;
	int		SHOWN;
	int		GONE;

	memset(&D, 0, sizeof(D));
	TEXT_ADDF(
		&ARGS,
		"%s\n%s\n%s\n%s\n%s\n%s\n%d",
		WHO,
		LANG(STR_H_TITLE),
		LANG(STR_H_TEXT),
		LANG(STR_H_ACCEPT),
		LANG(STR_H_CANCEL),
		LANG(STR_H_COUNTDOWN),
		INWAY_ASK_SECONDS
	);
	D.ARGS = UTF8_TO_WIDE(ARGS.DATA);
	TEXT_FREE(&ARGS);
	D.PAGE = L"ACCEPT.HTM";
	D.OPTIONS = L"dialogWidth:480px;dialogHeight:236px;center:yes;"
		L"resizable:no;scroll:no;status:no;help:no;";
	MessageBeep(MB_ICONQUESTION);
	START = GetTickCount();
	SHOWN = 0;
	GONE = 0;
	D.THREAD = CreateThread(
		NULL,
		0,
		DIALOG_THREAD,
		&D,
		0,
		&D.THREAD_ID
	);
	while (D.THREAD && WaitForSingleObject(D.THREAD, 250) == WAIT_TIMEOUT)
	{
		WINDOW = DIALOG_WINDOW(&D);
		if (WINDOW && !SHOWN)
		{
			SetWindowPos(
				WINDOW,
				HWND_TOPMOST,
				0,
				0,
				0,
				0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW
			);
			SetForegroundWindow(WINDOW);
			memset(&FLASH, 0, sizeof(FLASH));
			FLASH.cbSize = sizeof(FLASH);
			FLASH.hwnd = WINDOW;
			FLASH.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			FlashWindowEx(&FLASH);
			SHOWN = 1;
		}
		if (!GONE && PEER_GONE(S))
		{
			GONE = 1;
		}
		if (WINDOW && (GONE ||
			GetTickCount() - START > (DWORD)(INWAY_ASK_SECONDS + 5) * 1000))
		{
			PostMessageW(WINDOW, WM_CLOSE, 0, 0);
		}
	}
	if (D.THREAD)
	{
		CloseHandle(D.THREAD);
	}
	free(D.ARGS);
	if (GONE)
	{
		return (-1);
	}
	if (!D.THREAD || FAILED(D.HR))
	{
		return (ASK_FALLBACK(WHO)); /* NO MSHTML */
	}
	return (lstrcmpW(D.RESULT, L"accept") == 0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      ACCEPT WINDOW                       |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      SESSION BANNER                      |  \\//  */
/*   \/   #==========================================================#   \/   */
static DWORD WINAPI
	BANNER_THREAD(LPVOID PARAMETER)
{
	(void)PARAMETER;
	DIALOG_THREAD(&G__BANNER__);
	/* DISCONNECT WAS CLICKED: END THE SESSION */
	if (!G__BANNER_CLOSING__ && lstrcmpW(G__BANNER__.RESULT, L"stop") == 0)
	{
		InterlockedExchange(&G_RUNNING, 0);
		shutdown(G__BANNER_SOCKET__, SD_BOTH);
	}
	return (0);
}

void
	HTMLUI_BANNER_SHOW(const char *WHO, SOCKET S)
{
	struct S_TEXT	ARGS = {0};
	HWND		PREVIOUS;
	HWND		WINDOW;
	int		I;

	HTMLUI_BANNER_HIDE();
	PREVIOUS = GetForegroundWindow();
	memset(&G__BANNER__, 0, sizeof(G__BANNER__));
	TEXT_ADDF(
		&ARGS,
		"%s\n%s\n%s",
		WHO,
		LANG(STR_STATUS_CONTROLLING),
		LANG(STR_H_DISCONNECT)
	);
	G__BANNER__.ARGS = UTF8_TO_WIDE(ARGS.DATA);
	TEXT_FREE(&ARGS);
	G__BANNER__.PAGE = L"BANNER.HTM";
	G__BANNER__.OPTIONS = L"dialogWidth:420px;dialogHeight:74px;center:no;"
		L"resizable:no;scroll:no;status:no;help:no;";
	G__BANNER_SOCKET__ = S;
	InterlockedExchange(&G__BANNER_CLOSING__, 0);
	G__BANNER__.THREAD = CreateThread(
		NULL,
		0,
		BANNER_THREAD,
		NULL,
		0,
		&G__BANNER__.THREAD_ID
	);
	if (!G__BANNER__.THREAD)
	{
		return ;
	}
	/* ON TOP OF EVERYTHING, BUT GIVE THE KEYBOARD BACK TO THE OLD WINDOW */
	for (I = 0; I < 60; ++I)
	{
		WINDOW = DIALOG_WINDOW(&G__BANNER__);
		if (WINDOW)
		{
			SetWindowPos(
				WINDOW,
				HWND_TOPMOST,
				0,
				0,
				0,
				0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
			);
			if (PREVIOUS && IsWindow(PREVIOUS))
			{
				SetForegroundWindow(PREVIOUS);
			}
			break ;
		}
		if (WaitForSingleObject(G__BANNER__.THREAD, 50) == WAIT_OBJECT_0)
		{
			break ;
		}
	}
}

void
	HTMLUI_BANNER_HIDE(void)
{
	HWND	WINDOW;
	int	I;

	if (!G__BANNER__.THREAD)
	{
		return ;
	}
	InterlockedExchange(&G__BANNER_CLOSING__, 1);
	for (I = 0; I < 50; ++I)
	{
		WINDOW = DIALOG_WINDOW(&G__BANNER__);
		if (WINDOW)
		{
			PostMessageW(WINDOW, WM_CLOSE, 0, 0);
		}
		if (WaitForSingleObject(G__BANNER__.THREAD, 100) == WAIT_OBJECT_0)
		{
			break ;
		}
	}
	if (WaitForSingleObject(G__BANNER__.THREAD, 0) != WAIT_OBJECT_0)
	{
		return ; /* STILL BUSY: LEAVE ITS MEMORY ALONE */
	}
	CloseHandle(G__BANNER__.THREAD);
	free(G__BANNER__.ARGS);
	memset(&G__BANNER__, 0, sizeof(G__BANNER__));
	G__BANNER_SOCKET__ = INVALID_SOCKET;
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      SESSION BANNER                      |  //\\  */
/* '""""' #==========================================================# '""""' */
