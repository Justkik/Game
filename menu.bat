@echo off
chcp 65001 >nul
title C++ GAMES LAUNCHER
color 0A
cls

echo =================================
echo     C++ GAMES LAUNCHER
echo =================================
echo Compiles and runs C++ games automatically.
echo Teacher's PC has all compilers.
echo.
pause

:menu
cls
echo ================================
echo     SELECT C++ GAME TO RUN
echo ================================
echo.
echo     [1] COMPILE & RUN SANYA GAME
echo     [2] COMPILE & RUN SERGEY GAME
echo     [3] RUN NIKITA GAME
echo.
echo     [0] Exit
echo.
set /p choice="Choice: "

if "%choice%"=="1" goto compile_sanya
if "%choice%"=="2" goto compile_sergey
if "%choice%"=="3" goto run_nikita
if "%choice%"=="0" exit

echo Invalid!
pause
goto menu

:compile_sanya
cls
echo ================================
echo   COMPILING SANYA GAME (C++/Raylib)
echo ================================
echo.
echo Step 1: Getting files from sanya_game branch...
git checkout sanya_game
echo.
echo Step 2: Compiling with Raylib...
g++ main.cpp Game.cpp Locale.cpp -o sanya_game_now.exe -lraylib -lopengl32 -lgdi32 -lwinmm 2>nul
if errorlevel 1 (
    echo ERROR: Raylib not found or compilation failed!
    echo.
    echo Showing code instead:
    echo ====================
    type main.cpp
) else (
    echo COMPILATION SUCCESSFUL!
    echo.
    echo Step 3: Running game...
    echo.
    sanya_game_now.exe
    del sanya_game_now.exe 2>nul
)
echo.
echo Step 4: Returning to main...
git checkout main
pause
goto menu

:compile_sergey
cls
echo ================================
echo   COMPILING SERGEY GAME (C++)
echo ================================
echo.
echo Step 1: Switching to Sergey's branch...
git checkout Сергей-----игра
echo.
echo Step 2: Looking for C++ files...
dir *.cpp *.h 2>nul
echo.
echo Step 3: Trying to compile...
if exist "*.cpp" (
    g++ *.cpp -o sergey_game_now.exe 2>nul
    if exist "sergey_game_now.exe" (
        echo COMPILED! Running...
        sergey_game_now.exe
        del sergey_game_now.exe 2>nul
    ) else (
        echo Compilation failed. This is Visual Studio project.
        echo Open SGayGame.sln in Visual Studio.
    )
) else (
    echo No .cpp files found. This is Visual Studio project.
)
echo.
echo Step 4: Returning to main...
git checkout main
pause
goto menu

:run_nikita
cls
echo ================================
echo   RUNNING NIKITA GAME
echo ================================
echo.
echo Step 1: Switching to Nikita's branch...
git checkout Nikita---game
echo.
echo Step 2: Checking for launcher...
if exist "compile.bat" (
    echo Found compile.bat! Running...
    echo.
    call compile.bat
) else if exist "*.cpp" (
    echo Found C++ files! Compiling...
    g++ *.cpp -o nikita_game_now.exe 2>nul
    if exist "nikita_game_now.exe" (
        nikita_game_now.exe
        del nikita_game_now.exe 2>nul
    )
) else (
    echo No launcher found. Files:
    dir
)
echo.
echo Step 3: Returning to main...
git checkout main
pause
goto menu