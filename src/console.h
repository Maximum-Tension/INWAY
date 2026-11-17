/******************************************************************************\
# H - console                                    #       Maximum Tension       #
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
# THE BLACK WINDOW: COLOURS, BOXES, QUESTIONS AND SHORT NOTICES. ALL TEXT GOES
# THROUGH A SMALL COLOUR MARKUP: A LETTER IN CURLY BRACKETS. C IS CYAN, Y
# YELLOW, G GREEN, R RED, W WHITE, D GREY AND 0 BACK TO NORMAL. A LOWER CASE
# LETTER IS THE BRIGHT COLOUR, AN UPPER CASE ONE THE PLAIN COLOUR. ON AN OLD
# CONSOLE WITHOUT COLOURS THE MARKS ARE REMOVED AND THE BOXES ARE DRAWN WITH + -
# | CHARACTERS.
# BOX() WRAPS LONG LINES BY ITSELF, SO TRANSLATIONS NEVER BREAK A BOX.
# SAY(), ASK() AND NOTIFY() MAY BE CALLED FROM ANY THREAD: A NOTICE FROM THE
# SHARING SIDE PRINTS THE OPEN QUESTION AGAIN UNDER ITSELF.
*/

#ifndef CONSOLE_H
#define CONSOLE_H

/* **************************** [v] INCLUDES [v] **************************** */
#include <windows.h>
/******* """""""""""
 * typedef	CRITICAL_SECTION;
 */
#include <string.h>
/******* """"""""""
 * typedef	size_t;
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT;
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define BOX_INNER 44 /* INSIDE WIDTH OF EVERY BOX */

extern int				G_VT; /* 1 = COLOURS AND BOX LINES WORK */
extern CRITICAL_SECTION	G_CONSOLE_LOCK;
extern char				G_HOST_STATUS[64]; /* WHO CONTROLS THIS PC NOW */

extern void			CONSOLE_INIT(void);
extern void			CONSOLE_SETUP(void);
extern void			SAY(const char *MARKUP);
extern void			SAYF(const char *FORMAT, ...);
extern void			ASK(const char *PROMPT, char *OUTPUT, size_t CAP);
extern void			NOTIFY(const char *FORMAT, ...);
extern void			BOX(
	struct S_TEXT *OUTPUT,
	const char *TITLE,
	const char *const *LINES,
	int COUNT,
	const char *COLOUR,
	int DOUBLE_LINE
);
extern int			VISIBLE_LENGTH(const char *MARKUP);
extern const char	*SYM(const char *FANCY, const char *PLAIN);
extern const char	*CLEAR_SCREEN(void);

#endif /* !CONSOLE_H */
