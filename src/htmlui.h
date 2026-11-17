/******************************************************************************\
# H - HTMLUI                                     #       Maximum Tension       #
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
|*|                                  HTMLUI                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE STYLED POPUP WINDOWS, MADE WITH MSHTML (THE ENGINE BEHIND HTA        |*|
|*| FILES). THE PAGES ACCEPT.HTM AND BANNER.HTM ARE INSIDE INWAY.EXE AS      |*|
|*| RESOURCES AND ARE OPENED WITH SHOWHTMLDIALOG() FROM A "RES://" ADDRESS,  |*|
|*| SO NO FILE IS WRITTEN ANYWHERE. THE TEXTS COME FROM LANG AND ARE GIVEN   |*|
|*| TO THE PAGE AS WINDOW.DIALOGARGUMENTS; THE PAGE ANSWERS THROUGH          |*|
|*| WINDOW.RETURNVALUE.                                                      |*|
|*| HTMLUI_ASK() SHOWS "2.253 WANTS TO CONTROL THIS PC" WITH ACCEPT AND      |*|
|*| CANCEL. IT RETURNS 1 FOR ACCEPT, 0 FOR CANCEL OR TIME OUT, AND -1 WHEN   |*|
|*| THE OTHER PERSON GAVE UP WAITING. IF MSHTML IS MISSING IT FALLS BACK TO  |*|
|*| A NORMAL MESSAGE BOX.                                                    |*|
|*| HTMLUI_BANNER_SHOW() PUTS A SMALL "2.253 IS CONTROLLING THIS PC" WINDOW  |*|
|*| WITH A DISCONNECT BUTTON IN THE CORNER FOR THE WHOLE SESSION. DISCONNECT |*|
|*| CLOSES THE GIVEN SOCKET. HTMLUI_BANNER_HIDE() REMOVES IT.                |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| int  HTMLUI_ASK(SOCKET S, const char *WHO);                         : *|
|* : 3| void HTMLUI_BANNER_SHOW(const char *WHO, SOCKET S);                 : *|
|* : 4| void HTMLUI_BANNER_HIDE(void);                                      : *|
|* : 5|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef HTMLUI_H
#define HTMLUI_H

extern int	HTMLUI_ASK(SOCKET S, const char *WHO);
extern void	HTMLUI_BANNER_SHOW(const char *WHO, SOCKET S);
extern void	HTMLUI_BANNER_HIDE(void);

#endif /* !HTMLUI_H */
