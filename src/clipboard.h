/******************************************************************************\
# H - CLIPBOARD                                  #       Maximum Tension       #
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
|*|                                CLIPBOARD                                 |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| COPY ON ONE PC, PASTE ON THE OTHER, BOTH WAYS. WHILE A SESSION IS ON,    |*|
|*| BOTH SIDES WATCH THEIR CLIPBOARD. COPIED TEXT IS SENT AT ONCE. COPIED    |*|
|*| FILES AND FOLDERS ARE SENT RIGHT AWAY TOO, INTO A FOLDER IN TEMP ON THE  |*|
|*| OTHER PC, AND WHEN THE LAST BYTE ARRIVES THEY ARE PUT ON ITS CLIPBOARD.  |*|
|*| CTRL + V THEN PASTES THEM LIKE NORMAL FILES. A NEW COPY CANCELS A COPY   |*|
|*| STILL ON ITS WAY.                                                        |*|
|*| FILES GO AS A SIMPLE STREAM (NAME, SIZE, BYTES), NOT AS A ZIP: BOTH ENDS |*|
|*| ARE INWAY AND A LAN DOES NOT NEED COMPRESSION. NAMES THAT TRY TO LEAVE   |*|
|*| THE TEMP FOLDER ("..", "C:", "\\") ARE REFUSED.                          |*|
|*| ONE STRUCT S_CLIP PER SESSION. THE SEND FUNCTION GIVEN TO CLIP_START()   |*|
|*| PUTS A MESSAGE ON THAT SESSION'S LINK.                                   |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| struct S_CLIP *CLIP_START(struct S_LINK *LINK, const char *WHO);    : *|
|* : 3| void           CLIP_RECEIVE(struct S_CLIP *CLIP, uint32_t TYPE,     : *|
|* : 4|                             const uint8_t *DATA, uint32_t SIZE);    : *|
|* : 5| void           CLIP_STOP(struct S_CLIP *CLIP);                      : *|
|* : 6|                                                                     : *|
|* :                                                                        : *|
|* : EXAMPLES:                                                              : *|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| CLIP = CLIP_START(&LINK, "2.253");                                 : *|
|* :  3| ...                                                                : *|
|* :  4| CLIP_RECEIVE(CLIP, TYPE, BUFFER, SIZE);                            : *|
|* :  5| ...                                                                : *|
|* :  6| CLIP_STOP(CLIP);                                                   : *|
|* :  7|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#define CLIP_MAX_TEXT		(16U << 20)	/* 16 MB OF TEXT */
#define CLIP_MAX_FILES		(4ULL << 30)	/* 4 GB OF FILES IN ONE COPY */
#define CLIP_CHUNK		(64U << 10)	/* FILES GO IN 64 KB PIECES */

struct	S_CLIP;

extern struct S_CLIP	*CLIP_START(struct S_LINK *LINK, const char *WHO);
extern void		CLIP_RECEIVE(
	struct S_CLIP *CLIP,
	uint32_t TYPE,
	const uint8_t *DATA,
	uint32_t SIZE
);
extern void		CLIP_STOP(struct S_CLIP *CLIP);

#endif /* !CLIPBOARD_H */
