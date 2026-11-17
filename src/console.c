/******************************************************************************\
# C - CONSOLE                                    #       Maximum Tension       #
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
|*|                                 CONSOLE                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| COLOURS, BOXES, QUESTIONS AND NOTICES FOR THE BLACK WINDOW. SEE          |*|
|*| CONSOLE.H FOR THE COLOUR MARKUP.                                         |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| void SAY(const char *MARKUP);                                       : *|
|* : 3|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * struct S_TEXT, LANG()
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* **************************** [v] GLOBALS [v] ***************************** */
int			G_VT = 0;
CRITICAL_SECTION	G_CONSOLE_LOCK;
char			G_HOST_STATUS[64];
static char		G__PROMPT__[512];	/* QUESTION WAITING FOR AN ANSWER */
/* **************************** [^] GLOBALS [^] ***************************** */

void
	CONSOLE_INIT(void)
{
	InitializeCriticalSection(&G_CONSOLE_LOCK);
	G_HOST_STATUS[0] = '\0';
	G__PROMPT__[0] = '\0';
}

void
	CONSOLE_SETUP(void)
{
	HANDLE	OUTPUT;
	HANDLE	IN_HANDLE;
	DWORD	MODE;

	OUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleMode(OUTPUT, &MODE) && SetConsoleMode(
		OUTPUT,
		MODE | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING
	))
	{
		G_VT = 1;
	}
	SetConsoleOutputCP(CP_UTF8); /* TURKISH LETTERS */
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
	int		COUNT;

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

static void
	WRITE_MARKUP(const char *MARKUP)
{
	struct S_TEXT	T = {0};

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
	va_list		A;

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
	return (G_VT ? "\x1B[2J\x1B[3J\x1B[H" : "\n\n");
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
	va_list		A;

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
	const char	*P;
	const char	*Q;
	const char	*SPACE;
	const char	*CUT;
	const char	*NEXT;
	char		MARK[4];
	char		CARRY[4];
	int			COUNT;

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
	int		I;

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
