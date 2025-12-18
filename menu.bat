@echo off
chcp 65001 >nul
title MAIN MENU - GAME LAUNCHER
color 0A
cls

:menu
echo ================================
echo     LAUNCH GAME FROM BRANCH
echo ================================
echo.
echo 1. Launch Sanya Game
echo 2. Launch Sergey Game
echo 3. Launch Nikita Game
echo.
echo 4. Quick View All
echo 0. Exit
echo.
set /p c="Choice: "

if "%c%"=="1" goto launch_sanya
if "%c%"=="2" goto launch_sergey
if "%c%"=="3" goto launch_nikita
if "%c%"=="4" goto quick_view
if "%c%"=="0" exit

echo Invalid!
pause
goto menu

:launch_sanya
cls
echo Launching Sanya Game...
echo Switching to sanya_game...
git checkout sanya_game
echo.
if exist "launch.bat" (
    call launch.bat
) else (
    echo ERROR: launch.bat not found in sanya_game branch!
    echo Ask Sanya to create launch.bat
)
echo.
echo Returning to main...
git checkout main
pause
goto menu

:launch_sergey
cls
echo Launching Sergey Game...
echo Switching to Sergey branch...
git checkout Сергей-----игра
echo.
if exist "launch.bat" (
    call launch.bat
) else (
    echo ERROR: launch.bat not found!
    dir
)
echo.
echo Returning to main...
git checkout main
pause
goto menu

:launch_nikita
cls
echo Launching Nikita Game...
echo Switching to Nikita branch...
git checkout Nikita---game
echo.
if exist "launch.bat" (
    call launch.bat
) else (
    echo ERROR: launch.bat not found!
    dir
)
echo.
echo Returning to main...
git checkout main
pause
goto menu

:quick_view
cls
echo QUICK VIEW ALL BRANCHES:
echo.
echo sanya_game: C++ with Raylib
echo Сергей-----игра: Visual Studio project
echo Nikita---game: Has compile.bat
echo.
pause
goto menu