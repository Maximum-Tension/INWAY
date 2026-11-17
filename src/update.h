/******************************************************************************\
# H - UPDATE                                     #       Maximum Tension       #
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
|*|                                  UPDATE                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE GITHUB RELEASE AUTO UPDATER. ON EVERY START INWAY ASKS GITHUB FOR    |*|
|*| THE LATEST RELEASE OF MAXIMUM-TENSION/INWAY AND COMPARES ITS TAG WITH    |*|
|*| ITS OWN VERSION. ONLY THE NUMBERS ARE COMPARED: THE RUNNING EXE IS NEVER |*|
|*| HASHED.                                                                  |*|
|*| WHEN THE TAG IS NEWER:                                                   |*|
|*| 1. INWAY.EXE OF THAT RELEASE IS DOWNLOADED INTO %TMP% AND ITS SHA-256 IS |*|
|*| CHECKED AGAINST THE "DIGEST" GITHUB GIVES FOR THAT FILE. A WRONG OR      |*|
|*| MISSING DIGEST MEANS NO UPDATE.                                          |*|
|*| 2. THE NEW EXE IS STARTED, PAUSED, AND A NOTE NEXT TO IT IN %TMP% TELLS  |*|
|*| IT WHERE THE OLD EXE IS. THEN THE OLD ONE CLOSES.                        |*|
|*| 3. THE NEW ONE ("-UPDATE-FINISH") READS THE NOTE, WAITS FOR THE OLD ONE  |*|
|*| TO CLOSE, DELETES IT, MOVES ITSELF TO THAT PATH, DELETES THE NOTE AND    |*|
|*| GOES ON WITH THE SAME COMMAND LINE.                                      |*|
|*| NO NETWORK, A RATE LIMIT OR ANY ERROR: INWAY JUST STARTS AS IT IS.       |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| void UPDATE_CHECK(void);                                            : *|
|* : 3| void UPDATE_FINISH(void);                                           : *|
|* : 4|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef UPDATE_H
#define UPDATE_H

#define UPDATE_REPO		L"Maximum-Tension/INWAY"
#define UPDATE_ASSET		"INWAY.exe"

extern void	UPDATE_CHECK(void);
extern void	UPDATE_FINISH(void);

#endif /* !UPDATE_H */
