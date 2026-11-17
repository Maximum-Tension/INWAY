/******************************************************************************\
# C - service                                    #       Maximum Tension       #
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
# THE WINDOWS SERVICE, ITS HELPER, AND -INSTALL / -UNINSTALL / -REMOVEOLD.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define INWAY_PORT
 * #define INWAY_SAS_EVENT
 * #define WIN32_LEAN_AND_MEAN
 */
#include <winsock2.h>
/******* """"""""""""
 * #define INVALID_SOCKET
 */
#include <windows.h>
/******* """""""""""
 * #define CALLBACK
 * #define CP_UTF8
 * #define CREATE_NO_WINDOW
 * #define CREATE_UNICODE_ENVIRONMENT
 * #define ERROR_ALREADY_EXISTS
 * #define ERROR_CANCELLED
 * #define ERROR_FILE_NOT_FOUND
 * #define ERROR_SERVICE_ALREADY_RUNNING
 * #define ERROR_SUCCESS
 * #define EVENT_MODIFY_STATE
 * #define FALSE
 * #define HKEY_CURRENT_USER
 * #define HKEY_LOCAL_MACHINE
 * #define INFINITE
 * #define INVALID_FILE_ATTRIBUTES
 * #define KEY_READ
 * #define KEY_SET_VALUE
 * #define KEY_WRITE
 * #define MAX_PATH
 * #define MOVEFILE_DELAY_UNTIL_REBOOT
 * #define MOVEFILE_REPLACE_EXISTING
 * #define MOVEFILE_WRITE_THROUGH
 * #define REG_DWORD
 * #define REG_SZ
 * #define SC_MANAGER_ALL_ACCESS
 * #define SC_MANAGER_CONNECT
 * #define SE_PRIVILEGE_ENABLED
 * #define SERVICE_ACCEPT_SHUTDOWN
 * #define SERVICE_ACCEPT_STOP
 * #define SERVICE_ALL_ACCESS
 * #define SERVICE_AUTO_START
 * #define SERVICE_CONFIG_DESCRIPTION
 * #define SERVICE_CONFIG_FAILURE_ACTIONS
 * #define SERVICE_CONTROL_SHUTDOWN
 * #define SERVICE_CONTROL_STOP
 * #define SERVICE_ERROR_NORMAL
 * #define SERVICE_QUERY_CONFIG
 * #define SERVICE_QUERY_STATUS
 * #define SERVICE_RUNNING
 * #define SERVICE_START
 * #define SERVICE_START_PENDING
 * #define SERVICE_STOP
 * #define SERVICE_STOP_PENDING
 * #define SERVICE_STOPPED
 * #define SERVICE_WIN32_OWN_PROCESS
 * #define SW_HIDE
 * #define SYNCHRONIZE
 * #define TOKEN_ADJUST_PRIVILEGES
 * #define TOKEN_ALL_ACCESS
 * #define TOKEN_QUERY
 * #define TRUE
 * #define VOID
 * #define WAIT_OBJECT_0
 * #define WAIT_TIMEOUT
 * #define WINAPI
 * #define WM_CLOSE
 * typedef					BOOL;
 * typedef					BYTE;
 * typedef					DWORD;
 * enum _SC_ACTION_TYPE;
 * enum _SECURITY_IMPERSONATION_LEVEL;
 * enum _TOKEN_INFORMATION_CLASS;
 * enum _TOKEN_TYPE;
 * typedef					HANDLE;
 * typedef					HKEY;
 * typedef					HMODULE;
 * typedef					HWND;
 * typedef					LPARAM;
 * typedef					LPVOID;
 * typedef					LPWSTR;
 * typedef					PHANDLE;
 * typedef					PROCESS_INFORMATION;
 * typedef					QUERY_SERVICE_CONFIGW;
 * typedef					SC_ACTION;
 * typedef					SC_HANDLE;
 * typedef					SERVICE_DESCRIPTIONW;
 * typedef					SERVICE_FAILURE_ACTIONSW;
 * typedef					SERVICE_STATUS;
 * typedef					SERVICE_STATUS_HANDLE;
 * typedef					SERVICE_TABLE_ENTRYW;
 * typedef					STARTUPINFOW;
 * typedef					TOKEN_ELEVATION;
 * typedef					TOKEN_PRIVILEGES;
 * typedef					ULONG;
 * typedef					WCHAR;
 * BOOL						AdjustTokenPrivileges(
 * 	HANDLE,
 * 	BOOL,
 * 	PTOKEN_PRIVILEGES,
 * 	DWORD,
 * 	PTOKEN_PRIVILEGES,
 * 	PDWORD
 * );
 * BOOL						ChangeServiceConfig2W(SC_HANDLE, DWORD, LPVOID);
 * BOOL						ChangeServiceConfigW(
 * 	SC_HANDLE,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	LPDWORD,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	LPCWSTR
 * );
 * BOOL						CloseHandle(HANDLE);
 * BOOL						CloseServiceHandle(SC_HANDLE);
 * BOOL						ControlService(SC_HANDLE, DWORD, LPSERVICE_STATUS);
 * BOOL						CopyFileW(LPCWSTR, LPCWSTR, BOOL);
 * BOOL						CreateDirectoryW(LPCWSTR, LPSECURITY_ATTRIBUTES);
 * HANDLE					CreateEventW(
 * 	LPSECURITY_ATTRIBUTES,
 * 	BOOL,
 * 	BOOL,
 * 	LPCWSTR
 * );
 * BOOL						CreateProcessAsUserW(
 * 	HANDLE,
 * 	LPCWSTR,
 * 	LPWSTR,
 * 	LPSECURITY_ATTRIBUTES,
 * 	LPSECURITY_ATTRIBUTES,
 * 	BOOL,
 * 	DWORD,
 * 	LPVOID,
 * 	LPCWSTR,
 * 	LPSTARTUPINFOW,
 * 	LPPROCESS_INFORMATION
 * );
 * BOOL						CreateProcessW(
 * 	LPCWSTR,
 * 	LPWSTR,
 * 	LPSECURITY_ATTRIBUTES,
 * 	LPSECURITY_ATTRIBUTES,
 * 	BOOL,
 * 	DWORD,
 * 	LPVOID,
 * 	LPCWSTR,
 * 	LPSTARTUPINFOW,
 * 	LPPROCESS_INFORMATION
 * );
 * SC_HANDLE				CreateServiceW(
 * 	SC_HANDLE,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	DWORD,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	LPDWORD,
 * 	LPCWSTR,
 * 	LPCWSTR,
 * 	LPCWSTR
 * );
 * HANDLE					CreateThread(
 * 	LPSECURITY_ATTRIBUTES,
 * 	SIZE_T,
 * 	LPTHREAD_START_ROUTINE,
 * 	LPVOID,
 * 	DWORD,
 * 	LPDWORD
 * );
 * BOOL						DeleteFileW(LPCWSTR);
 * BOOL						DeleteService(SC_HANDLE);
 * BOOL						DuplicateTokenEx(
 * 	HANDLE,
 * 	DWORD,
 * 	LPSECURITY_ATTRIBUTES,
 * 	SECURITY_IMPERSONATION_LEVEL,
 * 	TOKEN_TYPE,
 * 	PHANDLE
 * );
 * BOOL						EnumWindows(WNDENUMPROC, LPARAM);
 * BOOL						FreeLibrary(HMODULE);
 * HANDLE					GetCurrentProcess(void);
 * DWORD					GetCurrentProcessId(void);
 * DWORD					GetEnvironmentVariableW(LPCWSTR, LPWSTR, DWORD);
 * BOOL						GetExitCodeProcess(HANDLE, LPDWORD);
 * DWORD					GetFileAttributesW(LPCWSTR);
 * DWORD					GetLastError(void);
 * DWORD					GetModuleFileNameW(HMODULE, LPWSTR, DWORD);
 * FARPROC					GetProcAddress(HMODULE, LPCSTR);
 * DWORD					GetProcessId(HANDLE);
 * BOOL						GetTokenInformation(
 * 	HANDLE,
 * 	TOKEN_INFORMATION_CLASS,
 * 	LPVOID,
 * 	DWORD,
 * 	PDWORD
 * );
 * DWORD					GetWindowThreadProcessId(HWND, LPDWORD);
 * HMODULE					LoadLibraryW(LPCWSTR);
 * BOOL						LookupPrivilegeValueW(LPCWSTR, LPCWSTR, PLUID);
 * LPWSTR					lstrcatW(LPWSTR, LPCWSTR);
 * int						lstrcmpiW(LPCWSTR, LPCWSTR);
 * LPWSTR					lstrcpynW(LPWSTR, LPCWSTR, int);
 * LPWSTR					lstrcpyW(LPWSTR, LPCWSTR);
 * int						lstrlenW(LPCWSTR);
 * BOOL						MoveFileExW(LPCWSTR, LPCWSTR, DWORD);
 * HANDLE					OpenEventW(DWORD, BOOL, LPCWSTR);
 * BOOL						OpenProcessToken(HANDLE, DWORD, PHANDLE);
 * SC_HANDLE				OpenSCManagerW(LPCWSTR, LPCWSTR, DWORD);
 * SC_HANDLE				OpenServiceW(SC_HANDLE, LPCWSTR, DWORD);
 * BOOL						PostMessageW(HWND, UINT, WPARAM, LPARAM);
 * BOOL						ProcessIdToSessionId(DWORD, DWORD *);
 * BOOL						QueryServiceConfigW(
 * 	SC_HANDLE,
 * 	LPQUERY_SERVICE_CONFIGW,
 * 	DWORD,
 * 	LPDWORD
 * );
 * BOOL						QueryServiceStatus(SC_HANDLE, LPSERVICE_STATUS);
 * LONG						RegCloseKey(HKEY);
 * LONG						RegCreateKeyExW(
 * 	HKEY,
 * 	LPCWSTR,
 * 	DWORD,
 * 	LPWSTR,
 * 	DWORD,
 * 	REGSAM,
 * 	LPSECURITY_ATTRIBUTES,
 * 	PHKEY,
 * 	LPDWORD
 * );
 * LONG						RegDeleteValueW(HKEY, LPCWSTR);
 * SERVICE_STATUS_HANDLE	RegisterServiceCtrlHandlerW(
 * 	LPCWSTR,
 * 	LPHANDLER_FUNCTION
 * );
 * LONG						RegOpenKeyExW(HKEY, LPCWSTR, DWORD, REGSAM, PHKEY);
 * LONG						RegQueryValueExW(
 * 	HKEY,
 * 	LPCWSTR,
 * 	LPDWORD,
 * 	LPDWORD,
 * 	LPBYTE,
 * 	LPDWORD
 * );
 * LONG						RegSetValueExW(
 * 	HKEY,
 * 	LPCWSTR,
 * 	DWORD,
 * 	DWORD,
 * 	const BYTE *,
 * 	DWORD
 * );
 * BOOL						RemoveDirectoryW(LPCWSTR);
 * BOOL						SetEvent(HANDLE);
 * BOOL						SetServiceStatus(
 * 	SERVICE_STATUS_HANDLE,
 * 	LPSERVICE_STATUS
 * );
 * BOOL						SetTokenInformation(
 * 	HANDLE,
 * 	TOKEN_INFORMATION_CLASS,
 * 	LPVOID,
 * 	DWORD
 * );
 * void						Sleep(DWORD);
 * BOOL						StartServiceCtrlDispatcherW(
 * 	const SERVICE_TABLE_ENTRYW *
 * );
 * BOOL						StartServiceW(SC_HANDLE, DWORD, LPCWSTR *);
 * BOOL						TerminateProcess(HANDLE, UINT);
 * DWORD					WaitForMultipleObjects(
 * 	DWORD,
 * 	const HANDLE *,
 * 	BOOL,
 * 	DWORD
 * );
 * DWORD					WaitForSingleObject(HANDLE, DWORD);
 * int						WideCharToMultiByte(
 * 	UINT,
 * 	DWORD,
 * 	LPCWCH,
 * 	int,
 * 	LPSTR,
 * 	int,
 * 	LPCCH,
 * 	LPBOOL
 * );
 * DWORD					WTSGetActiveConsoleSessionId(void);
 */
#include <shellapi.h>
/******* """"""""""""
 * #define SEE_MASK_NOCLOSEPROCESS
 * typedef	SHELLEXECUTEINFOW;
 * BOOL		ShellExecuteExW(SHELLEXECUTEINFOW *);
 */
#include <stdio.h>
/******* """""""""
 * int	snprintf(char *, size_t, const char *, ...);
 * int	swprintf(wchar_t *, size_t, const wchar_t *, ...);
 */
#include <stdlib.h>
/******* """"""""""
 * int	atoi(const char *);
 * void	free(void *);
 * void	*malloc(size_t);
 */
#include <string.h>
/******* """"""""""
 * #define NULL
 * void		*memset(void *, int, size_t);
 * wchar_t	*wcsrchr(const wchar_t *, wchar_t);
 * wchar_t	*wcsstr(const wchar_t *, const wchar_t *);
 */
#include <stdint.h>
/******* """"""""""
 * typedef	uintptr_t;
 */
#include "version.h"
/******* """""""""""
 * #define INWAY_VERSION_TEXT
 */
#include "text.h"
/******* """"""""
 * WCHAR	*UTF8_TO_WIDE(const char *);
 * char		*WIDE_TO_UTF8(const WCHAR *);
 */
#include "lang.h"
/******* """"""""
 * enum E_STR;
 * const char	*LANG(enum E_STR);
 * const char	*LANG_NAME(void);
 */
#include "console.h"
/******* """""""""""
 * void	SAY(const char *);
 * void	SAYF(const char *, ...);
 */
#include "net.h"
/******* """""""
 * uint32_t	G_MY_IP;
 * uint32_t	PICK_LOCAL_IP(void);
 */
#include "host.h"
/******* """"""""
 * int		G_BACKGROUND;
 * int		G_CODE;
 * SOCKET	G_LISTEN;
 * int		G_WORKER;
 * DWORD	HOST_THREAD(LPVOID);
 * SOCKET	OPEN_LISTENER(void);
 */
#include "service.h"
/******* """""""""""
 * #define SVC_COPY_FAILED
 * #define SVC_CREATE_FAILED
 * #define SVC_DECLINED
 * #define SVC_DONE
 * #define SVC_DONE_USER
 * #define SVC_NO_PERMISSION
 * #define SVC_NO_SCM
 * #define SVC_NOT_STARTED
 * #define SVC_USER_EVENT
 * char	G_SVC_CODE[8];
 */
#include "update.h"
/******* """"""""""
 * int		G_AUTO_UPDATE;
 * void		UPDATE_CHECK(void);
 * void		UPDATE_LOG(const char *, ...);
 * void		UPDATE_SERVICE_LOOP(HANDLE);
 * DWORD	UPDATE_USER_THREAD(LPVOID);
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* ***************************** [v] NAMES [v] ****************************** */
#define SVC_NAME L"INWAY"
#define SVC_SHOW L"INWAY Remote Desktop"
#define FW_RULE L"INWAY Remote Desktop"
#define SVC_FOLDER L"INWAY"
/* THE OLD TOOL, FOR -REMOVEOLD */
#define OLD_SVC L"ConnectRD"
#define OLD_FW L"CONNECT Remote Desktop"
#define OLD_FOLDER L"CONNECT"
/* ***************************** [^] NAMES [^] ****************************** */

/* **************************** [v] GLOBALS [v] ***************************** */
int								G_SVC_RUNNING = 0;
char							G_SVC_CODE[8];
static SERVICE_STATUS_HANDLE	G__SVC_HANDLE__ = NULL;
static SERVICE_STATUS			G__SVC_STATUS__;
static HANDLE					G__SVC_STOP__ = NULL;
static char						G__SVC_CODE_ARG__[8]; /* CODE GIVEN WITH -P */
static int						G__SVC_NO_UPDATE__ = 0; /* -NOUPDATE */
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
	int				I;

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
	HMODULE			SAS;
	T_SEND_SAS		SEND;

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
	HANDLE				ME;
	HANDLE				DUP;
	HANDLE				PROCESS;
	WCHAR				EXE[MAX_PATH];
	WCHAR				COMMAND[MAX_PATH + 64];
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

static DWORD WINAPI
	UPDATE_THREAD(LPVOID PARAMETER)
{
	UPDATE_SERVICE_LOOP((HANDLE)PARAMETER);
	return (0);
}

/* KEEPS ONE HELPER ALIVE IN THE SESSION THAT IS ON THE SCREEN */
static void WINAPI
	SERVICE_MAIN(DWORD ARGC, LPWSTR *ARGV)
{
	HANDLE	SAS_EVENT;
	HANDLE	WAITS[2];
	HANDLE	WORKER;
	HANDLE	UPDATER;
	DWORD	WORKER_SESSION;
	DWORD	SESSION;
	DWORD	WAKE;
	int		DEAD;

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
	UPDATER = NULL;
	if (!G__SVC_NO_UPDATE__) /* SERVERS UPDATE THEMSELVES, SEE UPDATE.C */
	{
		UPDATER = CreateThread(NULL, 0, UPDATE_THREAD, G__SVC_STOP__, 0, NULL);
	}
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
	if (UPDATER)
	{
		WaitForSingleObject(UPDATER, 3000);
		CloseHandle(UPDATER);
	}
	SET_STATE(SERVICE_STOPPED);
}

int
	SERVICE_RUN(const char *CODE, int NO_UPDATE)
{
	SERVICE_TABLE_ENTRYW	TABLE[2];

	snprintf(G__SVC_CODE_ARG__, sizeof(G__SVC_CODE_ARG__), "%s", CODE);
	G__SVC_NO_UPDATE__ = NO_UPDATE;
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
/*  \\//  |                 THE NORMAL-RIGHTS SERVER                 |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# WHEN "NO" IS CLICKED ON THE ADMIN QUESTION OF -INSTALL, INWAY STILL BECOMES
# A SERVER, WITH NORMAL RIGHTS: A COPY IN %LOCALAPPDATA%\INWAY STARTS WITH NO
# WINDOW WHEN THIS PERSON SIGNS IN ("RUN" KEY OF THIS USER). IT TAKES ONLY ITS
# CODE, CAN NOT USE ADMIN WINDOWS OR THE LOCK SCREEN, AND UPDATES ITSELF.
*/
#define USER_RUN_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define USER_VALUE L"INWAY"

/* "%LOCALAPPDATA%\INWAY" */
static int
	USER_DIR(WCHAR *OUTPUT)
{
	DWORD	LEN;

	LEN = GetEnvironmentVariableW(L"LOCALAPPDATA", OUTPUT, MAX_PATH - 16);
	if (!LEN || LEN >= MAX_PATH - 16)
	{
		return (0);
	}
	lstrcatW(OUTPUT, L"\\INWAY");
	return (1);
}

/* THE COMMAND LINE IN THE "RUN" KEY, "" WHEN THERE IS NONE */
static void
	USER_COMMAND(WCHAR *OUTPUT, DWORD CHARS)
{
	HKEY	KEY;
	DWORD	TYPE;
	DWORD	BYTES;

	OUTPUT[0] = L'\0';
	if (RegOpenKeyExW(HKEY_CURRENT_USER, USER_RUN_KEY, 0, KEY_READ, &KEY) !=
		ERROR_SUCCESS)
	{
		return ;
	}
	BYTES = (CHARS - 1) * sizeof(WCHAR);
	if (RegQueryValueExW(KEY, USER_VALUE, NULL, &TYPE, (BYTE *)OUTPUT,
		&BYTES) != ERROR_SUCCESS || TYPE != REG_SZ)
	{
		BYTES = 0;
	}
	OUTPUT[BYTES / sizeof(WCHAR)] = L'\0';
	RegCloseKey(KEY);
}

/*
# ASKS A RUNNING NORMAL-RIGHTS SERVER TO END AND WAITS UNTIL IT IS GONE (UP TO
# 10 SECONDS). 1 = ONE WAS RUNNING.
*/
static int
	STOP_USER_SERVER(void)
{
	HANDLE	EVENT;
	int		I;

	for (I = 0; I < 100; ++I)
	{
		EVENT = OpenEventW(EVENT_MODIFY_STATE, FALSE, SVC_USER_EVENT);
		if (!EVENT)
		{
			break ;
		}
		SetEvent(EVENT); /* AGAIN EACH TIME: A NEW ONE MAY HAVE STARTED */
		CloseHandle(EVENT);
		Sleep(100);
	}
	if (I)
	{
		Sleep(300); /* LET IT CLOSE ITS FILE */
	}
	return (I != 0);
}

int
	SERVICE_EXISTS(void)
{
	SC_HANDLE	M;
	SC_HANDLE	S;

	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
	if (!M)
	{
		return (0);
	}
	S = OpenServiceW(M, SVC_NAME, SERVICE_QUERY_STATUS);
	if (S)
	{
		CloseServiceHandle(S);
	}
	CloseServiceHandle(M);
	return (S != NULL);
}

int
	SERVICE_INSTALL_USER(const char *CODE, int NO_UPDATE)
{
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;
	WCHAR				DIR[MAX_PATH];
	WCHAR				TARGET[MAX_PATH];
	WCHAR				EXE[MAX_PATH];
	WCHAR				COMMAND[MAX_PATH + 64];
	char				*DIR_TEXT;
	HKEY				KEY;
	int					COPIED;
	int					I;

	if (!USER_DIR(DIR))
	{
		SAY_LANG("{r}", STR_I_COPY_FAIL);
		return (SVC_COPY_FAILED);
	}
	swprintf(TARGET, MAX_PATH, L"%ls\\INWAY.exe", DIR);
	EXE_PATH(EXE);
	STOP_USER_SERVER();
	DIR_TEXT = WIDE_TO_UTF8(DIR);
	SAY("  ");
	SAYF(LANG(STR_I_COPYING), DIR_TEXT ? DIR_TEXT : "");
	SAY("\n");
	free(DIR_TEXT);
	CreateDirectoryW(DIR, NULL);
	COPIED = (lstrcmpiW(EXE, TARGET) == 0);
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
		return (SVC_COPY_FAILED);
	}
	SAY_LANG("", STR_I_USER_SETUP);
	swprintf(
		COMMAND,
		MAX_PATH + 64,
		L"\"%ls\" -background -p %hs%ls",
		TARGET,
		CODE,
		NO_UPDATE ? L" -noupdate" : L""
	);
	if (RegCreateKeyExW(HKEY_CURRENT_USER, USER_RUN_KEY, 0, NULL, 0,
		KEY_SET_VALUE, NULL, &KEY, NULL) != ERROR_SUCCESS)
	{
		SAY_LANG("{r}", STR_I_USER_FAIL);
		return (SVC_CREATE_FAILED);
	}
	I = RegSetValueExW(
		KEY,
		USER_VALUE,
		0,
		REG_SZ,
		(const BYTE *)COMMAND,
		(DWORD)((lstrlenW(COMMAND) + 1) * sizeof(WCHAR))
	);
	RegCloseKey(KEY);
	if (I != ERROR_SUCCESS)
	{
		SAY_LANG("{r}", STR_I_USER_FAIL);
		return (SVC_CREATE_FAILED);
	}
	/* AND START IT NOW, WITHOUT A WINDOW */
	lstrcatW(COMMAND, L" -hidden");
	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(SI);
	if (!CreateProcessW(NULL, COMMAND, NULL, NULL, FALSE, CREATE_NO_WINDOW,
		NULL, DIR, &SI, &PI))
	{
		SAY_LANG("{r}", STR_I_NOT_STARTED);
		return (SVC_NOT_STARTED);
	}
	CloseHandle(PI.hThread);
	CloseHandle(PI.hProcess);
	return (SVC_DONE_USER);
}

/* 1 WHEN THERE WAS ONE TO REMOVE */
int
	SERVICE_UNINSTALL_USER(void)
{
	WCHAR	COMMAND[MAX_PATH + 64];
	WCHAR	DIR[MAX_PATH];
	WCHAR	TARGET[MAX_PATH];
	HKEY	KEY;
	int		FOUND;
	int		I;

	USER_COMMAND(COMMAND, MAX_PATH + 64);
	FOUND = STOP_USER_SERVER() || COMMAND[0];
	if (USER_DIR(DIR))
	{
		swprintf(TARGET, MAX_PATH, L"%ls\\INWAY.exe", DIR);
		FOUND = FOUND || GetFileAttributesW(TARGET) != INVALID_FILE_ATTRIBUTES;
	}
	if (!FOUND)
	{
		return (0);
	}
	if (RegOpenKeyExW(HKEY_CURRENT_USER, USER_RUN_KEY, 0, KEY_SET_VALUE,
		&KEY) == ERROR_SUCCESS)
	{
		RegDeleteValueW(KEY, USER_VALUE);
		RegCloseKey(KEY);
	}
	if (USER_DIR(DIR))
	{
		swprintf(TARGET, MAX_PATH, L"%ls\\INWAY.exe", DIR);
		for (I = 0; I < 20 && !DeleteFileW(TARGET) &&
			GetLastError() != ERROR_FILE_NOT_FOUND; ++I)
		{
			Sleep(250); /* IT MAY STILL BE CLOSING */
		}
		RemoveDirectoryW(DIR);
	}
	SAY_LANG("", STR_I_USER_REMOVED);
	return (1);
}

/* "INWAY.EXE -BACKGROUND -P 938572": THE NORMAL-RIGHTS SERVER ITSELF */
int
	SERVICE_RUN_USER(const char *CODE, int NO_UPDATE)
{
	HANDLE	STOP;
	HANDLE	T;

	STOP = CreateEventW(NULL, TRUE, FALSE, SVC_USER_EVENT);
	if (!STOP || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return (0); /* ONE IS ALREADY RUNNING */
	}
	G_BACKGROUND = 1;
	G_AUTO_UPDATE = !NO_UPDATE;
	G_CODE = atoi(CODE);
	G_MY_IP = PICK_LOCAL_IP();
	if (!NO_UPDATE)
	{
		UPDATE_CHECK(); /* ENDS THIS PROCESS WHEN A NEWER ONE TAKES OVER */
	}
	while ((G_LISTEN = OPEN_LISTENER()) == INVALID_SOCKET)
	{
		if (WaitForSingleObject(STOP, 2000) == WAIT_OBJECT_0)
		{
			return (0);
		}
	}
	T = CreateThread(NULL, 0, HOST_THREAD, NULL, 0, NULL);
	if (T)
	{
		CloseHandle(T);
	}
	if (!NO_UPDATE)
	{
		T = CreateThread(NULL, 0, UPDATE_USER_THREAD, STOP, 0, NULL);
		if (T)
		{
			CloseHandle(T);
		}
	}
	WaitForSingleObject(STOP, INFINITE);
	return (0);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                 THE NORMAL-RIGHTS SERVER                 |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |              THE BANNER OF THE FULL SERVER               |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# THE FULL SERVER RUNS WITH SYSTEM RIGHTS. A WINDOW WITH THOSE RIGHTS ON THE
# DESKTOP WOULD BE A WAY IN FOR ANYONE AT THE PC, SO THE "IS CONTROLLING THIS
# PC" BANNER COMES FROM "INWAY.EXE -NOTICE", STARTED AS THE PERSON SIGNED IN.
# NOBODY SIGNED IN (THE LOGIN SCREEN): NO BANNER, THERE IS NOBODY TO TELL.
*/
static void
	ENABLE_PRIVILEGE(const WCHAR *NAME)
{
	TOKEN_PRIVILEGES	TP;
	HANDLE				TOKEN;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES,
		&TOKEN))
	{
		return ;
	}
	TP.PrivilegeCount = 1;
	TP.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (LookupPrivilegeValueW(NULL, NAME, &TP.Privileges[0].Luid))
	{
		AdjustTokenPrivileges(TOKEN, FALSE, &TP, sizeof(TP), NULL, NULL);
	}
	CloseHandle(TOKEN);
}

/* A NORMAL INWAY STARTS THE BANNER AS ITSELF */
static HANDLE
	NOTICE_AS_ME(const char *WHO)
{
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;
	WCHAR				EXE[MAX_PATH];
	WCHAR				COMMAND[MAX_PATH + 64];

	EXE_PATH(EXE);
	swprintf(
		COMMAND,
		MAX_PATH + 64,
		L"\"%ls\" -notice %hs -lang %hs",
		EXE,
		WHO,
		LANG_NAME()
	);
	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(SI);
	memset(&PI, 0, sizeof(PI));
	if (!CreateProcessW(NULL, COMMAND, NULL, NULL, FALSE, CREATE_NO_WINDOW,
		NULL, NULL, &SI, &PI))
	{
		return (NULL);
	}
	CloseHandle(PI.hThread);
	return (PI.hProcess);
}

/*
# THE PROCESS OF THE BANNER ("INWAY.EXE -NOTICE"), OR NULL. THE FULL SERVER
# STARTS IT AS THE PERSON SIGNED IN (NULL WHEN NOBODY IS), ANY OTHER INWAY AS
# ITSELF.
*/
HANDLE
	SERVICE_NOTICE_START(const char *WHO)
{
	typedef BOOL		(WINAPI *T_USER_TOKEN)(ULONG SESSION, PHANDLE TOKEN);
	typedef BOOL		(WINAPI *T_MAKE_ENV)(LPVOID *ENV, HANDLE T, BOOL I);
	typedef BOOL		(WINAPI *T_FREE_ENV)(LPVOID ENV);
	STARTUPINFOW		SI;
	PROCESS_INFORMATION	PI;
	HMODULE				WTS;
	HMODULE				USERENV;
	T_USER_TOKEN		USER_TOKEN;
	T_MAKE_ENV			MAKE_ENV;
	T_FREE_ENV			FREE_ENV;
	HANDLE				TOKEN;
	HANDLE				PROCESS;
	LPVOID				ENV;
	DWORD				SESSION;
	WCHAR				EXE[MAX_PATH];
	WCHAR				COMMAND[MAX_PATH + 64];

	if (!G_WORKER)
	{
		return (NOTICE_AS_ME(WHO));
	}
	PROCESS = NULL;
	WTS = LoadLibraryW(L"wtsapi32.dll");
	USERENV = LoadLibraryW(L"userenv.dll");
	USER_TOKEN = WTS ? (T_USER_TOKEN)(void *)GetProcAddress(
		WTS,
		"WTSQueryUserToken"
	) : NULL;
	MAKE_ENV = USERENV ? (T_MAKE_ENV)(void *)GetProcAddress(
		USERENV,
		"CreateEnvironmentBlock"
	) : NULL;
	FREE_ENV = USERENV ? (T_FREE_ENV)(void *)GetProcAddress(
		USERENV,
		"DestroyEnvironmentBlock"
	) : NULL;
	ENABLE_PRIVILEGE(L"SeTcbPrivilege");
	ENABLE_PRIVILEGE(L"SeAssignPrimaryTokenPrivilege");
	ENABLE_PRIVILEGE(L"SeIncreaseQuotaPrivilege");
	TOKEN = NULL;
	if (USER_TOKEN && ProcessIdToSessionId(GetCurrentProcessId(), &SESSION) &&
		USER_TOKEN(SESSION, &TOKEN))
	{
		ENV = NULL;
		if (MAKE_ENV && !MAKE_ENV(&ENV, TOKEN, FALSE))
		{
			ENV = NULL;
		}
		EXE_PATH(EXE);
		swprintf(COMMAND, MAX_PATH + 64, L"\"%ls\" -notice %hs", EXE, WHO);
		memset(&SI, 0, sizeof(SI));
		SI.cb = sizeof(SI);
		SI.lpDesktop = L"winsta0\\default";
		memset(&PI, 0, sizeof(PI));
		if (CreateProcessAsUserW(
			TOKEN,
			NULL,
			COMMAND,
			NULL,
			NULL,
			FALSE,
			CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
			ENV,
			NULL,
			&SI,
			&PI
		))
		{
			CloseHandle(PI.hThread);
			PROCESS = PI.hProcess;
		}
		if (ENV && FREE_ENV)
		{
			FREE_ENV(ENV);
		}
		CloseHandle(TOKEN);
	}
	if (WTS)
	{
		FreeLibrary(WTS);
	}
	if (USERENV)
	{
		FreeLibrary(USERENV);
	}
	return (PROCESS);
}

static BOOL CALLBACK
	CLOSE_WINDOWS_OF(HWND WINDOW, LPARAM PID)
{
	DWORD	OWNER;

	OWNER = 0;
	GetWindowThreadProcessId(WINDOW, &OWNER);
	if (OWNER == (DWORD)PID)
	{
		PostMessageW(WINDOW, WM_CLOSE, 0, 0);
	}
	return (TRUE);
}

/*
# A NEW THREAD STARTS ON THE NORMAL DESKTOP, WHERE THE BANNER IS (THE SESSION
# THREAD MAY HAVE FOLLOWED THE SCREEN TO THE LOCK SCREEN).
*/
static DWORD WINAPI
	CLOSE_THREAD(LPVOID PARAMETER)
{
	EnumWindows(CLOSE_WINDOWS_OF, (LPARAM)(DWORD)(uintptr_t)PARAMETER);
	return (0);
}

/* CLOSES THE BANNER OF SERVICE_NOTICE_START() AND FORGETS THE HANDLE */
void
	SERVICE_NOTICE_STOP(HANDLE PROCESS)
{
	HANDLE	T;

	if (!PROCESS)
	{
		return ;
	}
	T = CreateThread(
		NULL,
		0,
		CLOSE_THREAD,
		(LPVOID)(uintptr_t)GetProcessId(PROCESS),
		0,
		NULL
	);
	if (T)
	{
		WaitForSingleObject(T, 2000);
		CloseHandle(T);
	}
	if (WaitForSingleObject(PROCESS, 3000) == WAIT_TIMEOUT)
	{
		TerminateProcess(PROCESS, 0);
	}
	CloseHandle(PROCESS);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |              THE BANNER OF THE FULL SERVER               |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                 SEEN FROM A NORMAL COPY                  |  \\//  */
/*   \/   #==========================================================#   \/   */
/*
# IS THE NORMAL-RIGHTS SERVER ON, AND WHAT IS ITS CODE? THE CODE IS READ ALSO
# WHEN IT IS NOT RUNNING: -INSTALL WITHOUT -P KEEPS IT.
*/
static void
	READ_USER_INFO(void)
{
	WCHAR	COMMAND[MAX_PATH + 64];
	WCHAR	*P;
	HANDLE	EVENT;

	USER_COMMAND(COMMAND, MAX_PATH + 64);
	P = wcsstr(COMMAND, L" -p ");
	if (!G_SVC_CODE[0] && P && lstrlenW(P + 4) >= 6)
	{
		WideCharToMultiByte(CP_UTF8, 0, P + 4, 6, G_SVC_CODE, 7, NULL, NULL);
		G_SVC_CODE[6] = '\0';
	}
	EVENT = OpenEventW(SYNCHRONIZE, FALSE, SVC_USER_EVENT);
	if (EVENT)
	{
		CloseHandle(EVENT);
		G_SVC_RUNNING = 2;
	}
}

/* IS THE SERVICE ON, AND WHAT IS ITS CODE? */
void
	SERVICE_READ_INFO(void)
{
	SC_HANDLE				M;
	SC_HANDLE				S;
	SERVICE_STATUS			ST;
	QUERY_SERVICE_CONFIGW	*CONFIG;
	DWORD					NEED;
	WCHAR					*P;

	G_SVC_RUNNING = 0;
	G_SVC_CODE[0] = '\0';
	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
	if (!M)
	{
		READ_USER_INFO();
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
	if (!G_SVC_RUNNING)
	{
		READ_USER_INFO();
	}
}

int
	SERVICE_IS_ADMIN(void)
{
	HANDLE			TOKEN;
	TOKEN_ELEVATION	E;
	DWORD			N;
	int				RESULT;

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

/*
# SHOWS THE WINDOWS "YES / NO" PROMPT, RUNS THE ADMIN COPY WITHOUT A WINDOW
# AND RETURNS ITS RESULT (SVC_DONE, SVC_COPY_FAILED, ...).
*/
int
	SERVICE_RELAUNCH_AS_ADMIN(const char *PARAMETERS)
{
	SHELLEXECUTEINFOW	SEI;
	WCHAR				EXE[MAX_PATH];
	WCHAR				*WIDE;
	DWORD				CODE;

	EXE_PATH(EXE);
	WIDE = UTF8_TO_WIDE(PARAMETERS);
	memset(&SEI, 0, sizeof(SEI));
	SEI.cbSize = sizeof(SEI);
	SEI.fMask = SEE_MASK_NOCLOSEPROCESS;
	SEI.lpVerb = L"runas";
	SEI.lpFile = EXE;
	SEI.lpParameters = WIDE;
	SEI.nShow = SW_HIDE;
	if (!ShellExecuteExW(&SEI))
	{
		CODE = GetLastError();
		free(WIDE);
		return (CODE == ERROR_CANCELLED ? SVC_DECLINED : SVC_NO_PERMISSION);
	}
	free(WIDE);
	CODE = SVC_DONE;
	if (SEI.hProcess)
	{
		WaitForSingleObject(SEI.hProcess, INFINITE);
		if (!GetExitCodeProcess(SEI.hProcess, &CODE))
		{
			CODE = SVC_DONE;
		}
		CloseHandle(SEI.hProcess);
	}
	return ((int)CODE);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                 SEEN FROM A NORMAL COPY                  |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                    INSTALL AND REMOVE                    |  \\//  */
/*   \/   #==========================================================#   \/   */
int
	SERVICE_INSTALL(const char *CODE, int NO_UPDATE)
{
	WCHAR						DIR[MAX_PATH];
	WCHAR						TARGET[MAX_PATH];
	WCHAR						EXE[MAX_PATH];
	WCHAR						BIN[MAX_PATH + 64];
	WCHAR						COMMAND[MAX_PATH + 256];
	char						*DIR_TEXT;
	SC_HANDLE					M;
	SC_HANDLE					S;
	SERVICE_DESCRIPTIONW		DESCRIPTION;
	SC_ACTION					ACTIONS[3];
	SERVICE_FAILURE_ACTIONSW	FAILURE;
	DWORD						ERROR_CODE;
	int							COPIED;
	int							I;
	int							STARTED;

	INSTALL_DIR(SVC_FOLDER, DIR);
	swprintf(TARGET, MAX_PATH, L"%ls\\INWAY.exe", DIR);
	EXE_PATH(EXE);
	SERVICE_UNINSTALL_USER(); /* THE FULL SERVER TAKES ITS PLACE */
	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!M)
	{
		SAY_LANG("{r}", STR_I_NO_SCM);
		return (SVC_NO_SCM);
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
			return (SVC_COPY_FAILED);
		}
	}
	SAY_LANG("", STR_I_SETUP);
	swprintf(
		BIN,
		MAX_PATH + 64,
		L"\"%ls\" -service -p %hs%ls",
		TARGET,
		CODE,
		NO_UPDATE ? L" -noupdate" : L""
	);
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
		ERROR_CODE = GetLastError();
		SAY("  {r}");
		SAYF(LANG(STR_I_CREATE_FAIL), ERROR_CODE);
		SAY("{0}\n");
		CloseServiceHandle(M);
		return (SVC_CREATE_FAILED | (int)((ERROR_CODE & 0XFFFF) << 8));
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
		return (SVC_NOT_STARTED);
	}
	return (SVC_DONE);
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
		return (SVC_NO_SCM);
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
	return (SVC_DONE);
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

/*
# "INWAY-NEW.EXE -SERVICE-SWAP", STARTED BY THE SERVICE WHEN AN UPDATE IS
# READY AND NOBODY IS CONNECTED: STOP THE SERVICE, PUT THIS EXE IN THE PLACE
# OF INWAY.EXE, START THE SERVICE AGAIN. WHEN THE MOVE FAILS, THE OLD EXE
# STAYS AND THE SERVICE STARTS WITH IT.
*/
int
	SERVICE_SWAP(void)
{
	SC_HANDLE	M;
	SC_HANDLE	S;
	WCHAR		SELF[MAX_PATH];
	WCHAR		TARGET[MAX_PATH];
	WCHAR		*SLASH;
	int			MOVED;
	int			I;

	GetModuleFileNameW(NULL, SELF, MAX_PATH);
	lstrcpynW(TARGET, SELF, MAX_PATH);
	SLASH = wcsrchr(TARGET, L'\\');
	if (!SLASH)
	{
		return (1);
	}
	lstrcpyW(SLASH + 1, L"INWAY.exe");
	M = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
	S = M ? OpenServiceW(
		M,
		SVC_NAME,
		SERVICE_STOP | SERVICE_START | SERVICE_QUERY_STATUS
	) : NULL;
	if (!S)
	{
		UPDATE_LOG(
			"swap: could not open the service (error %lu)",
			GetLastError()
		);
		if (M)
		{
			CloseServiceHandle(M);
		}
		return (1);
	}
	STOP_SERVICE(S);
	MOVED = 0;
	for (I = 0; I < 40 && !MOVED; ++I) /* THE HELPER MAY STILL BE CLOSING */
	{
		MOVED = MoveFileExW(
			SELF,
			TARGET,
			MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
		);
		if (!MOVED)
		{
			Sleep(250);
		}
	}
	StartServiceW(S, 0, NULL);
	CloseServiceHandle(S);
	CloseServiceHandle(M);
	if (MOVED)
	{
		UPDATE_LOG("updated (version %s)", INWAY_VERSION_TEXT);
		return (0);
	}
	UPDATE_LOG(
		"swap failed (error %lu), keeping the old version",
		GetLastError()
	);
	MoveFileExW(SELF, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	return (1);
}
/*   /\   #==========================================================#   /\   */
/*  //\\  |                    INSTALL AND REMOVE                    |  //\\  */
/* '""""' #==========================================================# '""""' */
