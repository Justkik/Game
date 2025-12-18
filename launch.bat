@echo off
echo SANYA GAME LAUNCHER
echo.
echo Compiling C++ with Raylib...
g++ main.cpp Game.cpp Locale.cpp -o game.exe -lraylib -lopengl32 -lgdi32 -lwinmm 2>nul
if exist "game.exe" (
    echo Launching game...
    game.exe
    del game.exe
) else (
    echo COMPILATION FAILED - Raylib missing
    echo.
    echo Files in this game:
    dir *.cpp *.h
    echo.
    echo To compile manually: g++ main.cpp Game.cpp Locale.cpp -lraylib
)
pause