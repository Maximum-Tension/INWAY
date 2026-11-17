/******************************************************************************\
# H - TEXT                                       #       Maximum Tension       #
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
|*|                                   TEXT                                   |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| A GROWING TEXT BUFFER, SO SCREENS AND MESSAGES CAN BE BUILT PIECE BY     |*|
|*| PIECE WITHOUT FIXED SIZE ARRAYS, AND THE UTF-8 <-> UTF-16 HELPERS        |*|
|*| WINDOWS NEEDS FOR TURKISH LETTERS IN TITLES, MENUS AND MESSAGE BOXES.    |*|
|*| A STRUCT S_TEXT STARTS AS {0}. TEXT_FREE() GIVES THE MEMORY BACK.        |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| void   TEXT_ADD(struct S_TEXT *TEXT, const char *STRING);          : *|
|* :  3| void   TEXT_ADDN(struct S_TEXT *TEXT, const char *STRING,          : *|
|* :  4|                  size_t N);                                        : *|
|* :  5| void   TEXT_ADDF(struct S_TEXT *TEXT, const char *FORMAT, ...);    : *|
|* :  6| void   TEXT_ADDV(struct S_TEXT *TEXT, const char *FORMAT,          : *|
|* :  7|                  va_list A);                                       : *|
|* :  8| void   TEXT_FREE(struct S_TEXT *TEXT);                             : *|
|* :  9| WCHAR *UTF8_TO_WIDE(const char *STRING);                           : *|
|* : 10| char  *WIDE_TO_UTF8(const WCHAR *STRING);                          : *|
|* : 11|                                                                    : *|
|* :                                                                        : *|
|* : EXAMPLES:                                                              : *|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| struct S_TEXT  T = {0};                                            : *|
|* :  3|                                                                    : *|
|* :  4| TEXT_ADDF(&T, "%s is controlling this PC", "2.253");               : *|
|* :  5| SAY(T.DATA);                                                       : *|
|* :  6| TEXT_FREE(&T);                                                     : *|
|* :  7|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef TEXT_H
#define TEXT_H

struct	S_TEXT
{
	char	*DATA;	/* ALWAYS ENDS WITH '\0' ONCE SOMETHING WAS ADDED */
	size_t	LEN;
	size_t	CAP;
};

extern void	TEXT_ADD(struct S_TEXT *TEXT, const char *STRING);
extern void	TEXT_ADDN(struct S_TEXT *TEXT, const char *STRING, size_t N);
extern void	TEXT_ADDF(struct S_TEXT *TEXT, const char *FORMAT, ...);
extern void	TEXT_ADDV(struct S_TEXT *TEXT, const char *FORMAT, va_list A);
extern void	TEXT_FREE(struct S_TEXT *TEXT);
extern WCHAR	*UTF8_TO_WIDE(const char *STRING);
extern char	*WIDE_TO_UTF8(const WCHAR *STRING);

#endif /* !TEXT_H */
