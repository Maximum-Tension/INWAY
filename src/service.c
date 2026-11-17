/******************************************************************************\
# C - SERVICE                                    #       Maximum Tension       #
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
|*|                                 SERVICE                                  |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| THE WINDOWS SERVICE, ITS HELPER, AND -INSTALL / -UNINSTALL / -REMOVEOLD. |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :  v [C LANGUAGE]                                                        : *|
|* : 1|                                                                     : *|
|* : 2| int SERVICE_INSTALL(const char *CODE);                              : *|
|* : 3|                                                                     : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * HOST_THREAD(), OPEN_LISTENER(), SAY()
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* ***************************** [v] NAMES [v] ****************************** */
#define SVC_NAME	L"INWAY"
#define SVC_SHOW	L"INWAY Remote Desktop"
#define FW_RULE	L"INWAY Remote Desktop"
#define SVC_FOLDER	L"INWAY"
/* THE OLD TOOL, FOR -REMOVEOLD */
#define OLD_SVC	L"ConnectRD"
#define OLD_FW	L"CONNECT Remote Desktop"
#define OLD_FOLDER	L"CONNECT"
/* ***************************** [^] NAMES [^] ****************************** */

/* **************************** [v] GLOBALS [v] ***************************** */
int				G_SVC_RUNNING = 0;
char				G_SVC_CODE[8];
static SERVICE_STATUS_HANDLE	G__SVC_HANDLE__ = NULL;
static SERVICE_STATUS		G__SVC_STATUS__;
static HANDLE			G__SVC_STOP__ = NULL;
static char			G__SVC_CODE_ARG__[8];	/* CODE GIVEN WITH -P */
/* **************************** [^] GLOBALS [^] ***************************** */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                      SMALL HELPERS                       |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	EXE_PATH(WCHAR *OUTPUT)
{
	OUTPUT[0] = L'\0';
	GetModuleFileNameW(NULL, OUTPUT, MAX_PATH);
}

/* "C:\PROGRAM FILES\<FOLDER>" */
static void
	INSTALL_DIR(const WCHAR *FOLDER, WCHAR *OUTPUT)
{
	WCHAR	PF[MAX_PATH];

	if (!GetEnvironmentVariableW(L"ProgramFiles", PF, MAX_PATH))
	{
		lstrcpyW(PF, L"C:\\Program Files");
	}
	swprintf(OUTPUT, MAX_PATH, L"%ls\\%ls", PF, FOLDER);
}

/* RUNS A COMMAND WITHOUT A WINDOW AND WAITS FOR IT */
static void
	RUN_QUIET(WCHAR *COMMAND)
{
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;

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
		WaitForSingleObject(PI.hProcess, 20000);
		CloseHandle(PI.hThread);
		CloseHandle(PI.hProcess);
	}
}

static void
	FIREWALL_DELETE(const WCHAR *RULE)
{
	WCHAR	COMMAND[512];

	swprintf(
		COMMAND,
		512,
		L"netsh advfirewall firewall delete rule name=\"%ls\"",
		RULE
	);
	RUN_QUIET(COMMAND);
}

static void
	STOP_SERVICE(SC_HANDLE S)
{
	SERVICE_STATUS	ST;
	int		I;

	ControlService(S, SERVICE_CONTROL_STOP, &ST);
	for (I = 0; I < 50; ++I)
	{
		if (!QueryServiceStatus(S, &ST) || ST.dwCurrentState == SERVICE_STOPPED)
		{
			break ;
		}
		Sleep(200);
	}
	Sleep(500); /* LET THE HELPER EXIT AND FREE THE FILE */
}

/* DELETES A FILE, OR MARKS IT FOR DELETION AT THE NEXT RESTART */
static void
	DELETE_LATER(const WCHAR *PATH)
{
	if (GetFileAttributesW(PATH) != INVALID_FILE_ATTRIBUTES &&
		!DeleteFileW(PATH))
	{
		MoveFileExW(PATH, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	}
}

static void
	SAY_LANG(const char *COLOUR, enum E_STR ID)
{
	SAYF("  %s%s{0}\n", COLOUR, LANG(ID));
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                      SMALL HELPERS                       |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                       THE SERVICE                        |  \\//  */
/*   \/   #==========================================================#   \/   */
static void
	SET_STATE(DWORD STATE)
{
	G__SVC_STATUS__.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	G__SVC_STATUS__.dwCurrentState = STATE;
	G__SVC_STATUS__.dwControlsAccepted = (STATE == SERVICE_RUNNING) ?
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN : 0;
	G__SVC_STATUS__.dwWaitHint = 5000;
	SetServiceStatus(G__SVC_HANDLE__, &G__SVC_STATUS__);
}

static void WINAPI
	CONTROL_HANDLER(DWORD CONTROL)
{
	if (CONTROL == SERVICE_CONTROL_STOP || CONTROL == SERVICE_CONTROL_SHUTDOWN)
	{
		SET_STATE(SERVICE_STOP_PENDING);
		SetEvent(G__SVC_STOP__);
	}
}

/* WINDOWS ONLY LETS SERVICES PRESS CTRL + ALT + DEL WHEN THIS POLICY SAYS SO */
static void
	ALLOW_SAS(void)
{
	HKEY	KEY;
	DWORD	VALUE;
	DWORD	SIZE;
	DWORD	TYPE;
	DWORD	NEW_VALUE;

	if (RegCreateKeyExW(
		HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		0,
		NULL,
		0,
		KEY_READ | KEY_WRITE,
		NULL,
		&KEY,
		NULL
	) != ERROR_SUCCESS)
	{
		return ;
	}
	VALUE = 0;
	SIZE = sizeof(VALUE);
	if (RegQueryValueExW(
		KEY,
		L"SoftwareSASGeneration",
		NULL,
		&TYPE,
		(BYTE *)&VALUE,
		&SIZE
	) != ERROR_SUCCESS || TYPE != REG_DWORD)
	{
		VALUE = 0;
	}
	if (VALUE != 1 && VALUE != 3) /* 1 = SERVICES, 3 = SERVICES + EASE */
	{
		NEW_VALUE = (VALUE == 2) ? 3 : 1;
		RegSetValueExW(
			KEY,
			L"SoftwareSASGeneration",
			0,
			REG_DWORD,
			(const BYTE *)&NEW_VALUE,
			sizeof(NEW_VALUE)
		);
	}
	RegCloseKey(KEY);
}

static void
	PRESS_CTRL_ALT_DEL(void)
{
	typedef VOID	(WINAPI *T_SEND_SAS)(BOOL AS_USER);
	HMODULE		SAS;
	T_SEND_SAS	SEND;

	SAS = LoadLibraryW(L"sas.dll");
	if (!SAS)
	{
		return ;
	}
	SEND = (T_SEND_SAS)(void *)GetProcAddress(SAS, "SendSAS");
	if (SEND)
	{
		SEND(FALSE);
	}
	FreeLibrary(SAS);
}

/* STARTS "INWAY.EXE -WORKER -P CODE" WITH SYSTEM RIGHTS IN THAT SESSION */
static HANDLE
	LAUNCH_WORKER(DWORD SESSION)
{
	HANDLE		ME;
	HANDLE		DUP;
	HANDLE		PROCESS;
	WCHAR		EXE[MAX_PATH];
	WCHAR		COMMAND[MAX_PATH + 64];
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;

	ME = NULL;
	DUP = NULL;
	PROCESS = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &ME))
	{
		return (NULL);
	}
	if (DuplicateTokenEx(
		ME,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityImpersonation,
		TokenPrimary,
		&DUP
	) && SetTokenInformation(DUP, TokenSessionId, &SESSION, sizeof(SESSION)))
	{
		EXE_PATH(EXE);
		swprintf(
			COMMAND,
			MAX_PATH + 64,
			L"\"%ls\" -worker -noupdate -p %hs",
			EXE,
			G__SVC_CODE_ARG__
		);
		memset(&SI, 0, sizeof(SI));
		SI.cb = sizeof(SI);
		SI.lpDesktop = L"winsta0\\default";
		memset(&PI, 0, sizeof(PI));
		if (CreateProcessAsUserW(
			DUP,
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
			CloseHandle(PI.hThread);
			PROCESS = PI.hProcess;
		}
	}
	if (DUP)
	{
		CloseHandle(DUP);
	}
	CloseHandle(ME);
	return (PROCESS);
}

/* KEEPS ONE HELPER ALIVE IN THE SESSION THAT IS ON THE SCREEN */
static void WINAPI
	SERVICE_MAIN(DWORD ARGC, LPWSTR *ARGV)
{
	HANDLE	SAS_EVENT;
	HANDLE	WAITS[2];
	HANDLE	WORKER;
	DWORD	WORKER_SESSION;
	DWORD	SESSION;
	DWORD	WAKE;
	int	DEAD;

	(void)ARGC;
	(void)ARGV;
	G__SVC_HANDLE__ = RegisterServiceCtrlHandlerW(
		SVC_NAME,
		CONTROL_HANDLER
	);
	if (!G__SVC_HANDLE__)
	{
		return ;
	}
	G__SVC_STOP__ = CreateEventW(NULL, TRUE, FALSE, NULL);
	ALLOW_SAS();
	SAS_EVENT = CreateEventW(NULL, FALSE, FALSE, INWAY_SAS_EVENT);
	WAITS[0] = G__SVC_STOP__;
	WAITS[1] = SAS_EVENT;
	SET_STATE(SERVICE_RUNNING);
	WORKER = NULL;
	WORKER_SESSION = 0XFFFFFFFF;
	for (;;)
	{
		SESSION = WTSGetActiveConsoleSessionId();
		DEAD = (!WORKER || WaitForSingleObject(WORKER, 0) == WAIT_OBJECT_0);
		if (SESSION != 0XFFFFFFFF && (DEAD || SESSION != WORKER_SESSION))
		{
			if (WORKER)
			{
				TerminateProcess(WORKER, 0);
				WaitForSingleObject(WORKER, 3000);
				CloseHandle(WORKER);
			}
			WORKER = LAUNCH_WORKER(SESSION);
			WORKER_SESSION = SESSION;
		}
		WAKE = WaitForMultipleObjects(SAS_EVENT ? 2 : 1, WAITS, FALSE, 1000);
		if (WAKE == WAIT_OBJECT_0)
		{
			break ; /* THE SERVICE IS STOPPING */
		}
		if (WAKE == WAIT_OBJECT_0 + 1)
		{
			PRESS_CTRL_ALT_DEL(); /* THE HELPER ASKED FOR IT */
		}
	}
	if (SAS_EVENT)
	{
		CloseHandle(SAS_EVENT);
	}
	if (WORKER)
	{
		TerminateProcess(WORKER, 0);
		CloseHandle(WORKER);
	}
	SET_STATE(SERVICE_STOPPED);
}

int
	SERVICE_RUN(const char *CODE)
{
	SERVICE_TABLE_ENTRYW	TABLE[2];

	snprintf(G__SVC_CODE_ARG__, sizeof(G__SVC_CODE_ARG__), "%s", CODE);
	TABLE[0].lpServiceName = SVC_NAME;
	TABLE[0].lpServiceProc = SERVICE_MAIN;
	TABLE[1].lpServiceName = NULL;
	TABLE[1].lpServiceProc = NULL;
	return (StartServiceCtrlDispatcherW(TABLE) ? 0 : 1);
}

/* THE HELPER: NO WINDOW OF ITS OWN, IT ONLY SHARES THE SCREEN */
int
	SERVICE_WORKER(void)
{
	G_WORKER = 1;
	while ((G_LISTEN = OPEN_LISTENER()) == INVALID_SOCKET)
	{
		Sleep(2000);
	}
	HOST_THREAD(NULL);
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                       THE SERVICE                        |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                 SEEN FROM A NORMAL COPY                  |  \\//  */
/*   \/   #==========================================================#   \/   */
/* IS THE SERVICE ON, AND WHAT IS ITS CODE? */
void
	SERVICE_READ_INFO(void)
{
	SC_HANDLE		M;
	SC_HANDLE		S;
	SERVICE_STATUS		ST;
	QUERY_SERVICE_CONFIGW	*CONFIG;
	DWORD		NEED;
	WCHAR		*P;

	G_SVC_RUNNING = 0;
	G_SVC_CODE[0] = '\0';
	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
	if (!M)
	{
		return ;
	}
	S = OpenServiceW(
		M,
		SVC_NAME,
		SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG
	);
	if (S)
	{
		if (QueryServiceStatus(S, &ST))
		{
			G_SVC_RUNNING = (ST.dwCurrentState == SERVICE_RUNNING ||
				ST.dwCurrentState == SERVICE_START_PENDING);
		}
		NEED = 0;
		QueryServiceConfigW(S, NULL, 0, &NEED);
		CONFIG = NEED ? (QUERY_SERVICE_CONFIGW *)malloc(NEED) : NULL;
		if (CONFIG && QueryServiceConfigW(S, CONFIG, NEED, &NEED))
		{
			P = wcsstr(CONFIG->lpBinaryPathName, L" -p ");
			if (P)
			{
				WideCharToMultiByte(
					CP_UTF8,
					0,
					P + 4,
					6,
					G_SVC_CODE,
					7,
					NULL,
					NULL
				);
				G_SVC_CODE[6] = '\0';
			}
		}
		free(CONFIG);
		CloseServiceHandle(S);
	}
	CloseServiceHandle(M);
}

int
	SERVICE_IS_ADMIN(void)
{
	HANDLE		TOKEN;
	TOKEN_ELEVATION	E;
	DWORD		N;
	int		RESULT;

	RESULT = 0;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &TOKEN))
	{
		if (GetTokenInformation(TOKEN, TokenElevation, &E, sizeof(E), &N))
		{
			RESULT = (E.TokenIsElevated != 0);
		}
		CloseHandle(TOKEN);
	}
	return (RESULT);
}

/* SHOWS THE WINDOWS "YES / NO" PROMPT AND WAITS FOR THE ADMIN COPY */
int
	SERVICE_RELAUNCH_AS_ADMIN(const char *PARAMETERS)
{
	SHELLEXECUTEINFOW	SEI;
	WCHAR		EXE[MAX_PATH];
	WCHAR		*WIDE;

	EXE_PATH(EXE);
	WIDE = UTF8_TO_WIDE(PARAMETERS);
	memset(&SEI, 0, sizeof(SEI));
	SEI.cbSize = sizeof(SEI);
	SEI.fMask = SEE_MASK_NOCLOSEPROCESS;
	SEI.lpVerb = L"runas";
	SEI.lpFile = EXE;
	SEI.lpParameters = WIDE;
	SEI.nShow = SW_SHOWNORMAL;
	if (!ShellExecuteExW(&SEI))
	{
		free(WIDE);
		return (0);
	}
	free(WIDE);
	if (SEI.hProcess)
	{
		WaitForSingleObject(SEI.hProcess, INFINITE);
		CloseHandle(SEI.hProcess);
	}
	return (1);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                 SEEN FROM A NORMAL COPY                  |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                    INSTALL AND REMOVE                    |  \\//  */
/*   \/   #==========================================================#   \/   */
int
	SERVICE_INSTALL(const char *CODE)
{
	WCHAR			DIR[MAX_PATH];
	WCHAR			TARGET[MAX_PATH];
	WCHAR			EXE[MAX_PATH];
	WCHAR			BIN[MAX_PATH + 64];
	WCHAR			COMMAND[MAX_PATH + 256];
	char			*DIR_TEXT;
	SC_HANDLE		M;
	SC_HANDLE		S;
	SERVICE_DESCRIPTIONW	DESCRIPTION;
	SC_ACTION		ACTIONS[3];
	SERVICE_FAILURE_ACTIONSW	FAILURE;
	int			COPIED;
	int			I;
	int			STARTED;

	INSTALL_DIR(SVC_FOLDER, DIR);
	swprintf(TARGET, MAX_PATH, L"%ls\\INWAY.exe", DIR);
	EXE_PATH(EXE);
	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!M)
	{
		SAY_LANG("{r}", STR_I_NO_SCM);
		return (0);
	}
	S = OpenServiceW(M, SVC_NAME, SERVICE_ALL_ACCESS);
	if (S)
	{
		SAY_LANG("", STR_I_STOPPING_OLD);
		STOP_SERVICE(S);
	}
	DIR_TEXT = WIDE_TO_UTF8(DIR);
	SAY("  ");
	SAYF(LANG(STR_I_COPYING), DIR_TEXT ? DIR_TEXT : "");
	SAY("\n");
	free(DIR_TEXT);
	CreateDirectoryW(DIR, NULL);
	if (lstrcmpiW(EXE, TARGET) != 0)
	{
		COPIED = 0;
		for (I = 0; I < 20 && !COPIED; ++I)
		{
			COPIED = (CopyFileW(EXE, TARGET, FALSE) != 0);
			if (!COPIED)
			{
				Sleep(250);
			}
		}
		if (!COPIED)
		{
			SAY_LANG("{r}", STR_I_COPY_FAIL);
			if (S)
			{
				CloseServiceHandle(S);
			}
			CloseServiceHandle(M);
			return (0);
		}
	}
	SAY_LANG("", STR_I_SETUP);
	swprintf(BIN, MAX_PATH + 64, L"\"%ls\" -service -p %hs", TARGET, CODE);
	if (S)
	{
		ChangeServiceConfigW(
			S,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			BIN,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			SVC_SHOW
		);
	}
	else
	{
		S = CreateServiceW(
			M,
			SVC_NAME,
			SVC_SHOW,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			BIN,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
	}
	if (!S)
	{
		SAY("  {r}");
		SAYF(LANG(STR_I_CREATE_FAIL), GetLastError());
		SAY("{0}\n");
		CloseServiceHandle(M);
		return (0);
	}
	DESCRIPTION.lpDescription = L"Shares this screen with INWAY on the local "
		L"network, also on the lock screen. Remove with: INWAY.exe -uninstall";
	ChangeServiceConfig2W(S, SERVICE_CONFIG_DESCRIPTION, &DESCRIPTION);
	for (I = 0; I < 3; ++I) /* IT CRASHED? START IT AGAIN */
	{
		ACTIONS[I].Type = SC_ACTION_RESTART;
		ACTIONS[I].Delay = 5000;
	}
	memset(&FAILURE, 0, sizeof(FAILURE));
	FAILURE.dwResetPeriod = 86400;
	FAILURE.cActions = 3;
	FAILURE.lpsaActions = ACTIONS;
	ChangeServiceConfig2W(S, SERVICE_CONFIG_FAILURE_ACTIONS, &FAILURE);
	SAY_LANG("", STR_I_FIREWALL);
	FIREWALL_DELETE(FW_RULE);
	swprintf(
		COMMAND,
		MAX_PATH + 256,
		L"netsh advfirewall firewall add rule name=\"%ls\" dir=in "
		L"action=allow protocol=TCP localport=%d remoteip=localsubnet "
		L"profile=any program=\"%ls\"",
		FW_RULE,
		INWAY_PORT,
		TARGET
	);
	RUN_QUIET(COMMAND);
	ALLOW_SAS(); /* CTRL + ALT + DEL ON THE LOCK SCREEN */
	STARTED = StartServiceW(S, 0, NULL) ||
		GetLastError() == ERROR_SERVICE_ALREADY_RUNNING;
	CloseServiceHandle(S);
	CloseServiceHandle(M);
	if (!STARTED)
	{
		SAY_LANG("{r}", STR_I_NOT_STARTED);
		return (0);
	}
	return (1);
}

/* STOPS AND DELETES A SERVICE, ITS FIREWALL RULE AND ITS FOLDER */
static int
	REMOVE_SERVICE(
		const WCHAR *NAME,
		const WCHAR *RULE,
		const WCHAR *FOLDER,
		const WCHAR *FILE_HANDLE,
		enum E_STR NOT_FOUND
	)
{
	SC_HANDLE	M;
	SC_HANDLE	S;
	WCHAR		DIR[MAX_PATH];
	WCHAR		TARGET[MAX_PATH];

	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!M)
	{
		SAY_LANG("{r}", STR_I_NO_SCM);
		return (0);
	}
	S = OpenServiceW(M, NAME, SERVICE_ALL_ACCESS);
	if (S)
	{
		SAY_LANG("", STR_I_STOPPING);
		STOP_SERVICE(S);
		DeleteService(S);
		CloseServiceHandle(S);
	}
	else
	{
		SAY_LANG("{d}", NOT_FOUND);
	}
	CloseServiceHandle(M);
	FIREWALL_DELETE(RULE);
	INSTALL_DIR(FOLDER, DIR);
	swprintf(TARGET, MAX_PATH, L"%ls\\%ls", DIR, FILE_HANDLE);
	DELETE_LATER(TARGET); /* IN USE: GONE AT THE NEXT RESTART */
	RemoveDirectoryW(DIR);
	return (1);
}

int
	SERVICE_UNINSTALL(void)
{
	return (REMOVE_SERVICE(
		SVC_NAME,
		FW_RULE,
		SVC_FOLDER,
		L"INWAY.exe",
		STR_I_NOT_INSTALLED
	));
}

int
	SERVICE_REMOVE_OLD(void)
{
	return (REMOVE_SERVICE(
		OLD_SVC,
		OLD_FW,
		OLD_FOLDER,
		L"CONNECT.exe",
		STR_I_OLD_NOT_FOUND
	));
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                    INSTALL AND REMOVE                    |  //\\  */
/* '""""' #==========================================================# '""""' */
