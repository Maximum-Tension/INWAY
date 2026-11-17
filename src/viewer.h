/******************************************************************************\
# H - VIEWER                                     #       Maximum Tension       #
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
|*|                                  VIEWER                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE CONTROLLING SIDE: CONNECTS TO ANOTHER PC AND SHOWS ITS SCREEN IN A   |*|
|*| WINDOW. MOUSE AND KEYS IN THAT WINDOW GO TO THE OTHER PC, THE WINDOWS    |*|
|*| KEY TOO (A LOW LEVEL KEYBOARD HOOK KEEPS IT FROM OPENING THIS PC'S START |*|
|*| MENU). THE MENU AT THE TOP SENDS CTRL + ALT + DEL, SO DOES CTRL + ALT +  |*|
|*| END.                                                                     |*|
|*| VIEWER_CONNECT() RETURNS ONLY WHEN THE WINDOW IS CLOSED (CR_ENDED) OR    |*|
|*| CONNECTING FAILED. CTRL + C IN THE BLACK WINDOW CLOSES THE SESSION AND   |*|
|*| SETS G_QUIT, SO THE PROGRAM CAN EXIT.                                    |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| void          VIEWER_INIT(void);                                    : *|
|* : 3| enum E_RESULT VIEWER_CONNECT(uint32_t IP, int32_t CODE,             : *|
|* : 4|                              const char *LABEL);                    : *|
|* : 5|                                                                     : *|
|* :                                                                        : *|
|* : EXAMPLES:                                                              : *|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| if (VIEWER_CONNECT(IP, 948248, "2.253") == CR_ENDED)               : *|
|* :  3|     SAY("bye\n");                                                  : *|
|* :  4|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef VIEWER_H
#define VIEWER_H

extern volatile LONG	G_QUIT;	/* CTRL + C WAS PRESSED DURING A SESSION */

extern void		VIEWER_INIT(void);
extern enum E_RESULT	VIEWER_CONNECT(
	uint32_t IP,
	int32_t CODE,
	const char *LABEL
);

#endif /* !VIEWER_H */
