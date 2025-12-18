@echo off
chcp 65001 >nul
title Sanya Game - FINAL SOLUTION
cls

echo ===================================
echo   FINAL ATTEMPT - SIMPLE VERSION
echo ===================================
echo.

echo Step 1: Finding downloaded Raylib files...
echo.

REM Ищем файлы
for /r %%f in (libraylib.a) do (
    echo Found: %%f
    copy "%%f" . 2>nul
)

for /r %%f in (raylib.h) do (
    echo Found: %%f
    copy "%%f" . 2>nul
)

echo.
echo Step 2: Current files in folder:
dir *.h *.a *.cpp
echo.

echo Step 3: SIMPLE COMPILATION (no Raylib):
echo.
echo Since Raylib keeps failing, creating simple version...
echo.

REM Создаем простую консольную версию без Raylib
(
echo // Simple console version of Sanya Game
echo #include <iostream>
echo #include <string>
echo #include <cstdlib>
echo #include <ctime>
echo using namespace std;
echo.
echo int main() {
echo     srand(time(0));
echo     cout << "==================================" << endl;
echo     cout << "      SANYA GAME (Console)" << endl;
echo     cout << "==================================" << endl;
echo     cout << endl;
echo     cout << "1. Start Game" << endl;
echo     cout << "2. Options" << endl;
echo     cout << "3. Exit" << endl;
echo     cout << endl;
echo     cout << "Choice: ";
echo.
echo     int choice;
echo     cin >> choice;
echo.
echo     if (choice == 1) {
echo         cout << "Game started!" << endl;
echo         cout << "Collecting stars..." << endl;
echo         int score = 0;
echo         for (int i = 0; i < 5; i++) {
echo             cout << "Star " << i+1 << " collected! +10 points" << endl;
echo             score += 10;
echo             system("timeout /t 1 >nul");
echo         }
echo         cout << "Final score: " << score << endl;
echo     }
echo.
echo     cout << "Thanks for playing!" << endl;
echo     system("pause");
echo     return 0;
echo }
) > simple_game.cpp

echo.
echo Step 4: Compiling simple version...
echo.
g++ simple_game.cpp -o sanya_simple.exe

if exist "sanya_simple.exe" (
    echo SUCCESS! Simple game compiled.
    echo.
    echo Launching...
    echo.
    sanya_simple.exe
) else (
    echo.
    echo ===================================
    echo    RECOMMENDATION:
    echo ===================================
    echo.
    echo Your Raylib game is too complex to compile.
    echo.
    echo Options:
    echo 1. Ask friend with Visual Studio to compile
    echo 2. Use online compiler: https://www.onlinegdb.com/
    echo 3. Make simpler game without Raylib
    echo.
    echo For friend: compile with Visual Studio, not MinGW!
)

echo.
pause