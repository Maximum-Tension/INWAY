/******************************************************************************\
# C - NET                                        #       Maximum Tension       #
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
|*|                                   NET                                    |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| SOCKET HELPERS, ADDRESSES AND CODES. SEE NET.H.                          |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| int PARSE_ADDRESS(const char *INPUT, uint32_t *OUT);                : *|
|* : 3|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * SOCKET, getaddrinfo()
 */
/* **************************** [^] INCLUDES [^] **************************** */

uint32_t	G_MY_IP = 0;

#define HOME_NET	0XC0A80000U	/* 192.168.0.0 */

int
	SEND_ALL(SOCKET S, const void *BUFFER, int SIZE)
{
	const char	*P;
	int		DONE;

	P = (const char *)BUFFER;
	while (SIZE > 0)
	{
		DONE = send(S, P, SIZE, 0);
		if (DONE <= 0)
		{
			return (0);
		}
		P += DONE;
		SIZE -= DONE;
	}
	return (1);
}

int
	RECV_ALL(SOCKET S, void *BUFFER, int SIZE)
{
	char	*P;
	int	DONE;

	P = (char *)BUFFER;
	while (SIZE > 0)
	{
		DONE = recv(S, P, SIZE, 0);
		if (DONE <= 0)
		{
			return (0);
		}
		P += DONE;
		SIZE -= DONE;
	}
	return (1);
}

/* 3 = 192.168.X.X, 2 = 10.X / 172.16-31.X, 1 = OTHER, 0 = NOT A LAN ADDRESS */
static int
	LAN_RANK(uint32_t IP)
{
	uint32_t	A;
	uint32_t	B;

	A = IP >> 24;
	B = (IP >> 16) & 255;
	if (A == 0 || A == 127 || (A == 169 && B == 254))
	{
		return (0);
	}
	if (A == 192 && B == 168)
	{
		return (3);
	}
	if (A == 10 || (A == 172 && B >= 16 && B <= 31))
	{
		return (2);
	}
	return (1);
}

/*
# THE ADDRESS WINDOWS WOULD USE TO REACH THE INTERNET IS ALMOST ALWAYS THE
# REAL HOME NETWORK. NO PACKET IS SENT: A UDP "CONNECT" ONLY PICKS A ROUTE.
*/
uint32_t
	PICK_LOCAL_IP(void)
{
	SOCKET		U;
	struct sockaddr_in	TARGET;
	struct sockaddr_in	ME;
	int			SIZE;
	uint32_t		ROUTED;
	uint32_t		BEST;
	uint32_t		IP;
	int			BEST_SCORE;
	int			SCORE;
	char			NAME[256];
	struct addrinfo	HINTS;
	struct addrinfo	*LIST;
	struct addrinfo	*P;

	ROUTED = 0;
	U = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (U != INVALID_SOCKET)
	{
		memset(&TARGET, 0, sizeof(TARGET));
		TARGET.sin_family = AF_INET;
		TARGET.sin_port = htons(53);
		TARGET.sin_addr.s_addr = inet_addr("8.8.8.8");
		if (connect(U, (struct sockaddr *)&TARGET, sizeof(TARGET)) == 0)
		{
			SIZE = sizeof(ME);
			memset(&ME, 0, sizeof(ME));
			if (getsockname(U, (struct sockaddr *)&ME, &SIZE) == 0)
			{
				ROUTED = ntohl(ME.sin_addr.s_addr);
			}
		}
		closesocket(U);
	}
	BEST = 0;
	BEST_SCORE = 0;
	if (gethostname(NAME, sizeof(NAME)) == 0)
	{
		memset(&HINTS, 0, sizeof(HINTS));
		HINTS.ai_family = AF_INET;
		LIST = NULL;
		if (getaddrinfo(NAME, NULL, &HINTS, &LIST) == 0)
		{
			for (P = LIST; P; P = P->ai_next)
			{
				IP = ntohl(((struct sockaddr_in *)P->ai_addr)->sin_addr.s_addr);
				SCORE = LAN_RANK(IP);
				if (!SCORE)
				{
					continue ;
				}
				SCORE += (IP == ROUTED) ? 4 : 0;
				if (SCORE > BEST_SCORE)
				{
					BEST_SCORE = SCORE;
					BEST = IP;
				}
			}
			freeaddrinfo(LIST);
		}
	}
	if (!BEST && LAN_RANK(ROUTED))
	{
		BEST = ROUTED;
	}
	return (BEST);
}

void
	IP_FULL(uint32_t IP, char *OUTPUT, size_t CAP)
{
	snprintf(
		OUTPUT,
		CAP,
		"%u.%u.%u.%u",
		IP >> 24,
		(IP >> 16) & 255,
		(IP >> 8) & 255,
		IP & 255
	);
}

/* 192.168.2.253 -> "2.253" WHEN IT IS ON THIS PC'S NETWORK */
void
	IP_SHORT(uint32_t IP, char *OUTPUT, size_t CAP)
{
	uint32_t	BASE;

	BASE = G_MY_IP ? G_MY_IP : HOME_NET;
	if ((IP >> 16) == (BASE >> 16))
	{
		snprintf(OUTPUT, CAP, "%u.%u", (IP >> 8) & 255, IP & 255);
		return ;
	}
	IP_FULL(IP, OUTPUT, CAP);
}

/* 942853 -> "942 853" */
void
	CODE_TEXT(int CODE, char *OUTPUT, size_t CAP)
{
	snprintf(OUTPUT, CAP, "%03d %03d", CODE / 1000, CODE % 1000);
}

/*
# ACCEPTS "2.253", "2,253", "2 253", "253" OR A FULL "192.168.2.253".
# - MISSING FRONT NUMBERS COME FROM THIS PC'S ADDRESS (OR 192.168).
# - RETURNS 0 WHEN IT WORKED, 1 WHEN IT DOES NOT LOOK LIKE AN ADDRESS.
*/
int
	PARSE_ADDRESS(const char *INPUT, uint32_t *OUTPUT)
{
	char		CLEAN[64];
	uint32_t	PARTS[4];
	size_t		LEN;
	size_t		COUNT;
	size_t		K;
	const char	*P;
	uint32_t	VALUE;
	uint32_t	BASE;
	uint32_t	IP;
	int		DIGITS;

	LEN = 0;
	for (P = INPUT; *P && LEN + 1 < sizeof(CLEAN); ++P)
	{
		if (*P >= '0' && *P <= '9')
		{
			CLEAN[LEN++] = *P;
		}
		else if (*P == '.' || *P == ',' || *P == ' ' || *P == '-' || *P == '\t')
		{
			if (LEN && CLEAN[LEN - 1] != '.')
			{
				CLEAN[LEN++] = '.';
			}
		}
		else
		{
			return (1);
		}
	}
	while (LEN && CLEAN[LEN - 1] == '.')
	{
		--LEN;
	}
	CLEAN[LEN] = '\0';
	if (!LEN)
	{
		return (1);
	}
	COUNT = 0;
	VALUE = 0;
	DIGITS = 0;
	for (P = CLEAN; ; ++P)
	{
		if (*P == '.' || *P == '\0')
		{
			if (!DIGITS || DIGITS > 3 || VALUE > 255 || COUNT == 4)
			{
				return (1);
			}
			PARTS[COUNT++] = VALUE;
			VALUE = 0;
			DIGITS = 0;
			if (*P == '\0')
			{
				break ;
			}
			continue ;
		}
		VALUE = VALUE * 10 + (uint32_t)(*P - '0');
		++DIGITS;
	}
	if (COUNT < 2 && !G_MY_IP)
	{
		return (1); /* "39" ALONE NEEDS THE THIRD NUMBER TOO */
	}
	BASE = G_MY_IP ? G_MY_IP : HOME_NET;
	IP = (COUNT < 4) ? (BASE >> (8 * COUNT)) << (8 * COUNT) : 0;
	for (K = 0; K < COUNT; ++K)
	{
		IP |= PARTS[K] << (8 * (COUNT - 1 - K));
	}
	*OUTPUT = IP;
	return (0);
}

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                         MESSAGES                         |  \\//  */
/*   \/   #==========================================================#   \/   */
void
	LINK_INIT(struct S_LINK *LINK, SOCKET S)
{
	LINK->S = S;
	InitializeCriticalSection(&LINK->LOCK);
}

void
	LINK_FREE(struct S_LINK *LINK)
{
	DeleteCriticalSection(&LINK->LOCK);
	LINK->S = INVALID_SOCKET;
}

/* ONE MESSAGE: HEADER, PIECE A, PIECE B. 0 WHEN THE CONNECTION IS GONE. */
int
	LINK_SEND(
		struct S_LINK *LINK,
		uint32_t TYPE,
		const void *A,
		uint32_t LA,
		const void *B,
		uint32_t LB
	)
{
	struct S_MSG_HDR	HDR;
	int			OK;

	HDR.TYPE = TYPE;
	HDR.BYTES = LA + LB;
	EnterCriticalSection(&LINK->LOCK);
	OK = SEND_ALL(LINK->S, &HDR, sizeof(HDR));
	if (OK && LA)
	{
		OK = SEND_ALL(LINK->S, A, (int)LA);
	}
	if (OK && LB)
	{
		OK = SEND_ALL(LINK->S, B, (int)LB);
	}
	LeaveCriticalSection(&LINK->LOCK);
	return (OK);
}

/*
# READS ONE MESSAGE INTO *BUFFER, WHICH GROWS WHEN NEEDED (*CAP IS ITS SIZE).
# A MESSAGE BIGGER THAN MAX MEANS SOMETHING IS WRONG: 0 IS RETURNED.
*/
int
	RECV_MSG(
		SOCKET S,
		uint32_t *TYPE,
		uint8_t **BUFFER,
		uint32_t *SIZE,
		uint32_t *CAP,
		uint32_t MAX
	)
{
	struct S_MSG_HDR	HDR;
	uint8_t			*BIGGER;

	if (!RECV_ALL(S, &HDR, sizeof(HDR)) || HDR.BYTES > MAX)
	{
		return (0);
	}
	if (HDR.BYTES + 1 > *CAP || !*BUFFER)
	{
		BIGGER = (uint8_t *)realloc(*BUFFER, HDR.BYTES + 1);
		if (!BIGGER)
		{
			return (0);
		}
		*BUFFER = BIGGER;
		*CAP = HDR.BYTES + 1;
	}
	if (HDR.BYTES && !RECV_ALL(S, *BUFFER, (int)HDR.BYTES))
	{
		return (0);
	}
	*TYPE = HDR.TYPE;
	*SIZE = HDR.BYTES;
	return (1);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                         MESSAGES                         |  //\\  */
/* '""""' #==========================================================# '""""' */
