@echo off
chcp 65001 >nul
title GAME LAUNCHER - NO SWITCHING
color 0A
cls

echo =================================
echo     GAME LAUNCHER v3.0
echo =================================
echo Shows games WITHOUT switching branches
echo.
pause

:menu
cls
echo ================================
echo     SELECT GAME TO VIEW
echo ================================
echo.
echo     [1] SANYA GAME (C++/Raylib)
echo     [2] SERGEY GAME (Visual Studio)
echo     [3] NIKITA GAME (compile.bat)
echo.
echo     [0] Exit
echo.
set /p choice="Your choice: "

if "%choice%"=="1" goto show_sanya
if "%choice%"=="2" goto show_sergey
if "%choice%"=="3" goto show_nikita
if "%choice%"=="0" exit

echo Invalid choice!
pause
goto menu

:show_sanya
cls
echo ================================
echo      SANYA GAME - C++/Raylib
echo ================================
echo.
echo Files in sanya_game branch:
git ls-tree -r sanya_game --name-only
echo.
echo Main.cpp code:
git show sanya_game:main.cpp
echo.
echo TO LAUNCH THIS GAME:
echo 1. Switch to sanya_game: git checkout sanya_game
echo 2. Compile: g++ main.cpp Game.cpp Locale.cpp -lraylib
echo 3. Run: game.exe
echo.
pause
goto menu

:show_sergey
cls
echo ================================
echo   SERGEY GAME - Visual Studio
echo ================================
echo.
echo Files in Sergey branch:
git ls-tree -r Сергей-----игра --name-only 2>nul || (
    echo Cannot list files directly.
    echo Branch name has special characters.
)
echo.
echo PROJECT STRUCTURE:
echo - SGayGame.sln (Visual Studio solution)
echo - SGayGame/ (project folder)
echo - x64/ (compiled files)
echo.
echo TO LAUNCH THIS GAME:
echo 1. Switch to branch: git checkout Сергей-----игра
echo 2. Open SGayGame.sln in Visual Studio
echo 3. Press F5 to run
echo.
pause
goto menu

:show_nikita
cls
echo ================================
echo      NIKITA GAME
echo ================================
echo.
echo Files in Nikita branch:
git ls-tree -r Nikita---game --name-only
echo.
echo TO LAUNCH THIS GAME:
echo 1. Switch to branch: git checkout Nikita---game
echo 2. Check for compile.bat
echo 3. Run: call compile.bat
echo.
pause
goto menu