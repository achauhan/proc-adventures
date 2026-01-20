@echo off
setlocal enableextensions enabledelayedexpansion

REM Build hello.pc -> hello.c -> hello.o -> hello_stub.exe (no Oracle libs; uses stubbed runtime)
REM Requirements:
REM   - Pro*C proc.exe at C:\Tools\oracle\bin\proc.exe
REM   - Oracle Pro*C headers at C:\Tools\oracle\precomp\public
REM   - MinGW gcc at C:\Tools\mingw64\bin\gcc.exe
REM
REM Output:
REM   - build\hello.c, build\hello.o, build\oracle_stub.o
REM   - bin\hello_stub.exe

set ORACLE_HOME=C:\Tools\oracle
set ORA_NLS10=C:\Tools\oracle
set GCC=C:\Tools\mingw64\bin\gcc.exe
set PROC=%ORACLE_HOME%\bin\proc.exe
set ORACLE_INCLUDE=%ORACLE_HOME%\precomp\public

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
if not exist "hello.pc" (
  echo ERROR: hello.pc not found in current directory: %CD%
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

echo [1/3] Pro*C: hello.pc -> build\hello.c
REM Some proc distributions treat 'out=' as ambiguous. Generate hello.c next to input, then copy it.
set PROC_INAME=%CD%
"%PROC%" "%CD%\hello.pc" MODE=ANSI CODE=ANSI_C SQLCHECK=SYNTAX
if errorlevel 1 (
  echo ERROR: proc failed.
  exit /b 1
)
if not exist "%CD%\hello.c" (
  echo ERROR: proc did not produce %CD%\hello.c
  exit /b 1
)
copy /y "%CD%\hello.c" "%CD%\build\hello.c" >nul


echo [2/3] Compile: build\hello.c -> build\hello.o
"%GCC%" -O2 -Wall -Wextra -std=c11 -I"%ORACLE_INCLUDE%" -c build\hello.c -o build\hello.o
if errorlevel 1 (
  echo ERROR: gcc compile failed.
  exit /b 1
)

echo [2b/3] Compile: stubs\oracle_stub.c -> build\oracle_stub.o
"%GCC%" -O2 -Wall -Wextra -std=c11 -c stubs\oracle_stub.c -o build\oracle_stub.o
if errorlevel 1 (
  echo ERROR: gcc compile for stub failed.
  exit /b 1
)

echo [3/3] Link: build\hello.o + build\oracle_stub.o -> bin\hello_stub.exe
"%GCC%" -o bin\hello_stub.exe build\hello.o build\oracle_stub.o
if errorlevel 1 (
  echo ERROR: gcc link failed.
  exit /b 1
)

echo SUCCESS: Built bin\hello_stub.exe
endlocal
exit /b 0
