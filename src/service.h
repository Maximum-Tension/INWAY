/******************************************************************************\
# H - service                                    #       Maximum Tension       #
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
# THE "ALWAYS ON" MODE FOR SERVERS AND WORKER PCS. "INWAY.EXE -INSTALL -P
# 938572" COPIES INWAY TO "C:\PROGRAM FILES\INWAY", INSTALLS A WINDOWS SERVICE
# CALLED INWAY AND OPENS THE FIREWALL FOR THE LOCAL NETWORK ONLY. THE SERVICE
# RUNS WITH SYSTEM RIGHTS AND STARTS A HELPER ("-WORKER") IN THE SESSION ON THE
# SCREEN. ONLY A PROGRAM WITH SYSTEM RIGHTS CAN SEE AND USE THE LOCK SCREEN,
# "ALLOW THIS APP?" POPUPS AND ADMIN WINDOWS LIKE TASK MANAGER, AND ONLY A
# SERVICE MAY PRESS CTRL + ALT + DEL.
# SERVICE_REMOVE_OLD() TAKES OUT THE OLD CONNECT TOOL (SERVICE CONNECTRD, ITS
# FIREWALL RULE AND "C:\PROGRAM FILES\CONNECT"). INWAY USES ANOTHER PORT,
# SERVICE NAME AND FOLDER, SO BOTH CAN RUN WHILE A PC IS MOVED OVER.
# INSTALL, UNINSTALL AND REMOVE_OLD RETURN SVC_DONE OR THE REASON IT DID NOT
# WORK. THE ADMIN COPY STARTED BY SERVICE_RELAUNCH_AS_ADMIN() RUNS HIDDEN AND
# HANDS THAT NUMBER BACK AS ITS EXIT CODE: A NORMAL PROGRAM (LIKE INWAY
# CONTROLLED FROM ANOTHER PC) CAN NOT CLICK OR TYPE INTO AN ADMIN WINDOW, SO THE
# RESULT IS SHOWN IN THE NORMAL WINDOW INSTEAD.
# "NO" ON THAT PROMPT DURING -INSTALL MAKES THE NORMAL-RIGHTS SERVER INSTEAD
# (SERVICE_INSTALL_USER): IT STARTS WHEN THIS PERSON SIGNS IN, WITH NO WINDOW.
# DURING A SESSION, SERVICE_NOTICE_START() SHOWS THE "IS CONTROLLING THIS PC"
# BANNER AS THE PERSON SIGNED IN ("-NOTICE"), NEVER WITH SYSTEM RIGHTS.
# WITHOUT THE CODE, SERVICE_ASK() ASKS THAT PERSON TO CLICK ACCEPT THE SAME
# WAY ("-ASK"), ALSO FOR GOING THROUGH THIS PC TO ANOTHER ONE (TO). IT
# RETURNS LIKE HTMLUI_ASK(), OR ASK_NOBODY WHEN THE SCREEN IS LOCKED OR NOBODY
# IS SIGNED IN: THEN ONLY THE CODE WORKS.
*/

#ifndef SERVICE_H
#define SERVICE_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <winsock2.h>
/******* """"""""""""
 * typedef	SOCKET;
 */
#include <windows.h>
/******* """""""""""
 * typedef	HANDLE;
 * typedef	WCHAR;
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define SVC_DONE 0
#define SVC_NO_SCM 2 /* NO RIGHTS TO THE SERVICE LIST */
#define SVC_COPY_FAILED 3
#define SVC_CREATE_FAILED 4 /* THE WINDOWS ERROR IS IN THE BITS ABOVE 8 */
#define SVC_NOT_STARTED 5
#define SVC_NO_PERMISSION 6 /* NO ADMIN RIGHTS */
#define SVC_DECLINED 7 /* "NO" WAS CLICKED ON THE WINDOWS PROMPT */
#define SVC_DONE_USER 8 /* INSTALLED AS THE NORMAL-RIGHTS SERVER */
#define SVC_USER_EVENT L"Local\\INWAY_SERVER" /* NORMAL-RIGHTS SERVER IS ON */
#define ASK_NOBODY 2 /* SERVICE_ASK: NOBODY CAN CLICK, ONLY THE CODE WORKS */

extern int	G_SVC_RUNNING; /* 1 = THE SERVICE IS ON, 2 = THE NORMAL SERVER */
extern char	G_SVC_CODE[8]; /* ITS CODE, "" WHEN UNKNOWN */

extern int		SERVICE_RUN(const char *CODE, int NO_UPDATE);
extern int		SERVICE_WORKER(void);
extern void		SERVICE_READ_INFO(void);
extern int		SERVICE_IS_ADMIN(void);
extern int		SERVICE_RELAUNCH_AS_ADMIN(const char *PARAMETERS);
extern int		SERVICE_INSTALL(const char *CODE, int NO_UPDATE);
extern int		SERVICE_SWAP(void);
extern int		SERVICE_UNINSTALL(void);
extern int		SERVICE_REMOVE_OLD(void);
extern int		SERVICE_EXISTS(void);
extern int		SERVICE_INSTALL_USER(const char *CODE, int NO_UPDATE);
extern int		SERVICE_UNINSTALL_USER(void);
extern int		SERVICE_RUN_USER(const char *CODE, int NO_UPDATE);
extern HANDLE	SERVICE_NOTICE_START(const char *WHO);
extern int		SERVICE_ASK(SOCKET S, const char *WHO, const char *TO);
extern void		SERVICE_NOTICE_STOP(HANDLE PROCESS);
extern void		SERVICE_DIR(WCHAR *OUTPUT);

#endif /* !SERVICE_H */
