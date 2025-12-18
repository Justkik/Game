@echo off
chcp 65001 >nul
title GAME LAUNCHER - AUTO COMPILE & RUN
color 0A
cls

echo =================================
echo     AUTO GAME LAUNCHER
echo =================================
echo This will:
echo 1. Switch to game branch
echo 2. Try to compile/run
echo 3. Return to main
echo.
pause

:menu
cls
echo ╔══════════════════════════════════╗
echo ║     SELECT GAME TO LAUNCH        ║
echo ╚══════════════════════════════════╝
echo.
echo     [1] LAUNCH SANYA GAME (C++/Raylib)
echo     [2] LAUNCH SERGEY GAME (Visual Studio)
echo     [3] LAUNCH NIKITA GAME (compile.bat)
echo.
echo     [4] View All Files
echo     [0] Exit
echo.
set /p choice="Your choice: "

if "%choice%"=="1" goto launch_sanya
if "%choice%"=="2" goto launch_sergey
if "%choice%"=="3" goto launch_nikita
if "%choice%"=="4" goto view_files
if "%choice%"=="0" exit

echo Invalid choice!
pause
goto menu

:launch_sanya
cls
echo ╔══════════════════════════════════╗
echo ║     LAUNCHING SANYA GAME         ║
echo ╚══════════════════════════════════╝
echo.
echo Step 1: Switching to sanya_game branch...
git checkout sanya_game
echo.
echo Step 2: Compiling C++ with Raylib...
echo Command: g++ main.cpp Game.cpp Locale.cpp -o game_now.exe -lraylib
echo.
g++ main.cpp Game.cpp Locale.cpp -o game_now.exe -lraylib -lopengl32 -lgdi32 -lwinmm 2>nul
if errorlevel 1 (
    echo Raylib not found! Trying without...
    echo Showing code instead:
    echo ====================
    type main.cpp
    echo.
    echo Game.cpp (first 10 lines):
    type Game.cpp | head -10
) else (
    echo Compilation successful! Launching...
    echo.
    game_now.exe
    del game_now.exe 2>nul
)
echo.
echo Step 3: Returning to main branch...
git checkout main
pause
goto menu

:launch_sergey
cls
echo ╔══════════════════════════════════╗
echo ║     LAUNCHING SERGEY GAME        ║
echo ╚══════════════════════════════════╝
echo.
echo Step 1: Switching to Sergey's branch...
git checkout Сергей-----игра
echo.
echo Step 2: Checking for executable...
if exist "*.exe" (
    echo Found .exe file! Launching...
    for %%f in (*.exe) do (
        echo Running: %%f
        start "" "%%f"
        goto sergey_return
    )
) else (
    echo No .exe found. This is a Visual Studio project.
    echo.
    echo TO LAUNCH SERGEY GAME:
    echo 1. Open SGayGame.sln in Visual Studio
    echo 2. Press F5 to build and run
    echo.
    echo Files in this branch:
    dir
)
:sergey_return
echo.
echo Step 3: Returning to main branch...
git checkout main
pause
goto menu

:launch_nikita
cls
echo ╔══════════════════════════════════╗
echo ║     LAUNCHING NIKITA GAME        ║
echo ╚══════════════════════════════════╝
echo.
echo Step 1: Switching to Nikita's branch...
git checkout Nikita---game
echo.
echo Step 2: Looking for launcher...
if exist "compile.bat" (
    echo Found compile.bat! Running...
    echo.
    call compile.bat
) else if exist "*.exe" (
    echo Found .exe file! Launching...
    for %%f in (*.exe) do (
        echo Running: %%f
        %%f
        goto nikita_return
    )
) else (
    echo No launcher found. Files:
    dir
)
:nikita_return
echo.
echo Step 3: Returning to main branch...
git checkout main
pause
goto menu

:view_files
cls
echo ╔══════════════════════════════════╗
echo ║     ALL GAMES - FILE LIST        ║
echo ╚══════════════════════════════════╝
echo.
echo SANYA GAME (sanya_game):
git ls-tree -r sanya_game --name-only
echo.
echo SERGEY GAME (Сергей-----игра):
git ls-tree -r Сергей-----игра --name-only 2>nul || echo [Switch to branch to see files]
echo.
echo NIKITA GAME (Nikita---game):
git ls-tree -r Nikita---game --name-only
echo.
pause
goto menu