@echo off
echo ========================
echo     GAMES MENU
echo ========================
echo.
echo 1. sanya_game
echo 2. Check other branches
echo 0. Exit
echo.
set /p c="Choice: "

if "%c%"=="1" (
    echo Switching to sanya_game...
    git checkout sanya_game
    echo Files in sanya_game:
    dir *.cpp *.h
    pause
    git checkout main
)

if "%c%"=="2" (
    echo All branches:
    git branch -a
    pause
)

pause