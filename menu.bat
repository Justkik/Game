@echo off
title GAMES MENU
cls

echo ========================
echo     CHOOSE GAME
echo ========================
echo.
echo 1. Sanya (main.cpp + Raylib)
echo 2. Sergey (SGayGame.cpp)
echo 3. Nikita (compile.bat)
echo.
set /p c="Select: "

if "%c%"=="1" (
    git checkout sanya_game
    if exist "main.cpp" g++ main.cpp Game.cpp Locale.cpp -o game.exe -lraylib
    if exist "game.exe" game.exe & del game.exe
    git checkout main
)

if "%c%"=="2" (
    git checkout Сергей-----игра
    if exist "SGayGame.cpp" g++ SGayGame.cpp -o game.exe && game.exe
    git checkout main
)

if "%c%"=="3" (
    git checkout Nikita---game
    if exist "compile.bat" call compile.bat
    git checkout main
)

pause