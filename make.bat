@echo off

echo Setup environment
REM Delete and recreate build directory
if exist build (
    rmdir /s /q build
)
mkdir build

echo Compiling project
REM Compile all .c files in src directory into a single executable
gcc src\*.c -o build\fb

echo Moving executable to debug
REM Move the executable to the debug directory
if not exist debug (
    mkdir debug
)
move build\fb.exe debug\fb.exe