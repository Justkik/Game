@echo off
chcp 1251 > nul
cls

echo ============================================
echo    КОМПИЛЯЦИЯ ИГРЫ "ФИЗИКА ДЛЯ ДЕТЕЙ"
echo ============================================
echo.

echo Проверка наличия Raylib...
if not exist "raylib\include\raylib.h" (
    echo ОШИБКА: Raylib не найден!
    echo Убедитесь, что папка raylib находится в той же директории
    echo и содержит include и lib папки.
    pause
    exit /b 1
)

echo Компиляция игры...
g++ -std=c++17 PhysicsGame.cpp ^
    -I"raylib/include" ^
    -L"raylib/lib" ^
    -o PhysicsGame.exe ^
    -lraylib ^
    -lopengl32 ^
    -lgdi32 ^
    -lwinmm ^
    -static-libgcc ^
    -static-libstdc++

if errorlevel 1 (
    echo.
    echo ОШИБКА КОМПИЛЯЦИИ!
    echo.
    echo Возможные причины:
    echo 1. Не установлен MinGW
    echo 2. Неправильный путь к Raylib
    echo 3. Отсутствуют необходимые библиотеки
    echo.
    pause
    exit /b 1
)

echo.
echo УСПЕШНО СКОМПИЛИРОВАНО! ✅
echo ============================================
echo Запуск игры...
echo.
PhysicsGame.exe

if errorlevel 1 (
    echo.
    echo ОШИБКА: Игра не запустилась!
    echo Проверьте наличие необходимых DLL файлов.
    echo.
    pause
    exit /b 1
)

pause