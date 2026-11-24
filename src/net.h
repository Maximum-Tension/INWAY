/******************************************************************************\
# H - net                                        #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/24 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# A LINK IS A SOCKET WITH A LOCK: THE SCREEN, THE MOUSE AND THE CLIPBOARD ARE
# SENT FROM DIFFERENT THREADS, AND A MESSAGE MUST NEVER BE CUT IN HALF BY
# ANOTHER ONE. LINK_SEND() SENDS ONE MESSAGE MADE OF TWO PIECES (A AND B).
# SOCKET HELPERS AND EVERYTHING ABOUT ADDRESSES AND CODES. A PC ON THE SAME HOME
# NETWORK IS SHOWN WITH ITS LAST TWO NUMBERS ONLY (192.168.2.253 IS "2.253"),
# AND MISSING FRONT NUMBERS ARE TAKEN FROM THIS PC'S OWN ADDRESS, OR 192.168
# WHEN THIS PC HAS NONE. A PC NAME ("OFIS-PC") WORKS TOO, SO IT STILL WORKS
# WHEN THE ROUTER GIVES THAT PC ANOTHER ADDRESS: INWAY ASKS THE NETWORK FOR IT
# ITSELF (THE INWAY ON THAT PC ANSWERS, SEE NAME_ANSWER_START) AND WINDOWS
# LOOKS IT UP TOO (DNS, LLMNR, NETBIOS, A VPN LIKE TAILSCALE).
*/

#ifndef NET_H
#define NET_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <winsock2.h>
/******* """"""""""""
 * typedef	SOCKET;
 */
#include <windows.h>
/******* """""""""""
 * typedef	CRITICAL_SECTION;
 */
#include <string.h>
/******* """"""""""
 * typedef	size_t;
 */
#include <stdint.h>
/******* """"""""""
 * typedef	uint32_t;
 * typedef	uint8_t;
 */
/* **************************** [^] INCLUDES [^] **************************** */

struct	S_LINK
{
	SOCKET				S;
	CRITICAL_SECTION	LOCK;
};

extern uint32_t	G_MY_IP; /* THIS PC'S LAN ADDRESS, HOST ORDER, 0 = NONE */

extern int		SEND_ALL(SOCKET S, const void *BUFFER, int SIZE);
extern int		RECV_ALL(SOCKET S, void *BUFFER, int SIZE);
extern int		NET_CONNECT(SOCKET S, uint32_t IP);
extern uint32_t	PICK_LOCAL_IP(void);
extern void		IP_FULL(uint32_t IP, char *OUTPUT, size_t CAP);
extern void		IP_SHORT(uint32_t IP, char *OUTPUT, size_t CAP);
extern void		CODE_TEXT(int CODE, char *OUTPUT, size_t CAP);
extern int		PARSE_ADDRESS(const char *INPUT, uint32_t *OUTPUT);
extern int		ADDRESS_NAME(const char *INPUT, char *OUTPUT, size_t CAP);
extern void		NAME_ANSWER_START(void);
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
extern int		SESSION_ACTIVE(void);

#endif /* !NET_H */
