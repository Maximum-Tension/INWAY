/******************************************************************************\
# C - MAIN                                       #       Maximum Tension       #
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
|*| INWAY: A SMALL REMOTE DESKTOP FOR WINDOWS PCS ON THE SAME LOCAL NETWORK. |*|
|*| ONE EXE IS BOTH SIDES. OPENED NORMALLY IT SHARES THIS PC (ADDRESS AND    |*|
|*| CODE ON THE SCREEN) AND ASKS RIGHT AWAY FOR THE ADDRESS OF ANOTHER PC TO |*|
|*| CONTROL. -C CONNECTS ONCE AND EXITS WHEN THAT SESSION ENDS.              |*|
|*| THIS FILE READS THE COMMAND LINE, PICKS THE MODE AND DRAWS THE MAIN      |*|
|*| SCREEN. EVERYTHING ELSE LIVES IN THE MODULES LISTED IN INWAY.H.          |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| INWAY.exe                        share + connect                   : *|
|* :  3| INWAY.exe -p 938572              fixed code for this PC            : *|
|* :  4| INWAY.exe -c 2.253 948248        connect once with a code          : *|
|* :  5| INWAY.exe -c 2.253               connect once, they click Accept   : *|
|* :  6| INWAY.exe -install -p 938572     always on,                        : *|
|* :  7|     also on the lock screen                                        : *|
|* :  8| INWAY.exe -uninstall             remove that again                 : *|
|* :  9| INWAY.exe -removeold             remove the old CONNECT tool       : *|
|* : 10| INWAY.exe -lang tr               language: en or tr                : *|
|* : 11| INWAY.exe -noupdate              do not look for a new version     : *|
|* : 12|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * ALL INWAY MODULES
 */
#include <bcrypt.h>
/******* """"""""""
 * BCryptGenRandom()
 */
/* **************************** [^] INCLUDES [^] **************************** */

struct	S_OPTIONS
{
	char	ADDRESS[64];	/* -C */
	char	CONNECT_CODE[8];	/* -C ADDRESS [CODE] */
	char	CODE[8];		/* -P */
	char	LANGUAGE[8];	/* -LANG */
	int	CONNECT;
	int	INSTALL;
	int	UNINSTALL;
	int	REMOVE_OLD;
	int	SERVICE;
	int	WORKER;
	int	NO_UPDATE;
	int	UPDATE_FINISH;
	int	HELP;
};

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                         START UP                         |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	DPI_AWARE(void)
{
	typedef BOOL	(WINAPI *T_SET_CONTEXT)(HANDLE CONTEXT);
	HMODULE		USER;
	T_SET_CONTEXT	SET;

	USER = GetModuleHandleW(L"user32.dll");
	SET = USER ? (T_SET_CONTEXT)(void *)GetProcAddress(
		USER,
		"SetProcessDpiAwarenessContext"
	) : NULL;
	if (!SET || !SET((HANDLE)-4)) /* -4 = PER MONITOR AWARE V2 */
	{
		SetProcessDPIAware();
	}
}

/* A RANDOM CODE FROM 100000 TO 999999 */
static int
	RANDOM_CODE(void)
{
	uint32_t	R;

	if (BCryptGenRandom(
		NULL,
		(PUCHAR)&R,
		sizeof(R),
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	))
	{
		srand((unsigned)time(NULL) ^ GetTickCount() ^ GetCurrentProcessId());
		R = ((uint32_t)rand() << 15) | (uint32_t)rand();
	}
	return ((int)(R % 900000) + 100000);
}

/* READS "948248" OR "948 248" (TWO WORDS) AFTER ARGV[*I] */
static void
	TAKE_CODE(int ARGC, char **ARGV, int *I, char *OUTPUT)
{
	size_t	LEN;
	size_t	K;
	char	*A;

	LEN = 0;
	OUTPUT[0] = '\0';
	while (*I + 1 < ARGC && LEN < 6)
	{
		A = ARGV[*I + 1];
		if (!A[0] || strspn(A, "0123456789") != strlen(A) ||
			LEN + strlen(A) > 6)
		{
			break ;
		}
		for (K = 0; A[K]; ++K)
		{
			OUTPUT[LEN++] = A[K];
		}
		OUTPUT[LEN] = '\0';
		++*I;
	}
}

static int
	IS_OPTION(const char *ARG, const char *NAME)
{
	if (ARG[0] == '/' || (ARG[0] == '-' && ARG[1] == '-'))
	{
		++ARG;
	}
	return (ARG[0] == '-' && _stricmp(ARG + 1, NAME) == 0);
}

static void
	READ_OPTIONS(
		int ARGC,
		char **ARGV,
		struct S_OPTIONS *O,
		struct S_TEXT *ERRORS
	)
{
	int	I;

	memset(O, 0, sizeof(*O));
	for (I = 1; I < ARGC; ++I)
	{
		if (IS_OPTION(ARGV[I], "p"))
		{
			TAKE_CODE(ARGC, ARGV, &I, O->CODE);
			if (strlen(O->CODE) != 6 || atoi(O->CODE) == 0)
			{
				TEXT_ADDF(ERRORS, "  {r}%s{0}\n", LANG(STR_E_P));
				O->CODE[0] = '\0';
			}
		}
		else if (IS_OPTION(ARGV[I], "c"))
		{
			if (I + 1 >= ARGC)
			{
				TEXT_ADDF(ERRORS, "  {r}%s{0}\n", LANG(STR_E_C));
				continue ;
			}
			O->CONNECT = 1;
			snprintf(O->ADDRESS, sizeof(O->ADDRESS), "%s", ARGV[++I]);
			TAKE_CODE(ARGC, ARGV, &I, O->CONNECT_CODE);
		}
		else if (IS_OPTION(ARGV[I], "lang") && I + 1 < ARGC)
		{
			snprintf(O->LANGUAGE, sizeof(O->LANGUAGE), "%s", ARGV[++I]);
		}
		else if (IS_OPTION(ARGV[I], "install"))
		{
			O->INSTALL = 1;
		}
		else if (IS_OPTION(ARGV[I], "uninstall"))
		{
			O->UNINSTALL = 1;
		}
		else if (IS_OPTION(ARGV[I], "removeold"))
		{
			O->REMOVE_OLD = 1;
		}
		else if (IS_OPTION(ARGV[I], "service"))
		{
			O->SERVICE = 1;
		}
		else if (IS_OPTION(ARGV[I], "worker"))
		{
			O->WORKER = 1;
		}
		else if (IS_OPTION(ARGV[I], "noupdate"))
		{
			O->NO_UPDATE = 1;
		}
		else if (IS_OPTION(ARGV[I], "update-finish"))
		{
			O->UPDATE_FINISH = 1;
		}
		else if (IS_OPTION(ARGV[I], "h") || IS_OPTION(ARGV[I], "?") ||
			IS_OPTION(ARGV[I], "help"))
		{
			O->HELP = 1;
		}
		else
		{
			TEXT_ADD(ERRORS, "  {r}");
			TEXT_ADDF(ERRORS, LANG(STR_E_UNKNOWN), ARGV[I]);
			TEXT_ADD(ERRORS, "{0}\n");
		}
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                         START UP                         |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                         SCREENS                          |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	TITLE_BOX(struct S_TEXT *OUTPUT, const char *TITLE)
{
	struct S_TEXT	LINE = {0};
	const char	*LINES[1];
	int		PAD;

	PAD = (BOX_INNER - 4 - VISIBLE_LENGTH(TITLE)) / 2;
	while (PAD-- > 0)
	{
		TEXT_ADD(&LINE, " ");
	}
	TEXT_ADD(&LINE, TITLE);
	LINES[0] = LINE.DATA;
	BOX(OUTPUT, "", LINES, 1, "{c}", 1);
	TEXT_FREE(&LINE);
}

static void
	DRAW_HOME(void)
{
	struct S_TEXT	OUTPUT = {0};
	struct S_TEXT	L[8];
	const char	*LINES[8];
	char		ADDRESS[24];
	char		CODE[16];
	char		STATUS[64];
	int		COUNT;
	int		I;

	memset(L, 0, sizeof(L));
	EnterCriticalSection(&G_CONSOLE_LOCK);
	snprintf(STATUS, sizeof(STATUS), "%s", G_HOST_STATUS);
	LeaveCriticalSection(&G_CONSOLE_LOCK);
	TEXT_ADD(&OUTPUT, CLEAR_SCREEN());
	TEXT_ADD(&OUTPUT, "\n");
	TITLE_BOX(&OUTPUT, "{w}I N W A Y{0}   {d}v" INWAY_VERSION_TEXT "{0}");
	TEXT_ADD(&OUTPUT, "\n");
	if (G_MY_IP)
	{
		IP_SHORT(G_MY_IP, ADDRESS, sizeof(ADDRESS));
	}
	COUNT = 0;
	if (G_SVC_RUNNING || G_LISTEN != INVALID_SOCKET)
	{
		if (G_SVC_RUNNING && strlen(G_SVC_CODE) == 6)
		{
			CODE_TEXT(atoi(G_SVC_CODE), CODE, sizeof(CODE));
		}
		else if (!G_SVC_RUNNING)
		{
			CODE_TEXT(G_CODE, CODE, sizeof(CODE));
		}
		else
		{
			snprintf(CODE, sizeof(CODE), "%s", "------");
		}
		TEXT_ADD(&L[COUNT++], "");
		TEXT_ADDF(&L[COUNT++], "%-9s {y}%s{0}", LANG(STR_ADDRESS),
			G_MY_IP ? ADDRESS : LANG(STR_NO_NETWORK));
		TEXT_ADDF(&L[COUNT++], "%-9s {y}%s{0}", LANG(STR_CODE),
			(G_SVC_RUNNING && strlen(G_SVC_CODE) != 6) ?
			LANG(STR_SVC_CODE_UNKNOWN) : CODE);
		TEXT_ADD(&L[COUNT++], "");
		TEXT_ADDF(&L[COUNT++], "{d}%s{0}",
			LANG(G_SVC_RUNNING ? STR_SVC_HINT : STR_SHARE_HINT));
		if (STATUS[0])
		{
			TEXT_ADDF(&L[COUNT], "{y}%s ", SYM("●", "*"));
			TEXT_ADDF(&L[COUNT], LANG(STR_STATUS_CONTROLLING), STATUS);
			TEXT_ADD(&L[COUNT++], "{0}");
		}
	}
	else
	{
		TEXT_ADDF(&L[COUNT++], "{r}%s{0}", LANG(STR_SHARE_OFF));
		TEXT_ADD(&L[COUNT++], LANG(STR_SHARE_OFF_HINT));
	}
	for (I = 0; I < COUNT; ++I)
	{
		LINES[I] = L[I].DATA ? L[I].DATA : "";
	}
	BOX(&OUTPUT, LANG(STR_SHARE_TITLE), LINES, COUNT, "{Y}", 0);
	TEXT_ADD(&OUTPUT, "\n");
	for (I = 0; I < COUNT; ++I)
	{
		TEXT_FREE(&L[I]);
	}
	TEXT_ADD(&L[0], LANG(STR_CONNECT_HINT));
	TEXT_ADDF(&L[1], "{d}%s{0}", LANG(STR_EXAMPLE));
	LINES[0] = L[0].DATA;
	LINES[1] = L[1].DATA;
	BOX(&OUTPUT, LANG(STR_CONNECT_TITLE), LINES, 2, "{G}", 0);
	TEXT_ADDF(&OUTPUT, "  {d}%s{0}\n", LANG(STR_CLOSING_STOPS));
	SAY(OUTPUT.DATA);
	TEXT_FREE(&L[0]);
	TEXT_FREE(&L[1]);
	TEXT_FREE(&OUTPUT);
}

/* A RED BOX WITH ONE "%s" SENTENCE (AND OPTIONAL MORE LINES) */
static void
	SHOW_RESULT_BOX(
		struct S_TEXT *OUTPUT,
		enum E_STR TITLE,
		enum E_STR TEXT,
		const char *LABEL,
		const char *const *MORE,
		int MORE_COUNT
	)
{
	struct S_TEXT	FIRST = {0};
	const char	*LINES[6];
	int		I;

	TEXT_ADDF(&FIRST, LANG(TEXT), LABEL);
	LINES[0] = FIRST.DATA;
	for (I = 0; I < MORE_COUNT && I < 5; ++I)
	{
		LINES[I + 1] = MORE[I];
	}
	BOX(
		OUTPUT,
		LANG(TITLE),
		LINES,
		1 + (MORE_COUNT < 5 ? MORE_COUNT : 5),
		"{r}",
		0
	);
	TEXT_FREE(&FIRST);
}

static void
	SHOW_RESULT(struct S_TEXT *OUTPUT, enum E_RESULT R, const char *LABEL)
{
	const char	*CHECKS[3];

	switch (R)
	{
		case (CR_ENDED):
			TEXT_ADDF(OUTPUT, "  {g}%s ", SYM("●", "*"));
			TEXT_ADDF(OUTPUT, LANG(STR_R_ENDED), LABEL);
			TEXT_ADD(OUTPUT, "{0}\n");
			return ;
		case (CR_WRONG_CODE):
			SHOW_RESULT_BOX(OUTPUT, STR_R_WRONG_T, STR_R_WRONG, LABEL, NULL, 0);
			return ;
		case (CR_NEED_CODE):
			SHOW_RESULT_BOX(
				OUTPUT,
				STR_R_NEED_T,
				STR_NEED_CODE_RETRY,
				LABEL,
				NULL,
				0
			);
			return ;
		case (CR_DECLINED):
			SHOW_RESULT_BOX(
				OUTPUT,
				STR_R_DECLINED_T,
				STR_R_DECLINED,
				LABEL,
				NULL,
				0
			);
			return ;
		case (CR_NO_ANSWER):
			SHOW_RESULT_BOX(
				OUTPUT,
				STR_R_NO_ANSWER_T,
				STR_R_NO_ANSWER,
				LABEL,
				NULL,
				0
			);
			return ;
		case (CR_BUSY):
			SHOW_RESULT_BOX(OUTPUT, STR_R_BUSY_T, STR_R_BUSY, LABEL, NULL, 0);
			return ;
		case (CR_NO_PC):
			CHECKS[0] = LANG(STR_R_NO_PC_1);
			CHECKS[1] = LANG(STR_R_NO_PC_2);
			CHECKS[2] = LANG(STR_R_NO_PC_3);
			SHOW_RESULT_BOX(
				OUTPUT,
				STR_R_NO_PC_T,
				STR_R_NO_PC,
				LABEL,
				CHECKS,
				3
			);
			return ;
		case (CR_VERSION):
		case (CR_DROPPED):
			SHOW_RESULT_BOX(
				OUTPUT,
				STR_R_NO_PC_T,
				STR_R_DROPPED,
				LABEL,
				NULL,
				0
			);
			return ;
		default:
			SHOW_RESULT_BOX(OUTPUT, STR_R_NO_PC_T, STR_R_ERROR, LABEL, NULL, 0);
			return ;
	}
}

static void
	PRESS_ENTER(enum E_STR ID)
{
	char	LINE[16];
	struct S_TEXT	T = {0};

	TEXT_ADDF(&T, "  {d}%s{0}", LANG(ID));
	ASK(T.DATA, LINE, sizeof(LINE));
	TEXT_FREE(&T);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                         SCREENS                          |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                          MODES                           |  \\//  */
/*   \/   #==========================================================#   \/   */
/* -INSTALL, -UNINSTALL, -REMOVEOLD: ASK FOR ADMIN RIGHTS, THEN DO IT */
static int
	ADMIN_JOB(const struct S_OPTIONS *O, const char *ERRORS)
{
	struct S_TEXT	OUTPUT = {0};
	struct S_TEXT	L[4];
	const char	*LINES[4];
	char		PARAMETERS[64];
	char		EXTRA[16];
	char		ADDRESS[24];
	char		CODE[16];
	int		OK;
	int		I;

	if (O->INSTALL && !O->CODE[0])
	{
		SAYF("\n%s  {r}%s{0}\n", ERRORS, LANG(STR_I_NEEDS_CODE));
		PRESS_ENTER(STR_PRESS_ENTER_CLOSE);
		return (1);
	}
	if (!SERVICE_IS_ADMIN())
	{
		SAYF("\n  {y}%s{0}\n", LANG(STR_I_PERMISSION));
		if (O->INSTALL)
		{
			snprintf(PARAMETERS, sizeof(PARAMETERS), "-install -p %s", O->CODE);
		}
		else
		{
			snprintf(PARAMETERS, sizeof(PARAMETERS), "%s",
				O->UNINSTALL ? "-uninstall" : "-removeold");
		}
		if (O->LANGUAGE[0])
		{
			snprintf(EXTRA, sizeof(EXTRA), " -lang %.2s", O->LANGUAGE);
			strcat(PARAMETERS, EXTRA);
		}
		if (!SERVICE_RELAUNCH_AS_ADMIN(PARAMETERS))
		{
			SAYF("  {r}%s{0}\n", LANG(STR_I_NO_PERMISSION));
			PRESS_ENTER(STR_PRESS_ENTER_CLOSE);
			return (1);
		}
		return (0);
	}
	SAY("\n");
	TITLE_BOX(&OUTPUT, O->INSTALL ? LANG(STR_I_TITLE_INSTALL) :
		O->UNINSTALL ? LANG(STR_I_TITLE_REMOVE) : LANG(STR_I_TITLE_OLD));
	TEXT_ADD(&OUTPUT, "\n");
	SAY(OUTPUT.DATA);
	OUTPUT.LEN = 0;
	if (O->INSTALL)
	{
		OK = SERVICE_INSTALL(O->CODE);
	}
	else if (O->UNINSTALL)
	{
		OK = SERVICE_UNINSTALL();
	}
	else
	{
		OK = SERVICE_REMOVE_OLD();
	}
	memset(L, 0, sizeof(L));
	if (OK && O->INSTALL)
	{
		G_MY_IP = PICK_LOCAL_IP();
		IP_SHORT(G_MY_IP, ADDRESS, sizeof(ADDRESS));
		CODE_TEXT(atoi(O->CODE), CODE, sizeof(CODE));
		TEXT_ADD(&L[0], LANG(STR_I_DONE));
		TEXT_ADDF(
			&L[1],
			"%-9s {y}%s{0}",
			LANG(STR_ADDRESS),
			G_MY_IP ? ADDRESS : "?"
		);
		TEXT_ADDF(&L[2], "%-9s {y}%s{0}", LANG(STR_CODE), CODE);
		TEXT_ADDF(&L[3], "{d}%s{0}", LANG(STR_I_REMOVE_HINT));
		for (I = 0; I < 4; ++I)
		{
			LINES[I] = L[I].DATA;
		}
		BOX(&OUTPUT, LANG(STR_I_DONE_T), LINES, 4, "{G}", 0);
	}
	else if (OK)
	{
		LINES[0] = LANG(O->UNINSTALL ? STR_I_REMOVED : STR_I_OLD_REMOVED);
		BOX(&OUTPUT, LANG(STR_I_DONE_T), LINES, 1, "{G}", 0);
	}
	if (OUTPUT.DATA)
	{
		SAY("\n");
		SAY(OUTPUT.DATA);
	}
	for (I = 0; I < 4; ++I)
	{
		TEXT_FREE(&L[I]);
	}
	TEXT_FREE(&OUTPUT);
	PRESS_ENTER(STR_PRESS_ENTER_CLOSE);
	return (OK ? 0 : 1);
}

/* -C: CONNECT ONCE. WHEN THAT SESSION ENDS, INWAY ENDS. */
static int
	CONNECT_ONCE(const struct S_OPTIONS *O)
{
	struct S_TEXT	OUTPUT = {0};
	enum E_RESULT	R;
	uint32_t	IP;
	char		LABEL[24];

	if (PARSE_ADDRESS(O->ADDRESS, &IP) != 0)
	{
		SAYF("\n  {r}%s{0}\n", LANG(STR_BAD_ADDRESS));
		PRESS_ENTER(STR_PRESS_ENTER_CLOSE);
		return (1);
	}
	IP_SHORT(IP, LABEL, sizeof(LABEL));
	SAY("\n  {c}");
	SAYF(LANG(O->CONNECT_CODE[0] ? STR_CONNECTING : STR_WAITING), LABEL);
	SAY("{0}\n");
	R = VIEWER_CONNECT(
		IP,
		O->CONNECT_CODE[0] ? atoi(O->CONNECT_CODE) : 0,
		LABEL
	);
	if (R == CR_ENDED)
	{
		return (0);
	}
	SHOW_RESULT(&OUTPUT, R, LABEL);
	SAY("\n");
	SAY(OUTPUT.DATA);
	TEXT_FREE(&OUTPUT);
	PRESS_ENTER(STR_PRESS_ENTER_CLOSE);
	return (1);
}

/* THE NORMAL SCREEN: SHARE THIS PC, ASK FOR ANOTHER ONE */
static int
	INTERACTIVE(const char *ERRORS)
{
	struct S_TEXT	NOTICE = {0};
	struct S_TEXT	Q = {0};
	enum E_RESULT	R;
	uint32_t	IP;
	char		INPUT[128];
	char		DIGITS[8];
	char		LABEL[24];
	const char	*ARROW;
	size_t		K;
	size_t		N;
	int		TYPED;

	TEXT_ADD(&NOTICE, ERRORS);
	ARROW = SYM("►", ">");
	for (;;)
	{
		DRAW_HOME();
		if (NOTICE.LEN)
		{
			SAY("\n");
			SAY(NOTICE.DATA);
			NOTICE.LEN = 0;
		}
		/* 1) THE ADDRESS */
		for (;;)
		{
			Q.LEN = 0;
			TEXT_ADDF(&Q, "  {g}%s{0} {w}%s{0} ", ARROW, LANG(STR_ASK_ADDRESS));
			ASK(Q.DATA, INPUT, sizeof(INPUT));
			if (!INPUT[strspn(INPUT, " \t")])
			{
				continue ;
			}
			if (PARSE_ADDRESS(INPUT, &IP) != 0)
			{
				SAYF("  {r}%s{0}\n", LANG(STR_BAD_ADDRESS));
				continue ;
			}
			if (IP == G_MY_IP)
			{
				SAYF("  {r}%s{0}\n", LANG(STR_SELF_ADDRESS));
				continue ;
			}
			break ;
		}
		IP_SHORT(IP, LABEL, sizeof(LABEL));
		/* 2) THE CODE, OR NOTHING TO ASK THEM */
		for (;;)
		{
			SAY("\n  {d}");
			SAYF(LANG(STR_CODE_HINT), LABEL);
			SAY("{0}\n");
			Q.LEN = 0;
			TEXT_ADDF(&Q, "  {g}%s{0} {w}%s{0} ", ARROW, LANG(STR_ASK_CODE));
			ASK(Q.DATA, INPUT, sizeof(INPUT));
			N = 0;
			TYPED = 0;
			for (K = 0; INPUT[K]; ++K)
			{
				if (INPUT[K] != ' ' && INPUT[K] != '\t')
				{
					TYPED = 1;
				}
				if (INPUT[K] >= '0' && INPUT[K] <= '9' && N < 7)
				{
					DIGITS[N++] = INPUT[K];
				}
			}
			DIGITS[N] = '\0';
			if (TYPED && N != 6)
			{
				SAYF("  {r}%s{0}\n", LANG(STR_CODE_6));
				continue ;
			}
			SAY("\n  {c}");
			SAYF(LANG(N ? STR_CONNECTING : STR_WAITING), LABEL);
			SAY("{0}\n");
			R = VIEWER_CONNECT(IP, N ? atoi(DIGITS) : 0, LABEL);
			if (G_QUIT)
			{
				return (0); /* CTRL + C */
			}
			if (R == CR_WRONG_CODE)
			{
				SAYF("  {r}%s{0}\n", LANG(STR_WRONG_CODE_RETRY));
				continue ;
			}
			if (R == CR_NEED_CODE)
			{
				SAY("  {r}");
				SAYF(LANG(STR_NEED_CODE_RETRY), LABEL);
				SAY("{0}\n");
				continue ;
			}
			break ;
		}
		/* 3) WHAT HAPPENED */
		SHOW_RESULT(&NOTICE, R, LABEL);
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                          MODES                           |  //\\  */
/* '""""' #==========================================================# '""""' */

int
	main(int ARGC, char **ARGV)
{
	struct S_OPTIONS	O;
	struct S_TEXT	ERRORS = {0};
	WSADATA		WD;
	int		RESULT;

	DPI_AWARE();
	CONSOLE_INIT();
	LANG_INIT(NULL);
	READ_OPTIONS(ARGC, ARGV, &O, &ERRORS);
	if (O.LANGUAGE[0])
	{
		LANG_INIT(O.LANGUAGE);
		ERRORS.LEN = 0; /* READ AGAIN, NOW IN THE CHOSEN LANGUAGE */
		READ_OPTIONS(ARGC, ARGV, &O, &ERRORS);
	}
	/* BACKGROUND MODES: NO WINDOW OF THEIR OWN */
	if (O.SERVICE)
	{
		return (SERVICE_RUN(O.CODE));
	}
	if (WSAStartup(MAKEWORD(2, 2), &WD) != 0)
	{
		return (1);
	}
	if (O.WORKER)
	{
		if (!O.CODE[0])
		{
			return (1);
		}
		G_CODE = atoi(O.CODE);
		G_MY_IP = PICK_LOCAL_IP();
		return (SERVICE_WORKER());
	}
	CONSOLE_SETUP();
	if (O.INSTALL || O.UNINSTALL || O.REMOVE_OLD)
	{
		RESULT = ADMIN_JOB(&O, ERRORS.DATA ? ERRORS.DATA : "");
		TEXT_FREE(&ERRORS);
		WSACleanup();
		return (RESULT);
	}
	if (O.UPDATE_FINISH)
	{
		UPDATE_FINISH();
	}
	else if (!O.NO_UPDATE)
	{
		UPDATE_CHECK(); /* ENDS THIS PROCESS WHEN A NEWER ONE TAKES OVER */
	}
	VIEWER_INIT();
	G_MY_IP = PICK_LOCAL_IP();
	G_CODE = O.CODE[0] ? atoi(O.CODE) : RANDOM_CODE();
	if (O.CONNECT)
	{
		if (ERRORS.LEN)
		{
			SAY("\n");
			SAY(ERRORS.DATA);
		}
		RESULT = CONNECT_ONCE(&O);
		TEXT_FREE(&ERRORS);
		WSACleanup();
		return (RESULT);
	}
	SERVICE_READ_INFO(); /* BACKGROUND SHARING ON? THEN IT DOES THE SHARING */
	if (!G_SVC_RUNNING)
	{
		HOST_START();
	}
	if (O.HELP)
	{
		SAYF("\n%s\n", LANG(STR_HELP));
		PRESS_ENTER(STR_PRESS_ENTER);
	}
	RESULT = INTERACTIVE(ERRORS.DATA ? ERRORS.DATA : "");
	TEXT_FREE(&ERRORS);
	WSACleanup();
	return (RESULT);
}
