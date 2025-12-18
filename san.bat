@echo off
chcp 65001 >nul
title Sanya Game Launcher
color 0A
cls

echo ===================================
echo       SANYA GAME LAUNCHER
echo ===================================
echo.
echo Checking for Raylib...

if not exist "raylib.h" (
    echo ERROR: raylib.h not found!
    echo.
    echo Run setup_raylib.bat first!
    echo.
    pause
    exit
)

echo Raylib found! Compiling game...
echo.

REM Компилируем с Raylib
g++ main.cpp Game.cpp Locale.cpp -o sanya_game.exe -lraylib -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo.
    echo Trying alternative compilation...
    g++ main.cpp Game.cpp Locale.cpp -o sanya_game.exe -lraylib
)

if exist "sanya_game.exe" (
    echo.
    echo ===================================
    echo      COMPILATION SUCCESSFUL!
    echo ===================================
    echo.
    echo Launching your game...
    echo.
    timeout /t 2 >nul
    sanya_game.exe
) else (
    echo.
    echo ===================================
    echo       COMPILATION FAILED
    echo ===================================
    echo.
    echo Last attempt with simple flags...
    g++ main.cpp Game.cpp Locale.cpp -o sanya_game.exe
    
    if exist "sanya_game.exe" (
        echo Simple compilation worked!
        sanya_game.exe
    ) else (
        echo Still failing. Possible issues:
        echo 1. Raylib not properly installed
        echo 2. Errors in C++ code
        echo 3. Missing other .cpp files
        echo.
        echo Files in directory:
        dir *.cpp *.h *.a
    )
)

echo.
pause