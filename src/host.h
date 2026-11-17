/******************************************************************************\
# H - HOST                                       #       Maximum Tension       #
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
|*|                                   HOST                                   |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE SHARING SIDE: THE PC THAT IS BEING CONTROLLED. IT LISTENS ON         |*|
|*| INWAY_PORT, CHECKS THE CODE (OR ASKS THE PERSON WITH THE ACCEPT WINDOW), |*|
|*| THEN SENDS THE SCREEN AS CHANGED TILES AND PLAYS BACK THE MOUSE AND      |*|
|*| KEYBOARD IT RECEIVES. ONLY ONE PERSON CAN BE CONNECTED AT A TIME.        |*|
|*| WHEN INWAY RUNS AS THE BACKGROUND SERVICE (G_WORKER = 1) IT FOLLOWS THE  |*|
|*| DESKTOP THAT IS ON THE SCREEN, SO THE LOCK SCREEN AND ADMIN POPUPS WORK, |*|
|*| AND IT NEVER SHOWS WINDOWS: THERE IS NOBODY TO CLICK ACCEPT.             |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| SOCKET OPEN_LISTENER(void);                                         : *|
|* : 3| void   HOST_START(void);                                            : *|
|* : 4| DWORD  WINAPI HOST_THREAD(LPVOID PARAMETER);                        : *|
|* : 5|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef HOST_H
#define HOST_H

extern volatile LONG	G_RUNNING;	/* 1 WHILE A SESSION IS ON */
extern int		G_WORKER;	/* 1 = BACKGROUND SERVICE HELPER */
extern int		G_CODE;		/* THIS PC'S 6 NUMBER CODE */
extern SOCKET		G_LISTEN;

extern SOCKET		OPEN_LISTENER(void);
extern void		HOST_START(void);
extern DWORD WINAPI	HOST_THREAD(LPVOID PARAMETER);

#endif /* !HOST_H */
