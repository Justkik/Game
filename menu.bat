@echo off
chcp 65001 >nul
title GAMES MENU
cls

:menu
echo ========================
echo     GAMES MENU
echo ========================
echo.
echo 1. sanya_game
echo 2. Check all branches
echo 3. Save menu.bat to git
echo 0. Exit
echo.
set /p c="Choice: "

if "%c%"=="1" goto switch_sanya
if "%c%"=="2" goto show_branches
if "%c%"=="3" goto save_to_git
if "%c%"=="0" exit

echo Invalid choice!
pause
goto menu

:switch_sanya
echo Saving menu.bat to git first...
git add menu.bat 2>nul
git commit -m "Auto-save menu" 2>nul
echo Switching to sanya_game...
git checkout sanya_game
echo.
echo Files in sanya_game:
dir *.cpp *.h
echo.
pause
git checkout main
goto menu

:show_branches
cls
echo All branches:
git branch -a
echo.
pause
goto menu

:save_to_git
echo Adding menu.bat to git...
git add menu.bat
git commit -m "Added game menu"
echo Saved! Now you can switch branches.
pause
goto menu