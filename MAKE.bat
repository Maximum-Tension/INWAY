:::::::::::::::::::::::::::::::::::::::::::::::::;::::::::::::::::::::::::::::::
:: BAT - MAKE                                    ;       Maximum Tension      ::
::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,;,,,,,,,,,,,,,,,,,,,,,,,,,,,,::
::                                               ;     -__            __-     ::
:: Teoman Deniz                                  ; :    :!1!-_    _-!1!:    : ::
:: maximum-tension.com                           ; ::                      :: ::
::                                               ; :!:    : :: : :  :  ::::!: ::
:: +.....................+.....................+ ;  :!:: :!:!1:!:!::1:::!!!:  ::
:: : C - Maximum Tension : Create - 2026/11/17 : ;  ::!::!!1001010!:!11!!::   ::
:: :---------------------:---------------------: ;  :!1!!11000000000011!!:    ::
:: : License - GPL-3.0   : Update - 2026/11/17 : ;   ::::!!!1!!1!!!1!!!::     ::
:: +.....................+.....................+ ;      ::::!::!:::!::::      ::
::...............................................;............................::

SETLOCAL ENABLEDELAYEDEXPANSION
CD /D "%~dp0"

IF /I "%~1"=="clean" (
	IF EXIST build RMDIR /S /Q build
	IF EXIST INWAY.exe DEL /Q INWAY.exe
	ECHO Cleaned.
	EXIT /B 0
)

WHERE gcc >NUL 2>NUL
IF ERRORLEVEL 1 (
	ECHO gcc was not found. Install MinGW-w64 and put its "bin" folder in PATH.
	EXIT /B 1
)
WHERE windres >NUL 2>NUL
IF ERRORLEVEL 1 (
	ECHO windres was not found. It comes with MinGW-w64, next to gcc.
	EXIT /B 1
)

IF NOT EXIST build MKDIR build

SET CFLAGS=-std=c99 -O2 -Wall -Wextra -Isrc
SET LIBS=-lws2_32 -lgdi32 -luser32 -ladvapi32 -lshell32 -lole32 -loleaut32
SET LIBS=%LIBS% -luuid -lurlmon -lwinhttp -lbcrypt -lwindowscodecs

ECHO [1/3] Resources
windres --codepage=65001 -I src -I res -i res\inway.rc -O coff -o build\inway.res.o
IF ERRORLEVEL 1 GOTO FAIL

ECHO [2/3] Compiling
SET OBJECTS=
FOR %%F IN (src\*.c) DO (
	ECHO        %%~nxF
	gcc %CFLAGS% -c "%%F" -o "build\%%~nF.o"
	IF ERRORLEVEL 1 GOTO FAIL
	SET OBJECTS=!OBJECTS! "build\%%~nF.o"
)

ECHO [3/3] Linking
gcc !OBJECTS! build\inway.res.o -o INWAY.exe -static -s %LIBS%
IF ERRORLEVEL 1 GOTO FAIL

ECHO.
ECHO Done: INWAY.exe
EXIT /B 0

:FAIL
ECHO.
ECHO Build failed.
EXIT /B 1
