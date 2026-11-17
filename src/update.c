/******************************************************************************\
# C - UPDATE                                     #       Maximum Tension       #
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
|*| GITHUB RELEASE CHECK, DOWNLOAD, SHA-256 CHECK AND THE EXE SWAP. SEE      |*|
|*| UPDATE.H FOR THE STEPS.                                                  |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| void UPDATE_CHECK(void);                                            : *|
|* : 3|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * INWAY_VERSION_A, SAYF(), struct S_TEXT
 */
#include <winhttp.h>
/******* """""""""""
 * WinHttpOpen(), WinHttpReadData()
 */
#include <bcrypt.h>
/******* """"""""""
 * BCryptHashData()
 */
/* **************************** [^] INCLUDES [^] **************************** */

#define HTTP_AGENT	L"INWAY/" L"" INWAY_VERSION_TEXT

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                           HTTP                           |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# GET URL. THE BODY GOES INTO BODY OR, WHEN FILE IS GIVEN, INTO THAT FILE.
# REDIRECTS ARE FOLLOWED (A RELEASE DOWNLOAD MOVES TO ANOTHER SERVER).
# RETURNS THE HTTP STATUS, 0 WHEN THERE WAS NO ANSWER AT ALL.
*/
static DWORD
	HTTP_GET(
		const WCHAR *URL,
		struct S_TEXT *BODY,
		HANDLE FILE_HANDLE,
		int API,
		int TIMEOUT
	)
{
	URL_COMPONENTS	PARTS;
	HINTERNET	SESSION;
	HINTERNET	CONNECT;
	HINTERNET	REQUEST;
	WCHAR		HOST[256];
	WCHAR		PATH[2048];
	char		PIECE[16384];
	DWORD		STATUS;
	DWORD		SIZE;
	DWORD		GOT;
	DWORD		DONE;
	int		OK;

	memset(&PARTS, 0, sizeof(PARTS));
	PARTS.dwStructSize = sizeof(PARTS);
	PARTS.lpszHostName = HOST;
	PARTS.dwHostNameLength = 256;
	PARTS.lpszUrlPath = PATH;
	PARTS.dwUrlPathLength = 2048;
	if (!WinHttpCrackUrl(URL, 0, 0, &PARTS))
	{
		return (0);
	}
	STATUS = 0;
	SESSION = WinHttpOpen(
		HTTP_AGENT,
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS,
		0
	);
	if (!SESSION)
	{
		return (0);
	}
	WinHttpSetTimeouts(SESSION, 3000, 3000, 4000, TIMEOUT);
	CONNECT = WinHttpConnect(SESSION, HOST, PARTS.nPort, 0);
	REQUEST = CONNECT ? WinHttpOpenRequest(
		CONNECT,
		L"GET",
		PATH,
		NULL,
		WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
		PARTS.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0
	) : NULL;
	if (REQUEST && API)
	{
		WinHttpAddRequestHeaders(
			REQUEST,
			L"Accept: application/vnd.github+json\r\n"
			L"X-GitHub-Api-Version: 2022-11-28",
			(DWORD)-1,
			WINHTTP_ADDREQ_FLAG_ADD
		);
	}
	if (REQUEST && WinHttpSendRequest(
		REQUEST,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0,
		WINHTTP_NO_REQUEST_DATA,
		0,
		0,
		0
	) && WinHttpReceiveResponse(REQUEST, NULL))
	{
		SIZE = sizeof(STATUS);
		WinHttpQueryHeaders(
			REQUEST,
			WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
			WINHTTP_HEADER_NAME_BY_INDEX,
			&STATUS,
			&SIZE,
			WINHTTP_NO_HEADER_INDEX
		);
		OK = (STATUS == 200);
		while (OK && WinHttpReadData(REQUEST, PIECE, sizeof(PIECE), &GOT) &&
			GOT)
		{
			if (FILE_HANDLE)
			{
				OK = WriteFile(
					FILE_HANDLE,
					PIECE,
					GOT,
					&DONE,
					NULL
				) && DONE == GOT;
			}
			else if (BODY->LEN + GOT > (2U << 20))
			{
				OK = 0; /* A RELEASE PAGE IS NEVER THAT BIG */
			}
			else
			{
				TEXT_ADDN(BODY, PIECE, GOT);
			}
		}
		if (!OK)
		{
			STATUS = 0;
		}
	}
	if (REQUEST)
	{
		WinHttpCloseHandle(REQUEST);
	}
	if (CONNECT)
	{
		WinHttpCloseHandle(CONNECT);
	}
	WinHttpCloseHandle(SESSION);
	return (STATUS);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                           HTTP                           |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                   READING THE RELEASE                    |  \\//  */
/*   \/   #==========================================================#   \/   */
/* THE STRING VALUE OF THE NEXT "KEY" AFTER FROM. RETURNS WHERE IT ENDS. */
static const char
	*JSON_STRING(
		const char *FROM,
		const char *KEY,
		char *OUTPUT,
		size_t CAP
	)
{
	char		NEEDLE[64];
	const char	*P;
	size_t		N;

	snprintf(NEEDLE, sizeof(NEEDLE), "\"%s\"", KEY);
	P = strstr(FROM, NEEDLE);
	if (!P)
	{
		return (NULL);
	}
	P += strlen(NEEDLE);
	while (*P == ' ' || *P == '\t' || *P == '\r' || *P == '\n' || *P == ':')
	{
		++P;
	}
	if (*P != '"') /* NULL OR NOT A STRING */
	{
		OUTPUT[0] = '\0';
		return (P);
	}
	++P;
	N = 0;
	while (*P && *P != '"' && N + 1 < CAP)
	{
		OUTPUT[N++] = *P++;
	}
	OUTPUT[N] = '\0';
	return (P);
}

/*
# FINDS THE INWAY.EXE ASSET: ITS DOWNLOAD URL ENDS WITH "/INWAY.EXE", AND ITS
# "DIGEST" IS THE ONE BETWEEN THE PREVIOUS ASSET'S URL AND THIS URL.
*/
static int
	FIND_ASSET(
		const char *JSON,
		char *URL,
		size_t URL_CAP,
		char *SHA,
		size_t SHA_CAP
	)
{
	const char	*P;
	const char	*AFTER;
	const char	*D;
	const char	*KEY_AT;
	const char	*LAST_END;
	char		VALUE[600];
	char		DIGEST[100];
	size_t		LEN;
	size_t		NAME_LEN;

	NAME_LEN = strlen(UPDATE_ASSET);
	LAST_END = JSON;
	P = JSON;
	while ((KEY_AT = strstr(P, "\"browser_download_url\"")) != NULL)
	{
		AFTER = JSON_STRING(
			KEY_AT,
			"browser_download_url",
			VALUE,
			sizeof(VALUE)
		);
		if (!AFTER)
		{
			return (0);
		}
		LEN = strlen(VALUE);
		if (LEN > NAME_LEN && VALUE[LEN - NAME_LEN - 1] == '/' &&
			_stricmp(VALUE + LEN - NAME_LEN, UPDATE_ASSET) == 0)
		{
			DIGEST[0] = '\0';
			for (D = strstr(LAST_END, "\"digest\""); D && D < KEY_AT;
				D = strstr(D + 1, "\"digest\""))
			{
				JSON_STRING(D, "digest", DIGEST, sizeof(DIGEST));
			}
			snprintf(URL, URL_CAP, "%s", VALUE);
			if (_strnicmp(DIGEST, "sha256:", 7) != 0 ||
				strlen(DIGEST + 7) != 64)
			{
				return (0); /* GITHUB GAVE NO SHA-256: DO NOT TRUST IT */
			}
			snprintf(SHA, SHA_CAP, "%s", DIGEST + 7);
			return (1);
		}
		LAST_END = AFTER;
		P = AFTER;
	}
	return (0);
}

/* "V20.26.11.17" -> 1 WHEN IT IS NEWER THAN THIS EXE */
static int
	IS_NEWER(const char *TAG)
{
	const int	MINE[4] = {
		INWAY_VERSION_A, INWAY_VERSION_B, INWAY_VERSION_C, INWAY_VERSION_D
	};
	int		THEIRS[4];
	int		I;
	const char	*P;

	P = TAG;
	while (*P && (*P < '0' || *P > '9'))
	{
		++P;
	}
	for (I = 0; I < 4; ++I)
	{
		THEIRS[I] = atoi(P);
		while (*P >= '0' && *P <= '9')
		{
			++P;
		}
		if (*P == '.')
		{
			++P;
		}
	}
	for (I = 0; I < 4; ++I)
	{
		if (THEIRS[I] != MINE[I])
		{
			return (THEIRS[I] > MINE[I]);
		}
	}
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                   READING THE RELEASE                    |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                          FILES                           |  \\//  */
/*   \/   #==========================================================#   \/   */
/* SHA-256 OF A FILE AS 64 LOWER CASE HEX LETTERS */
static int
	FILE_SHA256(const WCHAR *PATH, char *OUTPUT)
{
	static const char	HEX[] = "0123456789abcdef";
	BCRYPT_ALG_HANDLE	ALG;
	BCRYPT_HASH_HANDLE	HASH;
	HANDLE		FILE_HANDLE;
	uint8_t		DIGEST[32];
	uint8_t		*PIECE;
	DWORD		GOT;
	int		OK;
	int		I;

	OK = 0;
	FILE_HANDLE = CreateFileW(
		PATH,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		return (0);
	}
	PIECE = (uint8_t *)malloc(1 << 16);
	if (PIECE && BCryptOpenAlgorithmProvider(
		&ALG,
		BCRYPT_SHA256_ALGORITHM,
		NULL,
		0
	) == 0)
	{
		if (BCryptCreateHash(ALG, &HASH, NULL, 0, NULL, 0, 0) == 0)
		{
			OK = 1;
			while (OK && ReadFile(FILE_HANDLE, PIECE, 1 << 16, &GOT, NULL) &&
				GOT)
			{
				OK = (BCryptHashData(HASH, PIECE, GOT, 0) == 0);
			}
			OK = OK && BCryptFinishHash(HASH, DIGEST, sizeof(DIGEST), 0) == 0;
			BCryptDestroyHash(HASH);
		}
		BCryptCloseAlgorithmProvider(ALG, 0);
	}
	free(PIECE);
	CloseHandle(FILE_HANDLE);
	for (I = 0; OK && I < 32; ++I)
	{
		OUTPUT[I * 2] = HEX[DIGEST[I] >> 4];
		OUTPUT[I * 2 + 1] = HEX[DIGEST[I] & 15];
	}
	OUTPUT[64] = '\0';
	return (OK);
}

/* "%TMP%" WITHOUT THE LAST "\" */
static void
	TEMP_DIR(WCHAR *OUTPUT)
{
	DWORD	LEN;

	LEN = GetTempPathW(MAX_PATH, OUTPUT);
	while (LEN && OUTPUT[LEN - 1] == L'\\')
	{
		OUTPUT[--LEN] = L'\0';
	}
}

/* CAN WE REPLACE FILES NEXT TO THIS EXE? (NOT IN PROGRAM FILES) */
static int
	CAN_WRITE_HERE(const WCHAR *EXE)
{
	WCHAR	TEST[MAX_PATH + 32];
	WCHAR	*SLASH;
	HANDLE	FILE_HANDLE;

	swprintf(TEST, MAX_PATH + 32, L"%ls", EXE);
	SLASH = wcsrchr(TEST, L'\\');
	if (!SLASH)
	{
		return (0);
	}
	swprintf(
		SLASH + 1,
		(size_t)(TEST + MAX_PATH + 32 - SLASH - 1),
		L"INWAY-%lu.tmp",
		(unsigned long)GetCurrentProcessId()
	);
	FILE_HANDLE = CreateFileW(
		TEST,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
		NULL
	);
	if (FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		return (0);
	}
	CloseHandle(FILE_HANDLE);
	return (1);
}

/* LEFTOVERS OF AN EARLIER UPDATE THAT COULD NOT BE MOVED */
static void
	CLEAN_TEMP(void)
{
	WIN32_FIND_DATAW	FD;
	HANDLE		FIND;
	WCHAR		DIR[MAX_PATH];
	WCHAR		PATH[MAX_PATH * 2];

	TEMP_DIR(DIR);
	swprintf(PATH, MAX_PATH * 2, L"%ls\\INWAY-NEW-*.exe", DIR);
	FIND = FindFirstFileW(PATH, &FD);
	if (FIND == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	do
	{
		swprintf(PATH, MAX_PATH * 2, L"%ls\\%ls", DIR, FD.cFileName);
		DeleteFileW(PATH); /* FAILS QUIETLY WHILE IT RUNS */
	}
	while (FindNextFileW(FIND, &FD));
	FindClose(FIND);
}

/* THE COMMAND LINE WITHOUT THE EXE NAME, TO PASS IT ON */
static const WCHAR
	*ARGUMENTS(void)
{
	const WCHAR	*P;

	P = GetCommandLineW();
	if (*P == L'"')
	{
		++P;
		while (*P && *P != L'"')
		{
			++P;
		}
		if (*P)
		{
			++P;
		}
	}
	else
	{
		while (*P && *P != L' ' && *P != L'\t')
		{
			++P;
		}
	}
	while (*P == L' ' || *P == L'\t')
	{
		++P;
	}
	return (P);
}

static void
	NOTE_PATH(DWORD PID, WCHAR *OUTPUT)
{
	WCHAR	DIR[MAX_PATH];

	TEMP_DIR(DIR);
	swprintf(
		OUTPUT,
		MAX_PATH * 2,
		L"%ls\\INWAY-UPDATE-%lu.txt",
		DIR,
		(unsigned long)PID
	);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                          FILES                           |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                        THE UPDATE                        |  \\//  */
/*   \/   #==========================================================#   \/   */
void
	UPDATE_CHECK(void)
{
	struct S_TEXT		BODY = {0};
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;
	WCHAR			EXE[MAX_PATH];
	WCHAR			DIR[MAX_PATH];
	WCHAR			NEW_EXE[MAX_PATH * 2];
	WCHAR			NOTE[MAX_PATH * 2];
	WCHAR			*COMMAND;
	WCHAR			*WIDE_URL;
	char			TAG[64];
	char			URL[600];
	char			SHA[80];
	char			GOT[80];
	char			*NOTE_TEXT;
	size_t			CHARS;
	HANDLE			FILE_HANDLE;
	DWORD			DONE;
	int			OK;

	CLEAN_TEMP();
	GetModuleFileNameW(NULL, EXE, MAX_PATH);
	if (!CAN_WRITE_HERE(EXE) || HTTP_GET(
		L"https://api.github.com/repos/" UPDATE_REPO L"/releases/latest",
		&BODY,
		NULL,
		1,
		5000
	) != 200 || !BODY.DATA)
	{
		TEXT_FREE(&BODY);
		return ;
	}
	OK = JSON_STRING(BODY.DATA, "tag_name", TAG, sizeof(TAG)) &&
		IS_NEWER(TAG) &&
		FIND_ASSET(BODY.DATA, URL, sizeof(URL), SHA, sizeof(SHA));
	TEXT_FREE(&BODY);
	if (!OK)
	{
		return ;
	}
	SAY("\n  {c}");
	SAYF(LANG(STR_U_FOUND), TAG);
	SAY("{0}\n");
	/* 1. DOWNLOAD INTO %TMP% AND CHECK THE SHA-256 GITHUB GAVE */
	TEMP_DIR(DIR);
	swprintf(NEW_EXE, MAX_PATH * 2, L"%ls\\INWAY-NEW-%lu.exe", DIR,
		(unsigned long)GetTickCount());
	FILE_HANDLE = CreateFileW(
		NEW_EXE,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL
	);
	WIDE_URL = UTF8_TO_WIDE(URL);
	OK = (FILE_HANDLE != INVALID_HANDLE_VALUE && WIDE_URL &&
		HTTP_GET(WIDE_URL, NULL, FILE_HANDLE, 0, 60000) == 200);
	free(WIDE_URL);
	if (FILE_HANDLE != INVALID_HANDLE_VALUE)
	{
		CloseHandle(FILE_HANDLE);
	}
	OK = OK && FILE_SHA256(NEW_EXE, GOT) && _stricmp(GOT, SHA) == 0;
	if (!OK)
	{
		DeleteFileW(NEW_EXE);
		SAYF("  {r}%s{0}\n", LANG(STR_U_FAILED));
		return ;
	}
	/* 2. START IT PAUSED, LEAVE IT THE NOTE, THEN LET IT GO */
	CHARS = (size_t)lstrlenW(NEW_EXE) + (size_t)lstrlenW(ARGUMENTS()) + 64;
	COMMAND = (WCHAR *)malloc(CHARS * sizeof(WCHAR));
	if (!COMMAND)
	{
		DeleteFileW(NEW_EXE);
		return ;
	}
	swprintf(
		COMMAND,
		CHARS,
		L"\"%ls\" -update-finish %ls",
		NEW_EXE,
		ARGUMENTS()
	);
	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(SI);
	memset(&PI, 0, sizeof(PI));
	if (!CreateProcessW(
		NULL,
		COMMAND,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL,
		&SI,
		&PI
	))
	{
		free(COMMAND);
		DeleteFileW(NEW_EXE);
		SAYF("  {r}%s{0}\n", LANG(STR_U_FAILED));
		return ;
	}
	free(COMMAND);
	NOTE_PATH(PI.dwProcessId, NOTE);
	NOTE_TEXT = WIDE_TO_UTF8(EXE);
	FILE_HANDLE = CreateFileW(
		NOTE,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL
	);
	if (FILE_HANDLE != INVALID_HANDLE_VALUE && NOTE_TEXT)
	{
		snprintf(
			TAG,
			sizeof(TAG),
			"%lu\r\n",
			(unsigned long)GetCurrentProcessId()
		);
		WriteFile(FILE_HANDLE, TAG, (DWORD)strlen(TAG), &DONE, NULL);
		WriteFile(
			FILE_HANDLE,
			NOTE_TEXT,
			(DWORD)strlen(NOTE_TEXT),
			&DONE,
			NULL
		);
		CloseHandle(FILE_HANDLE);
	}
	free(NOTE_TEXT);
	ResumeThread(PI.hThread);
	CloseHandle(PI.hThread);
	CloseHandle(PI.hProcess);
	ExitProcess(0); /* THE NEW ONE TAKES OVER */
}

/* WE ARE THE NEW EXE IN %TMP%: TAKE THE OLD ONE'S PLACE */
void
	UPDATE_FINISH(void)
{
	WCHAR		NOTE[MAX_PATH * 2];
	WCHAR		SELF[MAX_PATH];
	WCHAR		*OLD;
	char		TEXT[2048];
	char		*LINE;
	HANDLE		FILE_HANDLE;
	HANDLE		PROCESS;
	DWORD		GOT;
	DWORD		PID;
	int		I;
	int		GONE;

	NOTE_PATH(GetCurrentProcessId(), NOTE);
	FILE_HANDLE = CreateFileW(
		NOTE,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	GOT = 0;
	ReadFile(FILE_HANDLE, TEXT, sizeof(TEXT) - 1, &GOT, NULL);
	CloseHandle(FILE_HANDLE);
	TEXT[GOT] = '\0';
	PID = (DWORD)strtoul(TEXT, &LINE, 10);
	while (*LINE == '\r' || *LINE == '\n')
	{
		++LINE;
	}
	LINE[strcspn(LINE, "\r\n")] = '\0';
	OLD = UTF8_TO_WIDE(LINE);
	PROCESS = OpenProcess(SYNCHRONIZE, FALSE, PID);
	if (PROCESS)
	{
		WaitForSingleObject(PROCESS, 15000);
		CloseHandle(PROCESS);
	}
	GetModuleFileNameW(NULL, SELF, MAX_PATH);
	/*
	# REPLACE_EXISTING DELETES THE OLD EXE AND PUTS THIS ONE IN ITS PLACE IN
	# ONE STEP, SO THE OLD ONE IS NEVER LOST WHEN THE MOVE FAILS. ON ANOTHER
	# DRIVE IT COPIES AND THE TEMP FILE IS CLEANED UP AT THE NEXT START.
	*/
	GONE = 0;
	for (I = 0; OLD && *OLD && I < 50 && !GONE; ++I)
	{
		GONE = MoveFileExW(
			SELF,
			OLD,
			MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED |
			MOVEFILE_WRITE_THROUGH
		);
		if (!GONE)
		{
			Sleep(100); /* THE OLD ONE MAY STILL BE CLOSING */
		}
	}
	if (GONE)
	{
		SAY("\n  {g}");
		SAYF(LANG(STR_U_DONE), INWAY_VERSION_TEXT);
		SAY("{0}\n");
	}
	else
	{
		SAYF("\n  {r}%s{0}\n", LANG(STR_U_FAILED));
	}
	DeleteFileW(NOTE);
	free(OLD);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                        THE UPDATE                        |  //\\  */
/* '""""' #==========================================================# '""""' */
