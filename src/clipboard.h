/******************************************************************************\
# H - clipboard                                  #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/28 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# COPY ON ONE PC, PASTE ON THE OTHER, BOTH WAYS. WHILE A SESSION IS ON, BOTH
# SIDES WATCH THEIR CLIPBOARD. COPIED TEXT IS SENT AT ONCE. COPIED FILES AND
# FOLDERS ARE SENT RIGHT AWAY TOO, INTO A FOLDER IN TEMP ON THE OTHER PC, AND
# WHEN THE LAST BYTE ARRIVES THEY ARE PUT ON ITS CLIPBOARD. CTRL + V THEN PASTES
# THEM LIKE NORMAL FILES. A NEW COPY CANCELS A COPY STILL ON ITS WAY.
# CLIP_FLUSH() SENDS A COPY THAT IS STILL WAITING, RIGHT BEFORE CTRL + V GOES
# TO THE OTHER PC.
# FILES GO AS A SIMPLE STREAM (NAME, SIZE, BYTES), NOT AS A ZIP: BOTH ENDS ARE
# INWAY AND A LAN DOES NOT NEED COMPRESSION. NAMES THAT TRY TO LEAVE THE TEMP
# FOLDER ("..", "C:", "\\") ARE REFUSED.
# ONE STRUCT S_CLIP PER SESSION. THE SEND FUNCTION GIVEN TO CLIP_START() PUTS A
# MESSAGE ON THAT SESSION'S LINK.
*/

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <stdint.h>
/******* """"""""""
 * typedef	uint32_t;
 * typedef	uint8_t;
 */
#include "net.h"
/******* """""""
 * struct S_LINK;
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define CLIP_MAX_TEXT (16U << 20) /* 16 MB OF TEXT */
#define CLIP_MAX_FILES (4ULL << 30) /* 4 GB OF FILES IN ONE COPY */
#define CLIP_CHUNK (64U << 10) /* FILES GO IN 64 KB PIECES */

struct	S_CLIP;

extern struct S_CLIP	*CLIP_START(struct S_LINK *LINK, const char *WHO);
extern void				CLIP_RECEIVE(
	struct S_CLIP *CLIP,
	uint32_t TYPE,
	const uint8_t *DATA,
	uint32_t SIZE
);
extern void				CLIP_FLUSH(struct S_CLIP *CLIP);
extern void				CLIP_STOP(struct S_CLIP *CLIP);

#endif /* !CLIPBOARD_H */
