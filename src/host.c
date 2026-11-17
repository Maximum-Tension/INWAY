/******************************************************************************\
# C - HOST                                       #       Maximum Tension       #
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
|*|                                   HOST                                   |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE SHARING SIDE. ONE THREAD ACCEPTS CONNECTIONS, ONE THREAD PER         |*|
|*| CONNECTION CHECKS THE CODE AND SENDS THE SCREEN, AND ONE MORE THREAD     |*|
|*| PLAYS BACK THE MOUSE AND KEYBOARD.                                       |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| void HOST_START(void);                                              : *|
|* : 3|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * ENCODE_TILE(), JPEG_ENCODE(), CLIP_START()
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* **************************** [v] GLOBALS [v] ***************************** */
volatile LONG	G_RUNNING = 0;
int		G_WORKER = 0;
int		G_CODE = 0;
SOCKET		G_LISTEN = INVALID_SOCKET;
static volatile LONG	G__BUSY__ = 0;	/* A REQUEST OR SESSION IS OPEN */
static int		G__WRONG_IN_A_ROW__ = 0;	/* GUARDED BY G__BUSY__ */
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
	HANDLE			EVENT;
	INPUT			ACTION;

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
	HDESK			DESK;
	uint8_t			*BUFFER;
	uint32_t		CAP;
	uint32_t		SIZE;
	uint32_t		TYPE;

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
	HDC		SCREEN;
	HDC		MEMORY;
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

	memset(&CI, 0, sizeof(CI));
	CI.cbSize = sizeof(CI);
	if (!GetCursorInfo(&CI) || !(CI.flags & CURSOR_SHOWING))
	{
		return ;
	}
	if (!GetIconInfo(CI.hCursor, &II))
	{
		return ;
	}
	DrawIconEx(
		MEMORY,
		CI.ptScreenPos.x - (int)II.xHotspot,
		CI.ptScreenPos.y - (int)II.yHotspot,
		CI.hCursor,
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
	uint8_t		*CHANGED;
	uint8_t		*LOSSY;
	uint8_t		*QUIET;	/* FRAMES SINCE THE LAST CHANGE, UP TO 255 */
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
	int		ROW;
	int		END;
	int		FROM;
	int		TO;
	int		F2;
	int		T2;
	int		X;
	int		Y;
	int		W;
	int		H;
	int		TX;
	int		TY;

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
	int		BUDGET;
	int		T;

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
	int	COUNT;
	int	TX;
	int	TY;
	int	TW;
	int	TH;
	int	Y;
	int	T;
	int	CHANGED;

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
	int32_t			HELLO[3];
	struct S_CAPTURE	CAP;
	struct S_LINK		LINK;
	struct S_SESSION	SESSION;
	struct S_JPEG		JPEG;
	struct S_GRID		GRID;
	struct S_TEXT		FRAME = {0};
	struct S_TEXT		SCRATCH = {0};
	HDESK			DESK;
	HANDLE			INPUT;
	BOOL			NO_DELAY;
	int			BUFFER_SIZE;
	uint32_t		*PREV;
	uint8_t			*PACKED;
	uint32_t		TILES;
	DWORD			T0;
	DWORD			DT;
	DWORD			WAIT;
	int			W;
	int			H;
	int			COUNT;
	int			FIRST;
	int			HAVE_JPEG;

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
/*  \\//  |                      ONE CONNECTION                      |  \\//  */
/*   \/   #==========================================================#   \/   */
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
	SOCKET			S;
	struct sockaddr_in	PEER;
	int			SIZE;
	char			WHO[24];
	DWORD			TIMEOUT;
	int32_t			GOT;
	int			ANSWER;

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
	if (GOT == 0 && G_WORKER)
	{
		REPLY(S, REPLY_NEED_CODE); /* NOBODY THERE TO CLICK */
		closesocket(S);
		InterlockedExchange(&G__BUSY__, 0);
		return (0);
	}
	if (GOT == 0)
	{
		TELL("{c}", STR_N_ASKING, WHO);
		ANSWER = HTMLUI_ASK(S, WHO);
		if (ANSWER != 1)
		{
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
			InterlockedExchange(&G__BUSY__, 0);
			return (0);
		}
	}
	else if (GOT != G_CODE)
	{
		TELL("{r}", STR_N_WRONG, WHO);
		/* SLOW DOWN GUESSING, A LOT AFTER 5 WRONG CODES IN A ROW */
		Sleep(++G__WRONG_IN_A_ROW__ >= 5 ? 30000 : 2000);
		REPLY(S, REPLY_WRONG_CODE);
		closesocket(S);
		InterlockedExchange(&G__BUSY__, 0);
		return (0);
	}
	G__WRONG_IN_A_ROW__ = 0;
	EnterCriticalSection(&G_CONSOLE_LOCK);
	snprintf(G_HOST_STATUS, sizeof(G_HOST_STATUS), "%s", WHO);
	LeaveCriticalSection(&G_CONSOLE_LOCK);
	TELL("{y}", STR_N_CONTROLLING, WHO);
	if (!G_WORKER)
	{
		HTMLUI_BANNER_SHOW(WHO, S); /* "2.253 IS CONTROLLING THIS PC" */
	}
	SERVE(S, WHO);
	if (!G_WORKER)
	{
		HTMLUI_BANNER_HIDE();
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
	SOCKET		LS;
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
