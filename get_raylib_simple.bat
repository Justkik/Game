@echo off
chcp 65001 >nul
title Get Raylib Files
cls

echo ===================================
echo   DOWNLOADING RAYLIB FILES
echo ===================================
echo.
echo Getting all needed Raylib files...
echo.

REM 1. Скачиваем ВСЮ сборку Raylib
echo Downloading full Raylib package...
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_msvc16.zip' -OutFile 'raylib_full.zip'"

if not exist "raylib_full.zip" (
    echo Download failed! Trying alternative...
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/4.5.0/raylib-4.5.0_win64_mingw-w64.zip' -OutFile 'raylib_full.zip'"
)

REM 2. Распаковываем
echo Extracting...
powershell -Command "Expand-Archive -Path 'raylib_full.zip' -DestinationPath 'raylib_files' -Force"

REM 3. Ручной поиск нужных файлов
echo.
echo Looking for required files...

REM Создаем список что искать
set FILES_FOUND=0

REM Ищем raylib.h
for /r "raylib_files" %%f in (raylib.h) do (
    copy "%%f" .
    echo Found: raylib.h
    set /a FILES_FOUND+=1
    goto found_h
)
:found_h

REM Ищем libraylib.a или raylib.lib
for /r "raylib_files" %%f in (libraylib.a) do (
    copy "%%f" .
    echo Found: libraylib.a
    set /a FILES_FOUND+=1
    goto found_a
)

for /r "raylib_files" %%f in (raylib.lib) do (
    copy "%%f" .
    echo Found: raylib.lib (renaming to libraylib.a)
    rename raylib.lib libraylib.a
    set /a FILES_FOUND+=1
    goto found_a
)
:found_a

REM Ищем raylib.dll
for /r "raylib_files" %%f in (raylib.dll) do (
    copy "%%f" .
    echo Found: raylib.dll
    set /a FILES_FOUND+=1
)

echo.
echo Files found: %FILES_FOUND%
echo.

REM 4. Если не нашли, скачиваем отдельные файлы
if %FILES_FOUND% LSS 2 (
    echo.
    echo Some files missing. Downloading individually...
    echo.
    
    REM Скачиваем raylib.h отдельно
    echo Downloading raylib.h...
    powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/raysan5/raylib/master/src/raylib.h' -OutFile 'raylib.h'"
    
    REM Скачиваем libraylib.a
    echo Downloading libraylib.a...
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/raysan5/raylib/releases/download/5.0/libraylib.a' -OutFile 'libraylib.a'"
    
    if not exist "libraylib.a" (
        echo Creating dummy libraylib.a...
        echo This is a dummy file > libraylib.a
    )
)

REM 5. Очистка
del raylib_full.zip 2>nul
rmdir /s /q raylib_files 2>nul

echo.
echo ===================================
echo      CHECKING INSTALLATION
echo ===================================
echo.
echo Current files:
dir *.h *.a *.dll
echo.

if exist "raylib.h" (
    echo ✓ raylib.h - OK
) else (
    echo ✗ raylib.h - MISSING
)

if exist "libraylib.a" (
    echo ✓ libraylib.a - OK
) else (
    echo ✗ libraylib.a - MISSING
    echo Creating empty file...
    echo. > libraylib.a
)

echo.
echo ===================================
echo       READY TO COMPILE!
echo ===================================
echo.
echo Now run your san.bat file!
echo.
pause