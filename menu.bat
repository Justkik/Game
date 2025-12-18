@echo off
chcp 65001 >nul
title GAMES COLLECTION - FINAL
color 0A
cls

echo =================================
echo     FINAL GAMES COLLECTION
echo =================================
echo.
echo This menu shows ALL 3 games.
echo Teacher can see code and how to run.
echo.
pause

:menu
cls
echo ╔══════════════════════════════════╗
echo ║     SELECT GAME TO VIEW          ║
echo ╚══════════════════════════════════╝
echo.
echo     [1] ▶ SANYA GAME (C++/Raylib)
echo     [2] ▶ SERGEY GAME (Visual Studio)
echo     [3] ▶ NIKITA GAME (compile.bat)
echo.
echo     [4] 📋 Quick All Games Summary
echo     [0] ❌ Exit
echo.
set /p choice="Your choice: "

if "%choice%"=="1" goto show_sanya
if "%choice%"=="2" goto show_sergey
if "%choice%"=="3" goto show_nikita
if "%choice%"=="4" goto show_summary
if "%choice%"=="0" exit

echo Invalid choice!
pause
goto menu

:show_sanya
cls
echo ╔══════════════════════════════════╗
echo ║     SANYA GAME - C++/Raylib      ║
echo ╚══════════════════════════════════╝
echo.
echo 📁 MAIN FILES:
echo   • main.cpp    (game launcher)
echo   • Game.cpp    (main game logic)
echo   • Game.h      (header)
echo   • Locale.cpp  (localization)
echo.
echo 🎮 GAME FEATURES:
echo   • Graphics: Raylib library
echo   • Images: PNG/JPG files in image/
echo   • Sounds: WAV files in sounds/
echo   • Fonts: TTF fonts
echo   • High scores: saved to file
echo.
echo 💻 HOW TO COMPILE & RUN:
echo   1. git checkout sanya_game
echo   2. g++ main.cpp Game.cpp Locale.cpp -o game.exe -lraylib
echo   3. game.exe
echo.
echo 📝 CODE (main.cpp):
git show sanya_game:main.cpp
echo.
pause
goto menu

:show_sergey
cls
echo ╔══════════════════════════════════╗
echo ║   SERGEY GAME - Visual Studio    ║
echo ╚══════════════════════════════════╝
echo.
echo 📁 PROJECT STRUCTURE:
echo   • SGayGame.sln  (Visual Studio solution)
echo   • SGayGame/     (C++ project folder)
echo   • x64/          (compiled binaries)
echo   • packages/     (NuGet packages)
echo.
echo 🎮 HOW TO LAUNCH:
echo   1. git checkout Сергей-----игра
echo   2. Open SGayGame.sln in Visual Studio
echo   3. Press F5 to build and run
echo.
echo 📊 FILES COUNT:
git ls-tree -r Сергей-----игра --name-only 2>nul | find /c /v ""
echo files in this branch
echo.
pause
goto menu

:show_nikita
cls
echo ╔══════════════════════════════════╗
echo ║        NIKITA GAME               ║
echo ╚══════════════════════════════════╝
echo.
echo 📁 FILES IN BRANCH:
git ls-tree -r Nikita---game --name-only | head -20
echo ...
echo.
echo 🎮 LAUNCH METHOD:
echo   • Has compile.bat for automated build
echo.
echo 💻 HOW TO RUN:
echo   1. git checkout Nikita---game
echo   2. Check for compile.bat
echo   3. Run: call compile.bat
echo.
pause
goto menu

:show_summary
cls
echo ╔══════════════════════════════════╗
echo ║      ALL 3 GAMES - SUMMARY       ║
echo ╚══════════════════════════════════╝
echo.
echo 🎮 1. SANYA GAME:
echo    • Language: C++ with Raylib graphics
echo    • Files: 5 .cpp/.h files + resources
echo    • Launch: Compile with g++ + Raylib
echo.
echo 🎮 2. SERGEY GAME:
echo    • Language: C++ with Visual Studio
echo    • Files: VS project (.sln, .vcxproj)
echo    • Launch: Open in Visual Studio, F5
echo.
echo 🎮 3. NIKITA GAME:
echo    • Language: Unknown (has compile.bat)
echo    • Files: Various + compile script
echo    • Launch: Run compile.bat
echo.
echo ════════════════════════════════════
echo 📊 REPOSITORY STATS:
echo   • 4 branches (main + 3 games)
echo   • Each game in separate branch
echo   • Main branch has this menu
echo.
pause
goto menu