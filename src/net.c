/******************************************************************************\
# C - net                                        #       Maximum Tension       #
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
# SOCKET HELPERS, ADDRESSES AND CODES. SEE NET.H.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define INWAY_PORT
 * #define WIN32_LEAN_AND_MEAN
 * struct S_MSG_HDR;
 */
#include <winsock2.h>
/******* """"""""""""
 * #define AF_INET
 * #define INVALID_SOCKET
 * #define IPPROTO_UDP
 * #define SOCK_DGRAM
 * #define SOCK_STREAM
 * typedef			SOCKET;
 * struct sockaddr;
 * struct sockaddr_in;
 * typedef			u_short;
 * int				closesocket(SOCKET);
 * int				connect(SOCKET, const struct sockaddr *, int);
 * int				gethostname(char *, int);
 * int				getsockname(SOCKET, struct sockaddr *, int *);
 * u_short			htons(u_short);
 * unsigned long	inet_addr(const char *);
 * u_long			ntohl(u_long);
 * u_short			ntohs(u_short);
 * int				recv(SOCKET, char *, int, int);
 * int				send(SOCKET, const char *, int, int);
 * SOCKET			socket(int, int, int);
 */
#include <ws2tcpip.h>
/******* """"""""""""
 * struct addrinfo;
 * void	freeaddrinfo(LPADDRINFO);
 * int	getaddrinfo(
 * 	const char *,
 * 	const char *,
 * 	const struct addrinfo *,
 * 	struct addrinfo **
 * );
 */
#include <windows.h>
/******* """""""""""
 * #define FALSE
 * #define NO_ERROR
 * #define WINAPI
 * typedef	BOOL;
 * typedef	DWORD;
 * typedef	HMODULE;
 * typedef	PDWORD;
 * typedef	PVOID;
 * typedef	ULONG;
 * void		DeleteCriticalSection(LPCRITICAL_SECTION);
 * void		EnterCriticalSection(LPCRITICAL_SECTION);
 * BOOL		FreeLibrary(HMODULE);
 * FARPROC	GetProcAddress(HMODULE, LPCSTR);
 * void		InitializeCriticalSection(LPCRITICAL_SECTION);
 * void		LeaveCriticalSection(LPCRITICAL_SECTION);
 * HMODULE	LoadLibraryW(LPCWSTR);
 */
#include <iphlpapi.h>
/******* """"""""""""
 * #define MIB_TCP_STATE_ESTAB
 * enum _TCP_TABLE_CLASS;
 * typedef	MIB_TCPTABLE_OWNER_PID;
 * typedef	TCP_TABLE_CLASS;
 */
#include <stdio.h>
/******* """""""""
 * int	snprintf(char *, size_t, const char *, ...);
 */
#include <stdlib.h>
/******* """"""""""
 * void	free(void *);
 * void	*malloc(size_t);
 * void	*realloc(void *, size_t);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * typedef	size_t;
 * void		*memcpy(void *, const void *, size_t);
 * void		*memset(void *, int, size_t);
 */
#include <stdint.h>
/******* """"""""""
 * typedef	uint32_t;
 * typedef	uint8_t;
 */
#include "net.h"
/******* """""""
 * struct S_LINK;
 */
/* **************************** [^] INCLUDES [^] **************************** */

uint32_t	G_MY_IP = 0;

#define HOME_NET 0XC0A80000U /* 192.168.0.0 */

int
	SEND_ALL(SOCKET S, const void *BUFFER, int SIZE)
{
	const char	*P;
	int			DONE;

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
	int		DONE;

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
	SOCKET				U;
	struct sockaddr_in	TARGET;
	struct sockaddr_in	ME;
	int					SIZE;
	uint32_t			ROUTED;
	uint32_t			BEST;
	uint32_t			IP;
	int					BEST_SCORE;
	int					SCORE;
	char				NAME[256];
	struct addrinfo		HINTS;
	struct addrinfo		*LIST;
	struct addrinfo		*P;

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
/*
# A PC NAME ("OFIS-PC", "OFIS-PC.LOCAL") INSTEAD OF AN ADDRESS: ANYTHING WITH
# A LETTER IN IT. 1 = IT IS A NAME, WRITTEN WITHOUT SPACES TO OUTPUT.
*/
int
	ADDRESS_NAME(const char *INPUT, char *OUTPUT, size_t CAP)
{
	const char	*P;
	size_t		LEN;
	int			LETTER;

	while (*INPUT == ' ' || *INPUT == '\t')
	{
		++INPUT;
	}
	LEN = 0;
	LETTER = 0;
	for (P = INPUT; *P && *P != ' ' && *P != '\t'; ++P)
	{
		if ((*P >= 'A' && *P <= 'Z') || (*P >= 'a' && *P <= 'z'))
		{
			LETTER = 1;
		}
		else if (!((*P >= '0' && *P <= '9') || *P == '-' || *P == '.' ||
			*P == '_'))
		{
			return (0);
		}
		++LEN;
	}
	if (!LETTER || !LEN || LEN > 63 || LEN >= CAP)
	{
		return (0);
	}
	memcpy(OUTPUT, INPUT, LEN);
	OUTPUT[LEN] = '\0';
	return (1);
}

/*
# WINDOWS FINDS THE NAME: FIRST THE ROUTER (DNS), THEN THE PCS AROUND (MDNS,
# LLMNR, NETBIOS). A PC WITH MORE THAN ONE ADDRESS: THE HOME NETWORK ONE.
*/
static int
	RESOLVE_NAME(const char *NAME, uint32_t *OUTPUT)
{
	struct addrinfo	HINTS;
	struct addrinfo	*LIST;
	struct addrinfo	*P;
	uint32_t		IP;
	int				BEST;
	int				RANK;

	memset(&HINTS, 0, sizeof(HINTS));
	HINTS.ai_family = AF_INET;
	HINTS.ai_socktype = SOCK_STREAM;
	LIST = NULL;
	if (getaddrinfo(NAME, NULL, &HINTS, &LIST) != 0 || !LIST)
	{
		return (2);
	}
	BEST = 0;
	for (P = LIST; P; P = P->ai_next)
	{
		IP = ntohl(((struct sockaddr_in *)P->ai_addr)->sin_addr.s_addr);
		RANK = LAN_RANK(IP);
		if (RANK > BEST)
		{
			BEST = RANK;
			*OUTPUT = IP;
		}
	}
	freeaddrinfo(LIST);
	return (BEST ? 0 : 2);
}

/* 0 = OK, 1 = NOT AN ADDRESS OR A NAME, 2 = NO PC WITH THAT NAME WAS FOUND */
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
	int			DIGITS;

	if (ADDRESS_NAME(INPUT, CLEAN, sizeof(CLEAN)))
	{
		return (RESOLVE_NAME(CLEAN, OUTPUT));
	}
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
	int					OK;

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
	uint8_t				*BIGGER;

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

/* 1 WHILE SOMEONE IS CONNECTED TO THIS PC (A TCP SESSION ON INWAY_PORT) */
int
	SESSION_ACTIVE(void)
{
	typedef DWORD			(WINAPI *T_GET_TABLE)(
		PVOID TABLE,
		PDWORD SIZE,
		BOOL SORT,
		ULONG FAMILY,
		TCP_TABLE_CLASS CLASS,
		ULONG RESERVED
	);
	HMODULE					IPHLP;
	T_GET_TABLE				GET_TABLE;
	MIB_TCPTABLE_OWNER_PID	*TABLE;
	DWORD					SIZE;
	DWORD					I;
	int						ACTIVE;

	IPHLP = LoadLibraryW(L"iphlpapi.dll");
	GET_TABLE = IPHLP ?
		(T_GET_TABLE)(void *)GetProcAddress(IPHLP, "GetExtendedTcpTable") :
		NULL;
	if (!GET_TABLE)
	{
		if (IPHLP)
		{
			FreeLibrary(IPHLP);
		}
		return (1); /* CAN NOT TELL: BETTER WAIT */
	}
	ACTIVE = 0;
	SIZE = 0;
	GET_TABLE(NULL, &SIZE, FALSE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0);
	TABLE = SIZE ? (MIB_TCPTABLE_OWNER_PID *)malloc(SIZE) : NULL;
	if (TABLE && GET_TABLE(
		TABLE,
		&SIZE,
		FALSE,
		AF_INET,
		TCP_TABLE_OWNER_PID_CONNECTIONS,
		0
	) == NO_ERROR)
	{
		for (I = 0; I < TABLE->dwNumEntries; ++I)
		{
			if (TABLE->table[I].dwState == MIB_TCP_STATE_ESTAB &&
				ntohs((u_short)TABLE->table[I].dwLocalPort) == INWAY_PORT)
			{
				ACTIVE = 1;
			}
		}
	}
	else
	{
		ACTIVE = 1;
	}
	free(TABLE);
	FreeLibrary(IPHLP);
	return (ACTIVE);
}
