/******************************************************************************\
# H - SERVICE                                    #       Maximum Tension       #
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
|*|                                 SERVICE                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE "ALWAYS ON" MODE FOR SERVERS AND WORKER PCS. "INWAY.EXE -INSTALL -P  |*|
|*| 938572" COPIES INWAY TO "C:\PROGRAM FILES\INWAY", INSTALLS A WINDOWS     |*|
|*| SERVICE CALLED INWAY AND OPENS THE FIREWALL FOR THE LOCAL NETWORK ONLY.  |*|
|*| THE SERVICE RUNS WITH SYSTEM RIGHTS AND STARTS A HELPER ("-WORKER") IN   |*|
|*| THE SESSION ON THE SCREEN. ONLY A PROGRAM WITH SYSTEM RIGHTS CAN SEE AND |*|
|*| USE THE LOCK SCREEN, "ALLOW THIS APP?" POPUPS AND ADMIN WINDOWS LIKE     |*|
|*| TASK MANAGER, AND ONLY A SERVICE MAY PRESS CTRL + ALT + DEL.             |*|
|*| SERVICE_REMOVE_OLD() TAKES OUT THE OLD CONNECT TOOL (SERVICE CONNECTRD,  |*|
|*| ITS FIREWALL RULE AND "C:\PROGRAM FILES\CONNECT"). INWAY USES ANOTHER    |*|
|*| PORT, SERVICE NAME AND FOLDER, SO BOTH CAN RUN WHILE A PC IS MOVED OVER. |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| int  SERVICE_RUN(const char *CODE);                                : *|
|* :  3| int  SERVICE_WORKER(void);                                         : *|
|* :  4| void SERVICE_READ_INFO(void);                                      : *|
|* :  5| int  SERVICE_IS_ADMIN(void);                                       : *|
|* :  6| int  SERVICE_RELAUNCH_AS_ADMIN(const char *PARAMETERS);            : *|
|* :  7| int  SERVICE_INSTALL(const char *CODE);                            : *|
|* :  8| int  SERVICE_UNINSTALL(void);                                      : *|
|* :  9| int  SERVICE_REMOVE_OLD(void);                                     : *|
|* : 10|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef SERVICE_H
#define SERVICE_H

extern int	G_SVC_RUNNING;	/* SEEN FROM A NORMAL COPY: IS THE SERVICE ON? */
extern char	G_SVC_CODE[8];	/* THE SERVICE'S CODE, "" WHEN UNKNOWN */

extern int	SERVICE_RUN(const char *CODE);
extern int	SERVICE_WORKER(void);
extern void	SERVICE_READ_INFO(void);
extern int	SERVICE_IS_ADMIN(void);
extern int	SERVICE_RELAUNCH_AS_ADMIN(const char *PARAMETERS);
extern int	SERVICE_INSTALL(const char *CODE);
extern int	SERVICE_UNINSTALL(void);
extern int	SERVICE_REMOVE_OLD(void);

#endif /* !SERVICE_H */
