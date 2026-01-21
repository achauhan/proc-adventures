@echo off
setlocal enableextensions enabledelayedexpansion

REM Build stubbed Pro*C hello apps (no Oracle libs; uses stubs\oracle_stub.c)
REM
REM Inputs:
REM   - hello.pc
REM   - hello_noinput.pc
REM
REM Outputs:
REM   - build\hello.c, build\hello.o, bin\hello_stub.exe
REM   - build\hello_noinput.c, build\hello_noinput.o, bin\hello_noinput_stub.exe
REM
REM Requirements:
REM   - Pro*C proc.exe at C:\Tools\oracle\bin\proc.exe
REM   - Oracle Pro*C headers at C:\Tools\oracle\precomp\public
REM   - MinGW gcc at C:\Tools\mingw64\bin\gcc.exe
REM   - Oracle stubs at stubs\oracle_stub.c
REM
REM Notes:
REM   - Uses SQLCHECK=SYNTAX so no database connectivity is required.

set ORACLE_HOME=C:\Tools\oracle
set ORA_NLS10=C:\Tools\oracle
set GCC=C:\Tools\mingw64\bin\gcc.exe
set PROC=%ORACLE_HOME%\bin\proc.exe
set ORACLE_INCLUDE=%ORACLE_HOME%\precomp\public
set ORACLE_RDBMS_INCLUDE=%ORACLE_HOME%\rdbms\public

if not exist "%PROC%" (
  echo ERROR: proc.exe not found at "%PROC%"
  exit /b 1
)
if not exist "%GCC%" (
  echo ERROR: gcc.exe not found at "%GCC%"
  exit /b 1
)
if not exist "%ORACLE_INCLUDE%\sqlca.h" (
  echo ERROR: sqlca.h not found under "%ORACLE_INCLUDE%" ^(expected %ORACLE_INCLUDE%\sqlca.h^)
  exit /b 1
)
if not exist "%ORACLE_RDBMS_INCLUDE%\oratypes.h" (
  echo ERROR: oratypes.h not found under "%ORACLE_RDBMS_INCLUDE%" ^(expected %ORACLE_RDBMS_INCLUDE%\oratypes.h^)
  exit /b 1
)
if not exist "stubs\oracle_stub.c" (
  echo ERROR: stubs\oracle_stub.c not found. It is required for stub linking.
  exit /b 1
)

REM Ensure tools are on PATH for proc to locate its message files
set PATH=%ORACLE_HOME%\bin;%PATH%

if not exist build mkdir build
if not exist bin mkdir bin

echo [0/3] Compile stub runtime once
"%GCC%" -O2 -Wall -Wextra -std=c11 -c stubs\oracle_stub.c -o build\oracle_stub.o
if errorlevel 1 (
  echo ERROR: gcc compile for stub failed.
  exit /b 1
)

set /a OK=0
set /a FAIL=0

call :build_one hello_noinput.pc

echo.
echo Done. OK=%OK%  FAIL=%FAIL%
if %FAIL% gtr 0 exit /b 1
exit /b 0

:build_one
set "PC=%~1"
set "NAME=%~n1"

if not exist "%PC%" (
  echo ERROR: missing input file: %PC%
  set /a FAIL+=1
  goto :eof
)

echo.
echo [1/3] Pro*C: %PC% ^> build\%NAME%.c

REM proc writes %NAME%.c next to input; copy into build\
"%PROC%" "%CD%\%PC%" MODE=ANSI CODE=ANSI_C SQLCHECK=SYNTAX
if errorlevel 1 (
  echo ERROR: proc failed for %PC%
  set /a FAIL+=1
  goto :eof
)

if not exist "%CD%\%NAME%.c" (
  echo ERROR: proc did not produce %CD%\%NAME%.c
  set /a FAIL+=1
  goto :eof
)

copy /y "%CD%\%NAME%.c" "%CD%\build\%NAME%.c" >nul

echo [2/3] GCC: build\%NAME%.c ^> build\%NAME%.o
"%GCC%" -O2 -Wall -Wextra -std=c11 -I"%ORACLE_INCLUDE%" -I"%ORACLE_RDBMS_INCLUDE%" -c "build\%NAME%.c" -o "build\%NAME%.o"
if errorlevel 1 (
  echo ERROR: gcc compile failed for %NAME%
  set /a FAIL+=1
  goto :eof
)

echo [3/3] Link: build\%NAME%.o ^+ build\oracle_stub.o ^> bin\%NAME%_stub.exe
"%GCC%" -o "bin\%NAME%_stub.exe" "build\%NAME%.o" "build\oracle_stub.o"
if errorlevel 1 (
  echo ERROR: gcc link failed for %NAME%
  set /a FAIL+=1
  goto :eof
)

echo SUCCESS: bin\%NAME%_stub.exe
set /a OK+=1

goto :eof
