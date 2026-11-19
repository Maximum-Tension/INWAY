:::::::::::::::::::::::::::::::::::::::::::::::::;::::::::::::::::::::::::::::::
:: BAT - MAKE32                                  ;       Maximum Tension      ::
::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,;,,,,,,,,,,,,,,,,,,,,,,,,,,,,::
::                                               ;     -__            __-     ::
:: Teoman Deniz                                  ; :    :!1!-_    _-!1!:    : ::
:: maximum-tension.com                           ; ::                      :: ::
::                                               ; :!:    : :: : :  :  ::::!: ::
:: +.....................+.....................+ ;  :!:: :!:!1:!:!::1:::!!!:  ::
:: : C - Maximum Tension : Create - 2026/11/18 : ;  ::!::!!1001010!:!11!!::   ::
:: :---------------------:---------------------: ;  :!1!!11000000000011!!:    ::
:: : License - GPL-3.0   : Update - 2026/11/18 : ;   ::::!!!1!!1!!!1!!!::     ::
:: +.....................+.....................+ ;      ::::!::!:::!::::      ::
::...............................................;............................::

@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
CD /D "%~dp0"

IF /I "%~1"=="clean" (
	IF EXIST build32 RMDIR /S /Q build32
	IF EXIST INWAY32.exe DEL /Q INWAY32.exe
	ECHO Cleaned.
	EXIT /B 0
)

:: A 32-BIT GCC: "i686-w64-mingw32-gcc" IN PATH, A "gcc" IN PATH THAT
:: ALREADY MAKES 32-BIT PROGRAMS, WINLIBS IN C:\MINGW32 OR MSYS2'S MINGW32
SET CC=
SET MACHINE=
WHERE i686-w64-mingw32-gcc >NUL 2>NUL
IF NOT ERRORLEVEL 1 SET CC=i686-w64-mingw32-gcc
IF NOT DEFINED CC (
	WHERE gcc >NUL 2>NUL
	IF NOT ERRORLEVEL 1 (
		FOR /F "delims=" %%M IN ('gcc -dumpmachine') DO SET MACHINE=%%M
		IF /I "!MACHINE:~0,1!"=="i" SET CC=gcc
	)
)
IF NOT DEFINED CC IF EXIST "C:\mingw32\bin\gcc.exe" (
	SET "PATH=C:\mingw32\bin;!PATH!"
	SET CC=gcc
)
IF NOT DEFINED CC IF EXIST "C:\msys64\mingw32\bin\gcc.exe" (
	SET "PATH=C:\msys64\mingw32\bin;!PATH!"
	SET CC=gcc
)
IF NOT DEFINED CC (
	ECHO No 32-bit gcc was found. MAKE32.bat looks for:
	ECHO   - i686-w64-mingw32-gcc in PATH
	ECHO   - a gcc in PATH that makes 32-bit programs
	ECHO   - C:\mingw32\bin\gcc.exe
	ECHO   - C:\msys64\mingw32\bin\gcc.exe
	ECHO Get the "Win32" MSVCRT zip from winlibs.com and unpack it to C:\
	ECHO so that C:\mingw32\bin\gcc.exe exists.
	EXIT /B 1
)

SET MACHINE=
FOR /F "delims=" %%M IN ('!CC! -dumpmachine') DO SET MACHINE=%%M
IF /I NOT "!MACHINE:~0,1!"=="i" (
	ECHO !CC! makes 64-bit programs ^(!MACHINE!^).
	ECHO MAKE32.bat needs a 32-bit gcc.
	EXIT /B 1
)

:: THE WINDRES NEXT TO THAT GCC
SET CCDIR=
FOR /F "delims=" %%P IN ('WHERE !CC!') DO (
	IF NOT DEFINED CCDIR SET "CCDIR=%%~dpP"
)
SET RC=
IF EXIST "!CCDIR!i686-w64-mingw32-windres.exe" (
	SET "RC=!CCDIR!i686-w64-mingw32-windres.exe"
)
IF NOT DEFINED RC IF EXIST "!CCDIR!windres.exe" SET "RC=!CCDIR!windres.exe"
IF NOT DEFINED RC (
	ECHO windres was not found next to !CC!. It comes with MinGW-w64.
	EXIT /B 1
)

IF NOT EXIST build32 MKDIR build32

SET RCFLAGS=--codepage=65001 -F pe-i386 -I src -I res
SET CFLAGS=-std=c99 -O2 -Wall -Wextra -Isrc
SET LIBS=-lws2_32 -lgdi32 -luser32 -ladvapi32 -lshell32 -lole32 -loleaut32
SET LIBS=%LIBS% -luuid -lurlmon -lwinhttp -lbcrypt -lwindowscodecs

ECHO Compiler: !CC! - !MACHINE!
ECHO [1/3] Resources
"!RC!" %RCFLAGS% -i res\inway.rc -O coff -o build32\inway.res.o
IF ERRORLEVEL 1 GOTO FAIL

ECHO [2/3] Compiling
SET OBJECTS=
FOR %%F IN (src\*.c) DO (
	ECHO        %%~nxF
	!CC! %CFLAGS% -c "%%F" -o "build32\%%~nF.o"
	IF ERRORLEVEL 1 GOTO FAIL
	SET OBJECTS=!OBJECTS! "build32\%%~nF.o"
)

ECHO [3/3] Linking
!CC! !OBJECTS! build32\inway.res.o -o INWAY32.exe -static -s %LIBS%
IF ERRORLEVEL 1 GOTO FAIL

ECHO.
ECHO Done: INWAY32.exe
EXIT /B 0

:FAIL
ECHO.
ECHO Build failed.
EXIT /B 1
