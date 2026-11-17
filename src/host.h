/******************************************************************************\
# H - host                                       #       Maximum Tension       #
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
# THE SHARING SIDE: THE PC THAT IS BEING CONTROLLED. IT LISTENS ON INWAY_PORT,
# CHECKS THE CODE (OR ASKS THE PERSON WITH THE ACCEPT WINDOW), THEN SENDS THE
# SCREEN AS CHANGED TILES AND PLAYS BACK THE MOUSE AND KEYBOARD IT RECEIVES.
# ONLY ONE PERSON CAN BE CONNECTED AT A TIME.
# WHEN INWAY RUNS AS THE BACKGROUND SERVICE (G_WORKER = 1) IT FOLLOWS THE
# DESKTOP THAT IS ON THE SCREEN, SO THE LOCK SCREEN AND ADMIN POPUPS WORK. IT
# TAKES ONLY ITS CODE (NOBODY THERE TO CLICK ACCEPT), AND ITS "IS CONTROLLING
# THIS PC" BANNER IS SHOWN BY A HELPER RUNNING AS THE PERSON SIGNED IN.
# A NORMAL INWAY ASKS WINDOWS FOR ADMIN RIGHTS WHEN A SESSION STARTS. YES: A
# HIDDEN ADMIN COPY TAKES THE CONNECTION OVER (HOST_TAKE_OVER). NO: THE
# SESSION GOES ON WITH NORMAL RIGHTS. THE NORMAL-RIGHTS SERVER (G_BACKGROUND)
# NEVER ASKS.
*/

#ifndef HOST_H
#define HOST_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <winsock2.h>
/******* """"""""""""
 * typedef	SOCKET;
 */
#include <windows.h>
/******* """""""""""
 * #define WINAPI
 * typedef	DWORD;
 * typedef	LONG;
 * typedef	LPVOID;
 */
/* **************************** [^] INCLUDES [^] **************************** */

extern volatile LONG	G_RUNNING; /* 1 WHILE A SESSION IS ON */
extern int				G_WORKER; /* 1 = BACKGROUND SERVICE HELPER */
extern int				G_BACKGROUND; /* 1 = NORMAL-RIGHTS SERVER, NO WINDOW */
extern int				G_CODE; /* THIS PC'S 6 NUMBER CODE */
extern SOCKET			G_LISTEN;

extern SOCKET		OPEN_LISTENER(void);
extern void			HOST_START(void);
extern DWORD WINAPI	HOST_THREAD(LPVOID PARAMETER);
extern int			HOST_TAKE_OVER(const char *PIPE_NAME);

#endif /* !HOST_H */
