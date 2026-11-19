/******************************************************************************\
# H - update                                     #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/18 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# THE GITHUB RELEASE AUTO UPDATER. ON EVERY START INWAY ASKS GITHUB FOR THE
# LATEST RELEASE OF MAXIMUM-TENSION/INWAY. IT UPDATES WHEN THE SHA-256 GITHUB
# GIVES FOR THAT RELEASE'S INWAY.EXE IS NOT THE SHA-256 OF THE RUNNING EXE, SO
# TWO RELEASES ON THE SAME DAY ("20.26.11.17-MK1145", "...-MK1151") WORK.
# TWO SAFETY RULES: A RELEASE WITH AN OLDER DATE THAN THIS EXE'S VERSION IS
# NEVER INSTALLED, AND AN EXE NEXT TO "SRC\INWAY.H" (A TEST BUILD IN THE SOURCE
# FOLDER) NEVER UPDATES ITSELF.
# WHEN AN UPDATE IS DUE:
# 1. INWAY.EXE OF THAT RELEASE IS DOWNLOADED INTO %TMP% AND ITS SHA-256 IS
# CHECKED AGAINST THE SAME DIGEST AGAIN. A WRONG OR MISSING DIGEST MEANS NO
# UPDATE.
# 2. THE NEW EXE IS STARTED, PAUSED, AND A NOTE NEXT TO IT IN %TMP% TELLS IT
# WHERE THE OLD EXE IS. THEN THE OLD ONE CLOSES.
# 3. THE NEW ONE ("-UPDATE-FINISH") READS THE NOTE, WAITS FOR THE OLD ONE TO
# CLOSE, DELETES IT, MOVES ITSELF TO THAT PATH, DELETES THE NOTE AND GOES ON
# WITH THE SAME COMMAND LINE.
# NO NETWORK, A RATE LIMIT OR ANY ERROR: INWAY JUST STARTS AS IT IS.
# SERVERS (THE BACKGROUND SERVICE) UPDATE THEMSELVES WITH UPDATE_SERVICE_LOOP():
# A CHECK EVERY 6 HOURS, AND THE SWAP ONLY WHEN NOBODY IS CONNECTED. "-INSTALL
# -P CODE -NOUPDATE" TURNS THAT OFF FOR ONE SERVER. WHAT HAPPENS IS WRITTEN TO
# INWAY.LOG NEXT TO THE SERVICE'S EXE. THE NORMAL-RIGHTS SERVER USES
# UPDATE_USER_THREAD() FOR THE SAME 6-HOUR CHECK.
# A SERVER ALSO ASKS UPDATE_DUE() JUST BEFORE IT LETS SOMEONE IN (AT MOST ONCE
# IN 10 MINUTES). A NEWER INWAY: IT ANSWERS "UPDATING", UPDATE_NOW() STARTS
# THE UPDATE AND THE OTHER PC CONNECTS AGAIN BY ITSELF.
*/

#ifndef UPDATE_H
#define UPDATE_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <windows.h>
/******* """""""""""
 * #define WINAPI
 * typedef	DWORD;
 * typedef	HANDLE;
 * typedef	LPVOID;
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define UPDATE_REPO L"Maximum-Tension/INWAY"

#ifdef _WIN64
#define UPDATE_ASSET "INWAY.exe"
#else /* !_WIN64 */
#define UPDATE_ASSET "INWAY32.exe" /* THE 32-BIT BUILD STAYS 32-BIT */
#endif /* _WIN64 */

#define UPDATE_NOW_EVENT L"Global\\INWAY_UPDATE_NOW" /* HELPER > SERVICE */

extern int	G_AUTO_UPDATE; /* THE NORMAL-RIGHTS SERVER UPDATES ITSELF */

extern void			UPDATE_CHECK(void);
extern void			UPDATE_FINISH(void);
extern void			UPDATE_SERVICE_LOOP(HANDLE STOP);
extern DWORD WINAPI	UPDATE_USER_THREAD(LPVOID PARAMETER);
extern int			UPDATE_DUE(void);
extern void			UPDATE_NOW(void);
extern void			UPDATE_LOG(const char *FORMAT, ...);

#endif /* !UPDATE_H */
