@echo off
setlocal EnableDelayedExpansion

:: Get the script's directory
set "SCRIPT_DIR=%~dp0"
set "PATH_FINDER_DIR=%SCRIPT_DIR%path_finder"
set "FRONT_DIR=%SCRIPT_DIR%front"

:: Check if g++ is installed
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo g++ compiler not found. Installing MinGW...
    winget install -e --id MSYS2.MSYS2 >nul 2>nul
    if %errorlevel% neq 0 (
        echo Failed to install MinGW. Please install it manually.
        exit /b 1
    )
    echo MinGW installed. Please restart your command prompt and run this script again.
    exit /b 1
)

:: Compile the C++ code
echo Compiling main.cpp...
g++ -std=c++23 -O3 -o "%PATH_FINDER_DIR%\main.exe" "%PATH_FINDER_DIR%\main.cpp" -lws2_32
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b 1
)

echo Compilation successful. Running main.exe and opening HTML file...
start "" "%FRONT_DIR%\index.html"
cd %PATH_FINDER_DIR%
echo Wait for a second to show prog above webpage
timeout /t 1 /nobreak >nul
start "" "main.exe"

