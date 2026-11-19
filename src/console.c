/******************************************************************************\
# C - console                                    #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/18 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# COLOURS, BOXES, QUESTIONS AND NOTICES FOR THE BLACK WINDOW. SEE CONSOLE.H FOR
# THE COLOUR MARKUP.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define WIN32_LEAN_AND_MEAN
 */
#include <windows.h>
/******* """""""""""
 * #define CP_UTF8
 * #define ENABLE_EXTENDED_FLAGS
 * #define ENABLE_PROCESSED_OUTPUT
 * #define ENABLE_QUICK_EDIT_MODE
 * #define ENABLE_VIRTUAL_TERMINAL_PROCESSING
 * #define STD_INPUT_HANDLE
 * #define STD_OUTPUT_HANDLE
 * typedef	CONSOLE_SCREEN_BUFFER_INFO;
 * typedef	COORD;
 * typedef	CRITICAL_SECTION;
 * typedef	DWORD;
 * typedef	HANDLE;
 * typedef	WCHAR;
 * typedef	WORD;
 * void		EnterCriticalSection(LPCRITICAL_SECTION);
 * void		ExitProcess(UINT);
 * BOOL		FillConsoleOutputAttribute(HANDLE, WORD, DWORD, COORD, LPDWORD);
 * BOOL		FillConsoleOutputCharacterW(HANDLE, WCHAR, DWORD, COORD, LPDWORD);
 * BOOL		GetConsoleMode(HANDLE, LPDWORD);
 * BOOL		GetConsoleScreenBufferInfo(HANDLE, PCONSOLE_SCREEN_BUFFER_INFO);
 * HANDLE	GetStdHandle(DWORD);
 * void		InitializeCriticalSection(LPCRITICAL_SECTION);
 * void		LeaveCriticalSection(LPCRITICAL_SECTION);
 * int		MultiByteToWideChar(UINT, DWORD, LPCCH, int, LPWSTR, int);
 * BOOL		SetConsoleCursorPosition(HANDLE, COORD);
 * BOOL		SetConsoleMode(HANDLE, DWORD);
 * BOOL		SetConsoleOutputCP(UINT);
 * BOOL		SetConsoleTextAttribute(HANDLE, WORD);
 * BOOL		SetConsoleTitleW(LPCWSTR);
 * BOOL		WriteConsoleW(HANDLE, const void *, DWORD, LPDWORD, LPVOID);
 * BOOL		WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
 */
#include <stdio.h>
/******* """""""""
 * #define stdin
 * #define stdout
 * int		fflush(FILE *);
 * char		*fgets(char *, int, FILE *);
 * size_t	fwrite(const void *, size_t, size_t, FILE *);
 * int		snprintf(char *, size_t, const char *, ...);
 */
#include <stdlib.h>
/******* """"""""""
 * void	free(void *);
 * void	*malloc(size_t);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * typedef	size_t;
 * void		*memcpy(void *, const void *, size_t);
 * size_t	strcspn(const char *, const char *);
 */
#include <stdarg.h>
/******* """"""""""
 * #define va_end(ap)
 * #define va_start(ap, param)
 * typedef	va_list;
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT;
 * void	TEXT_ADD(struct S_TEXT *, const char *);
 * void	TEXT_ADDF(struct S_TEXT *, const char *, ...);
 * void	TEXT_ADDN(struct S_TEXT *, const char *, size_t);
 * void	TEXT_ADDV(struct S_TEXT *, const char *, va_list);
 * void	TEXT_FREE(struct S_TEXT *);
 */
#include "console.h"
/******* """""""""""
 * #define BOX_INNER
 * CRITICAL_SECTION	G_CONSOLE_LOCK;
 * char				G_HOST_STATUS[64];
 */
/* **************************** [^] INCLUDES [^] **************************** */

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0X0004 /* OLD MINGW HEADERS */
#endif /* !ENABLE_VIRTUAL_TERMINAL_PROCESSING */

/* **************************** [v] GLOBALS [v] ***************************** */
int					G_VT = 0;
static int			G__OLD_CONSOLE__ = 0; /* A WINDOW, BUT NO VT: WINDOWS 7 */
static WORD			G__PLAIN__ = 0X07; /* THE WINDOW'S OWN COLOURS */
CRITICAL_SECTION	G_CONSOLE_LOCK;
char				G_HOST_STATUS[64];
static char			G__PROMPT__[512]; /* QUESTION WAITING FOR AN ANSWER */
/* **************************** [^] GLOBALS [^] ***************************** */

void
	CONSOLE_INIT(void)
{
	InitializeCriticalSection(&G_CONSOLE_LOCK);
	G_HOST_STATUS[0] = '\0';
	G__PROMPT__[0] = '\0';
}

/*
# WINDOWS 10 AND 11 UNDERSTAND COLOUR CODES (VT). WINDOWS 7 DOES NOT: THERE
# THE TEXT GOES OUT AS UTF-16 AND THE COLOURS ARE SET ON THE WINDOW, SO THE
# CODE PAGE IS LEFT ALONE (ITS OLD PIXEL FONT CAN NOT SHOW UTF-8).
*/
void
	CONSOLE_SETUP(void)
{
	HANDLE						OUTPUT;
	HANDLE						IN_HANDLE;
	DWORD						MODE;
	CONSOLE_SCREEN_BUFFER_INFO	INFO;

	OUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleMode(OUTPUT, &MODE))
	{
		if (SetConsoleMode(
			OUTPUT,
			MODE | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING
		))
		{
			G_VT = 1;
		}
		else
		{
			G__OLD_CONSOLE__ = 1;
			if (GetConsoleScreenBufferInfo(OUTPUT, &INFO))
			{
				G__PLAIN__ = INFO.wAttributes;
			}
		}
	}
	if (!G__OLD_CONSOLE__)
	{
		SetConsoleOutputCP(CP_UTF8); /* TURKISH LETTERS */
	}
	/* A STRAY CLICK MUST NOT FREEZE THE WINDOW IN "SELECT" MODE */
	IN_HANDLE = GetStdHandle(STD_INPUT_HANDLE);
	if (GetConsoleMode(IN_HANDLE, &MODE))
	{
		SetConsoleMode(
			IN_HANDLE,
			(MODE & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_EXTENDED_FLAGS
		);
	}
	SetConsoleTitleW(L"INWAY");
}

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      COLOUR MARKUP                       |  \\//  */
/*   \/   #==========================================================#   \/   */
static const char
	*COLOUR_CODE(char KEY)
{
	switch (KEY)
	{
		case ('c'): return ("\x1B[96m");
		case ('y'): return ("\x1B[1;93m");
		case ('Y'): return ("\x1B[33m");
		case ('g'): return ("\x1B[92m");
		case ('G'): return ("\x1B[32m");
		case ('r'): return ("\x1B[1;91m");
		case ('w'): return ("\x1B[1;97m");
		case ('d'): return ("\x1B[90m");
		case ('0'): return ("\x1B[0m");
	}
	return (NULL);
}

/* THE SAME COLOURS FOR WINDOWS 7, AS CONSOLE TEXT ATTRIBUTES */
static WORD
	COLOUR_ATTRIBUTE(char KEY)
{
	WORD	BACK;

	BACK = G__PLAIN__ & 0XF0;
	switch (KEY)
	{
		case ('c'): return (BACK | 0X0B); /* BRIGHT CYAN */
		case ('y'): return (BACK | 0X0E); /* BRIGHT YELLOW */
		case ('Y'): return (BACK | 0X06); /* YELLOW */
		case ('g'): return (BACK | 0X0A); /* BRIGHT GREEN */
		case ('G'): return (BACK | 0X02); /* GREEN */
		case ('r'): return (BACK | 0X0C); /* BRIGHT RED */
		case ('w'): return (BACK | 0X0F); /* BRIGHT WHITE */
		case ('d'): return (BACK | 0X08); /* GREY */
	}
	return (G__PLAIN__);
}

/* 1 IF P POINTS AT A COLOUR MARK LIKE "{y}" */
static int
	IS_MARK(const char *P)
{
	return (P[0] == '{' && P[1] && P[2] == '}' && COLOUR_CODE(P[1]));
}

static void
	RENDER(struct S_TEXT *OUTPUT, const char *MARKUP, int COLOURS)
{
	const char	*P;
	const char	*START;

	P = MARKUP;
	START = P;
	while (*P)
	{
		if (IS_MARK(P))
		{
			TEXT_ADDN(OUTPUT, START, (size_t)(P - START));
			if (COLOURS)
			{
				TEXT_ADD(OUTPUT, COLOUR_CODE(P[1]));
			}
			P += 3;
			START = P;
			continue ;
		}
		++P;
	}
	TEXT_ADDN(OUTPUT, START, (size_t)(P - START));
}

int
	VISIBLE_LENGTH(const char *MARKUP)
{
	const char	*P;
	int			COUNT;

	COUNT = 0;
	P = MARKUP;
	while (*P)
	{
		if (IS_MARK(P))
		{
			P += 3;
			continue ;
		}
		if ((*P & 0XC0) != 0X80) /* ONE PER UTF-8 CHARACTER */
		{
			++COUNT;
		}
		++P;
	}
	return (COUNT);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      COLOUR MARKUP                       |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                         WRITING                          |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	WRITE_RAW(const char *DATA, size_t SIZE)
{
	DWORD	DONE;

	if (!WriteFile(
		GetStdHandle(STD_OUTPUT_HANDLE),
		DATA,
		(DWORD)SIZE,
		&DONE,
		NULL
	))
	{
		fwrite(DATA, 1, SIZE, stdout);
		fflush(stdout);
	}
}

/* UTF-8 TEXT TO THE WINDOW AS UTF-16 */
static void
	WRITE_WIDE(HANDLE OUTPUT, const char *DATA, size_t SIZE)
{
	WCHAR	*WIDE;
	int		COUNT;
	DWORD	DONE;

	if (!SIZE)
	{
		return ;
	}
	COUNT = MultiByteToWideChar(CP_UTF8, 0, DATA, (int)SIZE, NULL, 0);
	WIDE = COUNT > 0 ? (WCHAR *)malloc((size_t)COUNT * sizeof(WCHAR)) : NULL;
	if (WIDE && MultiByteToWideChar(CP_UTF8, 0, DATA, (int)SIZE, WIDE, COUNT))
	{
		WriteConsoleW(OUTPUT, WIDE, (DWORD)COUNT, &DONE, NULL);
	}
	free(WIDE);
}

static void
	CLEAR_OLD_CONSOLE(HANDLE OUTPUT)
{
	CONSOLE_SCREEN_BUFFER_INFO	INFO;
	COORD						HOME;
	DWORD						CELLS;
	DWORD						DONE;

	if (!GetConsoleScreenBufferInfo(OUTPUT, &INFO))
	{
		return ;
	}
	HOME.X = 0;
	HOME.Y = 0;
	CELLS = (DWORD)INFO.dwSize.X * (DWORD)INFO.dwSize.Y;
	FillConsoleOutputCharacterW(OUTPUT, L' ', CELLS, HOME, &DONE);
	FillConsoleOutputAttribute(OUTPUT, G__PLAIN__, CELLS, HOME, &DONE);
	SetConsoleCursorPosition(OUTPUT, HOME);
}

/* WINDOWS 7: A COLOUR MARK SETS THE TEXT COLOUR, "\f" CLEARS THE WINDOW */
static void
	WRITE_OLD_CONSOLE(const char *MARKUP)
{
	HANDLE		OUTPUT;
	const char	*P;
	const char	*START;

	OUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	P = MARKUP;
	START = P;
	while (*P)
	{
		if (*P == '\f' || IS_MARK(P))
		{
			WRITE_WIDE(OUTPUT, START, (size_t)(P - START));
			if (*P == '\f')
			{
				CLEAR_OLD_CONSOLE(OUTPUT);
				P += 1;
			}
			else
			{
				SetConsoleTextAttribute(OUTPUT, COLOUR_ATTRIBUTE(P[1]));
				P += 3;
			}
			START = P;
			continue ;
		}
		++P;
	}
	WRITE_WIDE(OUTPUT, START, (size_t)(P - START));
}

static void
	WRITE_MARKUP(const char *MARKUP)
{
	struct S_TEXT	T = {0};

	if (G__OLD_CONSOLE__)
	{
		WRITE_OLD_CONSOLE(MARKUP);
		return ;
	}
	RENDER(&T, MARKUP, G_VT);
	if (T.DATA)
	{
		WRITE_RAW(T.DATA, T.LEN);
	}
	TEXT_FREE(&T);
}

void
	SAY(const char *MARKUP)
{
	EnterCriticalSection(&G_CONSOLE_LOCK);
	WRITE_MARKUP(MARKUP);
	LeaveCriticalSection(&G_CONSOLE_LOCK);
}

void
	SAYF(const char *FORMAT, ...)
{
	struct S_TEXT	T = {0};
	va_list			A;

	va_start(A, FORMAT);
	TEXT_ADDV(&T, FORMAT, A);
	va_end(A);
	if (T.DATA)
	{
		SAY(T.DATA);
	}
	TEXT_FREE(&T);
}

const char
	*SYM(const char *FANCY, const char *PLAIN)
{
	return (G_VT ? FANCY : PLAIN);
}

const char
	*CLEAR_SCREEN(void)
{
	if (G_VT)
	{
		return ("\x1B[2J\x1B[3J\x1B[H");
	}
	if (G__OLD_CONSOLE__)
	{
		return ("\f"); /* WRITE_OLD_CONSOLE CLEARS THE WINDOW */
	}
	return ("\n\n");
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                         WRITING                          |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                  QUESTIONS AND NOTICES                   |  \\//  */
/*   \/   #==========================================================#   \/   */
void
	ASK(const char *PROMPT, char *OUTPUT, size_t CAP)
{
	size_t	LEN;

	EnterCriticalSection(&G_CONSOLE_LOCK);
	snprintf(G__PROMPT__, sizeof(G__PROMPT__), "%s", PROMPT);
	WRITE_MARKUP("\n");
	WRITE_MARKUP(PROMPT);
	LeaveCriticalSection(&G_CONSOLE_LOCK);
	if (!fgets(OUTPUT, (int)CAP, stdin))
	{
		ExitProcess(0); /* THE WINDOW WAS CLOSED */
	}
	LEN = strcspn(OUTPUT, "\r\n");
	OUTPUT[LEN] = '\0';
	EnterCriticalSection(&G_CONSOLE_LOCK);
	G__PROMPT__[0] = '\0';
	LeaveCriticalSection(&G_CONSOLE_LOCK);
}

/*
# A NOTICE FROM THE SHARING SIDE (ANOTHER THREAD). IF A QUESTION IS OPEN, IT
# IS PRINTED AGAIN UNDER THE NOTICE SO IT NEVER GETS LOST.
*/
void
	NOTIFY(const char *FORMAT, ...)
{
	struct S_TEXT	T = {0};
	va_list			A;

	TEXT_ADD(&T, "\n\n");
	va_start(A, FORMAT);
	TEXT_ADDV(&T, FORMAT, A);
	va_end(A);
	TEXT_ADD(&T, "\n");
	EnterCriticalSection(&G_CONSOLE_LOCK);
	if (G__PROMPT__[0])
	{
		TEXT_ADD(&T, "\n");
		TEXT_ADD(&T, G__PROMPT__);
	}
	WRITE_MARKUP(T.DATA);
	LeaveCriticalSection(&G_CONSOLE_LOCK);
	TEXT_FREE(&T);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                  QUESTIONS AND NOTICES                   |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                          BOXES                           |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	BOX_REPEAT(struct S_TEXT *OUTPUT, const char *PIECE, int COUNT)
{
	while (COUNT-- > 0)
	{
		TEXT_ADD(OUTPUT, PIECE);
	}
}

/* ONE ROW INSIDE A BOX. ROW MUST ALREADY FIT. */
static void
	BOX_ROW(
		struct S_TEXT *OUTPUT,
		const char *ROW,
		size_t SIZE,
		const char *COLOUR,
		const char *SIDE
	)
{
	struct S_TEXT	PIECE = {0};

	TEXT_ADDN(&PIECE, ROW, SIZE);
	TEXT_ADDF(OUTPUT, "  %s%s{0}  ", COLOUR, SIDE);
	TEXT_ADD(OUTPUT, PIECE.DATA ? PIECE.DATA : "");
	TEXT_ADD(OUTPUT, "{0}");
	BOX_REPEAT(
		OUTPUT,
		" ",
		BOX_INNER - 2 - VISIBLE_LENGTH(PIECE.DATA ? PIECE.DATA : "")
	);
	TEXT_ADDF(OUTPUT, "%s%s{0}\n", COLOUR, SIDE);
	TEXT_FREE(&PIECE);
}

/*
# CUTS A LONG LINE INTO ROWS THAT FIT, AT SPACES WHEN IT CAN. A COLOUR THAT
# WAS ON WHEN A ROW ENDS IS TURNED ON AGAIN AT THE START OF THE NEXT ROW.
*/
static void
	BOX_WRAPPED(
		struct S_TEXT *OUTPUT,
		const char *LINE,
		const char *COLOUR,
		const char *SIDE
	)
{
	const int		WIDTH = BOX_INNER - 2;
	struct S_TEXT	ROW = {0};
	const char		*P;
	const char		*Q;
	const char		*SPACE;
	const char		*CUT;
	const char		*NEXT;
	char			MARK[4];
	char			CARRY[4];
	int				COUNT;

	if (!*LINE)
	{
		BOX_ROW(OUTPUT, "", 0, COLOUR, SIDE);
		return ;
	}
	CARRY[0] = '\0';
	P = LINE;
	while (*P)
	{
		COUNT = 0;
		SPACE = NULL;
		Q = P;
		while (*Q)
		{
			if (IS_MARK(Q))
			{
				Q += 3;
				continue ;
			}
			if ((*Q & 0XC0) != 0X80 && ++COUNT > WIDTH)
			{
				break ;
			}
			if (*Q == ' ')
			{
				SPACE = Q;
			}
			++Q;
		}
		if (!*Q)
		{
			CUT = Q;
			NEXT = Q;
		}
		else if (SPACE && SPACE > P)
		{
			CUT = SPACE;
			NEXT = SPACE + 1;
		}
		else
		{
			CUT = Q;
			NEXT = Q;
		}
		ROW.LEN = 0;
		TEXT_ADD(&ROW, CARRY);
		TEXT_ADDN(&ROW, P, (size_t)(CUT - P));
		BOX_ROW(OUTPUT, ROW.DATA, ROW.LEN, COLOUR, SIDE);
		/* REMEMBER THE LAST COLOUR MARK OF THIS ROW FOR THE NEXT ONE */
		for (Q = P; Q < CUT; ++Q)
		{
			if (IS_MARK(Q))
			{
				memcpy(MARK, Q, 3);
				MARK[3] = '\0';
				snprintf(CARRY, sizeof(CARRY), "%s", Q[1] == '0' ? "" : MARK);
				Q += 2;
			}
		}
		P = NEXT;
	}
	TEXT_FREE(&ROW);
}

void
	BOX(
		struct S_TEXT *OUTPUT,
		const char *TITLE,
		const char *const *LINES,
		int COUNT,
		const char *COLOUR,
		int DOUBLE_LINE
	)
{
	const char	*H;
	const char	*V;
	const char	*TL;
	const char	*TR;
	const char	*BL;
	const char	*BR;
	int			I;

	H = DOUBLE_LINE ? SYM("═", "=") : SYM("─", "-");
	V = DOUBLE_LINE ? SYM("║", "|") : SYM("│", "|");
	TL = DOUBLE_LINE ? SYM("╔", "+") : SYM("┌", "+");
	TR = DOUBLE_LINE ? SYM("╗", "+") : SYM("┐", "+");
	BL = DOUBLE_LINE ? SYM("╚", "+") : SYM("└", "+");
	BR = DOUBLE_LINE ? SYM("╝", "+") : SYM("┘", "+");
	TEXT_ADDF(OUTPUT, "  %s%s", COLOUR, TL);
	if (!TITLE || !*TITLE)
	{
		BOX_REPEAT(OUTPUT, H, BOX_INNER);
	}
	else
	{
		TEXT_ADDF(OUTPUT, "%s {w}%s{0}%s ", H, TITLE, COLOUR);
		BOX_REPEAT(OUTPUT, H, BOX_INNER - 3 - VISIBLE_LENGTH(TITLE));
	}
	TEXT_ADDF(OUTPUT, "%s{0}\n", TR);
	for (I = 0; I < COUNT; ++I)
	{
		BOX_WRAPPED(OUTPUT, LINES[I], COLOUR, V);
	}
	TEXT_ADDF(OUTPUT, "  %s%s", COLOUR, BL);
	BOX_REPEAT(OUTPUT, H, BOX_INNER);
	TEXT_ADDF(OUTPUT, "%s{0}\n", BR);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                          BOXES                           |  //\\  */
/* '""""' #==========================================================# '""""' */
