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
 * BCryptHashData(), BCryptGenRandom()
 */
#include <iphlpapi.h>
/******* """"""""""""
 * MIB_TCPTABLE_OWNER_PID (FUNCTION LOADED LATER)
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

/*
# "20.26.11.17-MK1151" -> 1 WHEN ITS DATE IS THE SAME AS THIS EXE'S VERSION OR
# NEWER. ONLY THE FIRST FOUR NUMBERS COUNT; WHAT COMES AFTER THEM (LIKE
# "-MK1151") CAN BE ANYTHING. SAME DAY RELEASES ARE TOLD APART BY SHA-256.
*/
static int
	NOT_OLDER(const char *TAG)
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
	return (1);
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

/*
# THE COPY IN "C:\PROGRAM FILES\INWAY" BELONGS TO THE BACKGROUND SERVICE,
# WHICH KEEPS IT OPEN. NOW THAT INWAY RUNS AS ADMIN IT COULD WRITE THERE, BUT
# THE SWAP WOULD FAIL ON EVERY START, SO THAT COPY IS LEFT TO -INSTALL.
*/
static int
	IS_SERVICE_COPY(const WCHAR *EXE)
{
	WCHAR	DIR[MAX_PATH];
	WCHAR	MINE[MAX_PATH];
	WCHAR	*SLASH;

	if (!GetEnvironmentVariableW(L"ProgramFiles", DIR, MAX_PATH - 8))
	{
		return (0);
	}
	lstrcatW(DIR, L"\\INWAY");
	lstrcpynW(MINE, EXE, MAX_PATH);
	SLASH = wcsrchr(MINE, L'\\');
	if (!SLASH)
	{
		return (0);
	}
	*SLASH = L'\0';
	return (lstrcmpiW(DIR, MINE) == 0);
}

/*
# AN EXE NEXT TO "SRC\INWAY.H" IS A TEST BUILD IN THE SOURCE FOLDER. IT MUST
# NOT REPLACE ITSELF WITH THE RELEASE THE MOMENT IT IS STARTED.
*/
static int
	IS_SOURCE_FOLDER(const WCHAR *EXE)
{
	WCHAR	PATH[MAX_PATH + 16];
	WCHAR	*SLASH;

	lstrcpynW(PATH, EXE, MAX_PATH);
	SLASH = wcsrchr(PATH, L'\\');
	if (!SLASH)
	{
		return (0);
	}
	lstrcpyW(SLASH + 1, L"src\\inway.h");
	return (GetFileAttributesW(PATH) != INVALID_FILE_ATTRIBUTES);
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
/*
# THE LATEST RELEASE. 1 = UPDATE: ITS INWAY.EXE HAS ANOTHER SHA-256 THAN THE
# RUNNING EXE, AND ITS DATE IS NOT OLDER. 0 = NOTHING TO DO. -1 = GITHUB
# COULD NOT BE ASKED. THE RUNNING EXE IS HASHED (ABOUT A MILLISECOND), SO A
# PACKED AND SIGNED RELEASE MATCHES ITS OWN DIGEST EXACTLY.
*/
static int
	LATEST_RELEASE(
		char *TAG,
		size_t TAG_CAP,
		char *URL,
		size_t URL_CAP,
		char *SHA,
		size_t SHA_CAP
	)
{
	struct S_TEXT	BODY = {0};
	WCHAR		EXE[MAX_PATH];
	char		MINE[80];
	int		OK;

	if (HTTP_GET(
		L"https://api.github.com/repos/" UPDATE_REPO L"/releases/latest",
		&BODY,
		NULL,
		1,
		5000
	) != 200 || !BODY.DATA)
	{
		TEXT_FREE(&BODY);
		return (-1);
	}
	OK = JSON_STRING(BODY.DATA, "tag_name", TAG, TAG_CAP) &&
		NOT_OLDER(TAG) &&
		FIND_ASSET(BODY.DATA, URL, URL_CAP, SHA, SHA_CAP);
	TEXT_FREE(&BODY);
	if (!OK)
	{
		return (0);
	}
	GetModuleFileNameW(NULL, EXE, MAX_PATH);
	if (FILE_SHA256(EXE, MINE) && _stricmp(MINE, SHA) == 0)
	{
		return (0); /* THIS IS ALREADY THAT EXACT FILE */
	}
	return (1);
}

/* DOWNLOADS URL TO TARGET AND KEEPS IT ONLY WHEN ITS SHA-256 IS RIGHT */
static int
	DOWNLOAD_VERIFIED(const char *URL, const char *SHA, const WCHAR *TARGET)
{
	HANDLE	FILE_HANDLE;
	WCHAR	*WIDE_URL;
	char	GOT[80];
	int	OK;

	FILE_HANDLE = CreateFileW(
		TARGET,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL
	);
	if (FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		return (0);
	}
	WIDE_URL = UTF8_TO_WIDE(URL);
	OK = WIDE_URL && HTTP_GET(WIDE_URL, NULL, FILE_HANDLE, 0, 60000) == 200;
	free(WIDE_URL);
	CloseHandle(FILE_HANDLE);
	OK = OK && FILE_SHA256(TARGET, GOT) && _stricmp(GOT, SHA) == 0;
	if (!OK)
	{
		DeleteFileW(TARGET);
	}
	return (OK);
}

void
	UPDATE_CHECK(void)
{
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;
	WCHAR			EXE[MAX_PATH];
	WCHAR			DIR[MAX_PATH];
	WCHAR			NEW_EXE[MAX_PATH * 2];
	WCHAR			NOTE[MAX_PATH * 2];
	WCHAR			*COMMAND;
	char			PID_LINE[32];
	char			TAG[64];
	char			URL[600];
	char			SHA[80];
	char			*NOTE_TEXT;
	size_t			CHARS;
	HANDLE			FILE_HANDLE;
	DWORD			DONE;

	CLEAN_TEMP();
	GetModuleFileNameW(NULL, EXE, MAX_PATH);
	if (IS_SERVICE_COPY(EXE) || IS_SOURCE_FOLDER(EXE))
	{
		return ; /* THE SERVICE UPDATES ITS OWN; TEST BUILDS STAY */
	}
	if (!CAN_WRITE_HERE(EXE) || LATEST_RELEASE(
		TAG,
		sizeof(TAG),
		URL,
		sizeof(URL),
		SHA,
		sizeof(SHA)
	) != 1)
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
	if (!DOWNLOAD_VERIFIED(URL, SHA, NEW_EXE))
	{
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
		/* PID OF THIS EXE, ITS PATH, THE RELEASE NAME: ONE PER LINE */
		snprintf(
			PID_LINE,
			sizeof(PID_LINE),
			"%lu\r\n",
			(unsigned long)GetCurrentProcessId()
		);
		WriteFile(FILE_HANDLE, PID_LINE, (DWORD)strlen(PID_LINE), &DONE, NULL);
		WriteFile(
			FILE_HANDLE,
			NOTE_TEXT,
			(DWORD)strlen(NOTE_TEXT),
			&DONE,
			NULL
		);
		WriteFile(FILE_HANDLE, "\r\n", 2, &DONE, NULL);
		WriteFile(FILE_HANDLE, TAG, (DWORD)strlen(TAG), &DONE, NULL);
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
	char		*TAG;
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
	TAG = LINE + strcspn(LINE, "\r\n");
	if (*TAG)
	{
		*TAG++ = '\0';
	}
	while (*TAG == '\r' || *TAG == '\n')
	{
		++TAG;
	}
	TAG[strcspn(TAG, "\r\n")] = '\0';
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
		SAYF(LANG(STR_U_DONE), *TAG ? TAG : INWAY_VERSION_TEXT);
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

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                SERVERS UPDATE THEMSELVES                 |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# THE BACKGROUND SERVICE HAS NO WINDOW, SO WHAT IT DOES ABOUT UPDATES GOES
# INTO INWAY.LOG NEXT TO ITS EXE. THE FILE STARTS OVER WHEN IT PASSES 64 KB.
*/
void
	UPDATE_LOG(const char *FORMAT, ...)
{
	WCHAR		PATH[MAX_PATH];
	WCHAR		*SLASH;
	HANDLE		FILE_HANDLE;
	SYSTEMTIME	NOW;
	char		LINE[512];
	va_list		A;
	int		N;
	DWORD		DONE;
	LARGE_INTEGER	SIZE;

	GetModuleFileNameW(NULL, PATH, MAX_PATH);
	SLASH = wcsrchr(PATH, L'\\');
	if (!SLASH || SLASH - PATH > MAX_PATH - 16)
	{
		return ;
	}
	lstrcpyW(SLASH + 1, L"INWAY.log");
	GetLocalTime(&NOW);
	N = snprintf(
		LINE,
		sizeof(LINE),
		"%04u-%02u-%02u %02u:%02u  ",
		NOW.wYear,
		NOW.wMonth,
		NOW.wDay,
		NOW.wHour,
		NOW.wMinute
	);
	va_start(A, FORMAT);
	N += vsnprintf(LINE + N, sizeof(LINE) - (size_t)N - 3, FORMAT, A);
	va_end(A);
	if (N > (int)sizeof(LINE) - 3)
	{
		N = (int)sizeof(LINE) - 3;
	}
	LINE[N++] = '\r';
	LINE[N++] = '\n';
	FILE_HANDLE = CreateFileW(
		PATH,
		FILE_APPEND_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL
	);
	if (FILE_HANDLE == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	if (GetFileSizeEx(FILE_HANDLE, &SIZE) && SIZE.QuadPart > 65536)
	{
		CloseHandle(FILE_HANDLE);
		FILE_HANDLE = CreateFileW(
			PATH,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL
		);
		if (FILE_HANDLE == INVALID_HANDLE_VALUE)
		{
			return ;
		}
	}
	WriteFile(FILE_HANDLE, LINE, (DWORD)N, &DONE, NULL);
	CloseHandle(FILE_HANDLE);
}

/* 1 WHILE SOMEONE IS CONNECTED TO THIS PC (A TCP SESSION ON INWAY_PORT) */
static int
	SESSION_ACTIVE(void)
{
	typedef DWORD	(WINAPI *T_GET_TABLE)(
		PVOID TABLE,
		PDWORD SIZE,
		BOOL SORT,
		ULONG FAMILY,
		TCP_TABLE_CLASS CLASS,
		ULONG RESERVED
	);
	HMODULE				IPHLP;
	T_GET_TABLE			GET_TABLE;
	MIB_TCPTABLE_OWNER_PID	*TABLE;
	DWORD				SIZE;
	DWORD				I;
	int				ACTIVE;

	IPHLP = LoadLibraryW(L"iphlpapi.dll");
	GET_TABLE = IPHLP ?
		(T_GET_TABLE)(void *)GetProcAddress(IPHLP, "GetExtendedTcpTable") :
		NULL;
	if (!GET_TABLE)
	{
		if (IPHLP)
		{
			FreeLibrary(IPHLP);
		}
		return (1); /* CAN NOT TELL: BETTER WAIT */
	}
	ACTIVE = 0;
	SIZE = 0;
	GET_TABLE(NULL, &SIZE, FALSE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0);
	TABLE = SIZE ? (MIB_TCPTABLE_OWNER_PID *)malloc(SIZE) : NULL;
	if (TABLE && GET_TABLE(
		TABLE,
		&SIZE,
		FALSE,
		AF_INET,
		TCP_TABLE_OWNER_PID_CONNECTIONS,
		0
	) == NO_ERROR)
	{
		for (I = 0; I < TABLE->dwNumEntries; ++I)
		{
			if (TABLE->table[I].dwState == MIB_TCP_STATE_ESTAB &&
				ntohs((u_short)TABLE->table[I].dwLocalPort) == INWAY_PORT)
			{
				ACTIVE = 1;
			}
		}
	}
	else
	{
		ACTIVE = 1;
	}
	free(TABLE);
	FreeLibrary(IPHLP);
	return (ACTIVE);
}

/* A RANDOM NUMBER OF MILLISECONDS FROM 0 TO RANGE */
static DWORD
	RANDOM_MS(DWORD RANGE)
{
	DWORD	R;

	R = GetTickCount() ^ (GetCurrentProcessId() << 16);
	BCryptGenRandom(
		NULL,
		(PUCHAR)&R,
		sizeof(R),
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	);
	return (RANGE ? R % RANGE : 0);
}

/*
# RUNS IN ITS OWN THREAD INSIDE THE SERVICE UNTIL STOP IS SET.
# - FIRST CHECK 3 TO 15 MINUTES AFTER START, THEN EVERY 6 HOURS (+/- 30
#   MINUTES), SO MANY SERVERS BEHIND ONE INTERNET CONNECTION DO NOT ALL ASK
#   GITHUB AT THE SAME MOMENT. AFTER A FAILED CHECK: AGAIN IN 1 HOUR.
# - A VERIFIED DOWNLOAD WAITS AS INWAY-NEW.EXE UNTIL NOBODY IS CONNECTED
#   (LOOKED AT EVERY 5 MINUTES), THEN "INWAY-NEW.EXE -SERVICE-SWAP" STOPS
#   THE SERVICE, TAKES THE PLACE OF INWAY.EXE AND STARTS THE SERVICE AGAIN.
*/
void
	UPDATE_SERVICE_LOOP(HANDLE STOP)
{
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;
	WCHAR			EXE[MAX_PATH];
	WCHAR			NEW_EXE[MAX_PATH + 16];
	WCHAR			COMMAND[MAX_PATH + 48];
	WCHAR			*SLASH;
	char			TAG[64];
	char			URL[600];
	char			SHA[80];
	DWORD			WAIT;
	DWORD			NEXT_CHECK;
	DWORD			WAITED;
	int			PENDING;
	int			FOUND;

	GetModuleFileNameW(NULL, EXE, MAX_PATH);
	lstrcpynW(NEW_EXE, EXE, MAX_PATH);
	SLASH = wcsrchr(NEW_EXE, L'\\');
	if (!SLASH)
	{
		return ;
	}
	lstrcpyW(SLASH + 1, L"INWAY-NEW.exe");
	DeleteFileW(NEW_EXE); /* LEFT FROM A SWAP THAT DID NOT HAPPEN */
	PENDING = 0;
	WAITED = 0;
	NEXT_CHECK = 3 * 60000 + RANDOM_MS(12 * 60000);
	WAIT = 60000;
	while (WaitForSingleObject(STOP, WAIT) == WAIT_TIMEOUT)
	{
		WAITED += WAIT;
		if (!PENDING && WAITED >= NEXT_CHECK)
		{
			WAITED = 0;
			NEXT_CHECK = 6 * 3600000 - 30 * 60000 + RANDOM_MS(60 * 60000);
			FOUND = LATEST_RELEASE(
				TAG,
				sizeof(TAG),
				URL,
				sizeof(URL),
				SHA,
				sizeof(SHA)
			);
			if (FOUND < 0)
			{
				NEXT_CHECK = 3600000; /* NO ANSWER: TRY AGAIN IN 1 HOUR */
			}
			if (FOUND != 1)
			{
				continue ;
			}
			if (!DOWNLOAD_VERIFIED(URL, SHA, NEW_EXE))
			{
				UPDATE_LOG("%s: download or SHA-256 check failed", TAG);
				NEXT_CHECK = 3600000; /* TRY AGAIN IN 1 HOUR */
				continue ;
			}
			UPDATE_LOG("%s downloaded, waiting until nobody is connected", TAG);
			PENDING = 1;
			WAIT = 5 * 60000;
		}
		if (!PENDING || SESSION_ACTIVE())
		{
			continue ;
		}
		swprintf(COMMAND, MAX_PATH + 48, L"\"%ls\" -service-swap", NEW_EXE);
		memset(&SI, 0, sizeof(SI));
		SI.cb = sizeof(SI);
		memset(&PI, 0, sizeof(PI));
		if (CreateProcessW(
			NULL,
			COMMAND,
			NULL,
			NULL,
			FALSE,
			CREATE_NO_WINDOW,
			NULL,
			NULL,
			&SI,
			&PI
		))
		{
			UPDATE_LOG("starting the swap to %s", TAG);
			CloseHandle(PI.hThread);
			CloseHandle(PI.hProcess);
			return ; /* THE HELPER STOPS THIS SERVICE NOW */
		}
		UPDATE_LOG("could not start the new exe (error %lu)", GetLastError());
		DeleteFileW(NEW_EXE);
		PENDING = 0;
		WAIT = 60000;
	}
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                SERVERS UPDATE THEMSELVES                 |  //\\  */
/* '""""' #==========================================================# '""""' */
