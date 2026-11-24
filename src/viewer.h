/******************************************************************************\
# H - viewer                                     #       Maximum Tension       #
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
# THE CONTROLLING SIDE: CONNECTS TO ANOTHER PC AND SHOWS ITS SCREEN IN A WINDOW.
# MOUSE AND KEYS IN THAT WINDOW GO TO THE OTHER PC, THE WINDOWS KEY TOO (A LOW
# LEVEL KEYBOARD HOOK KEEPS IT FROM OPENING THIS PC'S START MENU). THE MENU AT
# THE TOP SENDS CTRL + ALT + DEL, SO DOES CTRL + ALT + END.
# VIEWER_CONNECT() RETURNS ONLY WHEN THE WINDOW IS CLOSED (CR_ENDED) OR
# CONNECTING FAILED. CTRL + C IN THE BLACK WINDOW CLOSES THE SESSION AND SETS
# G_QUIT, SO THE PROGRAM CAN EXIT.
# VIEWER_CONNECT_VIA() REACHES THE LAST OF SEVERAL PCS THROUGH THE ONES BEFORE
# IT (-C 100.80.120.66 482382 2.253): PCS[0] IS AT IP, EVERY OTHER ONE IS
# LOOKED UP BY THE PC BEFORE IT, ON ITS OWN NETWORK (SEE INWAY.H). *FAILED
# SAYS WHICH PC THE RESULT IS ABOUT.
*/

#ifndef VIEWER_H
#define VIEWER_H

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * enum E_RESULT;
 */
#include <windows.h>
/******* """""""""""
 * typedef	LONG;
 */
#include <stdint.h>
/******* """"""""""
 * typedef	int32_t;
 * typedef	uint32_t;
 */
/* **************************** [^] INCLUDES [^] **************************** */

struct	S_HOP
{
	char	ADDRESS[64]; /* AS TYPED: THE PC BEFORE IT LOOKS IT UP */
	char	LABEL[64]; /* HOW THE MESSAGES CALL IT */
	int32_t	CODE; /* 0 = ASK FOR ACCEPT */
};

extern volatile LONG	G_QUIT; /* CTRL + C WAS PRESSED DURING A SESSION */
extern int				G_HIDE_CONSOLE; /* -C: HIDE THE TEXT WINDOW WHEN IN */

extern void				VIEWER_INIT(void);
extern enum E_RESULT	VIEWER_CONNECT(
	uint32_t IP,
	int32_t CODE,
	const char *LABEL
);
extern enum E_RESULT	VIEWER_CONNECT_VIA(
	uint32_t IP,
	const struct S_HOP *PCS,
	int COUNT,
	int *FAILED
);

#endif /* !VIEWER_H */
