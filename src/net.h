/******************************************************************************\
# H - NET                                        #       Maximum Tension       #
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
|*| A LINK IS A SOCKET WITH A LOCK: THE SCREEN, THE MOUSE AND THE CLIPBOARD  |*|
|*| ARE SENT FROM DIFFERENT THREADS, AND A MESSAGE MUST NEVER BE CUT IN HALF |*|
|*| BY ANOTHER ONE. LINK_SEND() SENDS ONE MESSAGE MADE OF TWO PIECES (A AND  |*|
|*| B).                                                                      |*|
|*| SOCKET HELPERS AND EVERYTHING ABOUT ADDRESSES AND CODES. A PC ON THE     |*|
|*| SAME HOME NETWORK IS SHOWN WITH ITS LAST TWO NUMBERS ONLY (192.168.2.253 |*|
|*| IS "2.253"), AND MISSING FRONT NUMBERS ARE TAKEN FROM THIS PC'S OWN      |*|
|*| ADDRESS, OR 192.168 WHEN THIS PC HAS NONE.                               |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| int      SEND_ALL(SOCKET S, const void *BUFFER, int SIZE);         : *|
|* :  3| int      RECV_ALL(SOCKET S, void *BUFFER, int SIZE);               : *|
|* :  4| uint32_t PICK_LOCAL_IP(void);                                      : *|
|* :  5| void     IP_FULL(uint32_t IP, char *OUT, size_t CAP);              : *|
|* :  6| void     IP_SHORT(uint32_t IP, char *OUT, size_t CAP);             : *|
|* :  7| void     CODE_TEXT(int CODE, char *OUT, size_t CAP);               : *|
|* :  8| int      PARSE_ADDRESS(const char *INPUT, uint32_t *OUT);          : *|
|* :  9| void     LINK_INIT(struct S_LINK *LINK, SOCKET S);                 : *|
|* : 10| int      LINK_SEND(struct S_LINK *LINK, uint32_t TYPE,             : *|
|* : 11|                    const void *A,                                  : *|
|* : 12|                    uint32_t LA, const void *B, uint32_t LB);       : *|
|* : 13| void     LINK_FREE(struct S_LINK *LINK);                           : *|
|* : 14| int      RECV_MSG(SOCKET S, uint32_t *TYPE, uint8_t **BUFFER,      : *|
|* : 15|                   uint32_t *SIZE, uint32_t *CAP, uint32_t MAX);    : *|
|* : 16|                                                                    : *|
|* :                                                                        : *|
|* : EXAMPLES:                                                              : *|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| uint32_t  IP;                                                      : *|
|* :  3| char      LABEL[24];                                               : *|
|* :  4|                                                                    : *|
|* :  5| if (PARSE_ADDRESS("42.39", &IP) == 0)                              : *|
|* :  6|     IP_SHORT(IP, LABEL, sizeof(LABEL));                            : *|
|* :  7|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef NET_H
#define NET_H

struct	S_LINK
{
	SOCKET		S;
	CRITICAL_SECTION	LOCK;
};

extern uint32_t	G_MY_IP;	/* THIS PC'S LAN ADDRESS, HOST ORDER, 0 = NONE */

extern int		SEND_ALL(SOCKET S, const void *BUFFER, int SIZE);
extern int		RECV_ALL(SOCKET S, void *BUFFER, int SIZE);
extern uint32_t	PICK_LOCAL_IP(void);
extern void		IP_FULL(uint32_t IP, char *OUTPUT, size_t CAP);
extern void		IP_SHORT(uint32_t IP, char *OUTPUT, size_t CAP);
extern void		CODE_TEXT(int CODE, char *OUTPUT, size_t CAP);
extern int		PARSE_ADDRESS(const char *INPUT, uint32_t *OUTPUT);
extern void		LINK_INIT(struct S_LINK *LINK, SOCKET S);
extern int		LINK_SEND(
	struct S_LINK *LINK,
	uint32_t TYPE,
	const void *A,
	uint32_t LA,
	const void *B,
	uint32_t LB
);
extern void		LINK_FREE(struct S_LINK *LINK);
extern int		RECV_MSG(
	SOCKET S,
	uint32_t *TYPE,
	uint8_t **BUFFER,
	uint32_t *SIZE,
	uint32_t *CAP,
	uint32_t MAX
);

#endif /* !NET_H */
