/******************************************************************************\
# H - htmlui                                     #       Maximum Tension       #
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
# THE STYLED POPUP WINDOWS, MADE WITH MSHTML (THE ENGINE BEHIND HTA FILES). THE
# PAGES ACCEPT.HTM AND BANNER.HTM ARE INSIDE INWAY.EXE AS RESOURCES. A PAGE IS
# COPIED FROM MEMORY TO A FILE IN %TMP% WHILE ITS WINDOW IS OPEN AND DELETED
# RIGHT AFTER, SO IT ALSO WORKS WHEN THE EXE IS PACKED WITH UPX. THE TEXTS COME
# FROM LANG AND ARE GIVEN TO THE PAGE AS WINDOW.DIALOGARGUMENTS; THE PAGE
# ANSWERS THROUGH WINDOW.RETURNVALUE.
# HTMLUI_ASK() SHOWS "2.253 WANTS TO CONTROL THIS PC" WITH ACCEPT AND CANCEL,
# OR "2.253 WANTS TO REACH 2.98 THROUGH THIS PC" WHEN TO IS GIVEN. IT
# RETURNS 1 FOR ACCEPT, 0 FOR CANCEL OR TIME OUT, AND -1 WHEN THE OTHER PERSON
# GAVE UP WAITING (NEVER WITH INVALID_SOCKET: THEN IT DOES NOT WATCH). IF
# MSHTML IS MISSING IT FALLS BACK TO A NORMAL MESSAGE BOX.
# HTMLUI_PEER_GONE() IS 1 ONCE THE OTHER SIDE CLOSED THE CONNECTION.
# HTMLUI_BANNER_SHOW() PUTS A SMALL "2.253 IS CONTROLLING THIS PC" WINDOW WITH A
# DISCONNECT BUTTON IN THE CORNER FOR THE WHOLE SESSION. DISCONNECT CLOSES THE
# GIVEN SOCKET. HTMLUI_BANNER_HIDE() REMOVES IT.
*/

#ifndef HTMLUI_H
#define HTMLUI_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <winsock2.h>
/******* """"""""""""
 * typedef	SOCKET;
 */
/* **************************** [^] INCLUDES [^] **************************** */

extern int	HTMLUI_ASK(SOCKET S, const char *WHO, const char *TO);
extern int	HTMLUI_PEER_GONE(SOCKET S);
extern void	HTMLUI_BANNER_SHOW(const char *WHO, SOCKET S);
extern void	HTMLUI_BANNER_HIDE(void);
extern int	HTMLUI_NOTICE(const char *WHO);

#endif /* !HTMLUI_H */
