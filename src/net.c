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
# : License - GPL-3.0   :: Update - 2026/11/23 : #    ::::!!!1!!1!!!1!!!::     #
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
 * #define FD_SET
 * #define FD_ZERO
 * #define INADDR_ANY
 * #define INADDR_BROADCAST
 * #define INVALID_SOCKET
 * #define IPPROTO_UDP
 * #define SO_BROADCAST
 * #define SOCK_DGRAM
 * #define SOCK_STREAM
 * #define SOCKET_ERROR
 * #define SOL_SOCKET
 * typedef			SOCKET;
 * typedef			fd_set;
 * struct sockaddr;
 * struct sockaddr_in;
 * struct timeval;
 * typedef			u_short;
 * int				bind(SOCKET, const struct sockaddr *, int);
 * int				closesocket(SOCKET);
 * int				connect(SOCKET, const struct sockaddr *, int);
 * int				gethostname(char *, int);
 * int				getsockname(SOCKET, struct sockaddr *, int *);
 * u_long			htonl(u_long);
 * u_short			htons(u_short);
 * unsigned long	inet_addr(const char *);
 * u_long			ntohl(u_long);
 * u_short			ntohs(u_short);
 * int				recv(SOCKET, char *, int, int);
 * int				recvfrom(
 * 	SOCKET,
 * 	char *,
 * 	int,
 * 	int,
 * 	struct sockaddr *,
 * 	int *
 * );
 * int				select(
 * 	int,
 * 	fd_set *,
 * 	fd_set *,
 * 	fd_set *,
 * 	const struct timeval *
 * );
 * int				send(SOCKET, const char *, int, int);
 * int				sendto(
 * 	SOCKET,
 * 	const char *,
 * 	int,
 * 	int,
 * 	const struct sockaddr *,
 * 	int
 * );
 * int				setsockopt(SOCKET, int, int, const char *, int);
 * SOCKET			socket(int, int, int);
 * int				WSAIoctl(
 * 	SOCKET,
 * 	DWORD,
 * 	LPVOID,
 * 	DWORD,
 * 	LPVOID,
 * 	DWORD,
 * 	LPDWORD,
 * 	LPWSAOVERLAPPED,
 * 	LPWSAOVERLAPPED_COMPLETION_ROUTINE
 * );
 */
#include <ws2tcpip.h>
/******* """"""""""""
 * #define IFF_LOOPBACK
 * #define IFF_UP
 * #define SIO_GET_INTERFACE_LIST
 * struct addrinfo;
 * typedef	INTERFACE_INFO;
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
 * #define ComputerNamePhysicalDnsHostname
 * #define TRUE
 * BOOL		CloseHandle(HANDLE);
 * HANDLE	CreateThread(
 * 	LPSECURITY_ATTRIBUTES,
 * 	SIZE_T,
 * 	LPTHREAD_START_ROUTINE,
 * 	LPVOID,
 * 	DWORD,
 * 	LPDWORD
 * );
 * void		DeleteCriticalSection(LPCRITICAL_SECTION);
 * void		EnterCriticalSection(LPCRITICAL_SECTION);
 * BOOL		FreeLibrary(HMODULE);
 * BOOL		GetComputerNameA(LPSTR, LPDWORD);
 * BOOL		GetComputerNameExA(COMPUTER_NAME_FORMAT, LPSTR, LPDWORD);
 * FARPROC	GetProcAddress(HMODULE, LPCSTR);
 * DWORD	GetTickCount(void);
 * void		InitializeCriticalSection(LPCRITICAL_SECTION);
 * LONG		InterlockedExchange(LONG volatile *, LONG);
 * void		LeaveCriticalSection(LPCRITICAL_SECTION);
 * void		Sleep(DWORD);
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
 * int		_strnicmp(const char *, const char *, size_t);
 * int		memcmp(const void *, const void *, size_t);
 * void		*memcpy(void *, const void *, size_t);
 * void		*memset(void *, int, size_t);
 * size_t	strcspn(const char *, const char *);
 * size_t	strlen(const char *);
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
#define ASK_TEXT "INWAY-WHO?" /* + A NAME: WHICH PC IS CALLED THIS? */
#define ANSWER_TEXT "INWAY-ME!" /* + THE NAME: THIS ONE, HERE I AM */
#define ASK_EVERY_MS 300 /* THE QUESTION GOES OUT AGAIN THIS OFTEN */
#define ASK_LONG_MS 1500 /* HOW LONG TO WAIT WHEN WINDOWS FOUND NOTHING */
#define ASK_SHORT_MS 300 /* HOW LONG TO WAIT WHEN WINDOWS FOUND AN ADDRESS */

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

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                    FINDING A PC NAME                     |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# WINDOWS FINDS A NAME WITH THE ROUTER (DNS) OR BY ASKING THE PCS AROUND
# (LLMNR, NETBIOS, MDNS). THE OTHER PC ONLY ANSWERS THOSE WHEN ITS FIREWALL
# LETS THEM IN: ON A "PUBLIC" NETWORK, OR WITH NETWORK DISCOVERY OFF, IT
# DOES NOT, AND THE NAME IS "NOT FOUND" WHILE ITS ADDRESS WORKS. SO INWAY
# ALSO ASKS ITSELF: "INWAY-WHO?NAME" GOES AS A BROADCAST TO EVERY NETWORK OF
# THIS PC (UDP, THE INWAY PORT), AND THE INWAY ON THE PC WITH THAT NAME
# ANSWERS "INWAY-ME!NAME". ITS ADDRESS IS WHERE THE ANSWER CAME FROM.
*/
/* 1 = NAME (THE PART BEFORE A ".") IS THIS PC'S NAME, IN ANY CASE */
static int
	IS_MY_NAME(const char *NAME)
{
	char	MINE[256];
	DWORD	SIZE;
	size_t	LEN;

	LEN = strcspn(NAME, ".");
	if (!LEN || LEN >= sizeof(MINE))
	{
		return (0);
	}
	SIZE = sizeof(MINE);
	if (GetComputerNameExA(ComputerNamePhysicalDnsHostname, MINE, &SIZE) &&
		strlen(MINE) == LEN && _strnicmp(MINE, NAME, LEN) == 0)
	{
		return (1);
	}
	SIZE = sizeof(MINE);
	if (GetComputerNameA(MINE, &SIZE) && strlen(MINE) == LEN &&
		_strnicmp(MINE, NAME, LEN) == 0)
	{
		return (1);
	}
	return (0);
}

/* 1 = THE NAME OF AN ANSWER IS THE NAME THAT WAS ASKED, IN ANY CASE */
static int
	SAME_NAME(const char *ASKED, const char *ANSWERED)
{
	const size_t	A = strcspn(ASKED, ".");
	const size_t	B = strcspn(ANSWERED, ".");

	return (A == B && A && _strnicmp(ASKED, ANSWERED, A) == 0);
}

/*
# THE QUESTION GOES TO 255.255.255.255 AND TO THE BROADCAST ADDRESS OF EVERY
# NETWORK OF THIS PC (192.168.2.255...). WINDOWS SENDS 255.255.255.255 OUT
# OF ONE NETWORK CARD ONLY, AND A PC WITH WI-FI, ETHERNET, HYPER-V OR A VPN
# MAY PICK THE WRONG ONE.
*/
static void
	ASK_SEND(SOCKET U, const char *NAME)
{
	INTERFACE_INFO		CARDS[32];
	struct sockaddr_in	TO;
	char				PACKET[128];
	DWORD				BYTES;
	int					COUNT;
	int					LEN;
	int					I;
	uint32_t			ADDRESS;
	uint32_t			MASK;

	LEN = snprintf(PACKET, sizeof(PACKET), "%s%s", ASK_TEXT, NAME);
	if (LEN <= 0 || LEN >= (int)sizeof(PACKET))
	{
		return ;
	}
	memset(&TO, 0, sizeof(TO));
	TO.sin_family = AF_INET;
	TO.sin_port = htons(INWAY_PORT);
	TO.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendto(U, PACKET, LEN, 0, (struct sockaddr *)&TO, sizeof(TO));
	BYTES = 0;
	if (WSAIoctl(U, SIO_GET_INTERFACE_LIST, NULL, 0, CARDS, sizeof(CARDS),
		&BYTES, NULL, NULL) != 0)
	{
		return ;
	}
	COUNT = (int)(BYTES / sizeof(INTERFACE_INFO));
	for (I = 0; I < COUNT; ++I)
	{
		if (!(CARDS[I].iiFlags & IFF_UP) || (CARDS[I].iiFlags & IFF_LOOPBACK))
		{
			continue ;
		}
		ADDRESS = ntohl(CARDS[I].iiAddress.AddressIn.sin_addr.s_addr);
		MASK = ntohl(CARDS[I].iiNetmask.AddressIn.sin_addr.s_addr);
		if (!LAN_RANK(ADDRESS) || !MASK || MASK == 0XFFFFFFFFU)
		{
			continue ;
		}
		TO.sin_addr.s_addr = htonl(ADDRESS | ~MASK);
		sendto(U, PACKET, LEN, 0, (struct sockaddr *)&TO, sizeof(TO));
	}
}

/* A UDP SOCKET THAT MAY BROADCAST, WITH THE FIRST QUESTION ALREADY SENT */
static SOCKET
	ASK_START(const char *NAME)
{
	SOCKET				U;
	struct sockaddr_in	A;
	BOOL				YES;

	U = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (U == INVALID_SOCKET)
	{
		return (INVALID_SOCKET);
	}
	YES = TRUE;
	setsockopt(U, SOL_SOCKET, SO_BROADCAST, (const char *)&YES, sizeof(YES));
	memset(&A, 0, sizeof(A));
	A.sin_family = AF_INET;
	A.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(U, (struct sockaddr *)&A, sizeof(A)) != 0)
	{
		closesocket(U);
		return (INVALID_SOCKET);
	}
	ASK_SEND(U, NAME);
	return (U);
}

/* 1 = THE PC CALLED NAME ANSWERED WITHIN WAIT_MS, ITS ADDRESS IN OUTPUT */
static int
	ASK_WAIT(SOCKET U, const char *NAME, DWORD WAIT_MS, uint32_t *OUTPUT)
{
	struct sockaddr_in	FROM;
	struct timeval		TV;
	fd_set				READ;
	char				PACKET[160];
	const size_t		LEN = sizeof(ANSWER_TEXT) - 1;
	const DWORD			START = GetTickCount();
	DWORD				PASSED;
	DWORD				LAST_SENT;
	int					SIZE;
	int					GOT;

	LAST_SENT = START;
	for (;;)
	{
		PASSED = GetTickCount() - START;
		if (PASSED >= WAIT_MS)
		{
			return (0);
		}
		if (GetTickCount() - LAST_SENT >= ASK_EVERY_MS)
		{
			ASK_SEND(U, NAME); /* A BROADCAST CAN GET LOST */
			LAST_SENT = GetTickCount();
		}
		FD_ZERO(&READ);
		FD_SET(U, &READ);
		TV.tv_sec = 0;
		TV.tv_usec = 50000;
		if (select(0, &READ, NULL, NULL, &TV) <= 0)
		{
			continue ;
		}
		SIZE = sizeof(FROM);
		GOT = recvfrom(U, PACKET, sizeof(PACKET) - 1, 0,
			(struct sockaddr *)&FROM, &SIZE);
		if (GOT <= (int)LEN)
		{
			continue ;
		}
		PACKET[GOT] = '\0';
		if (memcmp(PACKET, ANSWER_TEXT, LEN) == 0 &&
			SAME_NAME(NAME, PACKET + LEN) &&
			LAN_RANK(ntohl(FROM.sin_addr.s_addr)))
		{
			*OUTPUT = ntohl(FROM.sin_addr.s_addr);
			return (1);
		}
	}
}

/* THE SHARING SIDE: ANSWERS "INWAY-WHO?" WHEN THE NAME IS THIS PC'S NAME */
static DWORD WINAPI
	ANSWER_THREAD(LPVOID PARAMETER)
{
	SOCKET				U;
	struct sockaddr_in	FROM;
	char				PACKET[160];
	char				REPLY[192];
	const size_t		LEN = sizeof(ASK_TEXT) - 1;
	int					SIZE;
	int					GOT;
	int					REPLY_LEN;

	U = (SOCKET)(UINT_PTR)PARAMETER;
	for (;;)
	{
		SIZE = sizeof(FROM);
		GOT = recvfrom(U, PACKET, sizeof(PACKET) - 1, 0,
			(struct sockaddr *)&FROM, &SIZE);
		if (GOT == SOCKET_ERROR)
		{
			Sleep(50); /* AN OLD "PORT UNREACHABLE" OR A BAD PACKET */
			continue ;
		}
		if (GOT <= (int)LEN)
		{
			continue ;
		}
		PACKET[GOT] = '\0';
		if (memcmp(PACKET, ASK_TEXT, LEN) != 0 || !IS_MY_NAME(PACKET + LEN))
		{
			continue ;
		}
		REPLY_LEN = snprintf(
			REPLY,
			sizeof(REPLY),
			"%s%s",
			ANSWER_TEXT,
			PACKET + LEN
		);
		if (REPLY_LEN > 0 && REPLY_LEN < (int)sizeof(REPLY))
		{
			sendto(U, REPLY, REPLY_LEN, 0, (struct sockaddr *)&FROM, SIZE);
		}
	}
	return (0);
}

/*
# ONCE PER PROCESS, WHEN THIS PC STARTS SHARING (HOST.C, OPEN_LISTENER).
# WHEN ANOTHER INWAY ON THIS PC ALREADY ANSWERS, THE PORT IS TAKEN: FINE.
*/
void
	NAME_ANSWER_START(void)
{
	static volatile LONG	STARTED = 0;
	SOCKET					U;
	struct sockaddr_in		A;
	HANDLE					T;

	if (InterlockedExchange(&STARTED, 1))
	{
		return ;
	}
	U = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (U == INVALID_SOCKET)
	{
		return ;
	}
	memset(&A, 0, sizeof(A));
	A.sin_family = AF_INET;
	A.sin_port = htons(INWAY_PORT);
	A.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(U, (struct sockaddr *)&A, sizeof(A)) != 0)
	{
		closesocket(U);
		return ;
	}
	T = CreateThread(NULL, 0, ANSWER_THREAD, (LPVOID)(UINT_PTR)U, 0, NULL);
	if (!T)
	{
		closesocket(U);
		return ;
	}
	CloseHandle(T);
}

/*
# BOTH AT ONCE: THE QUESTION GOES OUT, THEN WINDOWS LOOKS THE NAME UP (IT
# MAY TAKE A SECOND OR TWO WHEN IT FINDS NOTHING); ANSWERS WAIT MEANWHILE.
# AN INWAY ANSWER WINS: IT IS ON THIS NETWORK AND REALLY RUNS INWAY. A
# WINDOWS ADDRESS IS USED WHEN NO INWAY ANSWERED (A NAME FROM A VPN LIKE
# TAILSCALE, OR A PC THAT DOES NOT ANSWER BROADCASTS). A PC WITH MORE THAN
# ONE ADDRESS: THE HOME NETWORK ONE.
*/
static int
	RESOLVE_NAME(const char *NAME, uint32_t *OUTPUT)
{
	struct addrinfo	HINTS;
	struct addrinfo	*LIST;
	struct addrinfo	*P;
	SOCKET			U;
	uint32_t		IP;
	uint32_t		FOUND;
	int				BEST;
	int				RANK;

	U = ASK_START(NAME);
	memset(&HINTS, 0, sizeof(HINTS));
	HINTS.ai_family = AF_INET;
	HINTS.ai_socktype = SOCK_STREAM;
	LIST = NULL;
	BEST = 0;
	FOUND = 0;
	if (getaddrinfo(NAME, NULL, &HINTS, &LIST) == 0 && LIST)
	{
		for (P = LIST; P; P = P->ai_next)
		{
			IP = ntohl(((struct sockaddr_in *)P->ai_addr)->sin_addr.s_addr);
			RANK = LAN_RANK(IP);
			if (RANK > BEST)
			{
				BEST = RANK;
				FOUND = IP;
			}
		}
		freeaddrinfo(LIST);
	}
	if (U != INVALID_SOCKET)
	{
		if (ASK_WAIT(U, NAME, BEST ? ASK_SHORT_MS : ASK_LONG_MS, &IP))
		{
			FOUND = IP;
			BEST = 1;
		}
		closesocket(U);
	}
	if (!BEST)
	{
		return (2);
	}
	*OUTPUT = FOUND;
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                    FINDING A PC NAME                     |  //\\  */
/* '""""' #==========================================================# '""""' */

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
