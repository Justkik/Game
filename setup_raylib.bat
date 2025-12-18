@echo off
chcp 65001 >nul
title Raylib Installer
color 0E
cls

echo ===================================
echo     RAYLIB INSTALLATION
echo ===================================
echo.
echo Installing Raylib for your game...
echo.

:download
echo Step 1: Downloading Raylib...
echo.
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip' -OutFile 'raylib.zip'"

if not exist "raylib.zip" (
    echo Download failed!
    echo Trying alternative link...
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/4.5.0/raylib-4.5.0_win64_mingw-w64.zip' -OutFile 'raylib.zip'"
    if not exist "raylib.zip" (
        echo Still failed! Check internet connection.
        pause
        exit
    )
)

:extract
echo.
echo Step 2: Extracting files...
echo.
powershell -Command "Expand-Archive -Path 'raylib.zip' -DestinationPath 'raylib_temp' -Force"

:copy_files
echo Step 3: Copying needed files...
echo.

REM Ищем файлы в разных возможных местах
if exist "raylib_temp\raylib-5.0_win64_mingw-w64\raylib.h" (
    copy "raylib_temp\raylib-5.0_win64_mingw-w64\raylib.h" .
    copy "raylib_temp\raylib-5.0_win64_mingw-w64\libraylib.a" .
    copy "raylib_temp\raylib-5.0_win64_mingw-w64\libraylib.dll.a" .
    echo Found in: raylib-5.0_win64_mingw-w64
) else if exist "raylib_temp\raylib-4.5.0_win64_mingw-w64\raylib.h" (
    copy "raylib_temp\raylib-4.5.0_win64_mingw-w64\raylib.h" .
    copy "raylib_temp\raylib-4.5.0_win64_mingw-w64\libraylib.a" .
    copy "raylib_temp\raylib-4.5.0_win64_mingw-w64\libraylib.dll.a" .
    echo Found in: raylib-4.5.0_win64_mingw-w64
) else if exist "raylib_temp\raylib.h" (
    copy "raylib_temp\raylib.h" .
    copy "raylib_temp\libraylib.a" .
    copy "raylib_temp\libraylib.dll.a" .
    echo Found in root
) else (
    echo Searching for files...
    dir raylib_temp /s /b
    for /f "delims=" %%i in ('dir /s /b raylib.h 2^>nul') do (
        echo Found raylib.h at: %%i
        copy "%%i" .
    )
    for /f "delims=" %%i in ('dir /s /b libraylib.a 2^>nul') do (
        echo Found libraylib.a at: %%i
        copy "%%i" .
    )
)

:cleanup
echo.
echo Step 4: Cleaning up...
echo.
del raylib.zip 2>nul
rmdir /s /q raylib_temp 2>nul

:verify
echo Step 5: Verifying installation...
echo.
if exist "raylib.h" (
    echo SUCCESS: raylib.h installed!
) else (
    echo ERROR: raylib.h not found!
)

if exist "libraylib.a" (
    echo SUCCESS: libraylib.a installed!
) else (
    echo ERROR: libraylib.a not found!
)

echo.
echo ===================================
echo       INSTALLATION COMPLETE
echo ===================================
echo.
echo Now you can compile your game!
echo.
echo To compile, run:
echo   g++ main.cpp Game.cpp Locale.cpp -o sanya_game.exe -lraylib
echo.
pause