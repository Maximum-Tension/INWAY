/******************************************************************************\
# H - inway                                      #       Maximum Tension       #
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

#ifndef INWAY_H
#define INWAY_H

/* ************************ [v] WINDOWS VERSION [v] ************************* */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0X0601 /* WINDOWS 7 AND NEWER API */
#endif /* !_WIN32_WINNT */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif /* !WIN32_LEAN_AND_MEAN */

#ifndef COBJMACROS
#define COBJMACROS /* IMONIKER_RELEASE() STYLE COM CALLS IN C */
#endif /* !COBJMACROS */
/* ************************ [^] WINDOWS VERSION [^] ************************* */

/* **************************** [v] INCLUDES [v] **************************** */
#include <stdint.h>
/******* """"""""""
 * typedef	int32_t;
 * typedef	uint16_t;
 * typedef	uint32_t;
 * typedef	uint64_t;
 * typedef	uint8_t;
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* *************************** [v] CONNECTION [v] *************************** */
#define INWAY_PORT 50506 /* TCP PORT OF THE SHARING SIDE */
#define INWAY_MAGIC 0X494E5731 /* "INW1": PROTOCOL NUMBER */
#define INWAY_TILE 64 /* SCREEN IS SENT IN 64X64 TILES */
#define INWAY_FRAME_MS 16 /* UP TO 60 FRAMES A SECOND */
#define INWAY_IDLE_MS 40 /* WHEN NOTHING MOVES */
#define INWAY_ASK_SECONDS 60 /* ACCEPT WINDOW GIVES UP AFTER */
#define INWAY_ADMIN_SECONDS 30 /* NO ANSWER TO "ADMIN RIGHTS?": NORMAL RIGHTS */
#define INWAY_SAS_EVENT L"Global\\INWAY_SAS" /* WORKER -> SERVICE */
#define INWAY_RELAY 0X52454C31 /* "REL1": SENT INSTEAD OF A CODE, SEE BELOW */
#define INWAY_MAX_HOPS 4 /* -C A B C ...: PCS IN BETWEEN, AT MOST */

/*
# REPLIES THE SHARING SIDE SENDS INSTEAD OF A SCREEN SIZE:
# - {MAGIC,  0,  0} THE PERSON CLICKED CANCEL
# - {MAGIC, -1, -1} SOMEONE ELSE IS ALREADY CONNECTED
# - {MAGIC, -2, -2} WRONG CODE
# - {MAGIC, -3, -3} NOBODY CAN CLICK ACCEPT HERE: ONLY THE CODE WORKS
# - {MAGIC, -4, -4} THIS SERVER IS UPDATING ITSELF: TRY AGAIN IN A MOMENT
# "PASS ME ON" (-C A 482382 2.253): INSTEAD OF A CODE, THE VIEWER SENDS
# {INWAY_RELAY, CODE OF A, LENGTH} AND THE NEXT PC'S ADDRESS AS TYPED. A
# CHECKS ITS CODE (OR ASKS FOR ACCEPT), CONNECTS TO THAT PC ON ITS OWN
# NETWORK AND ANSWERS:
# - {MAGIC, -10, -10} CONNECTED: FROM NOW ON A ONLY COPIES THE BYTES, AND IT
#   GOES ON AS IF THE VIEWER WAS CONNECTED TO THE NEXT PC (CODE, HELLO, ...)
# - {MAGIC, -11, -11} A COULD NOT FIND OR REACH THE NEXT PC
# OR ONE OF THE REPLIES ABOVE (WRONG CODE, CANCEL ...). AN INWAY OLDER THAN
# 20.26.11.24 TAKES INWAY_RELAY FOR A WRONG CODE.
*/
#define REPLY_DECLINED 0
#define REPLY_BUSY -1
#define REPLY_WRONG_CODE -2
#define REPLY_NEED_CODE -3
#define REPLY_UPDATING -4
#define REPLY_RELAY_OK -10
#define REPLY_NO_PC -11
/* *************************** [^] CONNECTION [^] *************************** */

/* ********************** [v] MESSAGES ON THE WIRE [v] ********************** */
/*
 * AFTER THE CODE AND THE {MAGIC, W, H} HELLO, BOTH SIDES ONLY SEND MESSAGES:
 * A S_MSG_HDR, THEN "BYTES" BYTES OF DATA. THE SAME FORMAT GOES BOTH WAYS.
 */

#pragma pack(push, 1)
struct	S_MSG_HDR
{
	uint32_t	TYPE; /* MSG_FRAME, MSG_INPUT, MSG_CLIP_... */
	uint32_t	BYTES; /* SIZE OF THE DATA THAT FOLLOWS */
};

struct	S_TILE_HDR
{
	uint16_t	X;
	uint16_t	Y;
	uint16_t	W;
	uint16_t	H;
	uint32_t	LEN; /* SIZE OF THE PACKED PIXELS */
	uint8_t		KIND; /* TILE_EXACT OR TILE_JPEG */
};

struct	S_INPUT_MSG
{
	int32_t	TYPE; /* IN_MOVE, IN_BUTTON, IN_WHEEL, IN_KEY, IN_SAS */
	int32_t	A;
	int32_t	B;
	int32_t	C;
};

struct	S_CLIP_FILE
{
	uint32_t	ID; /* WHICH COPY THIS BELONGS TO */
	uint32_t	IS_FOLDER;
	uint64_t	SIZE;
	/* THEN THE PATH INSIDE THE COPY, UTF-16, ENDING WITH 0 */
};
#pragma pack(pop)

#define MSG_FRAME 1 /* HOST > VIEWER: UINT32 TILE COUNT, THEN TILES */
#define MSG_INPUT 2 /* VIEWER > HOST: STRUCT S_INPUT_MSG */
#define MSG_CLIP_TEXT 3 /* BOTH WAYS: COPIED TEXT, UTF-16 */
#define MSG_CLIP_BEGIN 4 /* BOTH WAYS: UINT32 ID, FILES ARE COMING */
#define MSG_CLIP_FILE 5 /* BOTH WAYS: STRUCT S_CLIP_FILE + PATH */
#define MSG_CLIP_DATA 6 /* BOTH WAYS: UINT32 ID + A PIECE OF THE FILE */
#define MSG_CLIP_END 7 /* BOTH WAYS: UINT32 ID, ALL FILES ARRIVED */
#define MSG_CLIP_ABORT 8 /* BOTH WAYS: UINT32 ID, FORGET THAT COPY */

#define TILE_EXACT 0 /* LOSSLESS, SEE CODEC.C */
#define TILE_JPEG 1 /* JPEG, USED WHILE THE SCREEN MOVES */

#define IN_MOVE 1 /* A = X, B = Y */
#define IN_BUTTON 2 /* A = BUTTON 0-2, B = DOWN */
#define IN_WHEEL 3 /* A = WHEEL DELTA */
#define IN_KEY 4 /* A = VIRTUAL KEY, B = DOWN, C = EXTENDED */
#define IN_SAS 5 /* CTRL + ALT + DEL */
/* ********************** [^] MESSAGES ON THE WIRE [^] ********************** */

/* *********************** [v] CONNECTION RESULTS [v] *********************** */
enum	E_RESULT
{
	CR_ENDED, /* CONNECTED, THEN THE WINDOW WAS CLOSED */
	CR_NO_PC, /* NOBODY ANSWERED ON THAT ADDRESS */
	CR_WRONG_CODE,
	CR_NEED_CODE, /* A SERVER: ONLY ITS CODE WORKS */
	CR_DECLINED, /* THEY CLICKED CANCEL */
	CR_BUSY,
	CR_NO_ANSWER, /* NOBODY CLICKED ACCEPT IN TIME */
	CR_UPDATING, /* A SERVER WAS STILL UPDATING ITSELF AFTER A MINUTE */
	CR_DROPPED,
	CR_VERSION,
	CR_ERROR
};
/* *********************** [^] CONNECTION RESULTS [^] *********************** */

#endif /* !INWAY_H */
