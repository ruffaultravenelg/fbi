@echo off

REM Remove the build folder if it exists
if exist build (
    rmdir /s /q build
)
mkdir build

REM Compile the source code
gcc src\*.c -o build\fb

REM Copy the build to debug folder
if exist debug (
    xcopy build debug /s /e /y >nul
)