@echo off
chcp 65001 >nul
title GAMES LAUNCHER
cls

echo =================================
echo     GAMES LAUNCHER - FINAL
echo =================================
echo Teacher: Each game has build instructions!
echo.
pause

:menu
cls
echo ================================
echo     SELECT GAME
echo ================================
echo.
echo 1. Sanya Game (has build_and_run.bat)
echo 2. Sergey Game (Visual Studio .sln)
echo 3. Nikita Game (has compile.bat)
echo.
echo 0. Exit
echo.
set /p c="Choice: "

if "%c%"=="1" goto sanya_info
if "%c%"=="2" goto sergey_info
if "%c%"=="3" goto nikita_info
if "%c%"=="0" exit

echo Invalid!
pause
goto menu

:sanya_info
cls
echo ===== SANYA GAME =====
echo.
echo INSTRUCTIONS:
echo 1. Switch to branch: git checkout sanya_game
echo 2. Run: build_and_run.bat
echo 3. Or compile manually (see README)
echo.
echo FILES:
git ls-tree -r sanya_game --name-only | findstr "\.cpp \.h \.bat \.txt" | head -15
echo.
pause
goto menu

:sergey_info
cls
echo ===== SERGEY GAME =====
echo.
echo INSTRUCTIONS:
echo 1. Switch to branch: git checkout Сергей-----игра
echo 2. Open SGayGame.sln in Visual Studio
echo 3. Press F5
echo.
pause
goto menu

:nikita_info
cls
echo ===== NIKITA GAME =====
echo.
echo INSTRUCTIONS:
echo 1. Switch to branch: git checkout Nikita---game
echo 2. Check for compile.bat
echo 3. Run it
echo.
pause
goto menu