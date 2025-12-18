@echo off
echo Building Sanya Game...
echo.

REM Проверяем Raylib
if not exist "raylib.h" (
    echo ERROR: raylib.h not found!
    echo Download Raylib from: https://www.raylib.com/
    pause
    exit
)

echo Compiling...
g++ main.cpp Game.cpp Locale.cpp -o sanya_game.exe -lraylib -lopengl32 -lgdi32 -lwinmm

if exist "sanya_game.exe" (
    echo Build successful! Launching game...
    echo.
    sanya_game.exe
) else (
    echo Build failed!
    echo Check: 1. Raylib installed 2. C++ compiler
)

pause