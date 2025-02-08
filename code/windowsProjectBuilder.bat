:: The script checks if g++ is installed. If not, it installs it, compiles the code, and runs it.
@echo off
setlocal EnableDelayedExpansion

:: Get the script's directory
set "SCRIPT_DIR=%~dp0"
set "PATH_FINDER_DIR=%SCRIPT_DIR%path_finder"
set "FRONT_DIR=%SCRIPT_DIR%front"
set "MINGW_PATH=C:\msys64\mingw64\bin"
set "GPP=C:\msys64\mingw64\bin\g++.exe"

:: Check if g++ is installed
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo g++ compiler not found.

    echo Installing MinGW...
    winget install -e --id MSYS2.MSYS2
    echo MinGW installed.

    echo Installing g++...
    C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm --needed mingw-w64-x86_64-gcc"
    echo g++ installation completed.
    
    echo Adding mingw64 tools to the path
    
    if exist "%GPP%" (
        :: 
        goto path_changer

    ) else (
        echo "%GPP% not found."
        echo Installation failed. Please install the compiler manually.
        pause
    )
) else (
    echo g++ compiler found.
    echo Proceeding with the compilation process.
    goto compile
)



:path_changer

:: Create a temporary batch file to set the PATH variable for future sessions
echo @echo off > setpath.bat
echo setx PATH "%MINGW_PATH%;%%PATH%%" /M >> setpath.bat

:: Call the temporary batch file
call setpath.bat

:: Clean up the temporary batch file
del setpath.bat

:: Update the PATH variable for the current session
set PATH=%MINGW_PATH%;%PATH%

:: Display the updated PATH variable
echo %PATH%



:compile

:: Compile the C++ code
echo Compiling main.cpp..
cd %PATH_FINDER_DIR%
@echo on
g++ -std=c++23 -O3 -o main.exe main.cpp -lws2_32
@echo off
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit
)

echo Compilation successful. Running main.exe and opening HTML file...
start "" "%FRONT_DIR%\index.html"
cd %PATH_FINDER_DIR%
echo Wait for a second to show prog above webpage
timeout /t 1 /nobreak >nul
start "" "main.exe"
