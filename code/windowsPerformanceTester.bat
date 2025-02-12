@echo off
setlocal EnableDelayedExpansion

:: Get the script's directory
set "SCRIPT_DIR=%~dp0"
set "PATH_FINDER_DIR=%SCRIPT_DIR%path_finder"
set "FRONT_DIR=%SCRIPT_DIR%front"
set "MINGW_PATH=C:\msys64\mingw64\bin"
set "GPP=C:\msys64\mingw64\bin\g++.exe"
set "BENCH_DIR=%SCRIPT_DIR%benchmark_sampling"

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
g++ -std=c++23 -O3 -DTEST -DSPLSIZ=$SAMPLE_SIZE -DLMQTY=$LANDMARKS_QUANTITY -DTOPY -o tester.exe main.cpp -lws2_32
@echo off
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit
)

echo Compilation successful.

cd %PATH_FINDER_DIR%
echo Executing tester.exe...
call "tester.exe"


REM Move into the benchmark directory
cd /d "%BENCH_DIR%" || (
    echo Directory %BENCH_DIR% not found!
    exit /b 1
)

REM Define virtual environment path inside the benchmark directory
set VENV_DIR=.\.venv

REM Check if Python is installed
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo Python not found. Installing...
    echo Please install Python manually from https://www.python.org/downloads/
    exit /b 1
)

REM Create a virtual environment inside the benchmark directory if it doesn't exist
if not exist "%VENV_DIR%" (
    echo Creating a virtual environment in %VENV_DIR%...
    python -m venv "%VENV_DIR%"
)

REM Activate the virtual environment
echo Activating the virtual environment...
call "%VENV_DIR%\Scripts\activate.bat"

REM Install required Python packages
echo Installing required Python packages...
pip install --upgrade pip
pip install pandas matplotlib

REM Run the benchmark script
echo Running the python script...
python tester.py

REM Deactivate the virtual environment
deactivate

echo Python script executed. A picture was saved here: %BENCH_DIR%\test-results.png

echo Press any key to exit...
pause >nul
exit /b 0