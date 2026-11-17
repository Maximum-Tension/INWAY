/******************************************************************************\
# C - text                                       #       Maximum Tension       #
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
# GROWING TEXT BUFFER AND UTF-8 / UTF-16 CONVERSION. SEE TEXT.H.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define WIN32_LEAN_AND_MEAN
 */
#include <windows.h>
/******* """""""""""
 * #define CP_UTF8
 * typedef	WCHAR;
 * int		MultiByteToWideChar(UINT, DWORD, LPCCH, int, LPWSTR, int);
 * int		WideCharToMultiByte(
 * 	UINT,
 * 	DWORD,
 * 	LPCWCH,
 * 	int,
 * 	LPSTR,
 * 	int,
 * 	LPCCH,
 * 	LPBOOL
 * );
 */
#include <stdio.h>
/******* """""""""
 * int	vsnprintf(char *, size_t, const char *, va_list);
 */
#include <stdlib.h>
/******* """"""""""
 * void	*calloc(size_t, size_t);
 * void	free(void *);
 * void	*malloc(size_t);
 * void	*realloc(void *, size_t);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * typedef	size_t;
 * void		*memcpy(void *, const void *, size_t);
 * size_t	strlen(const char *);
 */
#include <stdarg.h>
/******* """"""""""
 * #define va_copy(dest, src)
 * #define va_end(ap)
 * #define va_start(ap, param)
 * typedef	va_list;
 */
#include "text.h"
/******* """"""""
 * struct S_TEXT;
 */
/* **************************** [^] INCLUDES [^] **************************** */

void
	TEXT_ADDN(struct S_TEXT *TEXT, const char *STRING, size_t N)
{
	size_t	NEW_CAP;
	char	*NEW_DATA;

	if (TEXT->LEN + N + 1 > TEXT->CAP)
	{
		NEW_CAP = TEXT->CAP ? TEXT->CAP : 256;
		while (TEXT->LEN + N + 1 > NEW_CAP)
		{
			NEW_CAP *= 2;
		}
		NEW_DATA = (char *)realloc(TEXT->DATA, NEW_CAP);
		if (!NEW_DATA)
		{
			return ;
		}
		TEXT->DATA = NEW_DATA;
		TEXT->CAP = NEW_CAP;
	}
	memcpy(TEXT->DATA + TEXT->LEN, STRING, N);
	TEXT->LEN += N;
	TEXT->DATA[TEXT->LEN] = '\0';
}

void
	TEXT_ADD(struct S_TEXT *TEXT, const char *STRING)
{
	TEXT_ADDN(TEXT, STRING, strlen(STRING));
}

void
	TEXT_ADDV(struct S_TEXT *TEXT, const char *FORMAT, va_list A)
{
	va_list	COPY;
	int		SIZE;
	char	SMALL[512];
	char	*BIG;

	va_copy(COPY, A);
	SIZE = vsnprintf(SMALL, sizeof(SMALL), FORMAT, COPY);
	va_end(COPY);
	if (SIZE < 0)
	{
		return ;
	}
	if ((size_t)SIZE < sizeof(SMALL))
	{
		TEXT_ADDN(TEXT, SMALL, (size_t)SIZE);
		return ;
	}
	BIG = (char *)malloc((size_t)SIZE + 1);
	if (!BIG)
	{
		return ;
	}
	vsnprintf(BIG, (size_t)SIZE + 1, FORMAT, A);
	TEXT_ADDN(TEXT, BIG, (size_t)SIZE);
	free(BIG);
}

void
	TEXT_ADDF(struct S_TEXT *TEXT, const char *FORMAT, ...)
{
	va_list	A;

	va_start(A, FORMAT);
	TEXT_ADDV(TEXT, FORMAT, A);
	va_end(A);
}

void
	TEXT_FREE(struct S_TEXT *TEXT)
{
	free(TEXT->DATA);
	TEXT->DATA = NULL;
	TEXT->LEN = 0;
	TEXT->CAP = 0;
}

WCHAR
	*UTF8_TO_WIDE(const char *STRING)
{
	int		SIZE;
	WCHAR	*OUTPUT;

	SIZE = MultiByteToWideChar(CP_UTF8, 0, STRING, -1, NULL, 0);
	if (SIZE <= 0)
	{
		SIZE = 1;
	}
	OUTPUT = (WCHAR *)calloc((size_t)SIZE, sizeof(WCHAR));
	if (OUTPUT)
	{
		MultiByteToWideChar(CP_UTF8, 0, STRING, -1, OUTPUT, SIZE);
	}
	return (OUTPUT);
}

char
	*WIDE_TO_UTF8(const WCHAR *STRING)
{
	int		SIZE;
	char	*OUTPUT;

	SIZE = WideCharToMultiByte(CP_UTF8, 0, STRING, -1, NULL, 0, NULL, NULL);
	if (SIZE <= 0)
	{
		SIZE = 1;
	}
	OUTPUT = (char *)calloc((size_t)SIZE, 1);
	if (OUTPUT)
	{
		WideCharToMultiByte(CP_UTF8, 0, STRING, -1, OUTPUT, SIZE, NULL, NULL);
	}
	return (OUTPUT);
}
