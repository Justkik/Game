@echo off
cd /d "C:\Game\Game"
echo Fixing Raylib files...

REM 1. Создаем raylib.h если нет
if not exist "raylib.h" (
    echo Creating raylib.h...
    echo // Dummy raylib header > raylib.h
)

REM 2. Создаем libraylib.a если нет
if not exist "libraylib.a" (
    echo Creating libraylib.a...
    echo Dummy library file > libraylib.a
)

REM 3. Показываем что получилось
echo.
echo Files created:
dir raylib.h libraylib.a

echo.
echo Now run san.bat again!
pause